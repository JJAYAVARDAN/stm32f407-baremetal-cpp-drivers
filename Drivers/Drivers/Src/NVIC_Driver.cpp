/*
 * NVIC_Driver.cpp
 *
 *  Created on: 19-Jul-2026
 *      Author: jolap
 */

#include "NVIC_Driver.h"

 /*********************************************************************
  *                  Enable IRQ
  *********************************************************************/
void NVIC::enableIRQ(uint8_t irq)
{
	if (irq < 240) // Check if the IRQ number is valid
	{
		NVIC_REG->ISER[irq / 32] = (1 << (irq % 32)); // Set the corresponding bit in ISER
	}
}
/*********************************************************************
 *                  Disable IRQ
 *********************************************************************/
void NVIC::disableIRQ(uint8_t irq)
{
	if (irq < 240) // Check if the IRQ number is valid
	{
		NVIC_REG->ICER[irq / 32] = (1 << (irq % 32)); // Clear the corresponding bit in ICER
	}
}
/*********************************************************************
 *                  Set Pending IRQ
 *********************************************************************/
void NVIC::setPendingIRQ(uint8_t irq) {
	if (irq < 240) // Check if the IRQ number is valid
	{
		NVIC_REG->ISPR[irq / 32] = (1 << (irq % 32)); // Set the corresponding bit in ISPR
	}
}
/*********************************************************************
 *                  Clear Pending IRQ
 *********************************************************************/
void NVIC::clearPendingIRQ(uint8_t irq) {
	if (irq < 240) // Check if the IRQ number is valid
	{
		NVIC_REG->ICPR[irq / 32] = (1 << (irq % 32)); // Clear the corresponding bit in ICPR
	}
}
/*********************************************************************
 *                  Get Active IRQ
 *********************************************************************/	
bool NVIC::getActiveIRQ(uint8_t irq) {
	if (irq < 240) // Check if the IRQ number is valid
	{
		return (NVIC_REG->IABR[irq / 32] & (1 << (irq % 32))) != 0; // Return the status of the corresponding bit in IABR
	}
	return false; // Return false if the IRQ number is invalid
}
/*********************************************************************
 *                  Set IRQ Priority
 *********************************************************************/
void NVIC::setPriority(uint8_t irq, uint8_t priority) {
	if (irq < 240) // Check if the IRQ number is valid
	{
		NVIC_REG->IPR[irq] = priority; // Set the priority for the specified IRQ
	}
}

