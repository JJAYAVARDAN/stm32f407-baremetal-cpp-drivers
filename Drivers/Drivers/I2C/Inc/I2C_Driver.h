/*
 * I2C_Driver.h
 *
 *  Created on: 19-Jul-2026
 *      Author: jolap
 */

#ifndef I2C_DRIVER_H_
#define I2C_DRIVER_H_

#include <cstdint>
#include "RCC_Driver.h"

#define _IO volatile

/*********************************************************************
 *                      I2C Peripheral Base Addresses
 *
 * STM32F407 Reference Manual (RM0090)
 *
 * I2C1 : 0x40005400 (APB1)
 * I2C2 : 0x40005800 (APB1)
 * I2C3 : 0x40005C00 (APB1)
 *
 *********************************************************************/

#define I2C1_BASE      0x40005400UL
#define I2C2_BASE      0x40005800UL
#define I2C3_BASE      0x40005C00UL


/*********************************************************************
 *                      I2C Register Definition
 *
 * Offset
 *
 * 0x00 CR1
 * 0x04 CR2
 * 0x08 OAR1
 * 0x0C OAR2
 * 0x10 DR
 * 0x14 SR1
 * 0x18 SR2
 * 0x1C CCR
 * 0x20 TRISE
 * 0x24 FLTR
 *
 *********************************************************************/

typedef struct
{
    _IO uint32_t CR1;      // 0x00
    _IO uint32_t CR2;      // 0x04
    _IO uint32_t OAR1;     // 0x08
    _IO uint32_t OAR2;     // 0x0C
    _IO uint32_t DR;       // 0x10
    _IO uint32_t SR1;      // 0x14
    _IO uint32_t SR2;      // 0x18
    _IO uint32_t CCR;      // 0x1C
    _IO uint32_t TRISE;    // 0x20
    _IO uint32_t FLTR;     // 0x24

} I2C_RegDef_t;


/*********************************************************************
 *                      Peripheral Definitions
 *********************************************************************/

#define I2C1_REG ((I2C_RegDef_t*)I2C1_BASE)
#define I2C2_REG ((I2C_RegDef_t*)I2C2_BASE)
#define I2C3_REG ((I2C_RegDef_t*)I2C3_BASE)


/*********************************************************************
 *                      I2C Instance
 *********************************************************************/

enum class I2C_Instance
{
    I2C1,
    I2C2,
    I2C3
};


/*********************************************************************
 *                      Duty Cycle
 *********************************************************************/

enum class I2C_DutyCycle
{
    DUTY_2 = 0,
    DUTY_16_9
};


/*********************************************************************
 *                      Acknowledge
 *********************************************************************/

enum class I2C_ACK
{
    DISABLE = 0,
    ENABLE
};


/*********************************************************************
 *                      Driver Class
 *********************************************************************/

class I2C
{
private:

    /*
     * Pointer to I2C Registers
     */

    I2C_RegDef_t* mI2C;

public:

    /**************************************************************
     * Constructor
     **************************************************************/
    I2C(I2C_Instance instance);

    /**************************************************************
     * Clock Control
     **************************************************************/
    void enableClock();

    /**************************************************************
     * Initialization
     **************************************************************/
    void init(uint32_t clockSpeed,
              I2C_ACK ack = I2C_ACK::ENABLE);

    /**************************************************************
     * Peripheral Control
     **************************************************************/
    void enable();

    void disable();

    /**************************************************************
     * Master Operations
     **************************************************************/
    void generateStart();

    void generateStop();

    void sendAddress(uint8_t address,
                     bool read);

    void sendByte(uint8_t data);

    uint8_t receiveByte();

    void masterTransmit(uint8_t slaveAddress,
                        const uint8_t* buffer,
                        uint32_t length);

    void masterReceive(uint8_t slaveAddress,
                       uint8_t* buffer,
                       uint32_t length);

    /**************************************************************
     * Status Flags
     **************************************************************/
    bool isStartGenerated();

    bool isAddressSent();

    bool isTXE();

    bool isRXNE();

    bool isTransferFinished();

    bool isBusy();
};

#endif /* I2C_DRIVER_H_ */