/*
 * USART_Driver.h
 *
 *  Created on: 19-Jul-2026
 *      Author: jolap
 */

#ifndef USART_DRIVER_H_
#define USART_DRIVER_H_

#include<cstdint>
#include "RCC_Driver.h"
#include "Gpio_Driver.h"

#define _IO volatile
 /*********************************************************************
  *                  USART Base Addresses
  *********************************************************************/
#define USART1_BASE 0x40011000UL
#define USART2_BASE 0x40004400UL
#define USART3_BASE 0x40004800UL
#define UART4_BASE 0x40004C00UL
#define UART5_BASE 0x40005000UL
#define USART6_BASE 0x40011400UL
 /*********************************************************************
  *                  USART Register Definition
  *********************************************************************/
typedef struct
{
    _IO uint32_t SR;      /* Status Register               Offset: 0x00 */
    _IO uint32_t DR;      /* Data Register                 Offset: 0x04 */
    _IO uint32_t BRR;     /* Baud Rate Register            Offset: 0x08 */
    _IO uint32_t CR1;     /* Control Register 1            Offset: 0x0C */
    _IO uint32_t CR2;     /* Control Register 2            Offset: 0x10 */
    _IO uint32_t CR3;     /* Control Register 3            Offset: 0x14 */
    _IO uint32_t GTPR;    /* Guard Time & Prescaler        Offset: 0x18 */

} USART_RegDef_t;

/*********************************************************************
 *                  USART Register Pointers
 *********************************************************************/
#define USART1_REG ((USART_RegDef_t*)USART1_BASE)
#define USART2_REG ((USART_RegDef_t*)USART2_BASE)
#define USART3_REG ((USART_RegDef_t *)USART3_BASE)
#define UART4_REG  ((USART_RegDef_t *)UART4_BASE)
#define UART5_REG  ((USART_RegDef_t *)UART5_BASE)
#define USART6_REG ((USART_RegDef_t *)USART6_BASE)

/*********************************************************************
 *                  USART Instance
 *********************************************************************/
enum class USART_Instance{
    USART1,
    USART2,
    USART3,
    UART4,
    UART5,
    USART6
};
#endif /* USART_DRIVER_H_ */
/*********************************************************************
 *                  Word Length
 *********************************************************************/
enum class USART_WordLength{
  BITS_8 = 0,
  BITS_9
};
/*********************************************************************
 *                  Stop Bits
 *********************************************************************/
enum class USART_StopBits
{
    STOP_1 = 0,
    STOP_0_5,
    STOP_2,
    STOP_1_5
};
/*********************************************************************
 *                  Parity
 *********************************************************************/
enum class USART_Parity
{
    NONE = 0,
    EVEN,
    ODD
};
/*********************************************************************
 *                  Mode
 *********************************************************************/
enum class USART_Mode
{
    RX = 1,
    TX,
    TX_RX
};
/*********************************************************************
 *                  USART Driver Class
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
     * Constructor
     **************************************************************/
    void enableClock();
    /**************************************************************
     * Initialization
     **************************************************************/
    void init(uint32_t baud,
              USART_Mode mode,
              USART_WordLength wordLength = USART_WordLength::BITS_8,
              USART_StopBits stop_bits = USART_StopBits::STOP_1,
              USART_Parity parity = USART_Parity::NONE);
    /**************************************************************
     * USART Control
     **************************************************************/
      void enable();
      void disable();
    /**************************************************************
     * Data Transfer
     **************************************************************/
      void transmit(uint8_t data);
      uint8_t receive();
      void txString(const char *str);

};