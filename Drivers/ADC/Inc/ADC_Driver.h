/*
 * ADC_Driver.h
 *
 * STM32F407 Bare-Metal ADC Driver
 *
 * Created on: 21-Jul-2026
 * Author: jolap
 *
 * Purpose:
 *   Register-level driver for the STM32F407 Analog-to-Digital Converter.
 *   The driver exposes APIs to initialize conversion settings, select
 *   channels, and start acquisitions.
 *
 * RM0090 Reference:
 *   Chapter 13 - Analog-to-digital converter (ADC)
 */

#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_
#include <cstdint>
#include "RCC_Driver.h"

#define _IO volatile

/*********************************************************************
 * Peripheral Base Addresses
 *********************************************************************/
#define ADC1_BASE 0x40012000UL
#define ADC2_BASE 0x40012100UL
#define ADC3_BASE 0x40012200UL

/*********************************************************************
 * Register Definitions
 *********************************************************************/
typedef struct
{
    _IO uint32_t SR;
    _IO uint32_t CR1;
    _IO uint32_t CR2;
    _IO uint32_t SMPR1;
    _IO uint32_t SMPR2;
    _IO uint32_t JOFR1;
    _IO uint32_t JOFR2;
    _IO uint32_t JOFR3;
    _IO uint32_t JOFR4;
    _IO uint32_t HTR;
    _IO uint32_t LTR;
    _IO uint32_t SQR1;
    _IO uint32_t SQR2;
    _IO uint32_t SQR3;
    _IO uint32_t JSQR;
    _IO uint32_t JDR1;
    _IO uint32_t JDR2;
    _IO uint32_t JDR3;
    _IO uint32_t JDR4;
    _IO uint32_t DR;
} ADC_RegDef_t;

/*********************************************************************
 * Register Structure
 *********************************************************************/
#define ADC1_REG ((ADC_RegDef_t *)ADC1_BASE)
#define ADC2_REG ((ADC_RegDef_t *)ADC2_BASE)
#define ADC3_REG ((ADC_RegDef_t *)ADC3_BASE)

/*********************************************************************
 * Enumerations
 *********************************************************************/
enum class ADC_Instance
{
    ADC1,
    ADC2,
    ADC3
};

enum class ADC_Resolution
{
    BITS_12 = 0,
    BITS_10,
    BITS_8,
    BITS_6
};

enum class ADC_Alignment
{
    RIGHT = 0,
    LEFT
};

enum class ADC_MODE
{
    SINGLE,
    CONTINOUS
};

enum class ADC_Channel
{
    CH0 = 0,
    CH1,
    CH2,
    CH3,
    CH4,
    CH5,
    CH6,
    CH7,
    CH8,
    CH9,
    CH10,
    CH11,
    CH12,
    CH13,
    CH14,
    CH15,
    CH16,
    CH17,
    CH18
};

enum class ADC_SampleTime
{
    CYCLES_3 = 0,
    CYCLES_15,
    CYCLES_28,
    CYCLES_56,
    CYCLES_84,
    CYCLES_112,
    CYCLES_144,
    CYCLES_480
};

/*********************************************************************
 * Driver Class
 *********************************************************************/
class ADC
{
private:
    ADC_RegDef_t* mADC;

public:
    /**************************************************************
     * Constructor
     **************************************************************/
    ADC(ADC_Instance instance);

    /**************************************************************
     * Clock APIs
     **************************************************************/
    /**
     * @brief Enables the clock for the selected ADC peripheral.
     */
    void enableClock();

    /**************************************************************
     * Initialization APIs
     **************************************************************/
    /**
     * @brief Configures ADC resolution, alignment, and conversion mode.
     * @param resolution ADC resolution selection.
     * @param alignment ADC data alignment selection.
     * @param mode Single or continuous conversion mode.
     */
    void init(ADC_Resolution resolution,
              ADC_Alignment alignment,
              ADC_MODE mode);

    /**************************************************************
     * Control APIs
     **************************************************************/
    /**
     * @brief Enables the ADC peripheral.
     */
    void enable();

    /**
     * @brief Disables the ADC peripheral.
     */
    void disable();

    /**
     * @brief Selects the active conversion channel.
     * @param channel ADC input channel identifier.
     */
    void selectChannel(ADC_Channel channel);

    /**
     * @brief Sets the sample time for a selected channel.
     * @param channel ADC input channel identifier.
     * @param sampletime Sample time value in ADC cycles.
     */
    void setSampleTime(ADC_Channel channel,
                       ADC_SampleTime sampletime);

    /**************************************************************
     * Conversion APIs
     **************************************************************/
    /**
     * @brief Starts a conversion sequence on the selected channel.
     */
    void startConversion();

    bool conversionComplete();

    uint16_t read();
    
    void stopConversion();


};

#endif /* ADC_DRIVER_H_ */
