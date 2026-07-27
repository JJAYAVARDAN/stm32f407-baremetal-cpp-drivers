/*
 * Driver_TestApp.cpp
 *
 * STM32F407 Bare Metal Driver Test
 *
 * Test:
 * RCC
 * GPIO
 * USART
 * SPI
 * I2C
 * ADC
 * TIM
 * EXTI
 *
 */


#include "RCC_Driver.h"
#include "Gpio_Driver.h"
#include "USART_Driver.h"
#include "SPI_Driver.h"
#include "I2C_Driver.h"
#include "ADC_Driver.h"
#include "TIM_Driver.h"
#include "EXTI_Driver.h"
#include "SysTick_Driver.h"
#include "NVIC_Driver.h"



int main()
{


/************************************************************
 * RCC TEST
 ************************************************************/

RCC rcc;
(void)rcc;


/************************************************************
 * GPIO TEST
 *
 * STM32F407 Discovery LED:
 *
 * PD12 Green LED
 ************************************************************/

GPIO led(
        GPIO_Port::PORTD,
        12
);


led.clockEnable();


led.setMode(
        GPIO_Mode::OUTPUT
);


led.setOutputType(
        GPIO_OutputType::PUSH_PULL
);


led.setSpeed(
        GPIO_Speed::HIGH
);


led.setPull(
        GPIO_Pull::NO_PULL
);



/************************************************************
 * SYSTICK TEST
 ************************************************************/

SysTick systick;

systick.init(1000);



/************************************************************
 * USART TEST
 *
 * USART2 PA2 TX
 *
 ************************************************************/

USART usart(
        USART_Instance::USART2
);


usart.enableClock();


usart.init(
        115200,
        USART_Mode::TX_RX
);


usart.enable();


usart.txString(
        "STM32 Driver Test Started\r\n"
);



/************************************************************
 * SPI TEST
 *
 ************************************************************/


SPI spi(
        SPI_Instance::SPI1
);


spi.enableClock();


spi.init(
        SPI_Mode::MASTER,
        SPI_BaudRate::DIV8,
        SPI_ClockPolarity::LOW,
        SPI_ClockPhase::FIRST_EDGE,
        SPI_DataSize::BITS_8,
        SPI_SSM::ENABLE
);


spi.enable();


spi.transmit(0x55);



/************************************************************
 * I2C TEST
 *
 ************************************************************/


I2C i2c(
        I2C_Instance::I2C1
);


i2c.enableClock();


i2c.init(
        100000,
        I2C_ACK::ENABLE
);


i2c.enable();



uint8_t txData = 0xAA;


i2c.masterTransmit(
        0x50,
        &txData,
        1
);



/************************************************************
 * ADC TEST
 *
 * ADC1 Channel 0
 *
 ************************************************************/


ADC adc(
        ADC_Instance::ADC1
);


adc.enableClock();


adc.init(
        ADC_Resolution::BITS_12,
        ADC_Alignment::RIGHT,
        ADC_MODE::SINGLE
);


adc.selectChannel(
        ADC_Channel::CH0
);


adc.setSampleTime(
        ADC_Channel::CH0,
        ADC_SampleTime::CYCLES_84
);


adc.enable();



/************************************************************
 * TIMER TEST
 *
 * TIM2 LED blink delay
 *
 ************************************************************/


TIM timer(
        TIM_Instance::TIM2
);


timer.enableClock();


timer.init(
        TIM_CounterMode::UP
);



/*
 * Timer clock:
 *
 * Assume APB1 Timer clock = 84MHz
 *
 * Prescaler:
 *
 * 84MHz / 8400 = 10KHz
 *
 */


timer.setPrescaler(
        8400-1
);



/*
 * ARR:
 *
 * 10KHz / 10000
 *
 * = 1 second
 *
 */


timer.setAutoReload(
        10000-1
);


timer.start();



/************************************************************
 * EXTI TEST
 *
 * User Button PA0
 *
 ************************************************************/


EXTI button(
        RCC_GPIO::PORTA,
        0
);


button.SYSCFGClock();


button.configureEXTILine();


button.setTrigger(
        EXTI_Trigger::RISING
);


button.enableInterrupt();



/************************************************************
 * NVIC TEST
 ************************************************************/


NVIC nvic;

(void)nvic;
/*
 * Enable required IRQ here
 *
 * According to your NVIC API
 *
 */



/************************************************************
 * MAIN LOOP
 ************************************************************/


while(1)
{


    /*
     * Timer update event
     *
     * Toggle LED every 1 second
     */


    if(timer.isUpdateEvent())
    {

        timer.clearUpdateEvent();


        led.toggle();


        usart.txString(
                "Timer Event\r\n"
        );

    }



    /*
     * ADC conversion example
     */


    adc.startConversion();


    if(adc.conversionComplete())
    {

        uint16_t value = adc.read();


        (void)value;

    }



}



return 0;

}
