/*
 * RCC_Driver.cpp
 *
 * Created on: 18-Jul-2026
 */


#include "RCC_Driver.h"



 /*********************************************************************
  *                      RCC Constructor
  *********************************************************************/

RCC::RCC()
{

	/*
	 * Point RCC object to RCC peripheral address
	 */

	mRcc = RCCx;

}



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


	mRcc->RCC_AHB1ENR |=
		(1U << static_cast<uint32_t>(port));


}




/*********************************************************************
 *                  Disable GPIO Clock
 *********************************************************************/


void RCC::disableGPIOClock(RCC_GPIO port)
{

	mRcc->RCC_AHB1ENR &=
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

	mRcc->RCC_AHB1RSTR |=
		(1U << static_cast<uint32_t>(port));



	/*
	 * Clear reset bit
	 */

	mRcc->RCC_AHB1RSTR &=
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
            mRcc->RCC_APB2ENR |= (1U << 4);
            break;

        case RCC_USART::USART2:
            mRcc->RCC_APB1ENR |= (1U << 17);
            break;

        case RCC_USART::USART3:
            mRcc->RCC_APB1ENR |= (1U << 18);
            break;

        case RCC_USART::UART4:
            mRcc->RCC_APB1ENR |= (1U << 19);
            break;

        case RCC_USART::UART5:
            mRcc->RCC_APB1ENR |= (1U << 20);
            break;

        case RCC_USART::USART6:
            mRcc->RCC_APB2ENR |= (1U << 5);
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
            mRcc->RCC_APB2ENR &= ~(1U << 4);
            break;

        case RCC_USART::USART2:
            mRcc->RCC_APB1ENR &= ~(1U << 17);
            break;

        case RCC_USART::USART3:
            mRcc->RCC_APB1ENR &= ~(1U << 18);
            break;

        case RCC_USART::UART4:
            mRcc->RCC_APB1ENR &= ~(1U << 19);
            break;

        case RCC_USART::UART5:
            mRcc->RCC_APB1ENR &= ~(1U << 20);
            break;

        case RCC_USART::USART6:
            mRcc->RCC_APB2ENR &= ~(1U << 5);
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
            mRcc->RCC_APB2ENR |= (1U << 12);
            break;

        case RCC_SPI::SPI2:
            mRcc->RCC_APB1ENR |= (1U << 14);
            break;

        case RCC_SPI::SPI3:
            mRcc->RCC_APB1ENR |= (1U << 15);
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
            mRcc->RCC_APB2ENR &= ~(1U << 12);
            break;

        case RCC_SPI::SPI2:
            mRcc->RCC_APB1ENR &= ~(1U << 14);
            break;

        case RCC_SPI::SPI3:
            mRcc->RCC_APB1ENR &= ~(1U << 15);
            break;
    }
}
