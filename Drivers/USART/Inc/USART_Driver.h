/*
 * USART_Driver.h
 *
 * STM32F407 Bare-Metal USART Driver
 *
 * Created on: 19-Jul-2026
 * Author: jolap
 *
 * Purpose:
 *   Register-level driver for the STM32F407 Universal Synchronous/
 *   Asynchronous Receiver-Transmitter peripheral. The driver provides APIs
 *   for baud-rate configuration, enable/disable control, and data transfer.
 *
 * RM0090 Reference:
 *   Chapter 30 - Universal synchronous asynchronous receiver transmitter (USART)
 */

#ifndef USART_DRIVER_H_
#define USART_DRIVER_H_

#include <cstdint>
#include "RCC_Driver.h"
#include "Gpio_Driver.h"

#define _IO volatile

/*********************************************************************
 * Peripheral Base Addresses
 *********************************************************************/
#define USART1_BASE 0x40011000UL
#define USART2_BASE 0x40004400UL
#define USART3_BASE 0x40004800UL
#define UART4_BASE 0x40004C00UL
#define UART5_BASE 0x40005000UL
#define USART6_BASE 0x40011400UL

/*********************************************************************
 * Register Definitions
 *********************************************************************/
typedef struct
{
    _IO uint32_t SR;      /*!< Status register,          Address offset: 0x00 */
    _IO uint32_t DR;      /*!< Data register,            Address offset: 0x04 */
    _IO uint32_t BRR;     /*!< Baud rate register,       Address offset: 0x08 */
    _IO uint32_t CR1;     /*!< Control register 1,       Address offset: 0x0C */
    _IO uint32_t CR2;     /*!< Control register 2,       Address offset: 0x10 */
    _IO uint32_t CR3;     /*!< Control register 3,       Address offset: 0x14 */
    _IO uint32_t GTPR;    /*!< Guard time and prescaler, Address offset: 0x18 */
} USART_RegDef_t;

/*********************************************************************
 * Register Structure
 *********************************************************************/
#define USART1_REG ((USART_RegDef_t *)USART1_BASE)
#define USART2_REG ((USART_RegDef_t *)USART2_BASE)
#define USART3_REG ((USART_RegDef_t *)USART3_BASE)
#define UART4_REG ((USART_RegDef_t *)UART4_BASE)
#define UART5_REG ((USART_RegDef_t *)UART5_BASE)
#define USART6_REG ((USART_RegDef_t *)USART6_BASE)

/*********************************************************************
 * Enumerations
 *********************************************************************/
enum class USART_Instance
{
    USART1,
    USART2,
    USART3,
    UART4,
    UART5,
    USART6
};

enum class USART_WordLength
{
    BITS_8 = 0,
    BITS_9
};

enum class USART_StopBits
{
    STOP_1 = 0,
    STOP_0_5,
    STOP_2,
    STOP_1_5
};

enum class USART_Parity
{
    NONE = 0,
    EVEN,
    ODD
};

enum class USART_Mode
{
    RX = 1,
    TX,
    TX_RX
};

/*********************************************************************
 * Driver Class
 *********************************************************************/
class USART
{
private:
    USART_RegDef_t *mUSART;

public:
    /**************************************************************
     * Constructor
     **************************************************************/
    USART(USART_Instance instance);

    /**************************************************************
     * Clock APIs
     **************************************************************/
    /**
     * @brief Enables the clock for the selected USART peripheral.
     */
    void enableClock();

    /**************************************************************
     * Initialization APIs
     **************************************************************/
    /**
     * @brief Configures the USART baud rate, mode, word length, stop bits,
     * and parity selection.
     */
    void init(uint32_t baud,
              USART_Mode mode,
              USART_WordLength wordLength = USART_WordLength::BITS_8,
              USART_StopBits stop_bits = USART_StopBits::STOP_1,
              USART_Parity parity = USART_Parity::NONE);

    /**************************************************************
     * Control APIs
     **************************************************************/
    void enable();
    void disable();

    /**************************************************************
     * Data Transfer APIs
     **************************************************************/
    void transmit(uint8_t data);
    uint8_t receive();
    void txString(const char *str);
};

#endif /* USART_DRIVER_H_ */
