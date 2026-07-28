/*
 * main.cpp
 *
 * STM32F407 Bare Metal C++
 */

#include "Gpio_Driver.h"


int main()
{

    /*
     * Create GPIO object
     *
     * GPIOD Pin 12 = Onboard LED (STM32F407 Discovery)
     *
     * GPIO constructor will:
     * 1. Store GPIO port address
     * 2. Enable GPIOD clock using RCC object
     */
    GPIO led(GPIOD, 12);



    /*
     * Configure LED pin
     */

    led.setMode(GPIO_Mode::OUTPUT);

    led.setOutputType(GPIO_OutputType::PUSH_PULL);

    led.setSpeed(GPIO_Speed::HIGH);

    led.setPull(GPIO_Pull::NO_PULL);



    while(1)
    {

        led.toggle();


        /*
         * Temporary delay
         * Replace with SysTick driver later
         *
        for(volatile uint32_t i = 0; i < 1000000; i++)
        {

        }*/

    }


    return 0;
}
