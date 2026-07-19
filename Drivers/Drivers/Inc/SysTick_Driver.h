/*
 * SysTick_Driver.h
 *
 *  Created on: 19-Jul-2026
 *      Author: Jolapuram Jayavardan
 */

#ifndef SYSTICK_DRIVER_H_
#define SYSTICK_DRIVER_H_

#include <cstdint>

#define _IO volatile

 /*********************************************************************
  *                  Cortex-M4 System Clock
  *********************************************************************/
#ifndef SYSTEM_CORE_CLOCK
#define SYSTEM_CORE_CLOCK    16000000UL
#endif

  /*********************************************************************
   *                  SysTick Base Address
   *********************************************************************/
#define SYSTICK_BASE         0xE000E010UL

   /*********************************************************************
    *                  SysTick Register Structure
    *********************************************************************/
struct SysTick_RegDef_t
{
    _IO uint32_t CTRL;      /* Control and Status Register */
    _IO uint32_t LOAD;      /* Reload Value Register */
    _IO uint32_t VAL;       /* Current Value Register */
    _IO uint32_t CALIB;     /* Calibration Register */
};

/*********************************************************************
 *                  SysTick Register Definition
 *********************************************************************/
#define SYSTICK ((SysTick_RegDef_t *)SYSTICK_BASE)

 /*********************************************************************
  *                  CTRL Register Bit Definitions
  *********************************************************************/
#define SYSTICK_CTRL_ENABLE         (1U << 0)
#define SYSTICK_CTRL_TICKINT        (1U << 1)
#define SYSTICK_CTRL_CLKSOURCE      (1U << 2)
#define SYSTICK_CTRL_COUNTFLAG      (1U << 16)

  /*********************************************************************
   *                  SysTick Driver Class
   *********************************************************************/
class SysTick
{
private:

    uint32_t mTickFreq;

    static volatile uint32_t mTickCount;

public:

    /**************** Constructor ****************/
    SysTick();

    /**************** Configuration ****************/
    void init(uint32_t tickFreq);

    /**************** Control ****************/
    void start();
    void stop();

    /**************** Delay ****************/
    void delayMs(uint32_t ms);
    void delayUs(uint32_t us);

    /**************** Tick Management ****************/
    uint32_t getTick();
    void resetTick();

    /**************** Interrupt ****************/
    static void incrementTick();
};

/*********************************************************************
 *                  Interrupt Handler
 *********************************************************************/
extern "C" void SysTick_Handler(void);

#endif /* SYSTICK_DRIVER_H_ */