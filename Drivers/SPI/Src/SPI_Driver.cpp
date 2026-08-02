/*
 * SPI_Driver.cpp
 *
 * STM32F407 Bare-Metal SPI Driver
 *
 * Created on: 19-Jul-2026
 * Author: jolap
 *
 * Purpose:
 *   Implementation of the STM32F407 SPI driver for master/slave serial data
 *   exchange, clock configuration, and status monitoring.
 *
 * State Machine:
 *   Idle -> configure -> enable SPI -> wait TXE/RXNE -> transfer -> wait BSY -> idle.
 *
 * RM0090 Reference:
 *   Chapter 28 - Serial peripheral interface (SPI)
 */

 /*********************************************************************
  *                      SPI Driver Design
  *
  * The Serial Peripheral Interface (SPI) is a synchronous serial
  * communication peripheral used for high-speed, full-duplex data
  * exchange between the STM32F407 microcontroller and external
  * devices such as sensors, displays, EEPROMs, Flash memories,
  * ADCs, DACs, SD cards, and other microcontrollers.
  *
  * The STM32F407 provides three SPI peripherals:
  *
  *      • SPI1
  *      • SPI2
  *      • SPI3
  *
  * SPI communication is based on a master-slave architecture where
  * one device generates the clock signal and controls the data
  * transfer while one or more slave devices respond to the master's
  * requests.
  *
  * SPI communication requires four signals:
  *
  *      • SCK  - Serial Clock
  *      • MOSI - Master Output Slave Input
  *      • MISO - Master Input Slave Output
  *      • NSS  - Slave Select (Chip Select)
  *
  * Depending on the application, SPI supports:
  *
  *      • Master Mode
  *      • Slave Mode
  *      • Full Duplex
  *      • Half Duplex
  *      • Simplex Receive
  *      • Simplex Transmit
  *
  * Before communication begins, software configures:
  *
  *      • Master/Slave Selection
  *      • Clock Polarity (CPOL)
  *      • Clock Phase (CPHA)
  *      • Baud Rate Prescaler
  *      • Data Frame Format (8/16-bit)
  *      • Bit Order (MSB/LSB First)
  *      • Software/Hardware NSS Management
  *
  * During transmission, software writes data into the Data Register
  * (DR). The SPI shift register serializes the data and transmits it
  * over the MOSI line while simultaneously receiving data from the
  * MISO line.
  *
  * SPI hardware continuously updates status flags including:
  *
  *      • TXE  - Transmit Buffer Empty
  *      • RXNE - Receive Buffer Not Empty
  *      • BSY  - SPI Busy
  *      • OVR  - Overrun
  *      • MODF - Mode Fault
  *      • CRCERR - CRC Error
  *
  * The SPI driver abstracts register-level communication while
  * providing an object-oriented interface for synchronous serial
  * communication.
  *
  *********************************************************************
  *                  Polling Mode State Machine
  *
  *              Enable SPI Clock
  *                      │
  *                      ▼
  *             Configure GPIO Pins
  *                      │
  *                      ▼
  *           Configure SPI Settings
  *                      │
  *                      ▼
  *              Enable SPI
  *                      │
  *                      ▼
  *         Application Sends Data
  *                      │
  *                      ▼
  *          Wait Until TXE = 1
  *                      │
  *                      ▼
  *         Write Data Register
  *                      │
  *                      ▼
  *       Hardware Shifts Data Out
  *                      │
  *                      ▼
  *       Hardware Receives Data
  *                      │
  *                      ▼
  *         Wait Until RXNE = 1
  *                      │
  *                      ▼
  *         Read Data Register
  *                      │
  *                      ▼
  *         Wait Until BSY = 0
  *                      │
  *                      ▼
  *        Transaction Complete
  *
  *********************************************************************
  *                Interrupt Mode State Machine
  *
  *              Enable SPI Clock
  *                      │
  *                      ▼
  *             Configure GPIO Pins
  *                      │
  *                      ▼
  *           Configure SPI Settings
  *                      │
  *                      ▼
  *       Enable SPI Interrupts
  *      (TXEIE / RXNEIE / ERRIE)
  *                      │
  *                      ▼
  *        Enable NVIC Interrupt
  *                      │
  *                      ▼
  *              Enable SPI
  *                      │
  *                      ▼
  *       Hardware Detects Event
  *                      │
  *                      ▼
  *         SPI Interrupt Request
  *                      │
  *                      ▼
  *         NVIC Executes ISR
  *                      │
  *                      ▼
  *     Determine Interrupt Source
  *                      │
  *      ┌────────┼───────────┐
  *      ▼        ▼           ▼
  *    TXE      RXNE        Error
  *      │        │           │
  *      ▼        ▼           ▼
  * Write DR   Read DR   Clear Error
  *      │        │           │
  *      └────────┼───────────┘
  *               ▼
  *      Continue Communication
  *
  *********************************************************************
  *                  Driver Responsibilities
  *
  * • Enable SPI peripheral clock.
  * • Configure SPI master/slave mode.
  * • Configure SPI clock polarity.
  * • Configure SPI clock phase.
  * • Configure baud rate prescaler.
  * • Configure data frame size.
  * • Configure bit transmission order.
  * • Configure NSS management.
  * • Enable/Disable SPI peripheral.
  * • Transmit data (Polling).
  * • Receive data (Polling).
  * • Full-duplex transfer.
  * • Transmit data (Interrupt).
  * • Receive data (Interrupt).
  * • Monitor SPI status flags.
  * • Handle SPI interrupts.
  *
  * RM0090 Reference:
  * Chapter 28 - Serial Peripheral Interface (SPI)
  *
  *********************************************************************/

 #include "SPI_Driver.h"

SPI::SPI(SPI_Instance instance)
{
    switch (instance)
    {
        case SPI_Instance::SPI1:
            mSPI = SPI1_REG;
            break;
        case SPI_Instance::SPI2:
            mSPI = SPI2_REG;
            break;
        case SPI_Instance::SPI3:
            mSPI = SPI3_REG;
            break;
        default:
            mSPI = SPI1_REG;
            break;
    }
}

void SPI::enableClock()
{
    if (mSPI == SPI1_REG)
    {
        RCC::enableSPIClock(RCC_SPI::SPI1);
    }
    else if (mSPI == SPI2_REG)
    {
        RCC::enableSPIClock(RCC_SPI::SPI2);
    }
    else
    {
        RCC::enableSPIClock(RCC_SPI::SPI3);
    }
}

 /*********************************************************************
 *                  Initialize SPI Peripheral
 *********************************************************************/

void SPI::init(SPI_Mode mode,
               SPI_BaudRate baudRate,
               SPI_ClockPolarity polarity,
               SPI_ClockPhase phase,
               SPI_DataSize dataSize,
               SPI_SSM ssm)
{
    /**************************************************************
     * Enable SPI Clock
     **************************************************************/

    enableClock();

    /**************************************************************
     * Disable SPI before configuration
     **************************************************************/

    disable();

    /**************************************************************
     * Clear Configuration Registers
     **************************************************************/

    mSPI->CR1 = 0;
    mSPI->CR2 = 0;

    /**************************************************************
     * Master / Slave
     *
     * Bit 2 : MSTR
     **************************************************************/

    if (mode == SPI_Mode::MASTER)
    {
        mSPI->CR1 |= (1U << 2);
    }

    /**************************************************************
     * Baud Rate
     *
     * Bits 5:3
     **************************************************************/

    mSPI->CR1 |=
        (static_cast<uint32_t>(baudRate) << 3);

    /**************************************************************
     * Clock Polarity
     *
     * Bit 1
     **************************************************************/

    if (polarity == SPI_ClockPolarity::HIGH)
    {
        mSPI->CR1 |= (1U << 1);
    }

    /**************************************************************
     * Clock Phase
     *
     * Bit 0
     **************************************************************/

    if (phase == SPI_ClockPhase::SECOND_EDGE)
    {
        mSPI->CR1 |= (1U << 0);
    }

    /**************************************************************
     * Data Frame Format
     *
     * Bit 11
     **************************************************************/

    if (dataSize == SPI_DataSize::BITS_16)
    {
        mSPI->CR1 |= (1U << 11);
    }

    /**************************************************************
     * Software Slave Management
     *
     * SSM -> Bit 9
     * SSI -> Bit 8
     **************************************************************/

    if (ssm == SPI_SSM::ENABLE)
    {
        mSPI->CR1 |= (1U << 9);
        mSPI->CR1 |= (1U << 8);
    }

    /**************************************************************
     * Enable SPI
     **************************************************************/

    enable();
}

/*********************************************************************
 *                  Enable SPI
 *********************************************************************/

void SPI::enable()
{
    /*
     * SPE bit (CR1 Bit 6)
     */

    mSPI->CR1 |= (1U << 6);
}

/*********************************************************************
 *                  Disable SPI
 *********************************************************************/

void SPI::disable()
{
    /*
     * Clear SPE bit
     */

    mSPI->CR1 &= ~(1U << 6);
}

/*********************************************************************
 *                  TX Buffer Empty Flag
 *
 * SR Bit 1 : TXE
 *
 * Returns:
 * true  -> Transmit buffer empty
 * false -> Transmit buffer not empty
 *
 *********************************************************************/

bool SPI::isTXE()
{
    return (mSPI->SR & (1U << 1));
}


/*********************************************************************
 *                  RX Buffer Not Empty Flag
 *
 * SR Bit 0 : RXNE
 *
 * Returns:
 * true  -> Receive buffer contains data
 * false -> No data available
 *
 *********************************************************************/

bool SPI::isRXNE()
{
    return (mSPI->SR & (1U << 0));
}


/*********************************************************************
 *                  SPI Busy Flag
 *
 * SR Bit 7 : BSY
 *
 * Returns:
 * true  -> SPI Busy
 * false -> SPI Idle
 *
 *********************************************************************/

bool SPI::isBusy()
{
    return (mSPI->SR & (1U << 7));
}

/*********************************************************************
 *                  Transmit One Byte
 *********************************************************************/

void SPI::transmit(uint8_t data)
{
    /*
     * Wait until TX buffer is empty
     */

    while (!isTXE());

    /*
     * Write data into Data Register
     */

    *(_IO uint8_t*)&mSPI->DR = data;

    /*
     * Wait until SPI is no longer busy
     */

    while (isBusy());
}

/*********************************************************************
 *                  Transmit One Half Word
 *********************************************************************/

void SPI::transmit16(uint16_t data)
{
    while (!isTXE());

    mSPI->DR = data;

    while (isBusy());
}   

/*********************************************************************
 *                  Receive One Byte
 *
 * SPI is full-duplex. To receive data, transmit a dummy byte.
 *********************************************************************/

uint8_t SPI::receive()
{
    /*
     * Wait until TX buffer is empty
     */
    while (!isTXE());

    /*
     * Send dummy data to generate clock
     */
    *(_IO uint8_t*)&mSPI->DR = 0xFF;

    /*
     * Wait until data is received
     */
    while (!isRXNE());

    /*
     * Read received data
     */
    return *(_IO uint8_t*)&mSPI->DR;
}

/*********************************************************************
 *                  Receive One Half Word
*********************************************************************/

uint16_t SPI::receive16()
{
    while (!isTXE());

    mSPI->DR = 0xFFFF;

    while (!isRXNE());

    return static_cast<uint16_t>(mSPI->DR);
}

/*********************************************************************
 *                  Transmit Buffer
 *********************************************************************/

void SPI::transmitBuffer(const uint8_t *buffer,
                         uint32_t length)
{
    while (length)
    {
        transmit(*buffer);

        buffer++;

        length--;
    }
}

/*********************************************************************
 *                  Receive Buffer
 *********************************************************************/

void SPI::receiveBuffer(uint8_t *buffer,
                        uint32_t length)
{
    while (length)
    {
        *buffer = receive();

        buffer++;

        length--;
    }
}
