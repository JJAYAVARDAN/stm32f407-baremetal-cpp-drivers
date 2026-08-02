/*
 * NVIC_Driver.h
 *
 * STM32F407 Bare-Metal NVIC Driver
 *
 * Created on: 19-Jul-2026
 * Author: jolap
 *
 * Purpose:
 *   Register-level driver for the ARM Cortex-M4 Nested Vectored Interrupt
 *   Controller. The driver exposes a compact interface for enabling,
 *   disabling, prioritizing, and monitoring interrupt requests.
 *
 * RM0090 Reference:
 *   ARM Cortex-M4 Generic User Guide - Nested Vectored Interrupt Controller
 */

#ifndef NVIC_DRIVER_H_
#define NVIC_DRIVER_H_

#include <cstdint>
#define _IO volatile

/*********************************************************************
 * Peripheral Base Address
 *********************************************************************/
#define NVIC_BASE 0xE000E100UL

/*********************************************************************
 * Register Definitions
 *********************************************************************/
typedef struct
{
    _IO uint32_t ISER[8];      /*!< Interrupt Set-Enable Registers, Address offset: 0x000 - 0x01C */
    uint32_t RESERVED0[24];   /*!< Reserved, 0x020 - 0x07C */
    _IO uint32_t ICER[8];     /*!< Interrupt Clear-Enable Registers, Address offset: 0x080 - 0x09C */
    uint32_t RESERVED1[24];   /*!< Reserved, 0x0A0 - 0x0FC */
    _IO uint32_t ISPR[8];     /*!< Interrupt Set-Pending Registers, Address offset: 0x100 - 0x11C */
    uint32_t RESERVED2[24];   /*!< Reserved, 0x120 - 0x17C */
    _IO uint32_t ICPR[8];     /*!< Interrupt Clear-Pending Registers, Address offset: 0x180 - 0x19C */
    uint32_t RESERVED3[24];   /*!< Reserved, 0x1A0 - 0x1FC */
    _IO uint32_t IABR[8];     /*!< Interrupt Active Bit Registers, Address offset: 0x200 - 0x21C */
    uint32_t RESERVED4[56];   /*!< Reserved, 0x220 - 0x2FC */
    _IO uint8_t IPR[240];     /*!< Interrupt Priority Registers, Address offset: 0x300 - 0x3EC */
} NVIC_RegDef_t;

/*********************************************************************
 * Register Structure
 *********************************************************************/
#define NVIC_REG ((NVIC_RegDef_t *)NVIC_BASE)

/*********************************************************************
 * Driver Class
 *********************************************************************/
class NVIC
{
public:
    /**************************************************************
     * Interrupt Control APIs
     **************************************************************/
    /**
     * @brief Enables a maskable IRQ in the NVIC.
     * @param irq IRQ number in the range 0-239.
     */
    static void enableIRQ(uint8_t irq);

    /**
     * @brief Disables a maskable IRQ in the NVIC.
     * @param irq IRQ number in the range 0-239.
     */
    static void disableIRQ(uint8_t irq);

    /**
     * @brief Sets the pending state for a maskable IRQ.
     * @param irq IRQ number in the range 0-239.
     */
    static void setPendingIRQ(uint8_t irq);

    /**
     * @brief Clears the pending state for a maskable IRQ.
     * @param irq IRQ number in the range 0-239.
     */
    static void clearPendingIRQ(uint8_t irq);

    /**
     * @brief Returns whether the selected IRQ is active.
     * @param irq IRQ number in the range 0-239.
     * @return true when the IRQ active bit is set.
     */
    static bool getActiveIRQ(uint8_t irq);

    /**
     * @brief Sets the priority field for a maskable IRQ.
     * @param irq IRQ number in the range 0-239.
     * @param priority Priority value written into the upper priority bits.
     */
    static void setPriority(uint8_t irq, uint8_t priority);

    /**
     * @brief Returns whether the selected IRQ is currently enabled.
     * @param irq IRQ number in the range 0-239.
     * @return true when the IRQ enable bit is set.
     */
    static bool isIRQEnabled(uint8_t irq);
};

#endif /* NVIC_DRIVER_H_ */
