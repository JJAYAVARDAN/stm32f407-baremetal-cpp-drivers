#include "Gpio_Driver.h"
#include "SysTick_Driver.h"
#include "EXTI_Driver.h"

extern "C" void EXTI0_IRQHandler(void)
{
    if (EXTI_REG->PR & (1U << 0))
    {
        GPIOD->ODR ^= (1U << 12);
        EXTI_REG->PR |= (1U << 0);
    }
}

int main()
{
    /* Initialize SysTick */
    SysTick sysTick;
    sysTick.init(1000);

    /* Configure LED */
    GPIO led(GPIOD, 12);
    led.setMode(GPIO_Mode::OUTPUT);
    led.setOutputType(GPIO_OutputType::PUSH_PULL);
    led.setSpeed(GPIO_Speed::HIGH);
    led.setPull(GPIO_Pull::NO_PULL);

    /* Configure input button on PA0 */
    GPIO button(GPIOA, 0);
    button.setMode(GPIO_Mode::INPUT);
    button.setPull(GPIO_Pull::PULL_DOWN);

    /* Configure EXTI */
    EXTI exti(RCC_GPIO::PORTA, 0);
    exti.configureTrigger(EXTI_Trigger::RISING);
    exti.enableInterrupt();

    /* Enable EXTI0 interrupt in NVIC */
    *(volatile uint32_t *)0xE000E100U |= (1U << 6);

    while (1)
    {
        __asm volatile("wfi");
        sysTick.delayMs(10);
    }
}
