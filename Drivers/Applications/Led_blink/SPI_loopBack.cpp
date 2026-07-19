/*
 * SPI_Loopback.cpp
 *
 * Created on: 19-Jul-2026
 * Author: jolap
 */

#include "GPIO_Driver.h"
#include "SPI_Driver.h"
#include "SysTick_Driver.h"

int main()
{
    /**************************************************************
     * Configure GPIO
     **************************************************************/

    GPIO sck(GPIO_Port::PORTA, 5);
    GPIO miso(GPIO_Port::PORTA, 6);
    GPIO mosi(GPIO_Port::PORTA, 7);

    sck.clockEnable();
    miso.clockEnable();
    mosi.clockEnable();

    sck.setMode(GPIO_Mode::ALTERNATE_FUNCTION);
    miso.setMode(GPIO_Mode::ALTERNATE_FUNCTION);
    mosi.setMode(GPIO_Mode::ALTERNATE_FUNCTION);

    /*
     * SPI1 uses AF5
     */
    sck.setAlternateFunction(5);
    miso.setAlternateFunction(5);
    mosi.setAlternateFunction(5);

    sck.setSpeed(GPIO_Speed::VERY_HIGH);
    miso.setSpeed(GPIO_Speed::VERY_HIGH);
    mosi.setSpeed(GPIO_Speed::VERY_HIGH);

    /**************************************************************
     * Configure SysTick
     **************************************************************/

    SysTick tick;
    tick.init(1000);

    /**************************************************************
     * Configure SPI1
     **************************************************************/

    SPI spi(SPI_Instance::SPI1);

    spi.init(
            SPI_Mode::MASTER,
            SPI_BaudRate::DIV16,
            SPI_ClockPolarity::LOW,
            SPI_ClockPhase::FIRST_EDGE,
            SPI_DataSize::BITS_8,
            SPI_SSM::ENABLE);

    /**************************************************************
     * Test Variables
     **************************************************************/

    uint8_t tx = 0x55;
    uint8_t rx = 0;

    /**************************************************************
     * Loopback Test
     **************************************************************/

    while (1)
    {
        spi.transmit(tx);

        rx = spi.receive();

        /*
         * If loopback is correct:
         * rx should equal tx
         */

        if (rx == tx)
        {
            tx++;
        }

        tick.delayMs(500);
    }
}