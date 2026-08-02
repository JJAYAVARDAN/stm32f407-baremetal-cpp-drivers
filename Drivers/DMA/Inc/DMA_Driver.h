/*
 * DMA_Driver.h
 *
 * STM32F407 Bare-Metal DMA Driver
 *
 * Created on: 28-Jul-2026
 * Author: jolap
 *
 * Purpose:
 *   Register-level driver for the STM32F407 Direct Memory Access controller.
 *   The driver exposes APIs to configure streams, channels, transfer
 *   direction, and interrupt behavior for memory-to-peripheral and
 *   peripheral-to-memory transfers.
 *
 * RM0090 Reference:
 *   Chapter 10 - Direct Memory Access Controller (DMA)
 */

#ifndef DMA_DRIVER_H_
#define DMA_DRIVER_H_

#include <cstdint>
#include "RCC_Driver.h"

#define _IO volatile

/*********************************************************************
 * Peripheral Base Addresses
 *********************************************************************/
#define DMA1_BASE 0x40026000UL
#define DMA2_BASE 0x40026400UL

/*********************************************************************
 * Register Definitions
 *********************************************************************/
typedef struct
{
    _IO uint32_t LISR;   /*!< DMA low interrupt status register,      Address offset: 0x00 */
    _IO uint32_t HISR;   /*!< DMA high interrupt status register,     Address offset: 0x04 */
    _IO uint32_t LIFCR;  /*!< DMA low interrupt flag clear register,  Address offset: 0x08 */
    _IO uint32_t HIFCR;  /*!< DMA high interrupt flag clear register, Address offset: 0x0C */
} DMA_RegDef_t;

typedef struct
{
    _IO uint32_t CR;    /*!< DMA stream x configuration register,      Address offset: 0x10 + 0x18 * (x - 1) */
    _IO uint32_t NDTR;  /*!< DMA stream x number of data register,     Address offset: 0x14 + 0x18 * (x - 1) */
    _IO uint32_t PAR;   /*!< DMA stream x peripheral address register, Address offset: 0x18 + 0x18 * (x - 1) */
    _IO uint32_t M0AR;  /*!< DMA stream x memory 0 address register,   Address offset: 0x1C + 0x18 * (x - 1) */
    _IO uint32_t M1AR;  /*!< DMA stream x memory 1 address register,   Address offset: 0x20 + 0x18 * (x - 1) */
    _IO uint32_t FCR;   /*!< DMA stream x FIFO control register,       Address offset: 0x24 + 0x18 * (x - 1) */
} DMA_StreamRegDef_t;

/*********************************************************************
 * Register Structure
 *********************************************************************/
#define DMA1_REG ((DMA_RegDef_t *)DMA1_BASE)
#define DMA2_REG ((DMA_RegDef_t *)DMA2_BASE)

/*********************************************************************
 * Peripheral Definitions
 *********************************************************************/
#define DMA1_STREAM0 ((DMA_StreamRegDef_t *)(DMA1_BASE + 0x10))
#define DMA1_STREAM1 ((DMA_StreamRegDef_t *)(DMA1_BASE + 0x28))
#define DMA1_STREAM2 ((DMA_StreamRegDef_t *)(DMA1_BASE + 0x40))
#define DMA1_STREAM3 ((DMA_StreamRegDef_t *)(DMA1_BASE + 0x58))
#define DMA1_STREAM4 ((DMA_StreamRegDef_t *)(DMA1_BASE + 0x70))
#define DMA1_STREAM5 ((DMA_StreamRegDef_t *)(DMA1_BASE + 0x88))
#define DMA1_STREAM6 ((DMA_StreamRegDef_t *)(DMA1_BASE + 0xA0))
#define DMA1_STREAM7 ((DMA_StreamRegDef_t *)(DMA1_BASE + 0xB8))
#define DMA2_STREAM0 ((DMA_StreamRegDef_t *)(DMA2_BASE + 0x10))
#define DMA2_STREAM1 ((DMA_StreamRegDef_t *)(DMA2_BASE + 0x28))
#define DMA2_STREAM2 ((DMA_StreamRegDef_t *)(DMA2_BASE + 0x40))
#define DMA2_STREAM3 ((DMA_StreamRegDef_t *)(DMA2_BASE + 0x58))
#define DMA2_STREAM4 ((DMA_StreamRegDef_t *)(DMA2_BASE + 0x70))
#define DMA2_STREAM5 ((DMA_StreamRegDef_t *)(DMA2_BASE + 0x88))
#define DMA2_STREAM6 ((DMA_StreamRegDef_t *)(DMA2_BASE + 0xA0))
#define DMA2_STREAM7 ((DMA_StreamRegDef_t *)(DMA2_BASE + 0xB8))

/*********************************************************************
 * Enumerations
 *********************************************************************/
enum class DMA_Instance
{
    DMA1,
    DMA2
};

enum class DMA_Stream
{
    STREAM0 = 0,
    STREAM1,
    STREAM2,
    STREAM3,
    STREAM4,
    STREAM5,
    STREAM6,
    STREAM7
};

enum class DMA_Channel
{
    CHANNEL0 = 0,
    CHANNEL1,
    CHANNEL2,
    CHANNEL3,
    CHANNEL4,
    CHANNEL5,
    CHANNEL6,
    CHANNEL7
};

enum class DMA_Direction
{
    PERIPHERAL_TO_MEMORY = 0,
    MEMORY_TO_PERIPHERAL,
    MEMORY_TO_MEMORY
};

enum class DMA_Priority
{
    LOW = 0,
    MEDIUM,
    HIGH,
    VERY_HIGH
};

enum class DMA_DataSize
{
    BYTE = 0,
    HALF_WORD,
    WORD
};

enum class DMA_Mode
{
    NORMAL = 0,
    CIRCULAR
};

enum class DMA_FIFOMode
{
    DISABLE = 0,
    ENABLE
};

enum class DMA_FIFOThreshold
{
    QUARTER_FULL = 0,
    HALF_FULL,
    THREE_QUARTERS_FULL,
    FULL
};

enum class DMA_IncrementMode
{
    DISABLE = 0,
    ENABLE
};

enum class DMA_Interrupt
{
    TRANSFER_COMPLETE,
    HALF_TRANSFER,
    TRANSFER_ERROR,
    DIRECT_MODE_ERROR,
    FIFO_ERROR
};

/*********************************************************************
 * Driver Class
 *********************************************************************/
class DMA
{
private:
    DMA_RegDef_t* mDMA;
    DMA_StreamRegDef_t* mStream;
    DMA_Instance mDMAInstance;

    /*
     * Selected DMA Stream
     */
    DMA_Stream mStreamNumber;

public:

    /**************************************************************
     * Constructor
     **************************************************************/
    DMA(DMA_Instance dma, DMA_Stream stream);

    /**************************************************************
     * Clock Control
     **************************************************************/
    void enableClock();

    /**************************************************************
     * DMA Initialization
     **************************************************************/
    void init();

    /**************************************************************
     * Stream Configuration
     **************************************************************/
    void setChannel(DMA_Channel channel);

    void setDirection(DMA_Direction direction);

    void setPriority(DMA_Priority priority);

    void setMode(DMA_Mode mode);

    void setPeripheralIncrement(DMA_IncrementMode mode);

    void setMemoryIncrement(DMA_IncrementMode mode);

    void setPeripheralDataSize(DMA_DataSize size);

    void setMemoryDataSize(DMA_DataSize size);

    void setPeripheralAddress(uint32_t address);

    void setMemoryAddress(uint32_t address);

    void setTransferLength(uint16_t length);

    void setFIFOThreshold(DMA_FIFOThreshold threshold);

    /**************************************************************
     * Stream Control
     **************************************************************/
    void start();

    void stop();

    bool isEnabled();

    /**************************************************************
     * DMA Status
     **************************************************************/
    bool isTransferComplete();

    bool isHalfTransfer();

    bool isTransferError();

    bool isFIFOError();

    bool isDirectModeError();

    /**************************************************************
     * DMA Flag Control
     **************************************************************/
    void clearTransferCompleteFlag();

    void clearHalfTransferFlag();

    void clearTransferErrorFlag();

    void clearFIFOErrorFlag();

    void clearDirectModeErrorFlag();

    void clearAllFlags();

    /**************************************************************
     * Interrupt Control
     **************************************************************/
    void enableTransferCompleteInterrupt();

    void disableTransferCompleteInterrupt();

    void enableHalfTransferInterrupt();

    void disableHalfTransferInterrupt();

    void enableTransferErrorInterrupt();

    void disableTransferErrorInterrupt();

    void enableDirectModeErrorInterrupt();

    void disableDirectModeErrorInterrupt();

    void enableFIFOErrorInterrupt();

    void disableFIFOErrorInterrupt();
};

#endif /* DMA_DRIVER_H_ */
