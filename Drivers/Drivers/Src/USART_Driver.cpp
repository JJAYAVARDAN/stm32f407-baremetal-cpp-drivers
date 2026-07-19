/*
 * USART_Driver.cpp
 *
 *  Created on: 19-Jul-2026
 *      Author: jolap
 */

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
    RCC rcc;

    if (mUSART == USART1_REG)
    {
        rcc.enableUSARTClock(RCC_USART::USART1);
    }
    else if (mUSART == USART2_REG)
    {
        rcc.enableUSARTClock(RCC_USART::USART2);
    }
    else if (mUSART == USART3_REG)
    {
        rcc.enableUSARTClock(RCC_USART::USART3);
    }
    else if (mUSART == UART4_REG)
    {
        rcc.enableUSARTClock(RCC_USART::UART4);
    }
    else if (mUSART == UART5_REG)
    {
        rcc.enableUSARTClock(RCC_USART::UART5);
    }
    else if (mUSART == USART6_REG)
    {
        rcc.enableUSARTClock(RCC_USART::USART6);
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
     * Configure Baud Rate
     * Assuming APB Clock = 16 MHz
     **************************************************************/
    mUSART->BRR = (16000000U / baudRate);

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