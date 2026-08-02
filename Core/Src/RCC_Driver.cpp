/*
 * RCC_Driver.cpp
 *
 * STM32F407 Bare-Metal RCC Driver
 *
 * Created on: 18-Jul-2026
 * Author: jolap
 *
 * Purpose:
 *   Implementation of the STM32F407 Reset and Clock Control driver.
 *   The source file contains the register-level routines for clock-source
 *   selection, prescaler configuration, and peripheral-clock control.
 *
 * State Machine:
 *   Clock source enable -> ready wait -> prescaler setup -> PLL configuration
 *   -> system-clock switch -> clock-frequency refresh.
 *
 * RM0090 Reference:
 *   Chapter 7 - Reset and clock control (RCC)
 */

#include "RCC_Driver.h"
#include "TIM_Driver.h" // Include TIM_Driver.h to get TIM_Instance enum

/*********************************************************************
 *                      RCC Driver Design
 *
 * The STM32F407 Reset and Clock Control (RCC) peripheral is responsible
 * for managing the system clocks, peripheral clocks, and reset signals.
 * Proper clock configuration is the first and most critical step in

 * setting up the microcontroller, as all system and peripheral
 * operations depend on it.
 *
 * The RCC can select one of three main clock sources for the system:
 *      1. High-Speed Internal (HSI) oscillator (16 MHz)
 *      2. High-Speed External (HSE) oscillator (typically 8 MHz)
 *      3. Main Phase-Locked Loop (PLL)
 *
 * The PLL can be sourced from either HSI or HSE and is used to generate
 * high-frequency system clocks (up to 168 MHz). The driver provides
 * an interface to configure these sources and the bus prescalers (AHB,
 * APB1, APB2) to control the clock speeds of the CPU, memory, and
 * peripherals.
 *
 *********************************************************************
 *                      Clock Configuration State Machine
 *
 *            Enable Clock Source (HSI/HSE)
 *                         │
 *                         ▼
 *           Wait for Clock Source to be Ready
 *                (e.g., HSIRDY, HSERDY)
 *                         │
 *                         ▼
 *      Configure Bus Prescalers (AHB, APB1, APB2)
 *             (RCC_CFGR: HPRE, PPRE1, PPRE2)
 *                         │
 *                         ▼
 *      Configure Main PLL (if PLL is the source)
 *      (RCC_PLLCFGR: PLLM, PLLN, PLLP, PLLSRC)
 *                         │
 *                         ▼
 *                  Enable Main PLL
 *                    (RCC_CR: PLLON)
 *                         │
 *                         ▼
 *              Wait for PLL to be Ready
 *                   (RCC_CR: PLLRDY)
 *                         │
 *                         ▼
 *              Select System Clock Source
 *                    (RCC_CFGR: SW)
 *                         │
 *                         ▼
 *      Wait for System Clock Switch to Complete
 *                   (RCC_CFGR: SWS)
 *                         │
 *                         ▼
 *             Update Stored Clock Frequencies
 *                         │
 *                         ▼
 *                  System Clock Ready
 *
 *********************************************************************
 *
 * Driver Responsibilities
 * -----------------------
 * • Configure the main system clock source (HSI, HSE, PLL).
 * • Configure the prescalers for AHB, APB1, and APB2 buses.
 * • Enable and disable peripheral clocks (GPIO, USART, SPI, etc.).
 * • Provide functions to get the frequencies of the various system and
 *   peripheral clocks.
 * • Handle peripheral reset operations.
 *
 * RM0090 Reference:
 * Chapter 7 - Reset and Clock Control (RCC)
 *
 *********************************************************************/


// Define HSI and HSE frequencies. HSE value depends on the external crystal used.
constexpr uint32_t HSI_FREQ = 16000000U;
constexpr uint32_t HSE_FREQ = 8000000U; // Assuming an 8MHz external crystal

/*********************************************************************
 *                      RCC Register Bit Definitions
 *********************************************************************/
namespace RCC_Bits {
    // CR Register
    constexpr uint32_t CR_HSION_Pos      = 0;
    constexpr uint32_t CR_HSIRDY_Pos     = 1;
    constexpr uint32_t CR_HSEON_Pos      = 16;
    constexpr uint32_t CR_HSERDY_Pos     = 17;
    constexpr uint32_t CR_PLLON_Pos      = 24;
    constexpr uint32_t CR_PLLRDY_Pos     = 25;
    constexpr uint32_t CR_PLLI2SON_Pos   = 26;

    // PLLCFGR Register
    constexpr uint32_t PLLCFGR_PLLM_Pos  = 0;
    constexpr uint32_t PLLCFGR_PLLN_Pos  = 6;
    constexpr uint32_t PLLCFGR_PLLP_Pos  = 16;
    constexpr uint32_t PLLCFGR_PLLSRC_Pos = 22;
    constexpr uint32_t PLLCFGR_PLLQ_Pos  = 24;
    constexpr uint32_t PLLCFGR_PLLM_Mask = 0x3FU;
    constexpr uint32_t PLLCFGR_PLLN_Mask = 0x7FC0U;
    constexpr uint32_t PLLCFGR_PLLP_Mask = 0x30000U;
    constexpr uint32_t PLLCFGR_PLLQ_Mask = 0xF000000U;


    // CFGR Register
    constexpr uint32_t CFGR_SW_Pos       = 0;
    constexpr uint32_t CFGR_SWS_Pos      = 2;
    constexpr uint32_t CFGR_HPRE_Pos     = 4;
    constexpr uint32_t CFGR_PPRE1_Pos    = 10;
    constexpr uint32_t CFGR_PPRE2_Pos    = 13;
    constexpr uint32_t CFGR_SW_Mask      = 0x3U;
    constexpr uint32_t CFGR_SWS_Mask     = 0xCU;
    constexpr uint32_t CFGR_HPRE_Mask    = 0xF0U;
    constexpr uint32_t CFGR_PPRE1_Mask   = 0x1C00U;
    constexpr uint32_t CFGR_PPRE2_Mask   = 0xE000U;

    // CIR Register
    constexpr uint32_t CIR_CSSC_Pos      = 23;

    // APB2ENR Register
    constexpr uint32_t APB2ENR_SYSCFGEN_Pos = 14;

    // Clock Sources for SWS field in CFGR
    enum SWS_ClockSource : uint8_t {
        SWS_HSI = 0,
        SWS_HSE = 1,
        SWS_PLL = 2
    };

} // namespace RCC_Bits

/*********************************************************************
 *                      Static Frequency Storage
 *********************************************************************/
uint32_t RCC::mSystemClockFreq = HSI_FREQ;
uint32_t RCC::mAHBClockFreq     = HSI_FREQ;
uint32_t RCC::mAPB1ClockFreq    = HSI_FREQ;
uint32_t RCC::mAPB2ClockFreq    = HSI_FREQ;

/*********************************************************************
 *                      System Clock Configuration
 *
 * This function sets the main system clock (SYSCLK) source. It
 * handles enabling the selected oscillator (HSI, HSE, or PLL) and
 * waiting for it to stabilize before switching the system clock to it.
 *
 * @param source The desired clock source (HSI, HSE, or PLL).
 * @param pll_config Configuration for the PLL if it is selected as the
 *                   system clock source.
 *
 * RM0090 Reference:
 * - RCC_CR Register (HSION, HSERDY, PLLON, PLLRDY)
 * - RCC_CFGR Register (SW, SWS)
 * - RCC_PLLCFGR Register
 *********************************************************************/
void RCC::setSystemClockSource(ClockSource source, PLL_Config pll_config)
{
    if (source == ClockSource::HSE)
    {
        RCC_REG->CR |= (1U << RCC_Bits::CR_HSEON_Pos);
        while (!(RCC_REG->CR & (1U << RCC_Bits::CR_HSERDY_Pos)));
        RCC_REG->CFGR = (RCC_REG->CFGR & ~RCC_Bits::CFGR_SW_Mask) | (RCC_Bits::SWS_HSE << RCC_Bits::CFGR_SW_Pos);
        while (((RCC_REG->CFGR & RCC_Bits::CFGR_SWS_Mask) >> RCC_Bits::CFGR_SWS_Pos) != RCC_Bits::SWS_HSE);
    }
    else if (source == ClockSource::PLL)
    {
        if (pll_config.source == ClockSource::HSE)
        {
            RCC_REG->CR |= (1U << RCC_Bits::CR_HSEON_Pos);
            while (!(RCC_REG->CR & (1U << RCC_Bits::CR_HSERDY_Pos)));
            RCC_REG->PLLCFGR |= (1U << RCC_Bits::PLLCFGR_PLLSRC_Pos);
        }
        else // HSI
        {
            RCC_REG->CR |= (1U << RCC_Bits::CR_HSION_Pos);
            while (!(RCC_REG->CR & (1U << RCC_Bits::CR_HSIRDY_Pos)));
            RCC_REG->PLLCFGR &= ~(1U << RCC_Bits::PLLCFGR_PLLSRC_Pos);
        }

        RCC_REG->PLLCFGR = (RCC_REG->PLLCFGR & ~(RCC_Bits::PLLCFGR_PLLM_Mask | RCC_Bits::PLLCFGR_PLLN_Mask | RCC_Bits::PLLCFGR_PLLP_Mask | RCC_Bits::PLLCFGR_PLLQ_Mask)) |
                       ((pll_config.M << RCC_Bits::PLLCFGR_PLLM_Pos) |
                        (pll_config.N << RCC_Bits::PLLCFGR_PLLN_Pos) |
                        (pll_config.P << RCC_Bits::PLLCFGR_PLLP_Pos) |
                        (pll_config.Q << RCC_Bits::PLLCFGR_PLLQ_Pos));

        RCC_REG->CR |= (1U << RCC_Bits::CR_PLLON_Pos);
        while (!(RCC_REG->CR & (1U << RCC_Bits::CR_PLLRDY_Pos)));

        RCC_REG->CFGR = (RCC_REG->CFGR & ~RCC_Bits::CFGR_SW_Mask) | (RCC_Bits::SWS_PLL << RCC_Bits::CFGR_SW_Pos);
        while (((RCC_REG->CFGR & RCC_Bits::CFGR_SWS_Mask) >> RCC_Bits::CFGR_SWS_Pos) != RCC_Bits::SWS_PLL);
    }
    else // HSI
    {
        RCC_REG->CR |= (1U << RCC_Bits::CR_HSION_Pos);
        while (!(RCC_REG->CR & (1U << RCC_Bits::CR_HSIRDY_Pos)));
        RCC_REG->CFGR &= ~RCC_Bits::CFGR_SW_Mask;
        while (((RCC_REG->CFGR & RCC_Bits::CFGR_SWS_Mask) >> RCC_Bits::CFGR_SWS_Pos) != RCC_Bits::SWS_HSI);
    }

    updateClockFrequencies();
}

/*********************************************************************
 *                      Bus Prescaler Configuration
 *
 * This function configures the division factors for the AHB, APB1,
 * and APB2 buses. These prescalers control the clock speeds of the
 * peripherals connected to these buses.
 *
 * @param ahb AHB prescaler value.
 * @param apb1 APB1 prescaler value.
 * @param apb2 APB2 prescaler value.
 *
 * RM0090 Reference:
 * - RCC_CFGR Register (HPRE, PPRE1, PPRE2)
 *********************************************************************/
void RCC::configurePrescalers(AHBPrescaler ahb, APBPrescaler apb1, APBPrescaler apb2)
{
    uint32_t temp_cfgr = RCC_REG->CFGR;
    temp_cfgr &= ~(RCC_Bits::CFGR_HPRE_Mask | RCC_Bits::CFGR_PPRE1_Mask | RCC_Bits::CFGR_PPRE2_Mask);
    temp_cfgr |= (static_cast<uint32_t>(ahb) << RCC_Bits::CFGR_HPRE_Pos) |
                 (static_cast<uint32_t>(apb1) << RCC_Bits::CFGR_PPRE1_Pos) |
                 (static_cast<uint32_t>(apb2) << RCC_Bits::CFGR_PPRE2_Pos);
    RCC_REG->CFGR = temp_cfgr;
    updateClockFrequencies();
}

/*********************************************************************
 *                      Frequency Calculation & Getters
 *********************************************************************/

/*********************************************************************
 *                  Update Clock Frequencies
 *
 * This private helper function recalculates the frequencies of the
 * system clock, AHB, and APB buses based on the current RCC register
 * settings. It should be called after any change in clock source or
 * prescalers.
 *
 *********************************************************************/
void RCC::updateClockFrequencies()
{
    const uint16_t ahb_prescaler[] = {1, 1, 1, 1, 1, 1, 1, 1, 2, 4, 8, 16, 64, 128, 256, 512};
    const uint8_t apb_prescaler[] = {1, 1, 1, 1, 2, 4, 8, 16};
    const uint8_t pllp_values[] = {2, 4, 6, 8};

    uint8_t clk_source = (RCC_REG->CFGR >> RCC_Bits::CFGR_SWS_Pos) & RCC_Bits::CFGR_SW_Mask;
    switch(clk_source)
    {
        case RCC_Bits::SWS_HSI:
            mSystemClockFreq = HSI_FREQ;
            break;
        case RCC_Bits::SWS_HSE:
            mSystemClockFreq = HSE_FREQ;
            break;
        case RCC_Bits::SWS_PLL:
        {
            uint32_t pll_input_freq = (RCC_REG->PLLCFGR & (1U << RCC_Bits::PLLCFGR_PLLSRC_Pos)) ? HSE_FREQ : HSI_FREQ;
            uint32_t pll_m = RCC_REG->PLLCFGR & RCC_Bits::PLLCFGR_PLLM_Mask;
            uint32_t pll_n = (RCC_REG->PLLCFGR & RCC_Bits::PLLCFGR_PLLN_Mask) >> RCC_Bits::PLLCFGR_PLLN_Pos;
            uint32_t pll_p_idx = (RCC_REG->PLLCFGR & RCC_Bits::PLLCFGR_PLLP_Mask) >> RCC_Bits::PLLCFGR_PLLP_Pos;
            uint32_t pll_p = pllp_values[pll_p_idx];
            mSystemClockFreq = ((pll_input_freq / pll_m) * pll_n) / pll_p;
            break;
        }
    }

    uint8_t hpre_val = (RCC_REG->CFGR >> RCC_Bits::CFGR_HPRE_Pos) & 0xF;
    mAHBClockFreq = mSystemClockFreq / ahb_prescaler[hpre_val];

    uint8_t ppre1_val = (RCC_REG->CFGR >> RCC_Bits::CFGR_PPRE1_Pos) & 0x7;
    mAPB1ClockFreq = mAHBClockFreq / apb_prescaler[ppre1_val];

    uint8_t ppre2_val = (RCC_REG->CFGR >> RCC_Bits::CFGR_PPRE2_Pos) & 0x7;
    mAPB2ClockFreq = mAHBClockFreq / apb_prescaler[ppre2_val];
}

uint32_t RCC::getSystemClockFreq() { return mSystemClockFreq; }
uint32_t RCC::getAHBClockFreq()    { return mAHBClockFreq; }
uint32_t RCC::getAPB1ClockFreq()    { return mAPB1ClockFreq; }
uint32_t RCC::getAPB2ClockFreq()    { return mAPB2ClockFreq; }
uint32_t RCC::getAHBFreq() { return getAHBClockFreq(); }
uint32_t RCC::getAPB1Freq() { return getAPB1ClockFreq(); }
uint32_t RCC::getAPB2Freq() { return getAPB2ClockFreq(); }

uint32_t RCC::getUSARTClock(RCC_USART usart)
{
    switch (usart)
    {
        case RCC_USART::USART1:
        case RCC_USART::USART6:
            return getAPB2ClockFreq();
        default:
            return getAPB1ClockFreq();
    }
}

uint32_t RCC::getSPIClock(RCC_SPI spi)
{
    switch (spi)
    {
        case RCC_SPI::SPI1:
            return getAPB2ClockFreq();
        default:
            return getAPB1ClockFreq();
    }
}

uint32_t RCC::getI2CClock(RCC_I2C i2c)
{
    (void)i2c;
    return getAPB1ClockFreq();
}

uint32_t RCC::getTIMClock(RCC_TIM tim)
{
    switch (tim)
    {
        case RCC_TIM::TIM1:
        case RCC_TIM::TIM8:
        case RCC_TIM::TIM9:
        case RCC_TIM::TIM10:
        case RCC_TIM::TIM11:
            return getAPB2ClockFreq();
        default:
            return getAPB1ClockFreq();
    }
}

uint32_t RCC::getTimerClockFreq(TIM_Instance tim)
{
    bool is_apb1_timer = false;
    switch(tim)
    {
        case TIM_Instance::TIM2:
        case TIM_Instance::TIM3:
        case TIM_Instance::TIM4:
        case TIM_Instance::TIM5:
       /* case TIM_Instance::TIM6:
        case TIM_Instance::TIM7:
        case TIM_Instance::TIM12:
        case TIM_Instance::TIM13:
        case TIM_Instance::TIM14:*/
            is_apb1_timer = true;
            break;
        default:
            is_apb1_timer = false;
            break;
    }
    
    if (is_apb1_timer)
    {
        // If APB1 prescaler is 1, timer clock is APB1 clock. Otherwise, it's doubled.
        // See RM0090, Figure 12. Clock tree.
        uint8_t ppre1_val = (RCC_REG->CFGR >> RCC_Bits::CFGR_PPRE1_Pos) & 0x7;
        if (ppre1_val < 4) // APB1 prescaler is 1
        {
            return mAPB1ClockFreq;
        }
        else
        {
            return mAPB1ClockFreq * 2;
        }
    }
    else // APB2 Timers
    {
        // If APB2 prescaler is 1, timer clock is APB2 clock. Otherwise, it's doubled.
        // See RM0090, Figure 12. Clock tree.
        uint8_t ppre2_val = (RCC_REG->CFGR >> RCC_Bits::CFGR_PPRE2_Pos) & 0x7;
         if (ppre2_val < 4) // APB2 prescaler is 1
        {
            return mAPB2ClockFreq;
        }
        else
        {
            return mAPB2ClockFreq * 2;
        }
    }
}

/*********************************************************************
 *                      Peripheral Clock/Reset Control
 *********************************************************************/
void RCC::enableGPIOClock(RCC_GPIO port) { RCC_REG->AHB1ENR |= (1U << static_cast<uint32_t>(port)); }
void RCC::disableGPIOClock(RCC_GPIO port) { RCC_REG->AHB1ENR &= ~(1U << static_cast<uint32_t>(port)); }
void RCC::resetGPIO(RCC_GPIO port)
{
	RCC_REG->AHB1RSTR |= (1U << static_cast<uint32_t>(port));
	RCC_REG->AHB1RSTR &= ~(1U << static_cast<uint32_t>(port));
}

void RCC::enableUSARTClock(RCC_USART usart)
{
    switch (usart) {
        case RCC_USART::USART1: RCC_REG->APB2ENR |= (1U << 4); break;
        case RCC_USART::USART2: RCC_REG->APB1ENR |= (1U << 17); break;
        case RCC_USART::USART3: RCC_REG->APB1ENR |= (1U << 18); break;
        case RCC_USART::UART4:  RCC_REG->APB1ENR |= (1U << 19); break;
        case RCC_USART::UART5:  RCC_REG->APB1ENR |= (1U << 20); break;
        case RCC_USART::USART6: RCC_REG->APB2ENR |= (1U << 5); break;
    }
}

void RCC::disableUSARTClock(RCC_USART usart)
{
    switch (usart) {
        case RCC_USART::USART1: RCC_REG->APB2ENR &= ~(1U << 4); break;
        case RCC_USART::USART2: RCC_REG->APB1ENR &= ~(1U << 17); break;
        case RCC_USART::USART3: RCC_REG->APB1ENR &= ~(1U << 18); break;
        case RCC_USART::UART4:  RCC_REG->APB1ENR &= ~(1U << 19); break;
        case RCC_USART::UART5:  RCC_REG->APB1ENR &= ~(1U << 20); break;
        case RCC_USART::USART6: RCC_REG->APB2ENR &= ~(1U << 5); break;
    }
}

void RCC::enableI2CClock(RCC_I2C i2c)
{
    switch(i2c) {
        case RCC_I2C::I2C1: RCC_REG->APB1ENR |= (1U << 21); break;
        case RCC_I2C::I2C2: RCC_REG->APB1ENR |= (1U << 22); break;
        case RCC_I2C::I2C3: RCC_REG->APB1ENR |= (1U << 23); break;
    }
}

void RCC::disableI2CClock(RCC_I2C i2c)
{
    switch(i2c) {
        case RCC_I2C::I2C1: RCC_REG->APB1ENR &= ~(1U << 21); break;
        case RCC_I2C::I2C2: RCC_REG->APB1ENR &= ~(1U << 22); break;
        case RCC_I2C::I2C3: RCC_REG->APB1ENR &= ~(1U << 23); break;
    }
}

void RCC::enableSPIClock(RCC_SPI spi)
{
    switch (spi) {
        case RCC_SPI::SPI1: RCC_REG->APB2ENR |= (1U << 12); break;
        case RCC_SPI::SPI2: RCC_REG->APB1ENR |= (1U << 14); break;
        case RCC_SPI::SPI3: RCC_REG->APB1ENR |= (1U << 15); break;
    }
}

void RCC::disableSPIClock(RCC_SPI spi)
{
    switch (spi) {
        case RCC_SPI::SPI1: RCC_REG->APB2ENR &= ~(1U << 12); break;
        case RCC_SPI::SPI2: RCC_REG->APB1ENR &= ~(1U << 14); break;
        case RCC_SPI::SPI3: RCC_REG->APB1ENR &= ~(1U << 15); break;
    }
}

void RCC::enableADCClock(RCC_ADC adc)
{
    switch(adc) {
       case RCC_ADC::ADC1: RCC_REG->APB2ENR |= (1U << 8); break;
       case RCC_ADC::ADC2: RCC_REG->APB2ENR |= (1U << 9); break;
       case RCC_ADC::ADC3: RCC_REG->APB2ENR |= (1U << 10); break;
    }
}

void RCC::disableADCClock(RCC_ADC adc)
{
    switch(adc) {
        case RCC_ADC::ADC1: RCC_REG->APB2ENR &= ~(1U << 8); break;
        case RCC_ADC::ADC2: RCC_REG->APB2ENR &= ~(1U << 9); break;
        case RCC_ADC::ADC3: RCC_REG->APB2ENR &= ~(1U << 10); break;
    }
}

void RCC::enableTIMClock(RCC_TIM tim)
{
    switch (tim) {
        case RCC_TIM::TIM1:  RCC_REG->APB2ENR |= (1U << 0); break;
        case RCC_TIM::TIM2:  RCC_REG->APB1ENR |= (1U << 0); break;
        case RCC_TIM::TIM3:  RCC_REG->APB1ENR |= (1U << 1); break;
        case RCC_TIM::TIM4:  RCC_REG->APB1ENR |= (1U << 2); break;
        case RCC_TIM::TIM5:  RCC_REG->APB1ENR |= (1U << 3); break;
        case RCC_TIM::TIM6:  RCC_REG->APB1ENR |= (1U << 4); break;
        case RCC_TIM::TIM7:  RCC_REG->APB1ENR |= (1U << 5); break;
        case RCC_TIM::TIM8:  RCC_REG->APB2ENR |= (1U << 1); break;
        case RCC_TIM::TIM9:  RCC_REG->APB2ENR |= (1U << 16); break;
        case RCC_TIM::TIM10: RCC_REG->APB2ENR |= (1U << 17); break;
        case RCC_TIM::TIM11: RCC_REG->APB2ENR |= (1U << 18); break;
        case RCC_TIM::TIM12: RCC_REG->APB1ENR |= (1U << 6); break;
        case RCC_TIM::TIM13: RCC_REG->APB1ENR |= (1U << 7); break;
        case RCC_TIM::TIM14: RCC_REG->APB1ENR |= (1U << 8); break;
    }
}

void RCC::disableTIMClock(RCC_TIM tim)
{
    switch (tim) {
        case RCC_TIM::TIM1:  RCC_REG->APB2ENR &= ~(1U << 0); break;
        case RCC_TIM::TIM2:  RCC_REG->APB1ENR &= ~(1U << 0); break;
        case RCC_TIM::TIM3:  RCC_REG->APB1ENR &= ~(1U << 1); break;
        case RCC_TIM::TIM4:  RCC_REG->APB1ENR &= ~(1U << 2); break;
        case RCC_TIM::TIM5:  RCC_REG->APB1ENR &= ~(1U << 3); break;
        case RCC_TIM::TIM6:  RCC_REG->APB1ENR &= ~(1U << 4); break;
        case RCC_TIM::TIM7:  RCC_REG->APB1ENR &= ~(1U << 5); break;
        case RCC_TIM::TIM8:  RCC_REG->APB2ENR &= ~(1U << 1); break;
        case RCC_TIM::TIM9:  RCC_REG->APB2ENR &= ~(1U << 16); break;
        case RCC_TIM::TIM10: RCC_REG->APB2ENR &= ~(1U << 17); break;
        case RCC_TIM::TIM11: RCC_REG->APB2ENR &= ~(1U << 18); break;
        case RCC_TIM::TIM12: RCC_REG->APB1ENR &= ~(1U << 6); break;
        case RCC_TIM::TIM13: RCC_REG->APB1ENR &= ~(1U << 7); break;
        case RCC_TIM::TIM14: RCC_REG->APB1ENR &= ~(1U << 8); break;
    }
}
void RCC::resetTIMClock(RCC_TIM tim)
{
    // Note: Resetting is a brief operation.
    switch(tim) {
        case RCC_TIM::TIM1:
            RCC_REG->APB2RSTR |= (1U << 0);
            RCC_REG->APB2RSTR &= ~(1U << 0);
            break;
        case RCC_TIM::TIM2:
            RCC_REG->APB1RSTR |= (1U << 0);
            RCC_REG->APB1RSTR &= ~(1U << 0);
            break;
        // Add other timers here following the same pattern
        default:
            break;
    }
}
bool RCC::isTIMClockEnabled(RCC_TIM tim)
{
    switch (tim) {
        case RCC_TIM::TIM1:  return (RCC_REG->APB2ENR & (1U << 0));
        case RCC_TIM::TIM2:  return (RCC_REG->APB1ENR & (1U << 0));
        // Add other timers here
        default:
         return false;
    }
}

void RCC::enableDMAClock(RCC_DMA dma)
{
	switch (dma) {
	    case RCC_DMA::DMA1: RCC_REG->AHB1ENR |= (1U << 21); break;
	    case RCC_DMA::DMA2: RCC_REG->AHB1ENR |= (1U << 22); break;
	}
}

void RCC::disableDMAClock(RCC_DMA dma)
{
	switch (dma) {
	    case RCC_DMA::DMA1: RCC_REG->AHB1ENR &= ~(1U << 21); break;
	    case RCC_DMA::DMA2: RCC_REG->AHB1ENR &= ~(1U << 22); break;
	}
}

void RCC::enableSYSCFGClock()
{
    RCC_REG->APB2ENR |= (1U << RCC_Bits::APB2ENR_SYSCFGEN_Pos);
}
