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