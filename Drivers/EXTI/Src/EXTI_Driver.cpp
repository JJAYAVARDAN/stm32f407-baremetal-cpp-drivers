/*
 * EXTI_Driver.cpp
 *
 * STM32F407 Bare-Metal EXTI Driver
 *
 * Created on: 19-Jul-2026
 * Author: jolap
 *
 * Purpose:
 *   Implementation of the STM32F407 EXTI driver for mapping GPIO inputs to
 *   interrupt lines and managing trigger selection and pending flags.
 *
 * State Machine:
 *   Enable SYSCFG -> map GPIO to EXTI line -> edge detection -> pending bit -> NVIC.
 *
 * RM0090 Reference:
 *   Chapter 12 - External interrupt/event controller (EXTI)
 */
 /*********************************************************************
  *                      EXTI Driver Design
  *
  * The External Interrupt/Event Controller (EXTI) enables the
  * STM32F407 microcontroller to detect external signal transitions
  * on GPIO pins and generate either an interrupt or an event.
  *
  * The EXTI peripheral provides a hardware interface between the
  * GPIO subsystem and the Nested Vectored Interrupt Controller
  * (NVIC). When a configured edge is detected on an input pin,
  * the EXTI controller sets the corresponding pending flag and,
  * if enabled, requests an interrupt from the NVIC.
  *
  * The STM32F407 provides twenty-three EXTI lines:
  *
  *      • EXTI0  - EXTI15  : GPIO External Interrupt Lines
  *      • EXTI16          : PVD Output
  *      • EXTI17          : RTC Alarm
  *      • EXTI18          : USB OTG FS Wakeup
  *      • EXTI19          : Ethernet Wakeup
  *      • EXTI20          : USB OTG HS Wakeup
  *      • EXTI21          : RTC Tamper/TimeStamp
  *      • EXTI22          : RTC Wakeup
  *
  * Every EXTI line can independently generate:
  *
  *      • Interrupt Request
  *      • Event Request
  *
  * Trigger conditions include:
  *
  *      • Rising Edge
  *      • Falling Edge
  *      • Rising and Falling Edge
  *
  * Before a GPIO pin can generate an EXTI interrupt, the
  * corresponding EXTI line must first be connected through the
  * SYSCFG external interrupt configuration registers.
  *
  * The EXTI driver abstracts the complete interrupt configuration
  * sequence, allowing application software to configure interrupt
  * sources without directly manipulating EXTI registers.
  *
  *********************************************************************
  *                  Polling Mode State Machine
  *
  *              Enable SYSCFG Clock
  *                      │
  *                      ▼
  *             Select GPIO Source
  *                      │
  *                      ▼
  *           Map GPIO To EXTI Line
  *                      │
  *                      ▼
  *          Configure Trigger Edge
  *                      │
  *                      ▼
  *          Enable EXTI Line
  *                      │
  *                      ▼
  *        Wait For External Signal
  *                      │
  *                      ▼
  *        Hardware Detects Edge
  *                      │
  *                      ▼
  *        Pending Bit Becomes Set
  *                      │
  *                      ▼
  *        Poll Pending Register
  *                      │
  *                      ▼
  *       Application Handles Event
  *                      │
  *                      ▼
  *       Clear Pending Bit
  *                      │
  *                      ▼
  *          Continue Monitoring
  *
  *********************************************************************
  *                Interrupt Mode State Machine
  *
  *              Enable SYSCFG Clock
  *                      │
  *                      ▼
  *             Select GPIO Source
  *                      │
  *                      ▼
  *           Map GPIO To EXTI Line
  *                      │
  *                      ▼
  *          Configure Trigger Edge
  *                      │
  *                      ▼
  *        Enable EXTI Interrupt
  *                      │
  *                      ▼
  *       Enable NVIC Interrupt
  *                      │
  *                      ▼
  *        Wait For External Signal
  *                      │
  *                      ▼
  *        Hardware Detects Edge
  *                      │
  *                      ▼
  *      EXTI Pending Bit Set
  *                      │
  *                      ▼
  *      NVIC Receives IRQ Request
  *                      │
  *                      ▼
  *       Execute Interrupt Handler
  *                      │
  *                      ▼
  *      Application Processes Event
  *                      │
  *                      ▼
  *       Clear Pending Flag
  *                      │
  *                      ▼
  *      Return From Interrupt
  *
  *********************************************************************
  *                  Driver Responsibilities
  *
  * • Configure EXTI line source.
  * • Connect GPIO pins to EXTI lines.
  * • Configure rising-edge trigger.
  * • Configure falling-edge trigger.
  * • Configure both-edge trigger.
  * • Enable EXTI interrupt mode.
  * • Enable EXTI event mode.
  * • Disable EXTI lines.
  * • Read pending interrupt status.
  * • Clear pending interrupt flags.
  * • Support software-generated interrupts.
  *
  * RM0090 Reference:
  * Chapter 12 - External Interrupt/Event Controller (EXTI)
  *
  *********************************************************************/

#include "EXTI_Driver.h"

EXTI::EXTI(RCC_GPIO port, uint8_t pin)
{
	if (pin > 15)
	{
		return;     // or assert(false)
	}

	mPort = static_cast<GPIO_Port>(port);
	mPin = pin;

	SYSCFGClock();
	ConfigureEXTI();
}
/*********************************************************************
 *                  Enable SYSCFG Clock
 *********************************************************************/

void EXTI::SYSCFGClock()
{
	/* Enable SYSCFG Clock */
	RCC_REG->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // Set bit 14 to enable SYSCFG clock

}

void EXTI::configureEXTILine()
{
	ConfigureEXTI();
}
/*********************************************************************
 *                  Configure EXTI Source
 *********************************************************************/
void EXTI::ConfigureEXTI()
{
	/* Configure the EXTI source for the specified GPIO port and pin */
	uint8_t extiIndex = mPin / 4; // Determine which EXTICR register to use
	uint8_t extiPosition = (mPin % 4) * 4; // Determine the position within the EXTICR register
	// Clear the previous configuration for this pin
	SYSCFG->EXTICR[extiIndex] &= ~(0xF << extiPosition);
	// Set the new configuration for this pin
	SYSCFG->EXTICR[extiIndex] |= (static_cast<uint8_t>(mPort) << extiPosition);
}
/*********************************************************************
 *                  Configure Trigger
 *********************************************************************/
void EXTI::setTrigger(EXTI_Trigger trigger)
{
	switch (trigger)
	{
	case EXTI_Trigger::RISING:
		EXTI_REG->RTSR |= (1 << mPin);  // Enable rising edge trigger
		EXTI_REG->FTSR &= ~(1 << mPin); // Disable falling edge trigger
		break;
	case EXTI_Trigger::FALLING:
		EXTI_REG->FTSR |= (1 << mPin);  // Enable falling edge trigger
		EXTI_REG->RTSR &= ~(1 << mPin); // Disable rising edge trigger
		break;
	case EXTI_Trigger::RISING_FALLING:
		EXTI_REG->RTSR |= (1 << mPin);  // Enable rising edge trigger
		EXTI_REG->FTSR |= (1 << mPin);  // Enable falling edge trigger
		break;
	default:
		break;
	}
}

/*********************************************************************
 *                  Enable Interrupt
 *********************************************************************/
void EXTI::enableInterrupt()
{
	EXTI_REG->IMR |= (1 << mPin); // Enable interrupt for the specified pin
}
/*********************************************************************
 *                  Disable Interrupt
 *********************************************************************/
void EXTI::disableInterrupt()
{
	EXTI_REG->IMR &= ~(1 << mPin); // Disable interrupt for the specified pin
}
/*********************************************************************
 *                  Enable Event
 *********************************************************************/

void EXTI::enableEvent()
{
	EXTI_REG->EMR |= (1 << mPin); // Enable event for the specified pin
}
/*********************************************************************
 *                  Disable Event
 *********************************************************************/
void EXTI::disableEvent()
{
	EXTI_REG->EMR &= ~(1 << mPin); // Disable event for the specified pin
}
/*********************************************************************
 *                  Generate Software Interrupt
 *********************************************************************/
void EXTI::softwareInterrupt()
{
	EXTI_REG->SWIER |= (1 << mPin); // Generate software interrupt for the specified pin
}
/*********************************************************************
 *                  Check Pending Status
 *********************************************************************/
bool EXTI::isPending()
{
	return ((EXTI_REG->PR & (1 << mPin)) != 0); // Check if the pending flag is set for the specified pin
}
/*********************************************************************
 *                  Clear Pending Flag
 *********************************************************************/
void EXTI::clearPending()
{
	EXTI_REG->PR |= (1 << mPin); // Clear the pending flag for the specified pin
}
