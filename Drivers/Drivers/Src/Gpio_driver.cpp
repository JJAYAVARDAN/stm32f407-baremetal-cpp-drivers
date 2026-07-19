/*
 * Gpio_driver.cpp
 *
 *  Created on: 18-Jul-2026
 *      Author: jolap
 */


#include "Gpio_Driver.h"


 /*********************************************************************
  *                      Constructor
  *********************************************************************/

GPIO::GPIO(GPIO_Port port, uint8_t pin)
{
	mPortId = port;
	mpin = pin;

	switch (port)
	{
		case GPIO_Port::PORTA:
			mport = GPIOA;
			break;
		case GPIO_Port::PORTB:
			mport = GPIOB;
			break;
		case GPIO_Port::PORTC:
			mport = GPIOC;
			break;
		case GPIO_Port::PORTD:
			mport = GPIOD;
			break;
		case GPIO_Port::PORTE:
			mport = GPIOE;
			break;
		case GPIO_Port::PORTF:
			mport = GPIOF;
			break;
		case GPIO_Port::PORTG:
			mport = GPIOG;
			break;
		case GPIO_Port::PORTH:
			mport = GPIOH;
			break;
		case GPIO_Port::PORTI:
			mport = GPIOI;
			break;
	}
}

void GPIO::clockEnable()
{
	mRCC.enableGPIOClock(static_cast<RCC_GPIO>(mPortId));
}


/*********************************************************************
 *                      Set GPIO Mode
 *********************************************************************/
void GPIO::setMode(GPIO_Mode mode)
{
	mport->MODER &= ~(3U << (mpin * 2));
	mport->MODER |= (static_cast<uint32_t>(mode) << (mpin * 2));
}

/*********************************************************************
 *                      Set Output Type
 *********************************************************************/

void GPIO::setOutputType(GPIO_OutputType type)
{
	mport->OTYPER &= ~(1U << mpin);
	mport->OTYPER |= (static_cast<uint32_t>(type) << mpin);
}

/*********************************************************************
 *                      Set Output Speed
 *********************************************************************/

void GPIO::setSpeed(GPIO_Speed speed)
{
	mport->OSPEEDR &= ~(3U << (mpin * 2));
	mport->OSPEEDR |= (static_cast<uint32_t>(speed) << (mpin * 2));
}

/*********************************************************************
 *                      Set Pull Configuration
 *********************************************************************/
void GPIO::setPull(GPIO_Pull pull)
{
	mport->PUPDR &= ~(3U << (mpin * 2));
	mport->PUPDR |= (static_cast<uint32_t>(pull) << (mpin * 2));
}


/*********************************************************************
 *                      Write GPIO Pin
 *********************************************************************/
void GPIO::write(GPIO_State state)
{
	if (state == GPIO_State::HIGH)
	{
		mport->BSRR |= (1U << mpin);
	}
	else
	{
		mport->BSRR |= (1U << (mpin + 16));
	}
}
/*********************************************************************
 *                      Read GPIO Pin
 *********************************************************************/
GPIO_State GPIO::read()
{
	if (mport->IDR & (1U << mpin))
	{
		return GPIO_State::HIGH;
	}
	return GPIO_State::LOW;
}

void GPIO::toggle()
{
	mport->ODR ^= (1U << mpin);
}

/*********************************************************************
 *                  Configure Alternate Function
 *********************************************************************/

void GPIO::setAlternateFunction(uint8_t Af)
{
	if (mpin < 8)
	{
		mport->AFRL &= ~(0xFU << (mpin * 4));
		mport->AFRL |= ((uint32_t)Af << (mpin * 4));
	}
	else
	{
		uint8_t pin = mpin - 8;
		mport->AFRH &= ~(0xFU << (pin * 4));
		mport->AFRH |= ((uint32_t)Af << (pin * 4));
	}
}
/*********************************************************************
 *                      Lock GPIO Configuration
 *********************************************************************/

void GPIO::lock()
{
	uint32_t temp = (1U << 16) | (1U << mpin);
	mport->LCKR = temp;
	mport->LCKR = (1U << mpin);
	mport->LCKR = temp;

	(void)mport->LCKR;
	(void)mport->LCKR;
}
/*********************************************************************
 *                      Write Entire Port
 *********************************************************************/
void GPIO::write_port(uint16_t value)
{
	mport->ODR = value;
}
/*********************************************************************
 *                      Read Entire Port
 *********************************************************************/
uint16_t GPIO::readport()
{
	return (uint16_t)mport->IDR;
}
/*********************************************************************
 *                      Reset GPIO Registers
 *********************************************************************/
void GPIO::deInit()
{
	mport->MODER = 0x00000000;
	mport->OTYPER = 0x00000000;
	mport->OSPEEDR = 0x00000000;
	mport->PUPDR = 0x00000000;
	mport->ODR = 0x00000000;
	mport->AFRL = 0x00000000;
	mport->AFRH = 0x00000000;
}




























