/*
 * RCC_Driver.h
 *
 *  Created on: 18-Jul-2026
 *      Author: jolap
 */

#ifndef RCC_DRIVER_H_
#define RCC_DRIVER_H_


#include <cstdint>


#define _IO volatile


 /*********************************************************************
  *                      RCC Base Address
  *
  * STM32F407 RCC Peripheral Base Address
  *
  * Reference Manual:
  * RCC = 0x40023800
  *********************************************************************/

#define RCC_BASE 0x40023800UL



  /*********************************************************************
   *                      RCC Register Structure
   *
   * STM32F407 RCC Register Map
   *
   * Offset:
   *
   * 0x00 RCC_CR
   * 0x04 RCC_PLLCFGR
   * 0x08 RCC_CFGR
   * 0x0C RCC_CIR
   * 0x10 RCC_AHB1RSTR
   * ...
   * 0x30 RCC_AHB1ENR
   *
   *********************************************************************/

typedef struct
{

	_IO uint32_t RCC_CR;          // 0x00 Clock Control Register

	_IO uint32_t RCC_PLLCFGR;     // 0x04 PLL Configuration Register

	_IO uint32_t RCC_CFGR;        // 0x08 Clock Configuration Register

	_IO uint32_t RCC_CIR;         // 0x0C Clock Interrupt Register


	_IO uint32_t RCC_AHB1RSTR;    // 0x10 AHB1 Peripheral Reset

	_IO uint32_t RCC_AHB2RSTR;    // 0x14 AHB2 Peripheral Reset

	_IO uint32_t RCC_AHB3RSTR;    // 0x18 AHB3 Peripheral Reset


	uint32_t RESERVED0;           // 0x1C Reserved


	_IO uint32_t RCC_APB1RSTR;    // 0x20 APB1 Peripheral Reset

	_IO uint32_t RCC_APB2RSTR;    // 0x24 APB2 Peripheral Reset


	uint32_t RESERVED1[2];        // 0x28 - 0x2C Reserved



	_IO uint32_t RCC_AHB1ENR;     // 0x30 AHB1 Clock Enable Register

	_IO uint32_t RCC_AHB2ENR;     // 0x34 AHB2 Clock Enable Register

	_IO uint32_t RCC_AHB3ENR;     // 0x38 AHB3 Clock Enable Register


	uint32_t RESERVED2;           // 0x3C Reserved


	_IO uint32_t RCC_APB1ENR;     // 0x40 APB1 Clock Enable Register

	_IO uint32_t RCC_APB2ENR;     // 0x44 APB2 Clock Enable Register



	uint32_t RESERVED3[2];        // 0x48 - 0x4C Reserved



	_IO uint32_t RCC_AHB1LPENR;   // 0x50 AHB1 Low Power Enable

	_IO uint32_t RCC_AHB2LPENR;   // 0x54 AHB2 Low Power Enable

	_IO uint32_t RCC_AHB3LPENR;   // 0x58 AHB3 Low Power Enable


	uint32_t RESERVED4;           // 0x5C Reserved


	_IO uint32_t RCC_APB1LPENR;   // 0x60 APB1 Low Power Enable

	_IO uint32_t RCC_APB2LPENR;   // 0x64 APB2 Low Power Enable



	uint32_t RESERVED5[2];        // 0x68 - 0x6C Reserved



	_IO uint32_t RCC_BDCR;        // 0x70 Backup Domain Control

	_IO uint32_t RCC_CSR;         // 0x74 Control/Status Register


	uint32_t RESERVED6[2];        // 0x78 - 0x7C Reserved



	_IO uint32_t RCC_SSCGR;       // 0x80 Spread Spectrum Clock Generation

	_IO uint32_t RCC_PLLI2SCFGR;  // 0x84 PLLI2S Configuration


}RCC_RegDef_t;




/*********************************************************************
 *                      RCC Peripheral Address
 *
 * Pointer to RCC Register Structure
 *
 *********************************************************************/

#define RCCx ((RCC_RegDef_t *)RCC_BASE)




 /*********************************************************************
  *                      GPIO Clock Selection
  *
  * AHB1ENR Register:
  *
  * Bit 0 -> GPIOA
  * Bit 1 -> GPIOB
  * Bit 2 -> GPIOC
  * Bit 3 -> GPIOD
  * Bit 4 -> GPIOE
  * Bit 5 -> GPIOF
  * Bit 6 -> GPIOG
  * Bit 7 -> GPIOH
  *
  *********************************************************************/


enum class RCC_GPIO
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
 *                  USART Clock Selection
 *
 * APB2ENR
 * Bit 4 -> USART1
 * Bit 5 -> USART6
 *
 * APB1ENR
 * Bit 17 -> USART2
 * Bit 18 -> USART3
 * Bit 19 -> UART4
 * Bit 20 -> UART5
 *********************************************************************/
enum class RCC_USART
{
    USART1,
    USART2,
    USART3,
    UART4,
    UART5,
    USART6
};
/*********************************************************************
 *                  SPI Clock Selection
 *
 * APB2ENR
 * Bit 12 -> SPI1
 *
 * APB1ENR
 * Bit 14 -> SPI2
 * Bit 15 -> SPI3
 *
 *********************************************************************/

enum class RCC_SPI
{
    SPI1,
    SPI2,
    SPI3
};





/*********************************************************************
 *                      RCC Driver Class
 *
 * Responsible for:
 *
 * 1. GPIO Clock Enable
 * 2. GPIO Clock Disable
 * 3. GPIO Peripheral Reset
 *
 *********************************************************************/


class RCC
{

private:


	/*
	 * RCC register pointer
	 *
	 * Points to RCC peripheral memory
	 *
	 */
	RCC_RegDef_t* mRcc;



public:


	/*
	 * Constructor
	 *
	 * Initializes RCC peripheral address
	 *
	 */
	RCC();



	/*
	 * Enable GPIO peripheral clock
	 *
	 * Example:
	 *
	 * enableGPIOClock(RCC_GPIO::PORTD);
	 *
	 * Enables GPIOD clock
	 *
	 */
	void enableGPIOClock(RCC_GPIO port);

	/*
	* Enable USART peripheral clock
	*/
	void enableUSARTClock(RCC_USART usart);

	/*
	* Disable USART peripheral clock
	*/
	void disableUSARTClock(RCC_USART usart);		


	/*
	 * Disable GPIO peripheral clock
	 *
	 */
	void disableGPIOClock(RCC_GPIO port);




	/*
	 * Reset GPIO peripheral
	 *
	 */
	void resetGPIO(RCC_GPIO port);

	/*********************************************************************
	 * Enable SPI Peripheral Clock
	 *********************************************************************/
	void enableSPIClock(RCC_SPI spi);

	/*********************************************************************
	 * Disable SPI Peripheral Clock
	 *********************************************************************/
	void disableSPIClock(RCC_SPI spi);



};



#endif /* RCC_DRIVER_H_ */
