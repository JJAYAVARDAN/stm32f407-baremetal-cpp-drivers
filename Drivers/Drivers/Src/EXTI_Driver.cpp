/*
 * EXTI_Driver.cpp
 *
 *  Created on: 19-Jul-2026
 *      Author: jolap
 */

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
	RCCx->RCC_APB2ENR |= RCC_APB2ENR_SYSCFGEN; // Set bit 14 to enable SYSCFG clock

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
void EXTI::configureTrigger(EXTI_Trigger trigger)
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