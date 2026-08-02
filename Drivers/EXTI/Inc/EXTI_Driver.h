/*
 * EXTI_Driver.h
 *
 * STM32F407 Bare-Metal EXTI Driver
 *
 * Created on: 19-Jul-2026
 * Author: jolap
 *
 * Purpose:
 *   Register-level driver for the STM32F407 External Interrupt/Event
 *   Controller. The class exposes APIs to map GPIO inputs to EXTI lines,
 *   configure edge triggers, and manage interrupt/event requests.
 *
 * RM0090 Reference:
 *   Chapter 12 - External interrupt/event controller (EXTI)
 */

#ifndef EXTI_DRIVER_H_
#define EXTI_DRIVER_H_

#include <cstdint>
#include "RCC_Driver.h"
#include "Gpio_Driver.h"
#define _IO volatile

/*********************************************************************
 * Peripheral Base Addresses
 *********************************************************************/
#define EXTI_BASE 0x40013C00UL
#define SYSCFG_BASE 0x40013800UL

/*********************************************************************
 * Peripheral Definitions
 *********************************************************************/
#define RCC_APB2ENR_SYSCFGEN (1 << 14)

/*********************************************************************
 * Register Definitions
 *********************************************************************/
typedef struct {
    _IO uint32_t MEMRMP;      /*!< SYSCFG memory remap register, Address offset: 0x00 */
    _IO uint32_t PMC;         /*!< SYSCFG peripheral mode configuration register, Address offset: 0x04 */
    _IO uint32_t EXTICR[4];   /*!< SYSCFG external interrupt configuration registers, Address offset: 0x08-0x14 */
    _IO uint32_t CMPCR;       /*!< SYSCFG compensation cell control register, Address offset: 0x20 */
} SYSCFG_RegDef_t;

typedef struct {
    _IO uint32_t IMR;         /*!< EXTI interrupt mask register, Address offset: 0x00 */
    _IO uint32_t EMR;         /*!< EXTI event mask register, Address offset: 0x04 */
    _IO uint32_t RTSR;        /*!< EXTI rising trigger selection register, Address offset: 0x08 */
    _IO uint32_t FTSR;        /*!< EXTI falling trigger selection register, Address offset: 0x0C */
    _IO uint32_t SWIER;       /*!< EXTI software interrupt event register, Address offset: 0x10 */
    _IO uint32_t PR;          /*!< EXTI pending register, Address offset: 0x14 */
} EXTI_RegDef_t;

/*********************************************************************
 * Register Structure
 *********************************************************************/
#define EXTI_REG ((EXTI_RegDef_t *)EXTI_BASE)
#define SYSCFG ((SYSCFG_RegDef_t *)SYSCFG_BASE)

/*********************************************************************
 * Enumerations
 *********************************************************************/
enum class EXTI_Trigger
{
    RISING,
    FALLING,
    RISING_FALLING
};

/*********************************************************************
 * Driver Class
 *********************************************************************/
class EXTI
{
private:
    GPIO_Port mPort;
    uint8_t mPin;

    void ConfigureEXTI();

public:
    /**************************************************************
     * Constructor
     **************************************************************/
    EXTI(RCC_GPIO port, uint8_t pin);

    /**************************************************************
     * Initialization APIs
     **************************************************************/
    /**
     * @brief Enables the SYSCFG clock used for EXTI line mapping.
     */
    void SYSCFGClock();

    /**
     * @brief Maps the selected GPIO pin to its EXTI line.
     */
    void configureEXTILine();

    /**************************************************************
     * Configuration APIs
     **************************************************************/
    /**
     * @brief Selects the trigger edge for the EXTI line.
     * @param trigger Rising, falling, or both edges.
     */
    void setTrigger(EXTI_Trigger trigger);

    /**************************************************************
     * Interrupt APIs
     **************************************************************/
    void enableInterrupt();
    void disableInterrupt();
    void enableEvent();
    void disableEvent();
    void softwareInterrupt();

    /**************************************************************
     * Status APIs
     **************************************************************/
    bool isPending();
    void clearPending();
};

#endif /* EXTI_DRIVER_H_ */
