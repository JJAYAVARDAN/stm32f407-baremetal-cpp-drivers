/*
 * TIM_Driver.cpp
 *
 * STM32F407 Bare-Metal TIM Driver
 *
 * Created on: 27-Jul-2026
 * Author: jolap
 *
 * Purpose:
 *   Implementation of the STM32F407 timer driver for counter configuration,
 *   PWM generation, input capture, and update-event handling.
 *
 * State Machine:
 *   Disabled -> configure PSC/ARR -> enable counter -> overflow/update -> ISR.
 *
 * RM0090 Reference:
 *   Chapter 18 - General-purpose timers (TIM)
 */

/*********************************************************************
 *                      TIM Driver Design
 *
 * The STM32F407 General-Purpose Timer (TIM) peripheral is a hardware
 * counter capable of generating precise time delays, periodic events,
 * PWM signals, output compare events, and input capture measurements.
 *
 * A timer operates by counting clock pulses supplied from the APB bus.
 * The timer clock is first divided by the Prescaler (PSC), after which
 * the Counter (CNT) increments or decrements until it reaches the
 * Auto-Reload Register (ARR). Depending on the operating mode, the
 * timer generates an update event, resets the counter, or changes the
 * output state.
 *
 * The TIM driver manages the timer configuration and operation while
 * abstracting the underlying register-level programming from the
 * application layer.
 *
 * Polling Mode State Machine
 * --------------------------
 *
 *      Enable TIM Clock
 *              │
 *              ▼
 *      Configure Counter Mode
 *              │
 *              ▼
 *      Configure PSC and ARR
 *              │
 *              ▼
 *      Generate Update Event
 *              │
 *              ▼
 *         Enable Counter
 *              │
 *              ▼
 *      Timer Counts
 *              │
 *              ▼
 *      Update Event (UIF)
 *              │
 *              ▼
 *      Poll UIF Flag
 *              │
 *              ▼
 *      Clear UIF Flag
 *              │
 *              ▼
 *      Continue Timer Operation
 *
 *
 * Interrupt Mode State Machine
 * ----------------------------
 *
 *      Enable TIM Clock
 *              │
 *              ▼
 *      Configure Counter Mode
 *              │
 *              ▼
 *      Configure PSC and ARR
 *              │
 *              ▼
 *      Enable Update Interrupt
 *              │
 *              ▼
 *         Enable Counter
 *              │
 *              ▼
 *      Timer Counts
 *              │
 *              ▼
 *      Update Event (UIF)
 *              │
 *              ▼
 *      NVIC Interrupt Generated
 *              │
 *              ▼
 *      Timer ISR Executes
 *              │
 *              ▼
 *      Clear UIF Flag
 *              │
 *              ▼
 *      Return From Interrupt
 *
 * RM0090 Reference:
 * Chapter 18 - General-Purpose Timers (TIM2–TIM5)
 *
 *********************************************************************/

#include "TIM_Driver.h"


 /*********************************************************************
  *                      TIM Constructor
  *
  * Select TIM peripheral instance
  *
  *********************************************************************/

TIM::TIM(TIM_Instance instance)
{

	switch (instance)
	{

	case TIM_Instance::TIM2:

		mTIM = TIM2_REG;

		break;


	case TIM_Instance::TIM3:

		mTIM = TIM3_REG;

		break;


	case TIM_Instance::TIM4:

		mTIM = TIM4_REG;

		break;


	case TIM_Instance::TIM5:

		mTIM = TIM5_REG;

		break;

	}

}


/*********************************************************************
 *                      Enable TIM Clock
 *
 * TIM2-TIM5 are connected to APB1 Bus
 *
 *********************************************************************/

void TIM::enableClock()
{

	if (mTIM == TIM2_REG)
	{
		RCC::enableTIMClock(RCC_TIM::TIM2);
	}

	else if (mTIM == TIM3_REG)
	{
		RCC::enableTIMClock(RCC_TIM::TIM3);
	}

	else if (mTIM == TIM4_REG)
	{
		RCC::enableTIMClock(RCC_TIM::TIM4);
	}

	else if (mTIM == TIM5_REG)
	{
		RCC::enableTIMClock(RCC_TIM::TIM5);
	}

}



/*********************************************************************
 *                      Timer Initialization
 *
 * Enable clock
 * Configure counter mode
 * Generate update event
 *
 *********************************************************************/

void TIM::init(TIM_CounterMode Mode)
{

	enableClock();


	switch (Mode)
	{

	case TIM_CounterMode::UP:

		mTIM->CR1 &= ~(1 << 4);   // DIR = 0

		break;


	case TIM_CounterMode::DOWN:

		mTIM->CR1 |= (1 << 4);    // DIR = 1

		break;


	case TIM_CounterMode::UP_DOWN:

		mTIM->CR1 |= (1 << 5);    // CMS bits

		break;

	}


	// Generate update event
	mTIM->EGR |= (1 << 0);

}



/*********************************************************************
 *                      Prescaler Configuration
 *
 * PSC register
 *
 *********************************************************************/

void TIM::setPrescaler(uint16_t prescaler)
{

	mTIM->PSC = prescaler;

}



/*********************************************************************
 *                      Auto Reload Configuration
 *
 * ARR register
 *
 *********************************************************************/

void TIM::setAutoReload(uint32_t arr)
{

	mTIM->ARR = arr;

}



/*********************************************************************
 *                      Counter Configuration
 *
 *********************************************************************/

void TIM::setCounter(uint32_t value)
{

	mTIM->CNT = value;

}



uint32_t TIM::getCounter()
{

	return mTIM->CNT;

}



/*********************************************************************
 *                      Timer Start
 *
 * CR1 CEN bit
 *
 *********************************************************************/

void TIM::start()
{

	mTIM->CR1 |= (1 << 0);

}



/*********************************************************************
 *                      Timer Stop
 *
 *********************************************************************/

void TIM::stop()
{

	mTIM->CR1 &= ~(1 << 0);

}



/*********************************************************************
 *                      Timer Reset
 *
 *********************************************************************/

void TIM::reset()
{

	mTIM->CNT = 0;

	mTIM->EGR |= (1 << 0);

}



/*********************************************************************
 *                      Update Event Check
 *
 * SR UIF bit
 *
 *********************************************************************/

bool TIM::isUpdateEvent()
{

	return (mTIM->SR & (1 << 0));

}



void TIM::clearUpdateEvent()
{

	mTIM->SR &= ~(1 << 0);

}



/*********************************************************************
 *                      PWM Initialization
 *
 * Supports PWM Mode 1/2
 *
 *********************************************************************/

void TIM::pwmInit(TIM_Channel channel, TIM_OCMode mode)
{

	switch (channel)
	{

	case TIM_Channel::CHANNEL_1:

		mTIM->CCMR1 &= ~(0xFF);

		mTIM->CCMR1 |=
			(static_cast<uint32_t>(mode) << 4);

		mTIM->CCER |= (1 << 0);

		break;



	case TIM_Channel::CHANNEL_2:

		mTIM->CCMR1 &= ~(0xFF << 8);

		mTIM->CCMR1 |=
			(static_cast<uint32_t>(mode) << 12);

		mTIM->CCER |= (1 << 4);

		break;



	case TIM_Channel::CHANNEL_3:

		mTIM->CCMR2 &= ~(0xFF);

		mTIM->CCMR2 |=
			(static_cast<uint32_t>(mode) << 4);

		mTIM->CCER |= (1 << 8);

		break;



	case TIM_Channel::CHANNEL_4:

		mTIM->CCMR2 &= ~(0xFF << 8);

		mTIM->CCMR2 |=
			(static_cast<uint32_t>(mode) << 12);

		mTIM->CCER |= (1 << 12);

		break;

	}

}



/*********************************************************************
 *                      Duty Cycle Configuration
 *
 * CCRx register
 *
 *********************************************************************/

void TIM::setDutyCycle(TIM_Channel channel, uint16_t dutyCycle)
{
    uint32_t arr = (mTIM->ARR != 0U) ? mTIM->ARR : 0xFFFFU;
    uint32_t value = dutyCycle;

    if (value > arr)
    {
        value = arr;
    }

    switch (channel)
    {
    case TIM_Channel::CHANNEL_1:
        mTIM->CCR1 = static_cast<uint32_t>(value);
        break;

    case TIM_Channel::CHANNEL_2:
        mTIM->CCR2 = static_cast<uint32_t>(value);
        break;

    case TIM_Channel::CHANNEL_3:
        mTIM->CCR3 = static_cast<uint32_t>(value);
        break;

    case TIM_Channel::CHANNEL_4:
        mTIM->CCR4 = static_cast<uint32_t>(value);
        break;
    }
}



/*********************************************************************
 *                      Output Compare
 *
 *********************************************************************/

void TIM::setCompareValue(TIM_Channel channel, uint32_t value)
{

	setDutyCycle(channel, value);

}



/*********************************************************************
 *                      Input Capture Initialization
 *
 *********************************************************************/

void TIM::inputCaptureInit(TIM_Channel channel,
	TIM_ICPolatity polarity)
{

	switch (channel)
	{

	case TIM_Channel::CHANNEL_1:
		mTIM->CCMR1 &= ~(0x3U << 0);
		mTIM->CCMR1 |= (0x1U << 0);

		if (polarity == TIM_ICPolatity::FALLING)
			mTIM->CCER |= (1U << 1);
		else
			mTIM->CCER &= ~(1U << 1);

		mTIM->CCER |= (1U << 0);

		break;

	case TIM_Channel::CHANNEL_2:
		mTIM->CCMR1 &= ~(0x3U << 8);
		mTIM->CCMR1 |= (0x1U << 8);

		if (polarity == TIM_ICPolatity::FALLING)
			mTIM->CCER |= (1U << 5);
		else
			mTIM->CCER &= ~(1U << 5);

		mTIM->CCER |= (1U << 4);

		break;


	default:

		break;

	}

}



/*********************************************************************
 *                      Capture Value
 *
 *********************************************************************/

uint32_t TIM::captureValue(TIM_Channel channel)
{

	switch (channel)
	{

	case TIM_Channel::CHANNEL_1:
		return mTIM->CCR1;


	case TIM_Channel::CHANNEL_2:
		return mTIM->CCR2;


	case TIM_Channel::CHANNEL_3:
		return mTIM->CCR3;


	case TIM_Channel::CHANNEL_4:
		return mTIM->CCR4;

	}


	return 0;

}



/*********************************************************************
 *                      Interrupt Control
 *
 * DIER UIE bit
 *
 *********************************************************************/

void TIM::enableInterrupt()
{

	mTIM->DIER |= (1 << 0);

}



void TIM::disableInterrupt()
{

	mTIM->DIER &= ~(1 << 0);

}