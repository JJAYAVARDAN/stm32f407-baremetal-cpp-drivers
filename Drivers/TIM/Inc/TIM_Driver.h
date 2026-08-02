/*
 * TIM_Driver.h
 *
 * STM32F407 Bare-Metal TIM Driver
 *
 * Created on: 27-Jul-2026
 * Author: jolap
 *
 * Purpose:
 *   Register-level driver for the STM32F407 general-purpose timer peripheral.
 *   The driver exposes APIs for configuration, counting, PWM generation,
 *   capture, and interrupt control.
 *
 * RM0090 Reference:
 *   Chapter 18 - General-purpose timers (TIM)
 */

#ifndef TIM_DRIVER_H_
#define TIM_DRIVER_H_

#include <cstdint>
#include "RCC_Driver.h"

#define _IO volatile

/*********************************************************************
 * Peripheral Base Addresses
 *********************************************************************/
#define TIM2_BASE 0x40000000UL
#define TIM3_BASE 0X40000400UL
#define TIM4_BASE 0X40000800UL
#define TIM5_BASE 0X40000C00UL

/*********************************************************************
 * Register Definitions
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
} TIM_RegDef_t;

/*********************************************************************
 * Register Structure
 *********************************************************************/
#define TIM2_REG ((TIM_RegDef_t *)TIM2_BASE)
#define TIM3_REG ((TIM_RegDef_t *)TIM3_BASE)
#define TIM4_REG ((TIM_RegDef_t *)TIM4_BASE)
#define TIM5_REG ((TIM_RegDef_t *)TIM5_BASE)

/*********************************************************************
 * Enumerations
 *********************************************************************/
enum class TIM_Instance
{
    TIM2,
    TIM3,
    TIM4,
    TIM5
};

enum class TIM_CounterMode
{
    UP = 0,
    DOWN,
    UP_DOWN
};

enum class TIM_Channel
{
    CHANNEL_1 = 0,
    CHANNEL_2,
    CHANNEL_3,
    CHANNEL_4
};

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

enum class TIM_ICPolatity
{
    RISING = 0,
    FALLING,
    BOTH
};

/*********************************************************************
 * Driver Class
 *********************************************************************/
class TIM
{
private:
    TIM_RegDef_t* mTIM;

public:
    /**************************************************************
     * Constructor
     **************************************************************/
    TIM(TIM_Instance instance);

    /**************************************************************
     * Clock APIs
     **************************************************************/
    void enableClock();

    /**************************************************************
     * Initialization APIs
     **************************************************************/
    void init(TIM_CounterMode Mode);

    /**************************************************************
     * Configuration APIs
     **************************************************************/
    void setPrescaler(uint16_t prescaler);
    void setAutoReload(uint32_t arr);
    void setCounter(uint32_t value);
    uint32_t getCounter();

    /**************************************************************
     * Control APIs
     **************************************************************/
    void start();
    void stop();
    void reset();

    /**************************************************************
     * Status APIs
     **************************************************************/
    bool isUpdateEvent();
    void clearUpdateEvent();

    /**************************************************************
     * PWM APIs
     **************************************************************/
    void pwmInit(TIM_Channel channel, TIM_OCMode mode);
    void setDutyCycle(TIM_Channel channel, uint16_t dutyCycle);

    /**************************************************************
     * Output Compare APIs
     **************************************************************/
    void setCompareValue(TIM_Channel channel, uint32_t value);

    /**************************************************************
     * Input Capture APIs
     **************************************************************/
    void inputCaptureInit(TIM_Channel channel, TIM_ICPolatity polarity);
    uint32_t captureValue(TIM_Channel channel);

    /**************************************************************
     * Interrupt APIs
     **************************************************************/
    void enableInterrupt();
    void disableInterrupt();
};

#endif /* TIM_DRIVER_H_ */
