/*
 * SPI_Driver.h
 *
 *  Created on: 19-Jul-2026
 *      Author: jolap
 */

#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_

#include <cstdint>
#include "RCC_Driver.h"

#define _IO volatile

/*********************************************************************
 *                      SPI Peripheral Base Addresses
 *
 * Reference Manual (RM0090)
 *
 * SPI1 : 0x40013000 (APB2)
 * SPI2 : 0x40003800 (APB1)
 * SPI3 : 0x40003C00 (APB1)
 *
 *********************************************************************/

#define SPI1_BASE      0x40013000UL
#define SPI2_BASE      0x40003800UL
#define SPI3_BASE      0x40003C00UL


/*********************************************************************
 *                      SPI Register Definition
 *
 * Offset
 *
 * 0x00 CR1
 * 0x04 CR2
 * 0x08 SR
 * 0x0C DR
 * 0x10 CRCPR
 * 0x14 RXCRCR
 * 0x18 TXCRCR
 * 0x1C I2SCFGR
 * 0x20 I2SPR
 *
 *********************************************************************/

typedef struct
{
    _IO uint32_t CR1;        // 0x00 Control Register 1

    _IO uint32_t CR2;        // 0x04 Control Register 2

    _IO uint32_t SR;         // 0x08 Status Register

    _IO uint32_t DR;         // 0x0C Data Register

    _IO uint32_t CRCPR;      // 0x10 CRC Polynomial Register

    _IO uint32_t RXCRCR;     // 0x14 RX CRC Register

    _IO uint32_t TXCRCR;     // 0x18 TX CRC Register

    _IO uint32_t I2SCFGR;    // 0x1C I2S Configuration Register

    _IO uint32_t I2SPR;      // 0x20 I2S Prescaler Register

} SPI_RegDef_t;


/*********************************************************************
 *                      SPI Peripheral Definitions
 *********************************************************************/

#define SPI1_REG    ((SPI_RegDef_t*)SPI1_BASE)
#define SPI2_REG    ((SPI_RegDef_t*)SPI2_BASE)
#define SPI3_REG    ((SPI_RegDef_t*)SPI3_BASE)


/*********************************************************************
 *                      SPI Instance
 *********************************************************************/

enum class SPI_Instance
{
    SPI1,
    SPI2,
    SPI3
};


/*********************************************************************
 *                      SPI Mode
 *********************************************************************/

enum class SPI_Mode
{
    SLAVE = 0,
    MASTER
};


/*********************************************************************
 *                      Clock Polarity
 *********************************************************************/

enum class SPI_ClockPolarity
{
    LOW = 0,
    HIGH
};


/*********************************************************************
 *                      Clock Phase
 *********************************************************************/

enum class SPI_ClockPhase
{
    FIRST_EDGE = 0,
    SECOND_EDGE
};


/*********************************************************************
 *                      Data Frame Format
 *********************************************************************/

enum class SPI_DataSize
{
    BITS_8 = 0,
    BITS_16
};


/*********************************************************************
 *                      Baud Rate Prescaler
 *********************************************************************/

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

/*********************************************************************
 *                  Software Slave Management
 *********************************************************************/

enum class SPI_SSM
{
    DISABLE = 0,
    ENABLE
};


/*********************************************************************
 *                      SPI Driver Class
 *********************************************************************/

class SPI
{
private:

    /*
     * Pointer to SPI peripheral registers
     */
    SPI_RegDef_t* mSPI;

public:

    /**************************************************************
     * Constructor
     **************************************************************/
    SPI(SPI_Instance instance);

    /**************************************************************
     * Clock Control
     **************************************************************/
    void enableClock();

    /**************************************************************
     * Initialize SPI Peripheral
     **************************************************************/
    void init(SPI_Mode mode,
              SPI_BaudRate baudRate,
              SPI_ClockPolarity polarity,
              SPI_ClockPhase phase,
              SPI_DataSize dataSize,
              SPI_SSM ssm);

    /**************************************************************
     * Peripheral Control
     **************************************************************/
    void enable();

    void disable();

    /**************************************************************
     * Data Transmission
     **************************************************************/
    void transmit(uint8_t data);

    void transmit16(uint16_t data);

    void transmitBuffer(const uint8_t* buffer,
                        uint32_t length);

    /**************************************************************
     * Data Reception
     **************************************************************/
    uint8_t receive();

    uint16_t receive16();

    void receiveBuffer(uint8_t* buffer,
                       uint32_t length);

    /**************************************************************
     * Status Flags
     **************************************************************/
    bool isTXE();

    bool isRXNE();

    bool isBusy();
};

#endif /* SPI_DRIVER_H_ */
