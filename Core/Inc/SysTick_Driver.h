/*
 * SysTick_Driver.h
 *
 * STM32F407 Bare-Metal SysTick Driver
 *
 * Created on: 19-Jul-2026
 * Author: jolap
 *
 * Purpose:
 *   Register-level driver for the ARM Cortex-M4 SysTick timer. The driver
 *   provides a simple interface for generating periodic time bases and
 *   blocking delays without HAL dependencies.
 *
 * RM0090 Reference:
 *   ARM Cortex-M4 Generic User Guide - System Timer (SysTick)
 */

#ifndef SYSTICK_DRIVER_H_
#define SYSTICK_DRIVER_H_

#include <cstdint>
#include "RCC_Driver.h"

/*********************************************************************
 * Peripheral Base Address
 *********************************************************************/
#define SYSTICK_BASE 0xE000E010UL

/*********************************************************************
 * Register Definitions
 *********************************************************************/
struct SysTick_RegDef_t
{
    _IO uint32_t CTRL;      /*!< Control and status register, Address offset: 0x00 */
    _IO uint32_t LOAD;      /*!< Reload value register,      Address offset: 0x04 */
    _IO uint32_t VAL;       /*!< Current value register,     Address offset: 0x08 */
    _IO uint32_t CALIB;     /*!< Calibration register,      Address offset: 0x0C */
};

/*********************************************************************
 * Register Structure
 *********************************************************************/
#define SYSTICK ((SysTick_RegDef_t *)SYSTICK_BASE)

/*********************************************************************
 * Peripheral Definitions
 *********************************************************************/
#define SYSTICK_CTRL_ENABLE    (1U << 0)
#define SYSTICK_CTRL_TICKINT   (1U << 1)
#define SYSTICK_CTRL_CLKSOURCE (1U << 2)
#define SYSTICK_CTRL_COUNTFLAG (1U << 16)

/*********************************************************************
 * Driver Class
 *********************************************************************/
class SysTick
{
private:
    static uint32_t mTickFreq;
    static volatile uint32_t mTickCount;

public:
    /**************************************************************
     * Initialization APIs
     **************************************************************/
    /**
     * @brief Initializes SysTick for periodic interrupts and tick counting.
     * @param tickFreq Requested tick frequency in hertz.
     */
    static void init(uint32_t tickFreq);

    /**************************************************************
     * Control APIs
     **************************************************************/
    /**
     * @brief Starts the SysTick counter.
     */
    static void start();

    /**
     * @brief Stops the SysTick counter.
     */
    static void stop();

    /**************************************************************
     * Delay APIs
     **************************************************************/
    /**
     * @brief Blocks execution for the requested number of milliseconds.
     * @param ms Delay duration in milliseconds.
     */
    static void delayMs(uint32_t ms);

    /**
     * @brief Blocks execution for the requested number of microseconds.
     * @param us Delay duration in microseconds.
     */
    static void delayUs(uint32_t us);

    /**************************************************************
     * Status APIs
     **************************************************************/
    /**
     * @brief Returns the current software tick count.
     * @return Current tick count value.
     */
    static uint32_t getTick();

    /**
     * @brief Resets the software tick counter.
     */
    static void resetTick();

    /**************************************************************
     * Interrupt APIs
     **************************************************************/
    /**
     * @brief Increments the software tick counter from the interrupt handler.
     */
    static void incrementTick();
};

/*********************************************************************
 * Interrupt Handler
 *********************************************************************/
extern "C" void SysTick_Handler(void);

#endif /* SYSTICK_DRIVER_H_ */