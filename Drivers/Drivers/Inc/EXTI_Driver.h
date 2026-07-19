/*
 * EXTI_Driver.h
 *
 *  Created on: 19-Jul-2026
 *      Author: jolap
 */

#ifndef EXTI_DRIVER_H_
#define EXTI_DRIVER_H_

#include<cstdint>
#include "RCC_Driver.h"
#include "Gpio_Driver.h"
#define _IO volatile

 /*********************************************************************
  *                      Base Address Definitions
  *********************************************************************/
#define EXTI_BASE 0x40013C00UL
#define SYSCFG_BASE 0x40013800UL

  /*********************************************************************
   *                      RCC APB2ENR Bits
   *********************************************************************/
#define RCC_APB2ENR_SYSCFGEN (1 << 14)
	/*********************************************************************
	 *                      SYSCFG Register Structure
	 *********************************************************************/
typedef struct {
	_IO uint32_t MEMRMP;      /*!< SYSCFG memory remap register, Address offset: 0x00 */
	_IO uint32_t PMC;         /*!< SYSCFG peripheral mode configuration register, Address offset: 0x04 */
	_IO uint32_t EXTICR[4];    /*!< SYSCFG external interrupt configuration registers, Address offset: 0x08-0x14 */
	_IO uint32_t CMPCR;       /*!< SYSCFG compensation cell control register, Address offset: 0x20 */

}SYSCFG_RegDef_t;

/*********************************************************************
 *                      EXTI Register Structure
 *********************************************************************/
typedef struct {
	_IO uint32_t IMR;        /*!< EXTI Interrupt mask register, Address offset: 0x00 */
	_IO uint32_t EMR;        /*!< EXTI Event mask register, Address offset: 0x04 */
	_IO uint32_t RTSR;       /*!< EXTI Rising trigger selection register, Address offset: 0x08 */
	_IO uint32_t FTSR;       /*!< EXTI Falling trigger selection register, Address offset: 0x0C */
	_IO uint32_t SWIER;      /*!< EXTI Software interrupt event register, Address offset: 0x10 */
	_IO uint32_t PR;         /*!< EXTI Pending register, Address offset: 0x14 */
}EXTI_RegDef_t;

/*********************************************************************
 *                      Peripheral Definitions
 *********************************************************************/
#define EXTI_REG ((EXTI_RegDef_t *)EXTI_BASE)
#define SYSCFG ((SYSCFG_RegDef_t *)SYSCFG_BASE)

 /*********************************************************************
  *                      Trigger Selection
  *********************************************************************/
enum class EXTI_Trigger
{
	RISING,
	FALLING,
	RISING_FALLING
};

/*********************************************************************
 *                      EXTI Driver Class
 *********************************************************************/
class EXTI
{
private:
	GPIO_Port mPort;
	uint8_t mPin;

	void ConfigureEXTI();
public:
	/* Constructor */
	EXTI(RCC_GPIO port, uint8_t pin);
	/* Initialize SYSCFG clock for EXTI */
	void SYSCFGClock();
	/* Configure EXTI line mapping for the selected GPIO pin */
	void configureEXTILine();
	/* Method to set the trigger type for the EXTI line */
	void setTrigger(EXTI_Trigger trigger);
	/*Interrupt configuration methods*/
	void enableInterrupt();
	void disableInterrupt();
	/*Event control methods*/
	void enableEvent();
	void disableEvent();
	/*Software interrupt methods*/
	void softwareInterrupt();
	/* Method to clear the pending interrupt flag */
	bool isPending();
	void clearPending();
	void configureTrigger(EXTI_Trigger trigger);

};

#endif /* EXTI_DRIVER_H_ */
