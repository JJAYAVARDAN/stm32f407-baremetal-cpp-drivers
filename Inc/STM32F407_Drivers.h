/*
 * STM32F407_Drivers.h
 *
 * STM32F407 Bare-Metal C++ Driver Framework
 * Umbrella Header
 *
 * Created on: 29-Jul-2026
 * Author: jolap
 *
 * Purpose:
 *   This is a convenience header file that includes all implemented drivers
 *   for the STM32F407VG bare-metal C++ framework. By including this single
 *   file, an application gains access to the entire driver suite without
 *   needing to include each peripheral driver header individually.
 *
 * Framework Overview:
 *   This driver framework provides a modern C++ bare-metal interface for the
 *   STM32F407VG microcontroller. It is built around direct register access,
 *   object-oriented driver classes, and strongly-typed enums, without any
 *   dependency on STM32 HAL or LL libraries.
 *
 *   The architecture separates core processor peripherals (RCC, NVIC, SysTick)
 *   from on-chip peripherals (GPIO, USART, I2C, etc.), providing a clean and
 *   modular structure.
 *
 * Usage Example:
 *   To use the framework, simply include this header in your main application
 *   file. This is the recommended approach for all new applications.
 *
 *   #include "STM32F407_Drivers.h"
 *
 *   int main()
 *   {
 *       // Enable the clock for GPIOA
 *       RCC::enableGPIOClock(RCC_GPIO::GPIOA);
 *
 *       // Configure a GPIO pin for LED output
 *       GPIO led(GPIO_Port::PORTA, 5);
 *       led.setMode(GPIO_Mode::OUTPUT);
 *       led.setOutputType(GPIO_OutputType::PUSH_PULL);
 *
 *       while(1)
 *       {
 *           led.toggle();
 *           // Add a delay
 *       }
 *
 *       return 0;
 *   }
 *
 */

#ifndef STM32F407_DRIVERS_H_
#define STM32F407_DRIVERS_H_

/* Core Drivers */
#include "RCC_Driver.h"
#include "NVIC_Driver.h"
#include "SysTick_Driver.h"
#include "SCB_Driver.h"

/* Peripheral Drivers */
#include "GPIO_Driver.h"
#include "EXTI_Driver.h"
#include "USART_Driver.h"
#include "SPI_Driver.h"
#include "I2C_Driver.h"
#include "ADC_Driver.h"
#include "TIM_Driver.h"
#include "DMA_Driver.h"

#endif /* STM32F407_DRIVERS_H_ */