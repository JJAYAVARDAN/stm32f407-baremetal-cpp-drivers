/*
 * Gpio_Driver.h
 *
 * STM32F407 Bare-Metal GPIO Driver
 *
 * Created on: 18-Jul-2026
 * Author: jolap
 *
 * Purpose:
 *   Register-level driver for the STM32F407 General-Purpose Input/Output
 *   peripheral. The driver exposes a compact C++ interface for configuring
 *   individual pins and reading or writing their digital state.
 *
 * RM0090 Reference:
 *   Chapter 8 - General-purpose I/Os (GPIO)
 */

#ifndef GPIO_DRIVER_H_
#define GPIO_DRIVER_H_

#include <cstdint>
#include "RCC_Driver.h"

#define _IO volatile

/*********************************************************************
 * Peripheral Base Addresses
 *********************************************************************/
#define GPIOA_BASE 0x40020000UL
#define GPIOB_BASE 0x40020400UL
#define GPIOC_BASE 0x40020800UL
#define GPIOD_BASE 0x40020C00UL
#define GPIOE_BASE 0x40021000UL
#define GPIOF_BASE 0x40021400UL
#define GPIOG_BASE 0x40021800UL
#define GPIOH_BASE 0x40021C00UL
#define GPIOI_BASE 0x40022000UL

/*********************************************************************
 * Register Definitions
 *********************************************************************/
typedef struct
{
    _IO uint32_t MODER;   /*!< GPIO port mode register,               Address offset: 0x00 */
    _IO uint32_t OTYPER;  /*!< GPIO port output type register,        Address offset: 0x04 */
    _IO uint32_t OSPEEDR; /*!< GPIO port output speed register,       Address offset: 0x08 */
    _IO uint32_t PUPDR;   /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C */
    _IO uint32_t IDR;     /*!< GPIO port input data register,         Address offset: 0x10 */
    _IO uint32_t ODR;     /*!< GPIO port output data register,        Address offset: 0x14 */
    _IO uint32_t BSRR;    /*!< GPIO port bit set/reset register,      Address offset: 0x18 */
    _IO uint32_t LCKR;    /*!< GPIO port configuration lock register, Address offset: 0x1C */
    _IO uint32_t AFRL;    /*!< GPIO alternate function low register,  Address offset: 0x20 */
    _IO uint32_t AFRH;    /*!< GPIO alternate function high register, Address offset: 0x24 */
} GPIO_RegDef_t;

/*********************************************************************
 * Register Structure
 *********************************************************************/
#define GPIOA ((GPIO_RegDef_t *)GPIOA_BASE)
#define GPIOB ((GPIO_RegDef_t *)GPIOB_BASE)
#define GPIOC ((GPIO_RegDef_t *)GPIOC_BASE)
#define GPIOD ((GPIO_RegDef_t *)GPIOD_BASE)
#define GPIOE ((GPIO_RegDef_t *)GPIOE_BASE)
#define GPIOF ((GPIO_RegDef_t *)GPIOF_BASE)
#define GPIOG ((GPIO_RegDef_t *)GPIOG_BASE)
#define GPIOH ((GPIO_RegDef_t *)GPIOH_BASE)
#define GPIOI ((GPIO_RegDef_t *)GPIOI_BASE)

/*********************************************************************
 * Enumerations
 *********************************************************************/
enum class GPIO_Port { PORTA=0, PORTB, PORTC, PORTD, PORTE, PORTF, PORTG, PORTH, PORTI };
enum class GPIO_Mode { INPUT = 0, OUTPUT, ALTERNATE_FUNCTION, ANALOG };
enum class GPIO_OutputType { PUSH_PULL = 0, OPEN_DRAIN };
enum class GPIO_Speed { LOW = 0, MEDIUM, HIGH, VERY_HIGH };
enum class GPIO_Pull { NO_PULL = 0, PULL_UP, PULL_DOWN };
enum class GPIO_State { LOW = 0, HIGH };

/*********************************************************************
 * Driver Class
 *********************************************************************/
class GPIO
{
private:
    GPIO_Port mPortId;
    uint8_t mPin;
    GPIO_RegDef_t *mPort;

public:
    /**************************************************************
     * Constructor
     **************************************************************/
    GPIO(GPIO_Port port, uint8_t pin);

    /**************************************************************
     * Clock APIs
     **************************************************************/
    /**
     * @brief Enables the clock for the selected GPIO port.
     * Hardware operation: updates the RCC AHB1 peripheral clock enable register.
     */
    void clockEnable();

    /**************************************************************
     * Initialization APIs
     **************************************************************/
    /**
     * @brief Configures the pin operating mode.
     * @param mode Input, output, alternate function, or analog mode.
     */
    void setMode(GPIO_Mode mode);

    /**
     * @brief Configures the pin output type as push-pull or open-drain.
     * @param type Desired output type.
     */
    void setOutputType(GPIO_OutputType type);

    /**
     * @brief Configures the output drive speed for the pin.
     * @param speed Low, medium, high, or very high speed.
     */
    void setSpeed(GPIO_Speed speed);

    /**
     * @brief Configures the internal pull-up or pull-down resistor.
     * @param pull No pull, pull-up, or pull-down.
     */
    void setPull(GPIO_Pull pull);

    /**
     * @brief Configures the alternate-function mapping for the pin.
     * @param af Alternate function number (0-15).
     */
    void setAlternateFunction(uint8_t af);

    /**************************************************************
     * Control APIs
     **************************************************************/
    /**
     * @brief Drives the selected pin to a logic high or low level.
     * @param state Desired output state.
     */
    void write(GPIO_State state);

    /**
     * @brief Reads the current logic level of the pin.
     * @return The current pin state.
     */
    GPIO_State read();

    /**
     * @brief Toggles the current output state of the pin.
     */
    void toggle();

    /**
     * @brief Writes a complete 16-bit value to the selected port.
     * @param value Data pattern to write to the output data register.
     */
    void writePort(uint16_t value);

    /**
     * @brief Reads the current 16-bit input value from the port.
     * @return Port input data value.
     */
    uint16_t readPort();

    /**************************************************************
     * Helper APIs
     **************************************************************/
    /**
     * @brief Locks the current pin configuration.
     */
    void lock();

    /**
     * @brief Resets the GPIO pin configuration to a safe default state.
     */
    void deInit();
};

#endif /* GPIO_DRIVER_H_ */
