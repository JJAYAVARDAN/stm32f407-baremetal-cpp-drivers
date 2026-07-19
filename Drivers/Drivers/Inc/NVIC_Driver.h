/*
 * NVIC_Driver.h
 *
 *  Created on: 19-Jul-2026
 *      Author: jolap
 */

#ifndef NVIC_DRIVER_H_
#define NVIC_DRIVER_H_

#include <cstdint>
#define _IO volatile

 /*********************************************************************
  *                  NVIC Base Address
  *********************************************************************/
#define NVIC_BASE 0xE000E100UL
  /*********************************************************************
   *                  NVIC Register Structure
   *********************************************************************/

typedef struct
{
	_IO uint32_t ISER[8];   /*!< Interrupt Set-Enable Registers, Address offset: 0x000 - 0x01C */
	uint32_t RESERVED0[24]; /*!< Reserved, 0x020 - 0x07C */
	_IO uint32_t ICER[8];   /*!< Interrupt Clear-Enable Registers, Address offset: 0x080 - 0x09C */
	uint32_t RESERVED1[24]; /*!< Reserved, 0x0A0 - 0x0FC */
	_IO uint32_t ISPR[8];   /*!< Interrupt Set-Pending Registers, Address offset: 0x100 - 0x11C */
	uint32_t RESERVED2[24]; /*!< Reserved, 0x120 - 0x17C */
	_IO uint32_t ICPR[8];   /*!< Interrupt Clear-Pending Registers, Address offset: 0x180 - 0x19C */
	uint32_t RESERVED3[24]; /*!< Reserved, 0x1A0 - 0x1FC */
	_IO uint32_t IABR[8];   /*!< Interrupt Active Bit Registers, Address offset: 0x200 - 0x21C */
	uint32_t RESERVED4[56];  /*!< Reserved, 0x220 - 0x2FC */
	_IO uint8_t IPR[240];   /*!< Interrupt Priority Registers, Address offset: 0x300 - 0x3EC */

}NVIC_RegDef_t;
/*********************************************************************
 *                  NVIC Definition
 *********************************************************************/
#define NVIC_REG ((NVIC_RegDef_t *)NVIC_BASE)

 /*********************************************************************
  *                  NVIC Driver Class
  *********************************************************************/ 

class NVIC
{
public:
    /*****************************************************************
     * Enable IRQ
     *****************************************************************/
    static void enableIRQ(uint8_t irq);

    /*****************************************************************
     * Disable IRQ
     *****************************************************************/
    static void disableIRQ(uint8_t irq);

    /*****************************************************************
     * Set Pending IRQ
     *****************************************************************/
    static void setPendingIRQ(uint8_t irq);

    /*****************************************************************
     * Clear Pending IRQ
     *****************************************************************/
    static void clearPendingIRQ(uint8_t irq);

    /*****************************************************************
     * Check Active IRQ
     *****************************************************************/
    static bool getActiveIRQ(uint8_t irq);

    /*****************************************************************
     * Set Interrupt Priority
     *****************************************************************/
    static void setPriority(uint8_t irq, uint8_t priority);
};

#endif /* NVIC_DRIVER_H_ */
