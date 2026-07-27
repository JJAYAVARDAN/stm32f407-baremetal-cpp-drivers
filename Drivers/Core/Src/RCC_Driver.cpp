/*
 * RCC_Driver.cpp
 *
 * Created on: 18-Jul-2026
 */


#include "RCC_Driver.h"



 /*********************************************************************
  *                      RCC Constructor
  *********************************************************************/


/*********************************************************************
 *                  Enable GPIO Clock
 *********************************************************************/


void RCC::enableGPIOClock(RCC_GPIO port)
{

	/*
	 * AHB1ENR register controls GPIO clock
	 *
	 * Bit position:
	 * PORTA = 0
	 * PORTB = 1
	 * PORTC = 2
	 * PORTD = 3
	 */


	RCC_REG->RCC_AHB1ENR |=
		(1U << static_cast<uint32_t>(port));


}




/*********************************************************************
 *                  Disable GPIO Clock
 *********************************************************************/


void RCC::disableGPIOClock(RCC_GPIO port)
{

	RCC_REG->RCC_AHB1ENR &=
		~(1U << static_cast<uint32_t>(port));

}





/*********************************************************************
 *                  Reset GPIO Peripheral
 *********************************************************************/


void RCC::resetGPIO(RCC_GPIO port)
{

	/*
	 * Set reset bit
	 */

	RCC_REG->RCC_AHB1RSTR |=
		(1U << static_cast<uint32_t>(port));



	/*
	 * Clear reset bit
	 */

	RCC_REG->RCC_AHB1RSTR &=
		~(1U << static_cast<uint32_t>(port));

}
/*********************************************************************
 *                  Enable USART Clock
 *********************************************************************/
void RCC::enableUSARTClock(RCC_USART usart)
{
    switch (usart)
    {
        case RCC_USART::USART1:
            RCC_REG->RCC_APB2ENR |= (1U << 4);
            break;

        case RCC_USART::USART2:
            RCC_REG->RCC_APB1ENR |= (1U << 17);
            break;

        case RCC_USART::USART3:
            RCC_REG->RCC_APB1ENR |= (1U << 18);
            break;

        case RCC_USART::UART4:
            RCC_REG->RCC_APB1ENR |= (1U << 19);
            break;

        case RCC_USART::UART5:
            RCC_REG->RCC_APB1ENR |= (1U << 20);
            break;

        case RCC_USART::USART6:
            RCC_REG->RCC_APB2ENR |= (1U << 5);
            break;
    }
}

/*********************************************************************
 *                  Disable USART Clock
 *********************************************************************/
void RCC::disableUSARTClock(RCC_USART usart)
{
    switch (usart)
    {
        case RCC_USART::USART1:
            RCC_REG->RCC_APB2ENR &= ~(1U << 4);
            break;

        case RCC_USART::USART2:
            RCC_REG->RCC_APB1ENR &= ~(1U << 17);
            break;

        case RCC_USART::USART3:
            RCC_REG->RCC_APB1ENR &= ~(1U << 18);
            break;

        case RCC_USART::UART4:
            RCC_REG->RCC_APB1ENR &= ~(1U << 19);
            break;

        case RCC_USART::UART5:
            RCC_REG->RCC_APB1ENR &= ~(1U << 20);
            break;

        case RCC_USART::USART6:
            RCC_REG->RCC_APB2ENR &= ~(1U << 5);
            break;
    }
}
/*********************************************************************
 *                  Enable I2C Clock
 *********************************************************************/

void RCC::enableI2CClock(RCC_I2C i2c)
{
    switch(i2c)
    {
        case RCC_I2C::I2C1:
            RCC_REG->RCC_APB1ENR |= (1U << 21);
            break;

        case RCC_I2C::I2C2:
            RCC_REG->RCC_APB1ENR |= (1U << 22);
            break;

        case RCC_I2C::I2C3:
            RCC_REG->RCC_APB1ENR |= (1U << 23);
            break;
    }
}

/*********************************************************************
 *                  Disable I2C Clock
 *********************************************************************/

void RCC::disableI2CClock(RCC_I2C i2c)
{
    switch(i2c)
    {
        case RCC_I2C::I2C1:
            RCC_REG->RCC_APB1ENR &= ~(1U << 21);
            break;

        case RCC_I2C::I2C2:
            RCC_REG->RCC_APB1ENR &= ~(1U << 22);
            break;

        case RCC_I2C::I2C3:
            RCC_REG->RCC_APB1ENR &= ~(1U << 23);
            break;
    }
}
/*********************************************************************
 *                  Enable SPI Clock
 *********************************************************************/
void RCC::enableSPIClock(RCC_SPI spi)
{
    switch (spi)
    {
        case RCC_SPI::SPI1:
            RCC_REG->RCC_APB2ENR |= (1U << 12);
            break;
        case RCC_SPI::SPI2:
            RCC_REG->RCC_APB1ENR |= (1U << 14);
            break;
        case RCC_SPI::SPI3:
            RCC_REG->RCC_APB1ENR |= (1U << 15);
            break;
    }
}

/*********************************************************************
 *                  Disable SPI Clock
 *********************************************************************/
void RCC::disableSPIClock(RCC_SPI spi)
{
    switch (spi)
    {
        case RCC_SPI::SPI1:
            RCC_REG->RCC_APB2ENR &= ~(1U << 12);
            break;
        case RCC_SPI::SPI2:
            RCC_REG->RCC_APB1ENR &= ~(1U << 14);
            break;
        case RCC_SPI::SPI3:
            RCC_REG->RCC_APB1ENR &= ~(1U << 15);
            break;
    }
}

/*********************************************************************
 *                  Enable ADC Clock
 *********************************************************************/
 void RCC::enableADCClock(RCC_ADC adc)
 {
     switch(adc)
     {
        case RCC_ADC::ADC1:
            RCC_REG->RCC_APB2ENR |= (1U << 8);
            break;
        case RCC_ADC::ADC2:
            RCC_REG->RCC_APB2ENR |= (1U << 9);
            break;
        case RCC_ADC::ADC3:
            RCC_REG->RCC_APB2ENR |= (1U << 10);
            break;

     }
 }

 /*********************************************************************
 *                  Disable ADC Clock
 *********************************************************************/
void RCC::disableADCClock(RCC_ADC adc)
{
    switch(adc)
    {
        case RCC_ADC::ADC1:
            RCC_REG->RCC_APB2ENR &= ~(1U << 8);
            break;
        case RCC_ADC::ADC2:
            RCC_REG->RCC_APB2ENR &= ~(1U << 9);
            break;
        case RCC_ADC::ADC3:
            RCC_REG->RCC_APB2ENR &= ~(1U << 10);
            break;
    }
   
}

/*********************************************************************
 *                  Enable TIM Clock
 *********************************************************************/
void RCC::enableTIMClock(RCC_TIM tim)
{
    switch (tim)
    {
        case RCC_TIM::TIM2:
            RCC_REG->RCC_APB1ENR |= (1U << 0);
            break;
        case RCC_TIM::TIM3:
            RCC_REG->RCC_APB1ENR |= (1U << 1);
            break;
        case RCC_TIM::TIM4:
            RCC_REG->RCC_APB1ENR |= (1U << 2);
            break;
        case RCC_TIM::TIM5:
            RCC_REG->RCC_APB1ENR |= (1U << 3);
            break;
    }
}

/*********************************************************************
 *                  Disable TIM Clock
 *********************************************************************/
void RCC::disableTIMClock(RCC_TIM tim)
{
    switch (tim)
    {
        case RCC_TIM::TIM2:
            RCC_REG->RCC_APB1ENR &= ~(1U << 0);
            break;
        case RCC_TIM::TIM3:
            RCC_REG->RCC_APB1ENR &= ~(1U << 1);
            break;
        case RCC_TIM::TIM4:
            RCC_REG->RCC_APB1ENR &= ~(1U << 2);
            break;
        case RCC_TIM::TIM5:
            RCC_REG->RCC_APB1ENR &= ~(1U << 3);
            break;
    }
}

/*********************************************************************
 *                  Reset TIM Peripheral
 *********************************************************************/
void RCC::resetTIMClock(RCC_TIM tim)
{
    switch (tim)
    {
        case RCC_TIM::TIM2:
            RCC_REG->RCC_APB1RSTR |= (1U << 0);
            RCC_REG->RCC_APB1RSTR &= ~(1U << 0);
            break;
        case RCC_TIM::TIM3:
            RCC_REG->RCC_APB1RSTR |= (1U << 1);
            RCC_REG->RCC_APB1RSTR &= ~(1U << 1);
            break;
        case RCC_TIM::TIM4:
            RCC_REG->RCC_APB1RSTR |= (1U << 2);
            RCC_REG->RCC_APB1RSTR &= ~(1U << 2);
            break;
        case RCC_TIM::TIM5:
            RCC_REG->RCC_APB1RSTR |= (1U << 3);
            RCC_REG->RCC_APB1RSTR &= ~(1U << 3);
            break;
    }
}

/*********************************************************************
 *                  TIM Clock Status
 *********************************************************************/
bool RCC::isTIMClockEnabled(RCC_TIM tim)
{
    switch (tim)
    {
        case RCC_TIM::TIM2:
            return (RCC_REG->RCC_APB1ENR & (1U << 0)) != 0U;
        case RCC_TIM::TIM3:
            return (RCC_REG->RCC_APB1ENR & (1U << 1)) != 0U;
        case RCC_TIM::TIM4:
            return (RCC_REG->RCC_APB1ENR & (1U << 2)) != 0U;
        case RCC_TIM::TIM5:
            return (RCC_REG->RCC_APB1ENR & (1U << 3)) != 0U;
    }
    return false;
}