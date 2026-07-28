/*
 * TIM_Driver.h
 *
 *  Created on: 27-Jul-2026
 *      Author: jolap
 */

#ifndef TIM_DRIVER_H_
#define TIM_DRIVER_H_

#include <cstdint>
#include "RCC_Driver.h"

#define _IO volatile

 /*********************************************************************
  *                      TIM Peripheral Base Addresses
  *
  * STM32F407 Reference Manual (RM0090)
  *
  * TIM2 : 0x40000000
  * TIM3 : 0x40000400
  * TIM4 : 0x40000800
  * TIM5 : 0x40000C00
  *********************************************************************/

#define TIM2_BASE 0x40000000UL
#define TIM3_BASE 0X40000400UL
#define TIM4_BASE 0X40000800UL
#define TIM5_BASE 0X40000C00UL

  /*********************************************************************
   *                      TIM Register Definition
   *
   * Offset
   *
   * 0x00 CR1
   * 0x04 CR2
   * 0x08 SMCR
   * 0x0C DIER
   * 0x10 SR
   * 0x14 EGR
   * 0x18 CCMR1
   * 0x1C CCMR2
   * 0x20 CCER
   * 0x24 CNT
   * 0x28 PSC
   * 0x2C ARR
   * 0x30 Reserved
   * 0x34 CCR1
   * 0x38 CCR2
   * 0x3C CCR3
   * 0x40 CCR4
   * 0x44 Reserved
   * 0x48 DCR
   * 0x4C DMAR
   *
   *********************************************************************/

typedef struct
{
	_IO uint32_t CR1;
	_IO uint32_t CR2;
	_IO uint32_t SMCR;
	_IO uint32_t DIER;
	_IO uint32_t SR;
	_IO uint32_t EGR;
	_IO uint32_t CCMR1;
	_IO uint32_t CCMR2;
	_IO uint32_t CCER;
	_IO uint32_t CNT;
	_IO uint32_t PSC;
	_IO uint32_t ARR;
	_IO uint32_t Reserved1;
	_IO uint32_t CCR1;
	_IO uint32_t CCR2;
	_IO uint32_t CCR3;
	_IO uint32_t CCR4;
	_IO uint32_t Reserved2;
	_IO uint32_t DCR;
	_IO uint32_t DMAR;
}TIM_RegDef_t;

/*********************************************************************
 *                      TIM Peripheral Definitions
 *********************************************************************/
#define TIM2_REG   ((TIM_RegDef_t*)TIM2_BASE)
#define TIM3_REG   ((TIM_RegDef_t*)TIM3_BASE)
#define TIM4_REG   ((TIM_RegDef_t*)TIM4_BASE)
#define TIM5_REG   ((TIM_RegDef_t*)TIM5_BASE)

 /*********************************************************************
  *                      TIM Instance
  *********************************************************************/
enum class TIM_Instance
{
	TIM2,
	TIM3,
	TIM4,
	TIM5
};
/*********************************************************************
 *                      Counter Mode
 *
 * CR1 DIR/CMS
 *********************************************************************/
enum class TIM_CounterMode
{
	UP = 0,
	DOWN,
	UP_DOWN
};

/*********************************************************************
 *                      Timer Channel
 *********************************************************************/
enum class TIM_Channel
{
	CHANNEL_1 = 0,
	CHANNEL_2,
	CHANNEL_3,
	CHANNEL_4	
};
/*********************************************************************
 *                      Output Compare Mode
 *********************************************************************/
enum class TIM_OCMode
{
	FROGEN = 0,
	ACTIVE,
	INACTIVE,
	TOGGLE,
	FORCE_LOW,
	FORCE_HIGH,
	PWM_MODE_1,
	PWM_MODE_2
};

/*********************************************************************
 *                      Input Capture Edge
 *********************************************************************/

enum class TIM_ICPolatity
{
	RISING = 0,
	FALLING,
	BOTH
};

/*********************************************************************
 *                      Timer Driver Class
 *********************************************************************/
class TIM
{
private:
	/*
	 * Pointer to TIM peripheral registers
	 */
	TIM_RegDef_t* mTIM;
public:
	/**************************************************************
	 * Constructor
	 **************************************************************/
	TIM(TIM_Instance instance);

	/**************************************************************
	 * Clock Control
	 **************************************************************/
	void enableClock();

	/**************************************************************
	 * Timer Initialization
	 **************************************************************/

	void init(TIM_CounterMode Mode);

	/**************************************************************
	 * Timer Configuration
	 **************************************************************/
	void setPrescaler(uint16_t prescaler);

	void setAutoReload(uint32_t arr);

	void setCounter(uint32_t value);
	uint32_t getCounter();

	/**************************************************************
	 * Timer Control
	 **************************************************************/

	void start();

	void stop();
	void reset();

	/**************************************************************
	 * Update Event
	 **************************************************************/

	bool isUpdateEvent();

	void clearUpdateEvent();

	/**************************************************************
	 * PWM
	 **************************************************************/

	void pwmInit(TIM_Channel channel, TIM_OCMode mode);

	void setDutyCycle(TIM_Channel channel, uint16_t dutyCycle);

	/**************************************************************
	 * Output Compare
	 **************************************************************/

	void setCompareValue(TIM_Channel channel, uint32_t value);

	/**************************************************************
	 * Input Capture
	 **************************************************************/
	void inputCaptureInit(TIM_Channel channel, TIM_ICPolatity polarity);

	uint32_t captureValue(TIM_Channel channel);

	/**************************************************************
	 * Interrupt Control
	 **************************************************************/

	void enableInterrupt();

	void disableInterrupt();

};


#endif /* TIM_DRIVER_H_ */
