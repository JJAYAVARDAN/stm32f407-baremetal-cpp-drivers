/*
 * ADC_Driver.cpp
 *
 * STM32F407 Bare-Metal ADC Driver
 *
 * Created on: 21-Jul-2026
 * Author: jolap
 *
 * Purpose:
 *   Implementation of the STM32F407 ADC driver for analog channel setup,
 *   conversion control, and data acquisition.
 *
 * State Machine:
 *   Enable clock -> configure channel/sample time -> start conversion -> EOC -> read DR.
 *
 * RM0090 Reference:
 *   Chapter 13 - Analog-to-digital converter (ADC)
 */

 /*********************************************************************
  *                      ADC Driver Design
  *
  * The Analog-to-Digital Converter (ADC) converts analog voltages
  * present on external input pins into corresponding digital values.
  * This allows the STM32F407 microcontroller to interface with
  * analog sensors such as temperature sensors, potentiometers,
  * pressure sensors, light sensors, battery monitors, and many
  * other analog devices.
  *
  * The STM32F407 provides three independent ADC peripherals:
  *
  *      • ADC1
  *      • ADC2
  *      • ADC3
  *
  * Each ADC supports up to 16 external analog input channels along
  * with several internal channels including:
  *
  *      • Internal Temperature Sensor
  *      • Internal Voltage Reference (VREFINT)
  *      • VBAT Battery Voltage
  *
  * The ADC uses a Successive Approximation Register (SAR)
  * architecture to perform analog-to-digital conversion.
  *
  * Before conversion begins, software configures:
  *
  *      • ADC Clock
  *      • Resolution
  *      • Sampling Time
  *      • Conversion Sequence
  *      • Continuous/Single Conversion
  *      • Scan Mode
  *      • Data Alignment
  *      • External Trigger (Optional)
  *
  * During conversion, the ADC samples the analog voltage and
  * converts it into a digital value which is stored in the
  * Data Register (DR). The conversion result may then be read
  * by software or automatically transferred using DMA.
  *
  * The ADC continuously updates status flags including:
  *
  *      • EOC    - End Of Conversion
  *      • JEOC   - Injected End Of Conversion
  *      • OVR    - Overrun
  *      • AWD    - Analog Watchdog
  *      • STRT   - Regular Conversion Started
  *      • JSTRT  - Injected Conversion Started
  *
  * The ADC driver abstracts the conversion sequence while
  * providing a simple object-oriented interface for analog
  * signal acquisition.
  *
  *********************************************************************
  *                  Polling Mode State Machine
  *
  *              Enable ADC Clock
  *                      │
  *                      ▼
  *             Configure GPIO Pin
  *               As Analog Mode
  *                      │
  *                      ▼
  *           Configure ADC Parameters
  *      (Resolution, Sample Time, Channel)
  *                      │
  *                      ▼
  *              Enable ADC
  *                      │
  *                      ▼
  *         Start ADC Conversion
  *                      │
  *                      ▼
  *        ADC Samples Analog Signal
  *                      │
  *                      ▼
  *       ADC Performs Conversion
  *                      │
  *                      ▼
  *           EOC Flag Becomes Set
  *                      │
  *                      ▼
  *          Poll EOC Flag
  *                      │
  *                      ▼
  *         Read ADC Data Register
  *                      │
  *                      ▼
  *      Conversion Complete
  *
  *********************************************************************
  *                Interrupt Mode State Machine
  *
  *              Enable ADC Clock
  *                      │
  *                      ▼
  *             Configure GPIO Pin
  *               As Analog Mode
  *                      │
  *                      ▼
  *           Configure ADC Parameters
  *                      │
  *                      ▼
  *        Enable ADC Interrupt
  *             (EOCIE/OVRIE)
  *                      │
  *                      ▼
  *         Enable NVIC Interrupt
  *                      │
  *                      ▼
  *              Enable ADC
  *                      │
  *                      ▼
  *         Start ADC Conversion
  *                      │
  *                      ▼
  *       ADC Performs Conversion
  *                      │
  *                      ▼
  *        End Of Conversion (EOC)
  *                      │
  *                      ▼
  *        ADC Generates Interrupt
  *                      │
  *                      ▼
  *         NVIC Executes ADC ISR
  *                      │
  *                      ▼
  *      Determine Interrupt Source
  *                      │
  *         ┌────────────┴────────────┐
  *         ▼                         ▼
  *      EOC Event             Error Event
  *         │                         │
  *         ▼                         ▼
  *   Read ADC Result         Handle Error
  *         │                         │
  *         └────────────┬────────────┘
  *                      ▼
  *           Return From Interrupt
  *
  *********************************************************************
  *                  DMA Mode State Machine
  *
  *              Enable ADC Clock
  *                      │
  *                      ▼
  *             Configure ADC
  *                      │
  *                      ▼
  *              Configure DMA
  *                      │
  *                      ▼
  *        Enable ADC DMA Request
  *                      │
  *                      ▼
  *          Start ADC Conversion
  *                      │
  *                      ▼
  *       ADC Conversion Complete
  *                      │
  *                      ▼
  *      DMA Transfers DR → Memory
  *                      │
  *                      ▼
  *      DMA Updates Memory Buffer
  *                      │
  *                      ▼
  *     Transfer Complete Interrupt
  *          (Optional)
  *                      │
  *                      ▼
  *       Continue Next Conversion
  *
  *********************************************************************
  *                  Driver Responsibilities
  *
  * • Enable ADC peripheral clock.
  * • Configure ADC resolution.
  * • Configure sampling time.
  * • Configure regular conversion channels.
  * • Configure injected conversion channels.
  * • Configure scan mode.
  * • Configure continuous conversion mode.
  * • Configure discontinuous mode.
  * • Configure data alignment.
  * • Configure external trigger source.
  * • Start regular conversion.
  * • Stop conversion.
  * • Read conversion result.
  * • Enable/Disable ADC interrupt.
  * • Configure DMA support.
  * • Handle ADC interrupts.
  * • Monitor ADC status flags.
  *
  * RM0090 Reference:
  * Chapter 13 - Analog-to-Digital Converter (ADC)
  *
  *********************************************************************/

#include "ADC_Driver.h"

 /*********************************************************************
  *                  Constructor
  *********************************************************************/

ADC::ADC(ADC_Instance instance)
{
	switch (instance)
	{
	case ADC_Instance::ADC1:
		mADC = ADC1_REG;
		break;
	case ADC_Instance::ADC2:
		mADC = ADC2_REG;
		break;
	case ADC_Instance::ADC3:
		mADC = ADC3_REG;
		break;
	}
}
/*********************************************************************
 *                  Enable ADC Clock
 *********************************************************************/
void ADC::enableClock()
{
	if (mADC == ADC1_REG)
	{
		RCC::enableADCClock(RCC_ADC::ADC1);
	}
	else if (mADC == ADC2_REG)
	{
		RCC::enableADCClock(RCC_ADC::ADC2);
	}
	else if (mADC == ADC3_REG)
	{
		RCC::enableADCClock(RCC_ADC::ADC3);
	}
}
/*********************************************************************
 *                  Initialize ADC
 *********************************************************************/
void ADC::init(ADC_Resolution resolution,
	ADC_Alignment alignment,
	ADC_MODE mode)
{
	/**************************************************************
	 * Enable ADC Clock
	 **************************************************************/
	enableClock();

	/**************************************************************
	 * Disable ADC before configuration
	 **************************************************************/

	disable();

	/**************************************************************
	 * Clear Configuration Registers
	 **************************************************************/

	mADC->CR1 = 0;
	mADC->CR2 = 0;
	/**************************************************************
	 * Configure Resolution
	 *
	 * CR1[25:24]
	 **************************************************************/

	mADC->CR1 |= (static_cast<uint32_t>(resolution) << 24);

	/**************************************************************
	 * Configure Data Alignment
	 *
	 * CR2 Bit 11
	 **************************************************************/

	if (alignment == ADC_Alignment::LEFT)
	{
		mADC->CR2 |= (1U << 11);
	}
	/**************************************************************
	 * Continuous Conversion Mode
	 *
	 * CR2 Bit 1
	 **************************************************************/
	if (mode == ADC_MODE::CONTINOUS)
	{
		mADC->CR2 |= (1U << 1);
	}
	/**************************************************************
	 * Software Trigger Enable
	 *
	 * EXTEN = 00
	 * SWSTART used for conversion
	 **************************************************************/
	mADC->CR2 &= ~(3U << 28);

	/**************************************************************
	 * Enable ADC
	 **************************************************************/
	enable();

}
/*********************************************************************
 *                  Enable ADC
 *********************************************************************/

void ADC::enable()
{
	/*
	 * ADON Bit (CR2 Bit 0)
	 */
	mADC->CR2 |= (1U << 0);
}

/*********************************************************************
 *                  Disable ADC
 *********************************************************************/
void ADC::disable()
{
	/*
	 * Clear ADON Bit
	 */
	mADC->CR2 &= ~(1U << 0);
}
/*********************************************************************
 *                  Select ADC Channel
 *********************************************************************/

void ADC::selectChannel(ADC_Channel channel)
{
	/*
	 * Configure first conversion in regular sequence.
	 * SQR3[4:0] -> Channel Number
	 */
	mADC->SQR3 &= ~(0x1FU);
	mADC->SQR3 |= static_cast<uint32_t>(channel);
}

/*********************************************************************
 *                  Configure ADC Sample Time
 *********************************************************************/
void ADC::setSampleTime(ADC_Channel channel,
	ADC_SampleTime sampletime)
{
	uint32_t ch = static_cast<uint32_t>(channel);
	/*
	 * Channels 0-9 -> SMPR2
	 * Channels 10-18 -> SMPR1
	 *
	 * Each channel occupies 3 bits.
	 */
	if (ch <= 9)
	{
		uint32_t shift = ch * 3;
		mADC->SMPR2 &= ~(0x7U << shift);
		mADC->SMPR2 |= (static_cast<uint32_t>(sampletime) << shift);
	}
	else
	{
		uint32_t shift = (ch - 10U) * 3;
		mADC->SMPR1 &= ~(0x7U << shift);
		mADC->SMPR1 |= (static_cast<uint32_t>(sampletime) << shift);
	}
}

/*********************************************************************
 *                  Start ADC Conversion
 *********************************************************************/
void ADC::startConversion()
{
	/*
	 * SWSTART (CR2 Bit30)
	 */
	mADC->CR2 |= (1U << 30);
}
/*********************************************************************
 *                  Conversion Complete
 *********************************************************************/
bool ADC::conversionComplete()
{
	/*
	 * EOC flag
	 * SR Bit1
	 */
	return ((mADC->SR & (1U << 1)) != 0U);
}

/*********************************************************************
 *                  Read ADC Conversion Data
 *********************************************************************/
uint16_t ADC::read()
{
	/*
	 * Wait until conversion completes.
	 */
	while (!conversionComplete());

	/*
	 * Reading DR clears EOC.
	 */
	return static_cast<uint16_t>(mADC->DR);
}

/*********************************************************************
 *                  Stop ADC Conversion
 *********************************************************************/
void ADC::stopConversion()
{
	/*
	 * Clear SWSTART.
	 */
	mADC->CR2 &= ~(1U << 30);
}
