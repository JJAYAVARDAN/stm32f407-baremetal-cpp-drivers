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
  *
  *********************************************************************/

#define RCC_BASE 0x40023800UL



  /*********************************************************************
   *                      RCC Register Structure
   *
   * STM32F407 RCC Register Map
   *
   *********************************************************************/

typedef struct
{

    _IO uint32_t RCC_CR;
    _IO uint32_t RCC_PLLCFGR;
    _IO uint32_t RCC_CFGR;
    _IO uint32_t RCC_CIR;

    _IO uint32_t RCC_AHB1RSTR;
    _IO uint32_t RCC_AHB2RSTR;
    _IO uint32_t RCC_AHB3RSTR;

    uint32_t RESERVED0;

    _IO uint32_t RCC_APB1RSTR;
    _IO uint32_t RCC_APB2RSTR;

    uint32_t RESERVED1[2];

    _IO uint32_t RCC_AHB1ENR;
    _IO uint32_t RCC_AHB2ENR;
    _IO uint32_t RCC_AHB3ENR;

    uint32_t RESERVED2;

    _IO uint32_t RCC_APB1ENR;
    _IO uint32_t RCC_APB2ENR;

    uint32_t RESERVED3[2];

    _IO uint32_t RCC_AHB1LPENR;
    _IO uint32_t RCC_AHB2LPENR;
    _IO uint32_t RCC_AHB3LPENR;

    uint32_t RESERVED4;

    _IO uint32_t RCC_APB1LPENR;
    _IO uint32_t RCC_APB2LPENR;

    uint32_t RESERVED5[2];

    _IO uint32_t RCC_BDCR;
    _IO uint32_t RCC_CSR;

    uint32_t RESERVED6[2];

    _IO uint32_t RCC_SSCGR;
    _IO uint32_t RCC_PLLI2SCFGR;


} RCC_RegDef_t;



/*********************************************************************
 *                      RCC Peripheral Address
 *
 *********************************************************************/

#define RCC_REG ((RCC_RegDef_t *)RCC_BASE)



 /*********************************************************************
  *                      GPIO Clock Selection
  *
  * AHB1ENR Register
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
 *                      USART Clock Selection
 *
 * APB1/APB2 Peripheral Clock
 *
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
 *                      SPI Clock Selection
 *
 *********************************************************************/

enum class RCC_SPI
{
    SPI1,
    SPI2,
    SPI3
};



/*********************************************************************
 *                      I2C Clock Selection
 *
 *********************************************************************/

enum class RCC_I2C
{
    I2C1,
    I2C2,
    I2C3
};



/*********************************************************************
 *                      ADC Clock Selection
 *
 *********************************************************************/

enum class RCC_ADC
{
    ADC1,
    ADC2,
    ADC3
};



/*********************************************************************
 *                      TIM Clock Selection
 *
 * APB1ENR Register
 *
 * TIM2 -> Bit 0
 * TIM3 -> Bit 1
 * TIM4 -> Bit 2
 * TIM5 -> Bit 3
 *
 *********************************************************************/

enum class RCC_TIM
{
    TIM2,
    TIM3,
    TIM4,
    TIM5
};



/*********************************************************************
 *                      RCC Driver Class
 *
 * Responsible for:
 *
 * 1. Peripheral Clock Enable
 * 2. Peripheral Clock Disable
 * 3. Peripheral Reset
 * 4. Clock Status Checking
 *
 *********************************************************************/


class RCC
{

public:


    /**************************************************************
     * GPIO Clock Control
     **************************************************************/

    static void enableGPIOClock(RCC_GPIO port);

    static void disableGPIOClock(RCC_GPIO port);

    static void resetGPIO(RCC_GPIO port);



    /**************************************************************
     * USART Clock Control
     **************************************************************/

    static void enableUSARTClock(RCC_USART usart);

    static void disableUSARTClock(RCC_USART usart);



    /**************************************************************
     * SPI Clock Control
     **************************************************************/

    static void enableSPIClock(RCC_SPI spi);

    static void disableSPIClock(RCC_SPI spi);



    /**************************************************************
     * I2C Clock Control
     **************************************************************/

    static void enableI2CClock(RCC_I2C i2c);

    static void disableI2CClock(RCC_I2C i2c);



    /**************************************************************
     * ADC Clock Control
     **************************************************************/

    static void enableADCClock(RCC_ADC adc);

    static void disableADCClock(RCC_ADC adc);



    /**************************************************************
     * TIM Clock Control
     *
     * Enables/disables/reset TIM2-TIM5 clocks
     *
     **************************************************************/

    static void enableTIMClock(RCC_TIM tim);

    static void disableTIMClock(RCC_TIM tim);

    static void resetTIMClock(RCC_TIM tim);

    static bool isTIMClockEnabled(RCC_TIM tim);


};


#endif /* RCC_DRIVER_H_ */