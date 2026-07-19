/*
 * SysTick_Driver.cpp
 *
 *  Created on: 19-Jul-2026
 *      Author: jolap
 */

#include "SysTick_Driver.h"

 /*********************************************************************
  *                  Static Tick Counter
  *********************************************************************/
volatile uint32_t SysTick::mTickCount = 0;

/*********************************************************************
 *                  Constructor
 *********************************************************************/
SysTick::SysTick()
{
    mTickFreq = 1000U;     // Default: 1ms tick
}

/*********************************************************************
 *                  Initialize SysTick
 *********************************************************************/
void SysTick::init(uint32_t tickFreq)
{
    mTickFreq = tickFreq;

    /* Disable SysTick */
    stop();

    /* Calculate Reload Value */
    uint32_t reload = (SYSTEM_CORE_CLOCK / mTickFreq) - 1U;

    /* SysTick LOAD register is only 24 bits */
    if (reload > 0xFFFFFFU)
    {
        while (1);
    }

    /* Configure Reload Register */
    SYSTICK->LOAD = reload;

    /* Clear Current Value Register */
    SYSTICK->VAL = 0;

    /* Select Processor Clock */
    SYSTICK->CTRL = SYSTICK_CTRL_CLKSOURCE;

    /* Enable SysTick Interrupt */
    SYSTICK->CTRL |= SYSTICK_CTRL_TICKINT;

    /* Start Counter */
    start();
}

/*********************************************************************
 *                  Start Counter
 *********************************************************************/
void SysTick::start()
{
    SYSTICK->CTRL |= SYSTICK_CTRL_ENABLE;
}

/*********************************************************************
 *                  Stop Counter
 *********************************************************************/
void SysTick::stop()
{
    SYSTICK->CTRL &= ~SYSTICK_CTRL_ENABLE;
}

/*********************************************************************
 *                  Millisecond Delay
 *********************************************************************/
void SysTick::delayMs(uint32_t ms)
{
    uint32_t startTick = getTick();

    while ((getTick() - startTick) < ms)
    {
        __asm volatile ("nop");
    }
}

/*********************************************************************
 *                  Microsecond Delay
 *********************************************************************/
void SysTick::delayUs(uint32_t us)
{
    (void)us;

    /* To be implemented using:
     * 1. DWT Cycle Counter
     * or
     * 2. General Purpose Timer
     */
}

/*********************************************************************
 *                  Get Tick Count
 *********************************************************************/
uint32_t SysTick::getTick()
{
    return mTickCount;
}

/*********************************************************************
 *                  Reset Tick Counter
 *********************************************************************/
void SysTick::resetTick()
{
    mTickCount = 0;
}

/*********************************************************************
 *                  Increment Tick Counter
 *********************************************************************/
void SysTick::incrementTick()
{
    mTickCount++;
}

/*********************************************************************
 *                  SysTick Interrupt Handler
 *********************************************************************/
extern "C" void SysTick_Handler(void)
{
    SysTick::incrementTick();
}