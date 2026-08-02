/*
 * SPI_Driver.h
 *
 * STM32F407 Bare-Metal SPI Driver
 *
 * Created on: 19-Jul-2026
 * Author: jolap
 *
 * Purpose:
 *   Register-level driver for the STM32F407 Serial Peripheral Interface.
 *   The driver exposes APIs for configuring SPI mode, clocking, data
 *   transfer, and status checking.
 *
 * RM0090 Reference:
 *   Chapter 28 - Serial peripheral interface (SPI)
 */

#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_

#include <cstdint>
#include "RCC_Driver.h"

#define _IO volatile

/*********************************************************************
 * Peripheral Base Addresses
 *********************************************************************/
#define SPI1_BASE 0x40013000UL
#define SPI2_BASE 0x40003800UL
#define SPI3_BASE 0x40003C00UL

/*********************************************************************
 * Register Definitions
 *********************************************************************/
typedef struct
{
    _IO uint32_t CR1;        /*!< Control register 1,      Address offset: 0x00 */
    _IO uint32_t CR2;        /*!< Control register 2,      Address offset: 0x04 */
    _IO uint32_t SR;         /*!< Status register,         Address offset: 0x08 */
    _IO uint32_t DR;         /*!< Data register,           Address offset: 0x0C */
    _IO uint32_t CRCPR;      /*!< CRC polynomial register, Address offset: 0x10 */
    _IO uint32_t RXCRCR;     /*!< RX CRC register,         Address offset: 0x14 */
    _IO uint32_t TXCRCR;     /*!< TX CRC register,         Address offset: 0x18 */
    _IO uint32_t I2SCFGR;    /*!< I2S configuration register, Address offset: 0x1C */
    _IO uint32_t I2SPR;      /*!< I2S prescaler register,  Address offset: 0x20 */
} SPI_RegDef_t;

/*********************************************************************
 * Register Structure
 *********************************************************************/
#define SPI1_REG ((SPI_RegDef_t *)SPI1_BASE)
#define SPI2_REG ((SPI_RegDef_t *)SPI2_BASE)
#define SPI3_REG ((SPI_RegDef_t *)SPI3_BASE)

/*********************************************************************
 * Enumerations
 *********************************************************************/
enum class SPI_Instance
{
    SPI1,
    SPI2,
    SPI3
};

enum class SPI_Mode
{
    SLAVE = 0,
    MASTER
};

enum class SPI_ClockPolarity
{
    LOW = 0,
    HIGH
};

enum class SPI_ClockPhase
{
    FIRST_EDGE = 0,
    SECOND_EDGE
};

enum class SPI_DataSize
{
    BITS_8 = 0,
    BITS_16
};

enum class SPI_BaudRate
{
    DIV2 = 0,
    DIV4,
    DIV8,
    DIV16,
    DIV32,
    DIV64,
    DIV128,
    DIV256
};

enum class SPI_SSM
{
    DISABLE = 0,
    ENABLE
};

/*********************************************************************
 * Driver Class
 *********************************************************************/
class SPI
{
private:
    SPI_RegDef_t* mSPI;

public:
    /**************************************************************
     * Constructor
     **************************************************************/
    SPI(SPI_Instance instance);

    /**************************************************************
     * Clock APIs
     **************************************************************/
    /**
     * @brief Enables the clock for the selected SPI peripheral.
     */
    void enableClock();

    /**************************************************************
     * Initialization APIs
     **************************************************************/
    /**
     * @brief Configures SPI mode, clocking, frame format, and NSS handling.
     */
    void init(SPI_Mode mode,
              SPI_BaudRate baudRate,
              SPI_ClockPolarity polarity,
              SPI_ClockPhase phase,
              SPI_DataSize dataSize,
              SPI_SSM ssm);

    /**************************************************************
     * Control APIs
     **************************************************************/
    /**
     * @brief Enables the SPI peripheral.
     */
    void enable();

    /**
     * @brief Disables the SPI peripheral.
     */
    void disable();

    /**************************************************************
     * Data Transfer APIs
     **************************************************************/
    void transmit(uint8_t data);
    void transmit16(uint16_t data);
    void transmitBuffer(const uint8_t* buffer,
                        uint32_t length);

    uint8_t receive();
    uint16_t receive16();
    void receiveBuffer(uint8_t* buffer,
                       uint32_t length);

    /**************************************************************
     * Status APIs
     **************************************************************/
    bool isTXE();
    bool isRXNE();
    bool isBusy();
};

#endif /* SPI_DRIVER_H_ */
