/*
 * USART_Driver.cpp
 *
 * STM32F407 Bare-Metal USART Driver
 *
 * Created on: 19-Jul-2026
 * Author: jolap
 *
 * Purpose:
 *   Implementation of the STM32F407 USART driver for serial communication,
 *   baud-rate setup, and transmit/receive operations.
 *
 * State Machine:
 *   Idle -> configure -> enable USART -> write DR / read DR -> TXE/RXNE -> complete.
 *
 * RM0090 Reference:
 *   Chapter 30 - Universal synchronous asynchronous receiver transmitter (USART)
 */

 /*********************************************************************
  *                      USART Driver Design
  *
  * The Universal Synchronous/Asynchronous Receiver Transmitter
  * (USART) is a full-duplex serial communication peripheral used for
  * exchanging data between the STM32F407 microcontroller and external
  * devices such as PCs, sensors, GPS modules, GSM modules, Bluetooth
  * modules, and other microcontrollers.
  *
  * The STM32F407 provides six USART/UART peripherals:
  *
  *      • USART1
  *      • USART2
  *      • USART3
  *      • UART4
  *      • UART5
  *      • USART6
  *
  * USART supports two communication modes:
  *
  *      • Asynchronous Communication
  *      • Synchronous Communication
  *
  * The peripheral consists of separate transmit and receive logic,
  * allowing simultaneous data transmission and reception (Full Duplex).
  *
  * Before communication can begin, software must configure:
  *
  *      • Peripheral Clock
  *      • Baud Rate
  *      • Word Length
  *      • Stop Bits
  *      • Parity
  *      • Oversampling Mode
  *      • Transmitter Enable
  *      • Receiver Enable
  *
  * During transmission, software writes data into the Data Register
  * (DR). The hardware automatically shifts the data serially through
  * the TX pin according to the configured baud rate.
  *
  * During reception, incoming serial data is assembled into a data
  * frame and stored in the Data Register. Software can then read the
  * received data from DR.
  *
  * The USART hardware continuously monitors status flags such as:
  *
  *      • TXE  - Transmit Data Register Empty
  *      • TC   - Transmission Complete
  *      • RXNE - Receive Data Register Not Empty
  *      • IDLE - Idle Line Detected
  *      • ORE  - Overrun Error
  *      • FE   - Framing Error
  *      • NE   - Noise Error
  *      • PE   - Parity Error
  *
  * The USART driver abstracts register-level operations and provides
  * a simple object-oriented interface for serial communication.
  *
  *********************************************************************
  *                  Polling Mode State Machine
  *
  *             Enable USART Clock
  *                     │
  *                     ▼
  *            Configure GPIO Pins
  *                     │
  *                     ▼
  *            Configure Baud Rate
  *                     │
  *                     ▼
  *         Configure Frame Format
  *                     │
  *                     ▼
  *       Enable TX and/or RX Engine
  *                     │
  *                     ▼
  *             Enable USART
  *                     │
  *                     ▼
  *          Application Requests TX
  *                     │
  *                     ▼
  *          Wait Until TXE = 1
  *                     │
  *                     ▼
  *          Write Data Register
  *                     │
  *                     ▼
  *        Hardware Transmits Frame
  *                     │
  *                     ▼
  *       Wait Until TC Flag = 1
  *                     │
  *                     ▼
  *          Transmission Complete
  *
  *                 OR
  *
  *        Wait Until RXNE Flag = 1
  *                     │
  *                     ▼
  *          Read Data Register
  *                     │
  *                     ▼
  *          Receive Complete
  *
  *********************************************************************
  *                Interrupt Mode State Machine
  *
  *             Enable USART Clock
  *                     │
  *                     ▼
  *            Configure GPIO Pins
  *                     │
  *                     ▼
  *            Configure USART
  *                     │
  *                     ▼
  *      Enable Required Interrupts
  *      (TXEIE / RXNEIE / TCIE)
  *                     │
  *                     ▼
  *         Enable NVIC Interrupt
  *                     │
  *                     ▼
  *             Enable USART
  *                     │
  *                     ▼
  *       Hardware Detects Event
  *                     │
  *                     ▼
  *         USART Interrupt Request
  *                     │
  *                     ▼
  *       NVIC Invokes USART ISR
  *                     │
  *                     ▼
  *      Determine Interrupt Source
  *                     │
  *      ┌────────┼────────┐
  *      ▼        ▼        ▼
  *    TXE      RXNE      TC
  *      │        │        │
  *      ▼        ▼        ▼
  * Write DR   Read DR   Finish TX
  *      │        │        │
  *      └────────┼────────┘
  *               ▼
  *      Clear Status Flags
  *               │
  *               ▼
  *      Return From Interrupt
  *
  *********************************************************************
  *                  Driver Responsibilities
  *
  * • Enable USART peripheral clock.
  * • Configure baud rate.
  * • Configure word length.
  * • Configure stop bits.
  * • Configure parity mode.
  * • Configure oversampling mode.
  * • Enable/Disable transmitter.
  * • Enable/Disable receiver.
  * • Enable/Disable USART peripheral.
  * • Transmit data (Polling).
  * • Receive data (Polling).
  * • Transmit data (Interrupt).
  * • Receive data (Interrupt).
  * • Read and clear status flags.
  * • Handle USART interrupts.
  *
  * RM0090 Reference:
  * Chapter 27 - Universal Synchronous/Asynchronous
  *              Receiver Transmitter (USART)
  *
  *********************************************************************/

#include "USART_Driver.h"

/*********************************************************************
 *                  Constructor
 *********************************************************************/
USART::USART(USART_Instance instance)
{
    switch (instance)
    {
    case USART_Instance::USART1:
        mUSART = USART1_REG;
        break;

    case USART_Instance::USART2:
        mUSART = USART2_REG;
        break;

    case USART_Instance::USART3:
        mUSART = USART3_REG;
        break;

    case USART_Instance::UART4:
        mUSART = UART4_REG;
        break;

    case USART_Instance::UART5:
        mUSART = UART5_REG;
        break;

    case USART_Instance::USART6:
        mUSART = USART6_REG;
        break;
    }
}

/*********************************************************************
 *                  Enable USART Clock
 *********************************************************************/
void USART::enableClock()
{
    if (mUSART == USART1_REG)
    {
        RCC::enableUSARTClock(RCC_USART::USART1);
    }
    else if (mUSART == USART2_REG)
    {
        RCC::enableUSARTClock(RCC_USART::USART2);
    }
    else if (mUSART == USART3_REG)
    {
        RCC::enableUSARTClock(RCC_USART::USART3);
    }
    else if (mUSART == UART4_REG)
    {
        RCC::enableUSARTClock(RCC_USART::UART4);
    }
    else if (mUSART == UART5_REG)
    {
        RCC::enableUSARTClock(RCC_USART::UART5);
    }
    else if (mUSART == USART6_REG)
    {
        RCC::enableUSARTClock(RCC_USART::USART6);
    }
}

/*********************************************************************
 *                  Initialize USART
 *********************************************************************/
void USART::init(uint32_t baudRate,
                 USART_Mode mode,
                 USART_WordLength wordLength,
                 USART_StopBits stopBits,
                 USART_Parity parity)
{
    /* Enable peripheral clock */
    enableClock();

    /* Disable USART before configuration */
    disable();

    /**************************************************************
     * Configure Baud Rate using the live peripheral clock
     **************************************************************/
    uint32_t peripheralClock = ((mUSART == USART1_REG) || (mUSART == USART6_REG))
        ? RCC::getAPB2ClockFreq()
        : RCC::getAPB1ClockFreq();
    uint32_t divider = (peripheralClock + (baudRate / 2U)) / baudRate;
    mUSART->BRR = divider;

    /**************************************************************
     * Configure Word Length
     **************************************************************/
    if (wordLength == USART_WordLength::BITS_9)
    {
        mUSART->CR1 |= (1U << 12);
    }
    else
    {
        mUSART->CR1 &= ~(1U << 12);
    }

    /**************************************************************
     * Configure Parity
     **************************************************************/
    switch (parity)
    {
    case USART_Parity::NONE:
        mUSART->CR1 &= ~(1U << 10);
        break;

    case USART_Parity::EVEN:
        mUSART->CR1 |= (1U << 10);
        mUSART->CR1 &= ~(1U << 9);
        break;

    case USART_Parity::ODD:
        mUSART->CR1 |= (1U << 10);
        mUSART->CR1 |= (1U << 9);
        break;
    }

    /**************************************************************
     * Configure Stop Bits
     **************************************************************/
    mUSART->CR2 &= ~(3U << 12);
    mUSART->CR2 |= (static_cast<uint32_t>(stopBits) << 12);

    /**************************************************************
     * Configure Mode
     **************************************************************/
    switch (mode)
    {
    case USART_Mode::RX:
        mUSART->CR1 |= (1U << 2);
        break;

    case USART_Mode::TX:
        mUSART->CR1 |= (1U << 3);
        break;

    case USART_Mode::TX_RX:
        mUSART->CR1 |= (1U << 2);
        mUSART->CR1 |= (1U << 3);
        break;
    }

    /* Enable USART */
    enable();
}

/*********************************************************************
 *                  Enable USART
 *********************************************************************/
void USART::enable()
{
    mUSART->CR1 |= (1U << 13);
}

/*********************************************************************
 *                  Disable USART
 *********************************************************************/
void USART::disable()
{
    mUSART->CR1 &= ~(1U << 13);
}

/*********************************************************************
 *                  Transmit One Byte
 *********************************************************************/
void USART::transmit(uint8_t data)
{
    /* Wait until TXE = 1 */
    while (!(mUSART->SR & (1U << 7)));

    mUSART->DR = data;

    /* Wait until TC = 1 */
    //while (!(mUSART->SR & (1U << 6)));
}

/*********************************************************************
 *                  Receive One Byte
 *********************************************************************/
uint8_t USART::receive()
{
    /* Wait until RXNE = 1 */
    while (!(mUSART->SR & (1U << 5)));

    return static_cast<uint8_t>(mUSART->DR);
}

/*********************************************************************
 *                  Transmit String
 *********************************************************************/
void USART::txString(const char *str)
{
    while (*str)
    {
        transmit(static_cast<uint8_t>(*str));
        str++;
    }
}