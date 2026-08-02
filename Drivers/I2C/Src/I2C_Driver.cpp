/*
 * I2C_Driver.cpp
 *
 * STM32F407 Bare-Metal I2C Driver
 *
 * Created on: 19-Jul-2026
 * Author: jolap
 *
 * Purpose:
 *   Implementation of the STM32F407 I2C master-mode driver for addressing,
 *   byte transfers, and bus-state management.
 *
 * State Machine:
 *   Idle -> START -> address -> ACK -> transfer -> STOP.
 *
 * RM0090 Reference:
 *   Chapter 27 - Inter-integrated circuit (I2C)
 */

 /*********************************************************************
  *                      I2C Driver Design
  *
  * The Inter-Integrated Circuit (I2C) is a synchronous, multi-master,
  * multi-slave serial communication protocol developed by Philips
  * (now NXP). It enables multiple devices to communicate over a
  * shared two-wire bus using address-based communication.
  *
  * The STM32F407 provides three I2C peripherals:
  *
  *      • I2C1
  *      • I2C2
  *      • I2C3
  *
  * Unlike SPI and USART, I2C allows multiple masters and multiple
  * slave devices to coexist on the same communication bus. Every
  * slave device is identified by either a 7-bit or 10-bit address.
  *
  * I2C communication uses only two signals:
  *
  *      • SCL - Serial Clock Line
  *      • SDA - Serial Data Line
  *
  * Since both lines are open-drain, external pull-up resistors are
  * required for proper bus operation.
  *
  * Every I2C transaction consists of:
  *
  *      • START Condition
  *      • Slave Address
  *      • Read/Write Bit
  *      • Address Acknowledge
  *      • Data Transfer
  *      • Acknowledge / Not Acknowledge
  *      • STOP Condition
  *
  * Before communication begins, software configures:
  *
  *      • Peripheral Clock
  *      • Clock Control Register (CCR)
  *      • Rise Time Register (TRISE)
  *      • Own Device Address
  *      • ACK Control
  *      • Duty Cycle (Fast Mode)
  *      • Standard/Fast Mode
  *
  * During transmission, the master generates the clock while data
  * is exchanged over the SDA line. Every transmitted byte is
  * acknowledged by the receiving device.
  *
  * The I2C peripheral continuously updates status flags such as:
  *
  *      • SB     - Start Bit
  *      • ADDR   - Address Sent/Matched
  *      • TXE    - Transmit Data Register Empty
  *      • RXNE   - Receive Data Register Not Empty
  *      • BTF    - Byte Transfer Finished
  *      • BUSY   - Bus Busy
  *      • AF     - Acknowledge Failure
  *      • ARLO   - Arbitration Lost
  *      • BERR   - Bus Error
  *
  * The I2C driver abstracts the protocol sequence and provides
  * high-level APIs for master and slave communication without
  * exposing register-level complexity to the application.
  *
  *********************************************************************
  *                  Polling Mode State Machine
  *
  *              Enable I2C Clock
  *                      │
  *                      ▼
  *             Configure GPIO Pins
  *              (Open Drain AF)
  *                      │
  *                      ▼
  *            Configure I2C Timing
  *           (CCR / TRISE / ACK)
  *                      │
  *                      ▼
  *              Enable I2C
  *                      │
  *                      ▼
  *         Generate START Condition
  *                      │
  *                      ▼
  *          Wait Until SB = 1
  *                      │
  *                      ▼
  *         Send Slave Address
  *                      │
  *                      ▼
  *        Wait Until ADDR = 1
  *                      │
  *                      ▼
  *       Clear ADDR Flag Sequence
  *                      │
  *                      ▼
  *          Transfer Data Bytes
  *                      │
  *                      ▼
  *        Wait For TXE / RXNE
  *                      │
  *                      ▼
  *       Generate STOP Condition
  *                      │
  *                      ▼
  *      Communication Complete
  *
  *********************************************************************
  *                Interrupt Mode State Machine
  *
  *              Enable I2C Clock
  *                      │
  *                      ▼
  *             Configure GPIO Pins
  *                      │
  *                      ▼
  *             Configure I2C
  *                      │
  *                      ▼
  *       Enable Event Interrupts
  *       Enable Buffer Interrupts
  *       Enable Error Interrupts
  *                      │
  *                      ▼
  *        Enable NVIC Interrupt
  *                      │
  *                      ▼
  *              Enable I2C
  *                      │
  *                      ▼
  *         Generate START Condition
  *                      │
  *                      ▼
  *       Hardware Generates Event
  *                      │
  *                      ▼
  *         I2C Interrupt Request
  *                      │
  *                      ▼
  *          NVIC Executes ISR
  *                      │
  *                      ▼
  *      Determine Interrupt Source
  *                      │
  *     ┌──────┼─────────┼────────┐
  *     ▼      ▼         ▼        ▼
  *    SB    ADDR      TXE     RXNE
  *     │      │         │        │
  *     ▼      ▼         ▼        ▼
  * Continue Address  Send    Read
  * Sequence  Phase   Data    Data
  *                      │
  *                      ▼
  *            Handle Errors (if any)
  *                      │
  *                      ▼
  *        Generate STOP Condition
  *                      │
  *                      ▼
  *       Return From Interrupt
  *
  *********************************************************************
  *                  Driver Responsibilities
  *
  * • Enable I2C peripheral clock.
  * • Configure standard mode (100 kHz).
  * • Configure fast mode (400 kHz).
  * • Configure CCR and TRISE.
  * • Configure own slave address.
  * • Enable/Disable ACK generation.
  * • Generate START condition.
  * • Generate RESTART condition.
  * • Generate STOP condition.
  * • Transmit data (Polling).
  * • Receive data (Polling).
  * • Master transmit operation.
  * • Master receive operation.
  * • Slave transmit operation.
  * • Slave receive operation.
  * • Enable/Disable interrupt mode.
  * • Handle I2C event interrupts.
  * • Handle I2C error interrupts.
  * • Detect and recover from bus errors.
  *
  * RM0090 Reference:
  * Chapter 25 - Inter-Integrated Circuit Interface (I2C)
  *
  *********************************************************************/

#include "I2C_Driver.h"

/*********************************************************************
 *                  Constructor
 *********************************************************************/

I2C::I2C(I2C_Instance instance)
{
    switch (instance)
    {
        case I2C_Instance::I2C1:
            mI2C = I2C1_REG;
            break;

        case I2C_Instance::I2C2:
            mI2C = I2C2_REG;
            break;

        case I2C_Instance::I2C3:
            mI2C = I2C3_REG;
            break;
    }
}

/*********************************************************************
 *                  Enable I2C Peripheral Clock
 *********************************************************************/

void I2C::enableClock()
{
    if (mI2C == I2C1_REG)
    {
        RCC::enableI2CClock(RCC_I2C::I2C1);
    }
    else if (mI2C == I2C2_REG)
    {
        RCC::enableI2CClock(RCC_I2C::I2C2);
    }
    else if (mI2C == I2C3_REG)
    {
        RCC::enableI2CClock(RCC_I2C::I2C3);
    }
}

/*********************************************************************
 *                  Enable I2C Peripheral
 *********************************************************************/

void I2C::enable()
{
    /*
     * CR1
     * Bit 0 : PE (Peripheral Enable)
     */

    mI2C->CR1 |= (1U << 0);
}

/*********************************************************************
 *                  Disable I2C Peripheral
 *********************************************************************/

void I2C::disable()
{
    /*
     * CR1
     * Bit 0 : PE (Peripheral Enable)
     */

    mI2C->CR1 &= ~(1U << 0);
}

/*********************************************************************
 *                  Initialize I2C Peripheral
 *********************************************************************/

void I2C::init(uint32_t clockSpeed,
               I2C_ACK ack)
{
    /**************************************************************
     * Enable Peripheral Clock
     **************************************************************/

    enableClock();

    /**************************************************************
     * Disable I2C before configuration
     **************************************************************/

    disable();

    /**************************************************************
     * Configure APB1 Frequency in MHz
     **************************************************************/
    uint32_t pclk1 = RCC::getAPB1ClockFreq();
    mI2C->CR2 &= ~(0x3FU);
    mI2C->CR2 |= (pclk1 / 1000000U);

    /**************************************************************
     * Configure Clock Control Register
     *
     * Standard Mode (<=100kHz)
     *
     * CCR = Fpclk1 / (2 * I2C Speed)
     *
     * Example:
     *
     * Fpclk1 = 16 MHz
     * Speed  = 100 kHz
     *
     * CCR = 16000000 / (2 * 100000)
     *     = 80
     **************************************************************/

    uint16_t ccr = static_cast<uint16_t>((pclk1 + (clockSpeed / 2U)) / (2U * clockSpeed));

    mI2C->CCR = ccr;

    /**************************************************************
     * Configure Maximum Rise Time
     *
     * Standard Mode
     *
     * TRISE = Fpclk(MHz) + 1
     *
     * = 16 + 1
     * = 17
     **************************************************************/

    mI2C->TRISE = static_cast<uint16_t>((pclk1 / 1000000U) + 1U);

    /**************************************************************
     * Configure ACK
     *
     * CR1 Bit10
     **************************************************************/

    if (ack == I2C_ACK::ENABLE)
    {
        mI2C->CR1 |= (1U << 10);
    }
    else
    {
        mI2C->CR1 &= ~(1U << 10);
    }

    /**************************************************************
     * Enable I2C Peripheral
     **************************************************************/

    enable();
}

/*********************************************************************
 *                  Generate START Condition
 *********************************************************************/

void I2C::generateStart()
{
    /*
     * CR1 Bit8 : START
     */

    mI2C->CR1 |= (1U << 8);

    /*
     * Wait until START condition generated
     *
     * SR1 Bit0 : SB
     */

    while (!(mI2C->SR1 & (1U << 0)));
}


/*********************************************************************
 *                  Generate STOP Condition
 *********************************************************************/

void I2C::generateStop()
{
    /*
     * CR1 Bit9 : STOP
     */

    mI2C->CR1 |= (1U << 9);
}


/*********************************************************************
 *                  Send Slave Address
 *********************************************************************/

void I2C::sendAddress(uint8_t address,
                      bool read)
{
    /*
     * 7-bit Address
     *
     * Bit0 = R/W
     */

    uint8_t addr = (address << 1);

    if (read)
    {
        addr |= 0x01;
    }

    /*
     * Write Address
     */

    mI2C->DR = addr;

    /*
     * Wait until Address Sent
     *
     * SR1 Bit1 : ADDR
     */

    while (!(mI2C->SR1 & (1U << 1)));

    /*
     * Clear ADDR flag
     *
     * Read SR1 followed by SR2
     */

    volatile uint32_t temp;

    temp = mI2C->SR1;
    temp = mI2C->SR2;

    (void)temp;
}


/*********************************************************************
 *                  Send One Byte
 *********************************************************************/

void I2C::sendByte(uint8_t data)
{
    /*
     * Wait until TXE = 1
     */

    while (!(mI2C->SR1 & (1U << 7)));

    mI2C->DR = data;

    /*
     * Wait until Byte Transfer Finished
     *
     * SR1 Bit2 : BTF
     */

    while (!(mI2C->SR1 & (1U << 2)));
}


/*********************************************************************
 *                  Receive One Byte
 *********************************************************************/

uint8_t I2C::receiveByte()
{
    /*
     * Wait until RXNE = 1
     */

    while (!(mI2C->SR1 & (1U << 6)));

    return static_cast<uint8_t>(mI2C->DR);
}

/*********************************************************************
 *                  Master Transmit
 *********************************************************************/

void I2C::masterTransmit(uint8_t slaveAddress,
                         const uint8_t* buffer,
                         uint32_t length)
{
    /**************************************************************
     * Generate START
     **************************************************************/
    generateStart();

    /**************************************************************
     * Send Slave Address (Write)
     **************************************************************/
    sendAddress(slaveAddress, false);

    /**************************************************************
     * Send Data Bytes
     **************************************************************/
    while (length)
    {
        sendByte(*buffer);

        buffer++;

        length--;
    }

    /**************************************************************
     * Generate STOP
     **************************************************************/
    generateStop();
}


/*********************************************************************
 *                  Master Receive
 *********************************************************************/

void I2C::masterReceive(uint8_t slaveAddress,
                        uint8_t* buffer,
                        uint32_t length)
{
    /**************************************************************
     * Generate START
     **************************************************************/
    generateStart();

    /**************************************************************
     * Send Slave Address (Read)
     **************************************************************/
    sendAddress(slaveAddress, true);

    /**************************************************************
     * Receive Data Bytes
     **************************************************************/
    if (length > 0U)
    {
        mI2C->CR1 |= (1U << 10);
    }

    while (length)
    {
        if (length == 1U)
        {
            mI2C->CR1 &= ~(1U << 10);
        }

        *buffer = receiveByte();

        buffer++;

        length--;
    }

    /**************************************************************
     * Generate STOP
     **************************************************************/
    generateStop();
}

/*********************************************************************
 *                  START Generated
 *********************************************************************/

bool I2C::isStartGenerated()
{
    return (mI2C->SR1 & (1U << 0));
}

/*********************************************************************
 *                  Address Sent
 *********************************************************************/

bool I2C::isAddressSent()
{
    return (mI2C->SR1 & (1U << 1));
}

/*********************************************************************
 *                  TXE Flag
 *********************************************************************/

bool I2C::isTXE()
{
    return (mI2C->SR1 & (1U << 7));
}

/*********************************************************************
 *                  RXNE Flag
 *********************************************************************/

bool I2C::isRXNE()
{
    return (mI2C->SR1 & (1U << 6));
}

/*********************************************************************
 *                  Byte Transfer Finished
 *********************************************************************/

bool I2C::isTransferFinished()
{
    return (mI2C->SR1 & (1U << 2));
}

/*********************************************************************
 *                  Bus Busy
 *********************************************************************/

bool I2C::isBusy()
{
    return (mI2C->SR2 & (1U << 1));
}