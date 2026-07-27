/*
 * Gpio_Driver.h
 *
 *  Created on: 18-Jul-2026
 *      Author: jolap
 */

#ifndef GPIO_DRIVER_H_
#define GPIO_DRIVER_H_

#include <cstdint>
#include "RCC_Driver.h"

#define _IO volatile

/*********************************************************************
 *                      GPIO Base Addresses
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
 *                      GPIO Register Structure
 *********************************************************************/
typedef struct
{
    _IO uint32_t MODER;
    _IO uint32_t OTYPER;
    _IO uint32_t OSPEEDR;
    _IO uint32_t PUPDR;
    _IO uint32_t IDR;
    _IO uint32_t ODR;
    _IO uint32_t BSRR;
    _IO uint32_t LCKR;
    _IO uint32_t AFRL;
    _IO uint32_t AFRH;
} GPIO_RegDef_t;

/*********************************************************************
 *                      Peripheral Definitions
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
 *                      Port Selection
 *********************************************************************/
enum class GPIO_Port
{
    PORTA = 0,
    PORTB,
    PORTC,
    PORTD,
    PORTE,
    PORTF,
    PORTG,
    PORTH,
    PORTI
};

/*********************************************************************
 *                      GPIO Mode Types
 *********************************************************************/
enum class GPIO_Mode
{
    INPUT = 0,
    OUTPUT = 1,
    ALTERNATE_FUNCTION = 2,
    ANALOG = 3
};

enum class GPIO_OutputType
{
    PUSH_PULL = 0,
    OPEN_DRAIN = 1
};

enum class GPIO_Speed
{
    LOW = 0,
    MEDIUM = 1,
    HIGH = 2,
    VERY_HIGH = 3
};

enum class GPIO_Pull
{
    NO_PULL = 0,
    PULL_UP = 1,
    PULL_DOWN = 2
};

enum class GPIO_State
{
    LOW = 0,
    HIGH = 1
};

/*********************************************************************
 *                      GPIO Driver Class
 *********************************************************************/
class GPIO
{
private:
    GPIO_Port mPortId;
    uint8_t mpin;
    GPIO_RegDef_t *mport;

public:
    GPIO(GPIO_Port port, uint8_t pin);
    void clockEnable();
    void setMode(GPIO_Mode mode);
    void setOutputType(GPIO_OutputType type);
    void setSpeed(GPIO_Speed speed);
    void setPull(GPIO_Pull pull);
    void write(GPIO_State state);
    GPIO_State read();
    void toggle();
    void setAlternateFunction(uint8_t Af);
    void lock();
    void write_port(uint16_t value);
    uint16_t readport();
    void deInit();
};

#endif /* GPIO_DRIVER_H_ */
