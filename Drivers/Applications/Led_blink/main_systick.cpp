/*
 * main_systick.cpp
 *
 *  Created on: 19-Jul-2026
 *      Author: jolap
 */
#include "Gpio_Driver.h"
#include "SysTick_Driver.h"

int main()
{
	SysTick systick;
	systick.init(1000);

	/*configure port*/
	GPIO led(GPIOD, 12);
	led.setMode(GPIO_Mode::OUTPUT);
	led.setOutputType(GPIO_OutputType::PUSH_PULL);
	led.setSpeed(GPIO_Speed::HIGH);
	while (1)
	{
		led.toggle();
		systick.delayMs(500);
	}
}

