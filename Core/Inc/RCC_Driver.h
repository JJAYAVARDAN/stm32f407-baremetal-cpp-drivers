/*
 * RCC_Driver.h
 *
 * STM32F407 Bare-Metal RCC Driver
 *
 * Created on: 18-Jul-2026
 * Author: jolap
 *
 * Purpose:
 *   Register-level driver for the STM32F407 Reset and Clock Control
 *   peripheral. This header defines the memory-mapped RCC register layout,
 *   clock-source enumerations, and the public RCC driver interface.
 *
 * RM0090 Reference:
 *   Chapter 7 - Reset and clock control (RCC)
 */

#ifndef RCC_DRIVER_H_
#define RCC_DRIVER_H_

#include <cstdint>

#define _IO volatile

/*********************************************************************
 * Peripheral Base Address
 *********************************************************************/
#define RCC_BASE 0x40023800UL

/*********************************************************************
 * Register Definitions
 *********************************************************************/
typedef struct
{
    _IO uint32_t CR;         /*!< RCC clock control register,                                   Address offset: 0x00 */
    _IO uint32_t PLLCFGR;    /*!< RCC PLL configuration register,                               Address offset: 0x04 */
    _IO uint32_t CFGR;       /*!< RCC clock configuration register,                             Address offset: 0x08 */
    _IO uint32_t CIR;        /*!< RCC clock interrupt register,                                 Address offset: 0x0C */
    _IO uint32_t AHB1RSTR;   /*!< RCC AHB1 peripheral reset register,                           Address offset: 0x10 */
    _IO uint32_t AHB2RSTR;   /*!< RCC AHB2 peripheral reset register,                           Address offset: 0x14 */
    _IO uint32_t AHB3RSTR;   /*!< RCC AHB3 peripheral reset register,                           Address offset: 0x18 */
    uint32_t RESERVED0;      /*!< Reserved,                                                     Address offset: 0x1C */
    _IO uint32_t APB1RSTR;   /*!< RCC APB1 peripheral reset register,                           Address offset: 0x20 */
    _IO uint32_t APB2RSTR;   /*!< RCC APB2 peripheral reset register,                           Address offset: 0x24 */
    uint32_t RESERVED1[2];   /*!< Reserved,                                                     Address offset: 0x28-0x2C */
    _IO uint32_t AHB1ENR;    /*!< RCC AHB1 peripheral clock enable register,                    Address offset: 0x30 */
    _IO uint32_t AHB2ENR;    /*!< RCC AHB2 peripheral clock enable register,                    Address offset: 0x34 */
    _IO uint32_t AHB3ENR;    /*!< RCC AHB3 peripheral clock enable register,                    Address offset: 0x38 */
    uint32_t RESERVED2;      /*!< Reserved,                                                     Address offset: 0x3C */
    _IO uint32_t APB1ENR;    /*!< RCC APB1 peripheral clock enable register,                    Address offset: 0x40 */
    _IO uint32_t APB2ENR;    /*!< RCC APB2 peripheral clock enable register,                    Address offset: 0x44 */
    uint32_t RESERVED3[2];   /*!< Reserved,                                                     Address offset: 0x48-0x4C */
    _IO uint32_t AHB1LPENR;  /*!< RCC AHB1 peripheral clock enable in low power mode register,  Address offset: 0x50 */
    _IO uint32_t AHB2LPENR;  /*!< RCC AHB2 peripheral clock enable in low power mode register,  Address offset: 0x54 */
    _IO uint32_t AHB3LPENR;  /*!< RCC AHB3 peripheral clock enable in low power mode register,  Address offset: 0x58 */
    uint32_t RESERVED4;      /*!< Reserved,                                                     Address offset: 0x5C */
    _IO uint32_t APB1LPENR;  /*!< RCC APB1 peripheral clock enable in low power mode register,  Address offset: 0x60 */
    _IO uint32_t APB2LPENR;  /*!< RCC APB2 peripheral clock enable in low power mode register,  Address offset: 0x64 */
    uint32_t RESERVED5[2];   /*!< Reserved,                                                     Address offset: 0x68-0x6C */
    _IO uint32_t BDCR;       /*!< RCC Backup domain control register,                           Address offset: 0x70 */
    _IO uint32_t CSR;        /*!< RCC clock control & status register,                          Address offset: 0x74 */
    uint32_t RESERVED6[2];   /*!< Reserved,                                                     Address offset: 0x78-0x7C */
    _IO uint32_t SSCGR;      /*!< RCC spread spectrum clock generation register,                Address offset: 0x80 */
    _IO uint32_t PLLI2SCFGR; /*!< RCC PLLI2S configuration register,                            Address offset: 0x84 */
} RCC_RegDef_t;

/*********************************************************************
 * Register Structure
 *********************************************************************/
#define RCC_REG ((RCC_RegDef_t *)RCC_BASE)

/*********************************************************************
 * Peripheral Definitions
 *********************************************************************/

/*********************************************************************
 * Enumerations
 *********************************************************************/

/**
 * @brief System clock source selection.
 * RM0090 Reference: CFGR register, SW bits.
 */
enum class ClockSource
{
    HSI,
    HSE,
    PLL
};

/**
 * @brief AHB bus prescaler values.
 * RM0090 Reference: CFGR register, HPRE bits.
 */
enum class AHBPrescaler
{
    DIV1 = 0,
    DIV2 = 8,
    DIV4 = 9,
    DIV8 = 10,
    DIV16 = 11,
    DIV64 = 12,
    DIV128 = 13,
    DIV256 = 14,
    DIV512 = 15
};

/**
 * @brief APB bus prescaler values.
 * RM0090 Reference: CFGR register, PPRE1/PPRE2 bits.
 */
enum class APBPrescaler
{
    DIV1 = 0,
    DIV2 = 4,
    DIV4 = 5,
    DIV8 = 6,
    DIV16 = 7
};

enum class RCC_GPIO
{
    PORTA = 0, PORTB, PORTC, PORTD, PORTE, PORTF, PORTG, PORTH, PORTI
};

enum class RCC_USART
{
    USART1, USART2, USART3, UART4, UART5, USART6
};

enum class RCC_SPI
{
    SPI1, SPI2, SPI3
};

enum class RCC_I2C
{
    I2C1, I2C2, I2C3
};

enum class RCC_ADC
{
    ADC1, ADC2, ADC3
};

enum class RCC_TIM
{
    TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8, TIM9, TIM10, TIM11, TIM12, TIM13, TIM14
};

enum class RCC_DMA
{
    DMA1, DMA2
};

enum class TIM_Instance;

/*********************************************************************
 * Configuration Structures
 *********************************************************************/
struct PLL_Config
{
    ClockSource source = ClockSource::HSI;
    uint32_t M = 8;
    uint32_t N = 168;
    uint32_t P = 0;
    uint32_t Q = 7;
};

/*********************************************************************
 * Driver Class
 *********************************************************************/
class RCC
{
private:
    static uint32_t mSystemClockFreq;
    static uint32_t mAHBClockFreq;
    static uint32_t mAPB1ClockFreq;
    static uint32_t mAPB2ClockFreq;

    /**
     * @brief Recalculates and updates the stored clock frequencies.
     */
    static void updateClockFrequencies();

public:
    /**************************************************************
     * Clock APIs
     **************************************************************/
    /**
     * @brief Selects the system clock source and configures PLL settings.
     * @param source The requested clock source.
     * @param pll_config PLL parameters used when PLL is selected.
     */
    static void setSystemClockSource(ClockSource source, PLL_Config pll_config = {});

    /**
     * @brief Configures the AHB and APB prescalers.
     * @param ahb AHB prescaler selection.
     * @param apb1 APB1 prescaler selection.
     * @param apb2 APB2 prescaler selection.
     */
    static void configurePrescalers(AHBPrescaler ahb, APBPrescaler apb1, APBPrescaler apb2);

    /**************************************************************
     * Status APIs
     **************************************************************/
    static uint32_t getSystemClockFreq();
    static uint32_t getAHBClockFreq();
    static uint32_t getAPB1ClockFreq();
    static uint32_t getAPB2ClockFreq();
    static uint32_t getAHBFreq();
    static uint32_t getAPB1Freq();
    static uint32_t getAPB2Freq();
    static uint32_t getUSARTClock(RCC_USART usart);
    static uint32_t getSPIClock(RCC_SPI spi);
    static uint32_t getI2CClock(RCC_I2C i2c);
    static uint32_t getTIMClock(RCC_TIM tim);
    static uint32_t getTimerClockFreq(TIM_Instance tim);

    /**************************************************************
     * Clock Control APIs
     **************************************************************/
    static void enableGPIOClock(RCC_GPIO port);
    static void disableGPIOClock(RCC_GPIO port);
    static void resetGPIO(RCC_GPIO port);

    static void enableUSARTClock(RCC_USART usart);
    static void disableUSARTClock(RCC_USART usart);

    static void enableSPIClock(RCC_SPI spi);
    static void disableSPIClock(RCC_SPI spi);

    static void enableI2CClock(RCC_I2C i2c);
    static void disableI2CClock(RCC_I2C i2c);

    static void enableADCClock(RCC_ADC adc);
    static void disableADCClock(RCC_ADC adc);

    static void enableTIMClock(RCC_TIM tim);
    static void disableTIMClock(RCC_TIM tim);
    static void resetTIMClock(RCC_TIM tim);
    static bool isTIMClockEnabled(RCC_TIM tim);

    static void enableDMAClock(RCC_DMA dma);
    static void disableDMAClock(RCC_DMA dma);

    static void enableSYSCFGClock();
};

#endif /* RCC_DRIVER_H_ */