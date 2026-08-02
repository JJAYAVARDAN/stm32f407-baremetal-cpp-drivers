/*
 * SysTick_Driver.cpp
 *
 * STM32F407 Bare-Metal SysTick Driver
 *
 * Created on: 19-Jul-2026
 * Author: jolap
 *
 * Purpose:
 *   Implementation of the ARM Cortex-M4 SysTick timer driver for periodic
 *   tick generation and blocking delays.
 *
 * State Machine:
 *   Configure reload -> clear value -> enable counter -> COUNTFLAG -> interrupt/handler.
 *
 * RM0090 Reference:
 *   ARM Cortex-M4 Generic User Guide - System Timer (SysTick)
 */

 /*********************************************************************
  *                      SysTick Driver Design
  *
  * The SysTick timer is a 24-bit down-counter integrated into the
  * ARM Cortex-M4 processor core. It is primarily designed to generate
  * periodic system interrupts and provide an accurate time base for
  * delays, scheduling, and operating systems.
  *
  * Unlike general-purpose timers, SysTick is a core peripheral and is
  * available on every Cortex-M processor. The timer reloads
  * automatically after reaching zero, allowing continuous periodic
  * timing without software intervention.
  *
  * The SysTick timer consists of:
  *
  *      • Control and Status Register (CTRL)
  *      • Reload Value Register (LOAD)
  *      • Current Value Register (VAL)
  *      • Calibration Register (CALIB)
  *
  * The timer clock source can be selected as either:
  *
  *      • Processor Clock (AHB)
  *      • Processor Clock / 8 (AHB/8)
  *
  * The SysTick driver provides a simple interface for generating
  * blocking delays and periodic timer interrupts while abstracting
  * the underlying register-level operations.
  *
  *********************************************************************
  *                  Polling Mode State Machine
  *
  *             Configure Clock Source
  *                     │
  *                     ▼
  *             Load Reload Value
  *                     │
  *                     ▼
  *            Clear Current Value
  *                     │
  *                     ▼
  *             Enable SysTick
  *                     │
  *                     ▼
  *          Counter Counts Down
  *                     │
  *                     ▼
  *        COUNTFLAG Becomes Set
  *                     │
  *                     ▼
  *        Poll COUNTFLAG Bit
  *                     │
  *                     ▼
  *         Delay Time Elapsed
  *                     │
  *                     ▼
  *           Disable SysTick
  *
  *********************************************************************
  *                Interrupt Mode State Machine
  *
  *             Configure Clock Source
  *                     │
  *                     ▼
  *             Load Reload Value
  *                     │
  *                     ▼
  *            Clear Current Value
  *                     │
  *                     ▼
  *          Enable SysTick Interrupt
  *                     │
  *                     ▼
  *             Enable SysTick
  *                     │
  *                     ▼
  *          Counter Counts Down
  *                     │
  *                     ▼
  *             Counter Reaches Zero
  *                     │
  *                     ▼
  *          SysTick Exception Generated
  *                     │
  *                     ▼
  *          SysTick Handler Executes
  *                     │
  *                     ▼
  *      Application Processes Time Event
  *                     │
  *                     ▼
  *      Counter Reloads Automatically
  *                     │
  *                     ▼
  *          Continue Periodic Timing
  *
  *********************************************************************
  *                  Driver Responsibilities
  *
  * • Configure SysTick clock source.
  * • Configure reload value.
  * • Enable and disable the SysTick timer.
  * • Generate blocking delays.
  * • Generate periodic interrupts.
  * • Read current counter value.
  * • Clear current counter value.
  * • Support millisecond time-base generation.
  *
  * ARM Cortex-M4 Reference:
  * Chapter 4 - System Timer (SysTick)
  *
  *********************************************************************/

#include "SysTick_Driver.h"

 /*********************************************************************
  *                  Static Tick Counter
  *********************************************************************/
volatile uint32_t SysTick::mTickCount = 0;
uint32_t SysTick::mTickFreq = 1000U;

/*********************************************************************
 *                  Initialize SysTick
 *********************************************************************/
void SysTick::init(uint32_t tickFreq)
{
    mTickFreq = tickFreq;

    /* Disable SysTick */
    stop();

    /* Calculate Reload Value from the live AHB clock */
    uint32_t ahb_clock = RCC::getAHBClockFreq();
    uint32_t reload = (ahb_clock / mTickFreq) - 1U;

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
    if (us == 0U)
    {
        return;
    }

    uint32_t start = SYSTICK->VAL;
    uint32_t cycles = (RCC::getAHBClockFreq() / 1000000U) * us;

    while ((start - SYSTICK->VAL) < cycles)
    {
        __asm volatile ("nop");
    }
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