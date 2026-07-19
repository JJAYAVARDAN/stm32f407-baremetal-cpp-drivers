/*
 * Gpio_Driver.h
 *
 *  Created on: 18-Jul-2026
 *      Author: jolap
 */

#ifndef GPIO_DRIVER_H_
#define GPIO_DRIVER_H_

#include <cstdint>
#include "RCC_Driver.h"	// Include RCC driver for clock management

#define _IO volatile

/*********************************************************************
 *                      Base Address Definitions
 *********************************************************************/
//#define RCC_BASE 0x40023800U

#define GPIOA_BASE 0x40020000UL
#define GPIOB_BASE 0x40020400UL
#define GPIOC_BASE 0x40020800UL
#define GPIOD_BASE 0x40020C00UL
#define GPIOE_BASE 0x40021000UL
#define GPIOF_BASE 0x40021400UL
#define GPIOG_BASE 0x40021800UL
#define GPIOH_BASE 0x40021C00UL
#define GPIOI_BASE 0x40022000UL
#define GPIOJ_BASE 0x40022400UL
#define GPIOK_BASE 0x40022800UL

/*********************************************************************
 *                      RCC Register Definitions
 *********************************************************************/
//#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30))


/*********************************************************************
 *                      GPIO Register Structure
 *********************************************************************/
typedef struct{
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

}Gpio_RegDef_t;


/*********************************************************************
 *                      GPIO Port Definitions
 *********************************************************************/

#define GPIOA ((Gpio_RegDef_t *)(GPIOA_BASE))
#define GPIOB ((Gpio_RegDef_t *)(GPIOB_BASE))
#define GPIOC ((Gpio_RegDef_t *)(GPIOC_BASE))
#define GPIOD ((Gpio_RegDef_t *)(GPIOD_BASE))
#define GPIOE ((Gpio_RegDef_t *)(GPIOE_BASE))
#define GPIOF ((Gpio_RegDef_t *)(GPIOF_BASE))
#define GPIOG ((Gpio_RegDef_t *)(GPIOG_BASE))
#define GPIOH ((Gpio_RegDef_t *)(GPIOH_BASE))
#define GPIOI ((Gpio_RegDef_t *)(GPIOI_BASE))
#define GPIOJ ((Gpio_RegDef_t *)(GPIOJ_BASE))
#define GPIOK ((Gpio_RegDef_t *)(GPIOK_BASE))

/*********************************************************************
 *                      GPIO Enumerations
 *********************************************************************/
enum class GPIO_Mode
{
    INPUT = 0,
    OUTPUT,
    ALTERNATE,
    ANALOG
};

enum class GPIO_OutputType
{
    PUSH_PULL = 0,
    OPEN_DRAIN
};

enum class GPIO_Pull
{
    NO_PULL = 0,
    PULL_UP,
    PULL_DOWN
};

enum class GPIO_Speed
{
    LOW = 0,
    MEDIUM,
    FAST,
    HIGH
};

enum class GPIO_State
{
    LOW = 0,
    HIGH
};

/*********************************************************************
 *                      GPIO Class Declaration
 *********************************************************************/
class GPIO{
private:
	Gpio_RegDef_t *mport;
	uint8_t mpin;
	RCC mRCC; // Instance of RCC class to manage clock
public:
	GPIO(Gpio_RegDef_t *port,uint8_t pin);
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
/*enum class GPIO_Port
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
};*/

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



#endif /* GPIO_DRIVER_H_ */
