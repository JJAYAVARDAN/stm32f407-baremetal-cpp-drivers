/*
 * ADC_Driver.cpp
 *
 *  Created on: 21-Jul-2026
 *      Author: jolap
 */

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
		mADC->SMPR2 &= ~(0x7U << shift);
		mADC->SMPR2 |= (static_cast<uint32_t>(sampletime) << shift);
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
