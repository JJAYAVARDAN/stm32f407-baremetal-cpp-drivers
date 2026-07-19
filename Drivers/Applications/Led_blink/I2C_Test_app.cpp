/*
 * I2C_Scanner.cpp
 *
 * Created on: 19-Jul-2026
 * Author: jolap
 */

#include "GPIO_Driver.h"
#include "I2C_Driver.h"
#include "USART_Driver.h"
#include "SysTick_Driver.h"

int main()
{
    /**************************************************************
     * Configure GPIO
     *
     * I2C1
     * SCL -> PB6
     * SDA -> PB7
     **************************************************************/

    GPIO scl(GPIO_Port::PORTB, 6);
    GPIO sda(GPIO_Port::PORTB, 7);

    scl.clockEnable();
    sda.clockEnable();

    scl.setMode(GPIO_Mode::ALTERNATE_FUNCTION);
    sda.setMode(GPIO_Mode::ALTERNATE_FUNCTION);

    scl.setAlternateFunction(4);
    sda.setAlternateFunction(4);

    scl.setOutputType(GPIO_OutputType::OPEN_DRAIN);
    sda.setOutputType(GPIO_OutputType::OPEN_DRAIN);

    scl.setPull(GPIO_Pull::PULL_UP);
    sda.setPull(GPIO_Pull::PULL_UP);

    scl.setSpeed(GPIO_Speed::VERY_HIGH);
    sda.setSpeed(GPIO_Speed::VERY_HIGH);

    /**************************************************************
     * USART2
     **************************************************************/

    GPIO tx(GPIO_Port::PORTA,2);
    GPIO rx(GPIO_Port::PORTA,3);

    tx.clockEnable();
    rx.clockEnable();

    tx.setMode(GPIO_Mode::ALTERNATE_FUNCTION);
    rx.setMode(GPIO_Mode::ALTERNATE_FUNCTION);

    tx.setAlternateFunction(7);
    rx.setAlternateFunction(7);

    USART uart(USART_Instance::USART2);

    uart.init(115200,
              USART_Mode::TX_RX,
              USART_WordLength::BITS_8,
              USART_StopBits::STOP_1,
              USART_Parity::NONE);

    /**************************************************************
     * SysTick
     **************************************************************/

    SysTick tick;
    tick.init(1000);

    /**************************************************************
     * I2C1
     **************************************************************/

    I2C i2c(I2C_Instance::I2C1);

    i2c.init(100000);

    /**************************************************************
     * Scan Bus
     **************************************************************/

    while(1)
    {
        for(uint8_t addr=1; addr<127; addr++)
        {
            i2c.generateStart();

            i2c.sendAddress(addr,false);

            if(i2c.isAddressSent())
            {
                uart.txString("Device Found\r\n");
            }

            i2c.generateStop();

            tick.delayMs(5);
        }

        tick.delayMs(1000);
    }
}
