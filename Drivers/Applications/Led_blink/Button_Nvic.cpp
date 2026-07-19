/*
 * Button_NVIC.cpp
 *
 *  Created on: 19-Jul-2026
 *      Author: jolap
 */

#include "Gpio_Driver.h"
#include "EXTI_Driver.h"
#include "NVIC_Driver.h"
#include "SysTick_Driver.h"

/*********************************************************************
 *                  Global Objects
 *********************************************************************/
GPIO led(GPIOD, 12);
GPIO button(GPIOA, 0);

EXTI exti(RCC_GPIO::PORTA, 0);

/*********************************************************************
 *                  Main
 *********************************************************************/
int main(void)
{
    /**************** GPIO Configuration ****************/

    led.setMode(GPIO_Mode::OUTPUT);
    led.setOutputType(GPIO_OutputType::PUSH_PULL);
    led.setSpeed(GPIO_Speed::HIGH);
    led.setPull(GPIO_Pull::NO_PULL);

    button.setMode(GPIO_Mode::INPUT);
    button.setPull(GPIO_Pull::PULL_DOWN);

    /**************** EXTI Configuration ****************/

    exti.SYSCFGClock();

    exti.configureEXTILine();

    exti.configureTrigger(EXTI_Trigger::RISING);

    exti.enableInterrupt();

    /**************** NVIC Configuration ****************/

    NVIC::setPriority(6, 2);      // EXTI0_IRQn = 6

    NVIC::enableIRQ(6);

    /**************** SysTick ****************/

    SysTick systick;
    systick.init(1000);

    while (1)
    {
        /* Interrupt Driven */

        systick.delayMs(100);
    }
}

/*********************************************************************
 *                  EXTI0 Interrupt Handler
 *********************************************************************/
extern "C" void EXTI0_IRQHandler(void)
{
    if (exti.isPending())
    {
        led.toggle();

        exti.clearPending();
    }
}
