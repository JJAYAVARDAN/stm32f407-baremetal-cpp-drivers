/*
 * Gpio_driver.cpp
 *
 * STM32F407 Bare-Metal GPIO Driver
 *
 * Created on: 18-Jul-2026
 * Author: jolap
 *
 * Purpose:
 *   Implementation of the STM32F407 GPIO driver for configuring pin mode,
 *   output type, speed, pull settings, and digital I/O operations.
 *
 * State Machine:
 *   Enable clock -> configure mode -> configure output type/speed/pull -> pin ready.
 *
 * RM0090 Reference:
 *   Chapter 8 - General-purpose I/Os (GPIO)
 */

#include "GPIO_Driver.h"

/*********************************************************************
 *                      GPIO Driver Design
 *
 * The General Purpose Input/Output (GPIO) peripheral provides a
 * programmable digital interface between the STM32F407
 * microcontroller and external hardware devices. Each GPIO pin can
 * be independently configured to operate as an input, output,
 * alternate function, or analog pin.
 *
 * The STM32F407 has nine GPIO ports (GPIOA to GPIOI), each with 16
 * pins. Before using any GPIO pin, its corresponding port clock must
 * be enabled via the RCC peripheral.
 *
 *********************************************************************
 *                      GPIO Initialization Flow
 *
 *                 Enable GPIO Port Clock (via RCC)
 *                             │
 *                             ▼
 *      Configure Pin Mode (Input, Output, AF, Analog)
 *                       (GPIOx_MODER)
 *                             │
 *                             ▼
 *      Configure Output Type (Push-Pull / Open-Drain)
 *                         (GPIOx_OTYPER)
 *                             │
 *                             ▼
 *      Configure Output Speed (Low, Medium, High, V.High)
 *                        (GPIOx_OSPEEDR)
 *                             │
 *                             ▼
 *      Configure Pull-up/Pull-down Resistors
 *                         (GPIOx_PUPDR)
 *                             │
 *                             ▼
 *      Configure Alternate Function (if AF mode is selected)
 *                    (GPIOx_AFRL / GPIOx_AFRH)
 *                             │
 *                             ▼
 *                       GPIO Pin Ready
 *
 *********************************************************************
 *
 * Driver Responsibilities
 * -----------------------
 * • Enable GPIO peripheral clocks.
 * • Configure pin mode, speed, output type, and pull configuration.
 * • Set and read the digital state of pins.
 * • Configure alternate function mappings for peripherals like USART, SPI, I2C.
 *
 * RM0090 Reference:
 * Chapter 8 - General-Purpose I/Os (GPIO)
 *
 *********************************************************************/

/*********************************************************************
 *                      Constructor
 *
 * Initializes a GPIO object by mapping the port and pin number to the
 * corresponding memory-mapped register address.
 *
 * @param port The GPIO port (e.g., GPIO_Port::PORTA).
 * @param pin The pin number (0-15).
 *********************************************************************/
GPIO::GPIO(GPIO_Port port, uint8_t pin) : mPortId(port), mPin(pin)
{
    switch (port)
    {
        case GPIO_Port::PORTA: mPort = GPIOA; break;
        case GPIO_Port::PORTB: mPort = GPIOB; break;
        case GPIO_Port::PORTC: mPort = GPIOC; break;
        case GPIO_Port::PORTD: mPort = GPIOD; break;
        case GPIO_Port::PORTE: mPort = GPIOE; break;
        case GPIO_Port::PORTF: mPort = GPIOF; break;
        case GPIO_Port::PORTG: mPort = GPIOG; break;
        case GPIO_Port::PORTH: mPort = GPIOH; break;
        case GPIO_Port::PORTI: mPort = GPIOI; break;
    }
}

/*********************************************************************
 *                      Enable Clock
 *
 * Enables the clock for the GPIO port associated with this object.
 * This must be called before any other configuration.
 *
 * RM0090 Reference: RCC_AHB1ENR register.
 *********************************************************************/
void GPIO::clockEnable()
{
    RCC::enableGPIOClock(static_cast<RCC_GPIO>(mPortId));
}

/*********************************************************************
 *                      Set GPIO Mode
 *
 * Configures the operating mode of the pin.
 *
 * @param mode The desired mode (INPUT, OUTPUT, AF, ANALOG).
 *
 * RM0090 Reference: GPIOx_MODER register.
 *********************************************************************/
void GPIO::setMode(GPIO_Mode mode)
{
    mPort->MODER &= ~(3U << (mPin * 2));
    mPort->MODER |= (static_cast<uint32_t>(mode) << (mPin * 2));
}

/*********************************************************************
 *                      Set Output Type
 *
 * Configures the output type of the pin (Push-Pull or Open-Drain).
 * This is only applicable for output and alternate function modes.
 *
 * @param type The desired output type.
 *
 * RM0090 Reference: GPIOx_OTYPER register.
 *********************************************************************/
void GPIO::setOutputType(GPIO_OutputType type)
{
    mPort->OTYPER &= ~(1U << mPin);
    mPort->OTYPER |= (static_cast<uint32_t>(type) << mPin);
}

/*********************************************************************
 *                      Set Output Speed
 *
 * Configures the output speed of the pin.
 * This is only applicable for output and alternate function modes.
 *
 * @param speed The desired output speed.
 *
 * RM0090 Reference: GPIOx_OSPEEDR register.
 *********************************************************************/
void GPIO::setSpeed(GPIO_Speed speed)
{
    mPort->OSPEEDR &= ~(3U << (mPin * 2));
    mPort->OSPEEDR |= (static_cast<uint32_t>(speed) << (mPin * 2));
}

/*********************************************************************
 *                      Set Pull Configuration
 *
 * Configures the internal pull-up or pull-down resistor for the pin.
 *
 * @param pull The desired pull configuration.
 *
 * RM0090 Reference: GPIOx_PUPDR register.
 *********************************************************************/
void GPIO::setPull(GPIO_Pull pull)
{
    mPort->PUPDR &= ~(3U << (mPin * 2));
    mPort->PUPDR |= (static_cast<uint32_t>(pull) << (mPin * 2));
}

/*********************************************************************
 *                      Write to Pin
 *
 * Sets or clears the output data register for the pin.
 *
 * @param state The desired state (HIGH or LOW).
 *
 * RM0090 Reference: GPIOx_BSRR register.
 *********************************************************************/
void GPIO::write(GPIO_State state)
{
    if (state == GPIO_State::HIGH)
    {
        mPort->BSRR = (1U << mPin);
    }
    else
    {
        mPort->BSRR = (1U << (mPin + 16));
    }
}

/*********************************************************************
 *                      Read from Pin
 *
 * Reads the state of the input data register for the pin.
 *
 * @return The current state of the pin (HIGH or LOW).
 *
 * RM0090 Reference: GPIOx_IDR register.
 *********************************************************************/
GPIO_State GPIO::read()
{
    return (mPort->IDR & (1U << mPin)) ? GPIO_State::HIGH : GPIO_State::LOW;
}

/*********************************************************************
 *                      Toggle Pin
 *
 * Toggles the state of the output data register for the pin.
 *
 * RM0090 Reference: GPIOx_ODR register.
 *********************************************************************/
void GPIO::toggle()
{
    mPort->ODR ^= (1U << mPin);
}

/*********************************************************************
 *                  Configure Alternate Function
 *
 * Configures the alternate function mapping for the pin.
 *
 * @param af The alternate function number (0-15).
 *
 * RM0090 Reference: GPIOx_AFRL and GPIOx_AFRH registers.
 *********************************************************************/
void GPIO::setAlternateFunction(uint8_t af)
{
    if (mPin < 8)
    {
        mPort->AFRL &= ~(0xFU << (mPin * 4));
        mPort->AFRL |= ((uint32_t)af << (mPin * 4));
    }
    else
    {
        mPort->AFRH &= ~(0xFU << ((mPin - 8) * 4));
        mPort->AFRH |= ((uint32_t)af << ((mPin - 8) * 4));
    }
}

/*********************************************************************
 *                      Lock GPIO Configuration
 *
 * Locks the configuration of the specified pin until the next reset.
 *
 * RM0090 Reference: GPIOx_LCKR register.
 *********************************************************************/
void GPIO::lock()
{
    uint32_t temp = (1U << 16) | (1U << mPin);
    mPort->LCKR = temp; // Write sequence: LCKK=1, LCKx=1
    mPort->LCKR = (1U << mPin); // Write sequence: LCKK=0, LCKx=1
    mPort->LCKR = temp; // Write sequence: LCKK=1, LCKx=1
    (void)mPort->LCKR; // Read LCKR
    (void)mPort->LCKR; // Read LCKR again to confirm lock
}

/*********************************************************************
 *                      Write to Entire Port
 *
 * Writes a 16-bit value to the entire port's output data register.
 *
 * @param value The 16-bit value to write to the port.
 *
 * RM0090 Reference: GPIOx_ODR register.
 *********************************************************************/
void GPIO::writePort(uint16_t value)
{
    mPort->ODR = value;
}

/*********************************************************************
 *                      Read from Entire Port
 *
 * Reads the 16-bit value from the entire port's input data register.
 *
 * @return The 16-bit value of the port's input.
 *
 * RM0090 Reference: GPIOx_IDR register.
 *********************************************************************/
uint16_t GPIO::readPort()
{
    return (uint16_t)mPort->IDR;
}

/*********************************************************************
 *                      Reset GPIO Registers
 *
 * Resets the GPIO port registers to their default values.
 *********************************************************************/
void GPIO::deInit()
{
    // This is a simplified de-init. A full de-init would involve
    // using the RCC peripheral to reset the port.
    mPort->MODER   = 0x00000000;
    mPort->OTYPER  = 0x00000000;
    mPort->OSPEEDR = 0x00000000;
    mPort->PUPDR   = 0x00000000;
    mPort->ODR     = 0x00000000;
    mPort->AFRL    = 0x00000000;
    mPort->AFRH    = 0x00000000;
}
