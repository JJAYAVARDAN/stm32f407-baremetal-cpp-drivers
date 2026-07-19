/*
 * USART_Loopback.cpp
 *
 *  Created on: 19-Jul-2026
 *      Author: jolap
 */

#include "GPIO_Driver.h"
#include "USART_Driver.h"
#include "SysTick_Driver.h"

int main()
{
    /**************************************************************
     * Configure GPIO
     **************************************************************/

    GPIO tx(GPIO_Port::PORTA, 2);
    GPIO rx(GPIO_Port::PORTA, 3);

    tx.clockEnable();
    rx.clockEnable();

    tx.setMode(GPIO_Mode::ALTERNATE_FUNCTION);
    rx.setMode(GPIO_Mode::ALTERNATE_FUNCTION);

    tx.setAlternateFunction(7);      // AF7 = USART2
    rx.setAlternateFunction(7);

    tx.setOutputType(GPIO_OutputType::PUSH_PULL);
    tx.setSpeed(GPIO_Speed::VERY_HIGH);
    tx.setPull(GPIO_Pull::PULL_UP);

    rx.setPull(GPIO_Pull::PULL_UP);

    /**************************************************************
     * Configure SysTick
     **************************************************************/

    SysTick tick;
    tick.init(1000);

    /**************************************************************
     * Configure USART2
     **************************************************************/

    USART usart(USART_Instance::USART2);

    usart.init(115200,
               USART_Mode::TX_RX,
               USART_WordLength::BITS_8,
               USART_StopBits::STOP_1,
               USART_Parity::NONE);

    /**************************************************************
     * Loopback Test
     **************************************************************/

    while (1)
    {



        uint8_t rxData = usart.receive();
        usart.transmit(rxData);


        tick.delayMs(1000);
    }
}
