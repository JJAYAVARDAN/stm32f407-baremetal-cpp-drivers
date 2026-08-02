/*
 * DMA_Driver.cpp
 *
 * STM32F407 Bare-Metal DMA Driver
 *
 * Created on: 28-Jul-2026
 * Author: jolap
 *
 * Purpose:
 *   Implementation of the STM32F407 DMA driver for stream setup, transfer
 *   control, and interrupt-driven memory transfers.
 *
 * State Machine:
 *   Enable clock -> select stream/channel -> configure registers -> enable stream -> TC/TE/FE handling.
 *
 * RM0090 Reference:
 *   Chapter 10 - Direct Memory Access Controller (DMA)
 */

 #include "DMA_Driver.h"
 /*********************************************************************
  *                      DMA Driver Design
  *
  * The STM32F407 Direct Memory Access (DMA) controller is a hardware
  * peripheral that transfers data between memory and peripherals
  * without CPU intervention. By offloading data movement from the CPU,
  * DMA significantly improves system performance and reduces processor
  * overhead during large or continuous data transfers.
  *
  * The STM32F407 provides two independent DMA controllers:
  *
  *      • DMA1
  *      • DMA2
  *
  * Each DMA controller contains eight independent streams, and every
  * stream can be mapped to one of eight available channels. This
  * flexible architecture allows multiple peripherals to share the DMA
  * hardware while supporting different transfer requests.
  *
  * Every DMA transfer is controlled through the following sequence:
  *
  *      • Select DMA Controller
  *      • Select Stream
  *      • Select Channel
  *      • Configure Transfer Direction
  *      • Configure Peripheral Address
  *      • Configure Memory Address
  *      • Configure Number of Data Items
  *      • Configure Data Width
  *      • Configure Increment Modes
  *      • Configure Priority
  *      • Configure FIFO (Optional)
  *      • Enable Stream
  *      • Hardware Performs Transfer
  *      • Transfer Complete / Error
  *
  * According to RM0090, a DMA stream must always be disabled before
  * modifying any configuration registers. Software must wait until the
  * EN bit is cleared by hardware before updating stream
  * configuration.
  *
  * The DMA driver manages this hardware sequence internally so that
  * application code only interacts with high-level driver APIs instead
  * of individual register operations.
  *
  *********************************************************************
  *                      Polling Mode State Machine
  *
  *                 Enable DMA Clock
  *                         │
  *                         ▼
  *                Select DMA Controller
  *                         │
  *                         ▼
  *                  Select DMA Stream
  *                         │
  *                         ▼
  *                 Disable DMA Stream
  *                         │
  *                         ▼
  *                 Wait Until EN = 0
  *                         │
  *                         ▼
  *               Configure Stream Registers
  *         (CR, NDTR, PAR, M0AR, M1AR, FCR)
  *                         │
  *                         ▼
  *                Clear Pending Flags
  *                         │
  *                         ▼
  *                  Enable DMA Stream
  *                         │
  *                         ▼
  *               DMA Hardware Transfer
  *                         │
  *                         ▼
  *             Poll Transfer Complete Flag
  *                         │
  *                         ▼
  *               Clear Status Flags
  *                         │
  *                         ▼
  *                DMA Stream Ready
  *
  *********************************************************************
  *                    Interrupt Mode State Machine
  *
  *                 Enable DMA Clock
  *                         │
  *                         ▼
  *                Select DMA Controller
  *                         │
  *                         ▼
  *                  Select DMA Stream
  *                         │
  *                         ▼
  *                 Disable DMA Stream
  *                         │
  *                         ▼
  *                 Wait Until EN = 0
  *                         │
  *                         ▼
  *               Configure Stream Registers
  *                         │
  *                         ▼
  *             Enable DMA Interrupt Sources
  *        (TCIE, HTIE, TEIE, DMEIE, FEIE)
  *                         │
  *                         ▼
  *                  Enable DMA Stream
  *                         │
  *                         ▼
  *               DMA Hardware Transfer
  *                         │
  *                         ▼
  *                DMA Interrupt Generated
  *                         │
  *                         ▼
  *                  DMA ISR Executes
  *                         │
  *                         ▼
  *           Determine Interrupt Source
  *      (Transfer Complete / Half Transfer /
  *       Transfer Error / FIFO Error /
  *       Direct Mode Error)
  *                         │
  *                         ▼
  *                 Clear Interrupt Flags
  *                         │
  *                         ▼
  *                DMA Stream Ready
  *
  *********************************************************************
  *
  * Driver Responsibilities
  * -----------------------
  * • Enable or disable DMA peripheral clocks.
  * • Configure DMA streams and channels.
  * • Configure transfer direction.
  * • Configure peripheral and memory addresses.
  * • Configure transfer length.
  * • Configure memory and peripheral data sizes.
  * • Configure increment modes.
  * • Configure stream priority.
  * • Configure FIFO operation.
  * • Start and stop DMA transfers.
  * • Monitor transfer status.
  * • Handle DMA interrupts.
  *
  * RM0090 Reference:
  * Chapter 10 - Direct Memory Access Controller (DMA)
  *
  *********************************************************************/


  /*********************************************************************
   *                      DMA Constructor
   *
   * Select DMA controller and Stream instance.
   *
   * The STM32F407 contains two DMA controllers:
   *
   *      DMA1
   *      DMA2
   *
   * Each DMA controller provides eight independent streams
   * (Stream0 to Stream7). Every stream has its own configuration
   * registers and can be assigned to different peripheral requests
   * through channel selection.
   *
   * The constructor stores:
   *
   *      • Pointer to selected DMA controller
   *      • Pointer to selected DMA Stream
   *
   *********************************************************************/

DMA::DMA(DMA_Instance dma,DMA_Stream stream)
    : mDMA(nullptr), mStream(nullptr), mDMAInstance(dma), mStreamNumber(stream)
{
    /**************************************************************
     * Select DMA Controller
     **************************************************************/
    switch(dma)
    {
      case DMA_Instance::DMA1:
         mDMA = DMA1_REG;
         break;
      case DMA_Instance::DMA2:
         mDMA = DMA2_REG;
         break;
    }
    /**************************************************************
     * Select DMA Stream
     **************************************************************/
    if (mDMA == DMA1_REG)
    {
        switch (stream)
        {
            case DMA_Stream::STREAM0:
                mStream = DMA1_STREAM0;
                break;

            case DMA_Stream::STREAM1:
                mStream = DMA1_STREAM1;
                break;

            case DMA_Stream::STREAM2:
                mStream = DMA1_STREAM2;
                break;

            case DMA_Stream::STREAM3:
                mStream = DMA1_STREAM3;
                break;

            case DMA_Stream::STREAM4:
                mStream = DMA1_STREAM4;
                break;

            case DMA_Stream::STREAM5:
                mStream = DMA1_STREAM5;
                break;

            case DMA_Stream::STREAM6:
                mStream = DMA1_STREAM6;
                break;

            case DMA_Stream::STREAM7:
                mStream = DMA1_STREAM7;
                break;
        }
    }
    else
    {
        switch (stream)
        {
            case DMA_Stream::STREAM0:
                mStream = DMA2_STREAM0;
                break;

            case DMA_Stream::STREAM1:
                mStream = DMA2_STREAM1;
                break;

            case DMA_Stream::STREAM2:
                mStream = DMA2_STREAM2;
                break;

            case DMA_Stream::STREAM3:
                mStream = DMA2_STREAM3;
                break;

            case DMA_Stream::STREAM4:
                mStream = DMA2_STREAM4;
                break;

            case DMA_Stream::STREAM5:
                mStream = DMA2_STREAM5;
                break;

            case DMA_Stream::STREAM6:
                mStream = DMA2_STREAM6;
                break;

            case DMA_Stream::STREAM7:
                mStream = DMA2_STREAM7;
                break;
        }
    }
}

/*********************************************************************
 *                      Enable DMA Clock
 *
 * DMA1 and DMA2 are connected to the AHB1 Bus.
 *
 * This function enables the clock for the selected DMA controller
 * before any stream configuration or data transfer is performed.
 *
 *********************************************************************/

 void DMA::enableClock()
 {
   if(mDMA == DMA1_REG)
   {
         RCC().enableDMAClock(RCC_DMA::DMA1);
   }
   else if(mDMA == DMA2_REG)
   {
      RCC().enableDMAClock(RCC_DMA::DMA2);
   }
 }

 /*********************************************************************
 *                      DMA Initialization
 *
 * Enable DMA controller clock.
 *
 * Before configuring any DMA Stream, the Stream must be disabled.
 * The EN bit in the Stream Configuration Register (DMA_SxCR)
 * indicates whether the Stream is currently enabled.
 *
 * If the Stream is active, software must:
 *
 *      • Clear the EN bit.
 *      • Wait until the hardware clears the EN bit.
 *
 * After the Stream is disabled, all Stream configuration
 * registers are reset to their default values.
 *
 * This ensures the DMA Stream starts from a known state before
 * any transfer configuration is performed.
 *
 *********************************************************************/

 void DMA::init()
 {
   /* Enable DMA Controller Clock */
   enableClock();

   /* Enable DMA Controller Clock */
   mStream->CR &= ~(1 << 0);

    /* Wait until Stream is disabled */
    while(mStream-> CR & (1 << 0));
   /* Reset Stream Configuration Register */
   mStream->CR = 0;
   /* Reset Stream Configuration Register */
   mStream->NDTR =0;
   /* Reset Stream Configuration Register */
   mStream->PAR = 0;
    /* Reset Memory 0 Address Register */
    mStream->M0AR = 0;
    /* Reset Memory 1 Address Register */
    mStream ->M1AR = 0;
   /* Reset FIFO Control Register */
   mStream->FCR = 0;
 }
/*********************************************************************
 *                      DMA Channel Selection
 *
 * Configure the DMA Channel for the selected Stream.
 *
 * Each DMA Stream can be connected to one of eight hardware
 * request channels (Channel 0 to Channel 7). The channel
 * selection determines which peripheral generates DMA requests
 * for the selected Stream.
 *
 * CHSEL[2:0] bits:
 *
 *      000 : Channel 0
 *      001 : Channel 1
 *      010 : Channel 2
 *      011 : Channel 3
 *      100 : Channel 4
 *      101 : Channel 5
 *      110 : Channel 6
 *      111 : Channel 7
 *
 * RM0090 Reference:
 * DMA_SxCR Register
 * CHSEL[2:0] Bits [27:25]
 *
 *********************************************************************/

 void DMA::setChannel(DMA_Channel channel)
 {
    /* Clear Channel Selection Bits */
    mStream->CR &= ~(0x7 << 25);

    /* Configure Selected Channel */
    mStream->CR |= (static_cast<uint32_t>(channel) << 25);
 }
/*********************************************************************
 *                      DMA Transfer Direction
 *
 * Configure the data transfer direction for the selected DMA Stream.
 *
 * The DIR[1:0] bits determine the direction of data movement
 * between the peripheral and memory.
 *
 * DIR[1:0]:
 *
 *      00 : Peripheral-to-Memory
 *      01 : Memory-to-Peripheral
 *      10 : Memory-to-Memory
 *
 * The selected direction controls the DMA data flow during
 * each transfer request.
 *
 * RM0090 Reference:
 * DMA_SxCR Register
 * DIR[1:0] Bits [7:6]
 *
 *********************************************************************/

 void DMA::setDirection(DMA_Direction direction)
 {
    /* Clear Transfer Direction Bits */
    mStream->CR &= ~(0x3 << 6);

    /* Configure Transfer Direction */
    mStream->CR |= (static_cast<uint32_t>(direction) << 6);

 }

 /*********************************************************************
 *                      DMA Stream Priority
 *
 * Configure the priority level for the selected DMA Stream.
 *
 * The Priority Level (PL) determines the arbitration priority
 * between multiple DMA Streams requesting access to the bus
 * simultaneously.
 *
 * If two Streams request the bus at the same time, the Stream
 * with the higher priority level is serviced first.
 *
 * PL[1:0]:
 *
 *      00 : Low
 *      01 : Medium
 *      10 : High
 *      11 : Very High
 *
 * RM0090 Reference:
 * DMA_SxCR Register
 * PL[1:0] Bits [17:16]
 *
 *********************************************************************/

 void DMA::setPriority(DMA_Priority priority)
 {
   /* Clear Priority Level Bits */
   mStream->CR &= ~(0x3 << 16);

    /* Configure Stream Priority */
    mStream->CR |= (static_cast<uint32_t>(priority) << 16);
 }

 /*********************************************************************
 *                      DMA Transfer Mode
 *
 * Configure the transfer mode for the selected DMA Stream.
 *
 * The Circular Mode (CIRC) bit determines whether the DMA
 * Stream stops after completing the configured number of data
 * transfers or automatically reloads the transfer counter and
 * continues transferring data.
 *
 * CIRC:
 *
 *      0 : Normal Mode
 *          - DMA stops after transfer completion.
 *
 *      1 : Circular Mode
 *          - DMA automatically restarts the transfer.
 *          - Commonly used for ADC, USART RX, DAC, etc.
 *
 * RM0090 Reference:
 * DMA_SxCR Register
 * CIRC Bit [8]
 *
 *********************************************************************/

 void DMA::setMode(DMA_Mode mode)
 {
    /* Clear Circular Mode Bit */
    mStream->CR &= ~(1 << 8);

    /* Configure Transfer Mode */
    mStream->CR |= (static_cast<uint32_t>(mode) << 8);
 }

 /*********************************************************************
 *                      Memory Increment Mode
 *
 * Configure the Memory Increment Mode for the selected DMA Stream.
 *
 * The Memory Increment (MINC) bit controls whether the memory
 * address is automatically incremented after each data transfer.
 *
 * MINC:
 *
 *      0 : Fixed Memory Address
 *          - Memory address remains unchanged.
 *
 *      1 : Increment Memory Address
 *          - Memory address increments according to the
 *            configured Memory Data Size (MSIZE).
 *
 * Memory Increment mode is commonly enabled when transferring
 * data to or from memory buffers.
 *
 * RM0090 Reference:
 * DMA_SxCR Register
 * MINC Bit [10]
 *
 *********************************************************************/
void DMA::setMemoryIncrement(DMA_IncrementMode mode)
{
   /* Clear Memory Increment Bit */
   mStream->CR &= ~(1 << 10);

   /* Configure Memory Increment Mode */
   mStream->CR |= (static_cast<uint32_t>(mode) << 10);
}

/*********************************************************************
 *                      Peripheral Data Size
 *
 * Configure the Peripheral Data Width for the selected DMA Stream.
 *
 * The Peripheral Data Size (PSIZE) bits determine the width of
 * each data item transferred between the peripheral and memory.
 *
 * PSIZE[1:0]:
 *
 *      00 : 8-bit (Byte)
 *      01 : 16-bit (Half Word)
 *      10 : 32-bit (Word)
 *
 * The selected data size should match the peripheral register
 * width to ensure correct data transfer.
 *
 * RM0090 Reference:
 * DMA_SxCR Register
 * PSIZE[1:0] Bits [12:11]
 *
 *********************************************************************/
void DMA::setPeripheralDataSize(DMA_DataSize size)
{
   /* Clear Peripheral Data Size Bits */
   mStream->CR &= ~(0x3 << 11);

    /* Configure Peripheral Data Size */
   mStream->CR |= (static_cast<uint32_t>(size) << 11);
}

/*********************************************************************
 *                      Memory Data Size
 *
 * Configure the Memory Data Width for the selected DMA Stream.
 *
 * The Memory Data Size (MSIZE) bits determine the width of
 * each data item transferred between memory and the peripheral.
 *
 * MSIZE[1:0]:
 *
 *      00 : 8-bit (Byte)
 *      01 : 16-bit (Half Word)
 *      10 : 32-bit (Word)
 *
 * The selected memory data size should match the memory buffer
 * element size. For efficient transfers, the memory data width
 * should generally match the peripheral data width.
 *
 * RM0090 Reference:
 * DMA_SxCR Register
 * MSIZE[1:0] Bits [14:13]
 *
 *********************************************************************/

 void DMA::setMemoryDataSize(DMA_DataSize size)
 {
   /* Clear Memory Data Size Bits */
   mStream->CR &= ~(0x3 << 13);

   mStream->CR |= (static_cast<uint32_t>(size) << 13);
 }

 /*********************************************************************
 *                      Peripheral Address Configuration
 *
 * Configure the Peripheral Address Register (PAR) for the selected
 * DMA Stream.
 *
 * The PAR register stores the address of the peripheral data
 * register involved in the DMA transfer.
 *
 * Depending on the configured transfer direction:
 *
 *      • Peripheral-to-Memory
 *          DMA reads data from the peripheral address.
 *
 *      • Memory-to-Peripheral
 *          DMA writes data to the peripheral address.
 *
 *      • Memory-to-Memory
 *          PAR is used as the source memory address.
 *
 * The address written to PAR should be the address of the
 * peripheral data register (e.g., USARTx->DR, SPIx->DR,
 * ADCx->DR, DAC->DHRx).
 *
 * RM0090 Reference:
 * DMA_SxPAR Register
 *
 *********************************************************************/

 void DMA::setPeripheralAddress(uint32_t address)
 {
   /* Configure Peripheral Address Register */
   mStream->PAR = address;
 }

 /*********************************************************************
 *                      Memory Address Configuration
 *
 * Configure the Memory 0 Address Register (M0AR) for the selected
 * DMA Stream.
 *
 * The M0AR register stores the starting address of the memory
 * buffer involved in the DMA transfer.
 *
 * Depending on the configured transfer direction:
 *
 *      • Peripheral-to-Memory
 *          DMA writes received data into the memory buffer.
 *
 *      • Memory-to-Peripheral
 *          DMA reads data from the memory buffer.
 *
 *      • Memory-to-Memory
 *          M0AR is used as the destination memory address.
 *
 * The address written to M0AR should be the starting address
 * of the memory buffer.
 *
 * Example:
 *
 *      uint8_t rxBuffer[100];
 *      dma.setMemoryAddress((uint32_t)rxBuffer);
 *
 * RM0090 Reference:
 * DMA_SxM0AR Register
 *
 *********************************************************************/

void DMA::setMemoryAddress(uint32_t address)
{
    /* Configure Memory 0 Address Register */
    mStream->M0AR = address;
}

/*********************************************************************
 *                      Transfer Length Configuration
 *
 * Configure the Number of Data Register (NDTR) for the selected
 * DMA Stream.
 *
 * The NDTR register specifies the total number of data items to
 * be transferred during a DMA transaction.
 *
 * The transfer counter is automatically decremented by the DMA
 * controller after each successful data transfer.
 *
 * When the counter reaches zero:
 *
 *      • In Normal Mode:
 *          - The transfer is completed.
 *          - The Transfer Complete (TCIF) flag is set.
 *          - The DMA Stream stops automatically.
 *
 *      • In Circular Mode:
 *          - NDTR is automatically reloaded.
 *          - The transfer restarts from the beginning of the
 *            configured memory buffer.
 *
 * The value programmed into NDTR represents the number of data
 * items, not the number of bytes.
 *
 * Examples:
 *
 *      uint8_t  buffer[100];
 *          NDTR = 100
 *
 *      uint16_t adcBuffer[256];
 *          NDTR = 256
 *
 *      uint32_t data[64];
 *          NDTR = 64
 *
 * RM0090 Reference:
 * DMA_SxNDTR Register
 *
 *********************************************************************/

void DMA::setTransferLength(uint16_t length)
{
    /* Configure Number of Data Items */
    mStream->NDTR = length;
}

/*********************************************************************
 *                      FIFO Threshold Configuration
 *
 * Configure the FIFO Threshold level for the selected DMA Stream.
 *
 * The FIFO Threshold (FTH) bits determine the level at which the
 * DMA FIFO generates a transfer request between the FIFO and the
 * destination interface.
 *
 * The STM32F407 DMA contains a 4-word FIFO buffer that can improve
 * transfer efficiency by reducing bus access overhead.
 *
 * FTH[1:0]:
 *
 *      00 : 1/4 Full FIFO
 *      01 : 1/2 Full FIFO
 *      10 : 3/4 Full FIFO
 *      11 : Full FIFO
 *
 * The FIFO threshold is only used when FIFO mode is enabled.
 *
 * RM0090 Reference:
 * DMA_SxFCR Register
 * FTH[1:0] Bits [1:0]
 *
 *********************************************************************/

void DMA::setFIFOThreshold(DMA_FIFOThreshold threshold)
{
    /* Clear FIFO Threshold Bits */
    mStream->FCR &= ~(0x3 << 0);

    /* Configure FIFO Threshold */
    mStream->FCR |= (static_cast<uint32_t>(threshold) << 0);
}

/*********************************************************************
 *                      Start DMA Transfer
 *
 * Enable the selected DMA Stream.
 *
 * Before calling this function, the following configuration should
 * already be completed:
 *
 *      • DMA Controller Clock Enabled
 *      • Stream Initialized
 *      • Channel Selected
 *      • Transfer Direction Configured
 *      • Priority Configured
 *      • Increment Modes Configured
 *      • Data Sizes Configured
 *      • Peripheral Address Configured
 *      • Memory Address Configured
 *      • Number of Data Items Configured
 *
 * Setting the EN bit starts the DMA Stream. Once enabled, the DMA
 * hardware begins servicing DMA requests from the configured
 * peripheral according to the selected transfer direction.
 *
 * RM0090 Reference:
 * DMA_SxCR Register
 * EN Bit [0]
 *
 *********************************************************************/

void DMA::start()
{
    /* Enable DMA Stream */
    mStream->CR |= (1 << 0);
}

/*********************************************************************
 *                      Stop DMA Transfer
 *
 * Disable the selected DMA Stream.
 *
 * Clearing the EN bit requests the DMA controller to stop the
 * current transfer. The DMA hardware completes any ongoing bus
 * transaction before clearing the EN bit.
 *
 * Software must wait until the EN bit becomes zero before
 * modifying any Stream configuration registers.
 *
 * This procedure prevents undefined behavior and ensures that
 * the Stream is completely disabled before reconfiguration.
 *
 * RM0090 Reference:
 * DMA_SxCR Register
 * EN Bit [0]
 *
 *********************************************************************/

void DMA::stop()
{
    /* Disable DMA Stream */
    mStream->CR &= ~(1 << 0);

    /* Wait until the Stream is disabled */
    while (mStream->CR & (1 << 0));
}

/*********************************************************************
 *                      DMA Stream Status
 *
 * Check whether the selected DMA Stream is currently enabled.
 *
 * The EN bit indicates the operational state of the DMA Stream.
 *
 * EN:
 *
 *      0 : DMA Stream Disabled
 *      1 : DMA Stream Enabled
 *
 * This function is useful for verifying whether a DMA transfer
 * has been started or whether the Stream has been successfully
 * disabled before reconfiguring its registers.
 *
 * Return:
 *
 *      true  -> DMA Stream Enabled
 *      false -> DMA Stream Disabled
 *
 * RM0090 Reference:
 * DMA_SxCR Register
 * EN Bit [0]
 *
 *********************************************************************/

bool DMA::isEnabled()
{
    /* Return DMA Stream Enable Status */
    return (mStream->CR & (1 << 0));
}
/*********************************************************************
 *                      Transfer Complete Status
 *
 * Check whether the DMA transfer has completed.
 *
 * During a DMA transfer, the DMA controller decrements the Number
 * of Data Register (NDTR) after each successful transfer. When
 * NDTR reaches zero, the Transfer Complete Flag (TCIFx) is set by
 * the DMA hardware.
 *
 * The Transfer Complete Flag is located in either the Low Interrupt
 * Status Register (LISR) or the High Interrupt Status Register
 * (HISR), depending on the selected DMA Stream.
 *
 * Stream Mapping:
 *
 *      Stream 0 : LISR TCIF0
 *      Stream 1 : LISR TCIF1
 *      Stream 2 : LISR TCIF2
 *      Stream 3 : LISR TCIF3
 *      Stream 4 : HISR TCIF4
 *      Stream 5 : HISR TCIF5
 *      Stream 6 : HISR TCIF6
 *      Stream 7 : HISR TCIF7
 *
 * Return:
 *
 *      true  -> Transfer Complete Flag Set
 *      false -> Transfer Complete Flag Cleared
 *
 * RM0090 Reference:
 * DMA_LISR Register
 * DMA_HISR Register
 * TCIFx Bits
 *
 *********************************************************************/

bool DMA::isTransferComplete()
{
    switch (mStreamNumber)
    {
        case DMA_Stream::STREAM0:
            return (mDMA->LISR & (1U << 5));

        case DMA_Stream::STREAM1:
            return (mDMA->LISR & (1U << 11));

        case DMA_Stream::STREAM2:
            return (mDMA->LISR & (1U << 21));

        case DMA_Stream::STREAM3:
            return (mDMA->LISR & (1U << 27));

        case DMA_Stream::STREAM4:
            return (mDMA->HISR & (1U << 5));

        case DMA_Stream::STREAM5:
            return (mDMA->HISR & (1U << 11));

        case DMA_Stream::STREAM6:
            return (mDMA->HISR & (1U << 21));

        case DMA_Stream::STREAM7:
            return (mDMA->HISR & (1U << 27));

        default:
            return false;
    }
}

bool DMA::isHalfTransfer()
{
    switch (mStreamNumber)
    {
        case DMA_Stream::STREAM0:
            return (mDMA->LISR & (1U << 4));

        case DMA_Stream::STREAM1:
            return (mDMA->LISR & (1U << 10));

        case DMA_Stream::STREAM2:
            return (mDMA->LISR & (1U << 20));

        case DMA_Stream::STREAM3:
            return (mDMA->LISR & (1U << 26));

        case DMA_Stream::STREAM4:
            return (mDMA->HISR & (1U << 4));

        case DMA_Stream::STREAM5:
            return (mDMA->HISR & (1U << 10));

        case DMA_Stream::STREAM6:
            return (mDMA->HISR & (1U << 20));

        case DMA_Stream::STREAM7:
            return (mDMA->HISR & (1U << 26));

        default:
            return false;
    }
}

/*********************************************************************
 *                      Transfer Error Status
 *
 * Check whether a Transfer Error has occurred during the DMA
 * transfer.
 *
 * The Transfer Error Flag (TEIFx) is set by the DMA controller
 * whenever a transfer error occurs. Possible causes include:
 *
 *      • Bus Error
 *      • Invalid Memory Access
 *      • Invalid Peripheral Access
 *      • Configuration Error
 *
 * The TEIF flag is located in either the Low Interrupt Status
 * Register (LISR) or the High Interrupt Status Register (HISR),
 * depending on the selected DMA Stream.
 *
 * Stream Mapping:
 *
 *      Stream 0 : LISR TEIF0
 *      Stream 1 : LISR TEIF1
 *      Stream 2 : LISR TEIF2
 *      Stream 3 : LISR TEIF3
 *      Stream 4 : HISR TEIF4
 *      Stream 5 : HISR TEIF5
 *      Stream 6 : HISR TEIF6
 *      Stream 7 : HISR TEIF7
 *
 * Return:
 *
 *      true  -> Transfer Error Flag Set
 *      false -> No Transfer Error
 *
 * RM0090 Reference:
 * DMA_LISR Register
 * DMA_HISR Register
 * TEIFx Bits
 *
 *********************************************************************/

bool DMA::isTransferError()
{
    switch (mStreamNumber)
    {
        case DMA_Stream::STREAM0:
            return (mDMA->LISR & (1U << 3));

        case DMA_Stream::STREAM1:
            return (mDMA->LISR & (1U << 9));

        case DMA_Stream::STREAM2:
            return (mDMA->LISR & (1U << 19));

        case DMA_Stream::STREAM3:
            return (mDMA->LISR & (1U << 25));

        case DMA_Stream::STREAM4:
            return (mDMA->HISR & (1U << 3));

        case DMA_Stream::STREAM5:
            return (mDMA->HISR & (1U << 9));

        case DMA_Stream::STREAM6:
            return (mDMA->HISR & (1U << 19));

        case DMA_Stream::STREAM7:
            return (mDMA->HISR & (1U << 25));

        default:
            return false;
    }
}

/*********************************************************************
 *                      FIFO Error Status
 *
 * Check whether a FIFO Error has occurred during the DMA transfer.
 *
 * The FIFO Error Flag (FEIFx) is set by the DMA controller whenever
 * a FIFO overrun or underrun condition is detected while FIFO mode
 * is enabled.
 *
 * The FEIF flag is located in either the Low Interrupt Status
 * Register (LISR) or the High Interrupt Status Register (HISR),
 * depending on the selected DMA Stream.
 *
 * Stream Mapping:
 *
 *      Stream 0 : LISR FEIF0
 *      Stream 1 : LISR FEIF1
 *      Stream 2 : LISR FEIF2
 *      Stream 3 : LISR FEIF3
 *      Stream 4 : HISR FEIF4
 *      Stream 5 : HISR FEIF5
 *      Stream 6 : HISR FEIF6
 *      Stream 7 : HISR FEIF7
 *
 * Return:
 *
 *      true  -> FIFO Error Flag Set
 *      false -> No FIFO Error
 *
 * RM0090 Reference:
 * DMA_LISR Register
 * DMA_HISR Register
 * FEIFx Bits
 *
 *********************************************************************/

bool DMA::isFIFOError()
{
    switch (mStreamNumber)
    {
        case DMA_Stream::STREAM0:
            return (mDMA->LISR & (1U << 0));

        case DMA_Stream::STREAM1:
            return (mDMA->LISR & (1U << 6));

        case DMA_Stream::STREAM2:
            return (mDMA->LISR & (1U << 16));

        case DMA_Stream::STREAM3:
            return (mDMA->LISR & (1U << 22));

        case DMA_Stream::STREAM4:
            return (mDMA->HISR & (1U << 0));

        case DMA_Stream::STREAM5:
            return (mDMA->HISR & (1U << 6));

        case DMA_Stream::STREAM6:
            return (mDMA->HISR & (1U << 16));

        case DMA_Stream::STREAM7:
            return (mDMA->HISR & (1U << 22));

        default:
            return false;
    }
}

/*********************************************************************
 *                      Direct Mode Error Status
 *
 * Check whether a Direct Mode Error has occurred during the DMA
 * transfer.
 *
 * The Direct Mode Error Flag (DMEIFx) is set by the DMA controller
 * whenever an error occurs while operating in Direct Mode.
 *
 * Direct Mode bypasses the internal FIFO and transfers data
 * directly between the peripheral and memory.
 *
 * The DMEIF flag is located in either the Low Interrupt Status
 * Register (LISR) or the High Interrupt Status Register (HISR),
 * depending on the selected DMA Stream.
 *
 * Stream Mapping:
 *
 *      Stream 0 : LISR DMEIF0
 *      Stream 1 : LISR DMEIF1
 *      Stream 2 : LISR DMEIF2
 *      Stream 3 : LISR DMEIF3
 *      Stream 4 : HISR DMEIF4
 *      Stream 5 : HISR DMEIF5
 *      Stream 6 : HISR DMEIF6
 *      Stream 7 : HISR DMEIF7
 *
 * Return:
 *
 *      true  -> Direct Mode Error Flag Set
 *      false -> No Direct Mode Error
 *
 * RM0090 Reference:
 * DMA_LISR Register
 * DMA_HISR Register
 * DMEIFx Bits
 *
 *********************************************************************/

bool DMA::isDirectModeError()
{
    switch (mStreamNumber)
    {
        case DMA_Stream::STREAM0:
            return (mDMA->LISR & (1U << 2));

        case DMA_Stream::STREAM1:
            return (mDMA->LISR & (1U << 8));

        case DMA_Stream::STREAM2:
            return (mDMA->LISR & (1U << 18));

        case DMA_Stream::STREAM3:
            return (mDMA->LISR & (1U << 24));

        case DMA_Stream::STREAM4:
            return (mDMA->HISR & (1U << 2));

        case DMA_Stream::STREAM5:
            return (mDMA->HISR & (1U << 8));

        case DMA_Stream::STREAM6:
            return (mDMA->HISR & (1U << 18));

        case DMA_Stream::STREAM7:
            return (mDMA->HISR & (1U << 24));

        default:
            return false;
    }
}

void DMA::clearTransferCompleteFlag()
{
    switch (mStreamNumber)
    {
        case DMA_Stream::STREAM0: mDMA->LIFCR |= (1U << 5); break;
        case DMA_Stream::STREAM1: mDMA->LIFCR |= (1U << 11); break;
        case DMA_Stream::STREAM2: mDMA->LIFCR |= (1U << 21); break;
        case DMA_Stream::STREAM3: mDMA->LIFCR |= (1U << 27); break;
        case DMA_Stream::STREAM4: mDMA->HIFCR |= (1U << 5); break;
        case DMA_Stream::STREAM5: mDMA->HIFCR |= (1U << 11); break;
        case DMA_Stream::STREAM6: mDMA->HIFCR |= (1U << 21); break;
        case DMA_Stream::STREAM7: mDMA->HIFCR |= (1U << 27); break;
    }
}

void DMA::clearHalfTransferFlag()
{
    switch (mStreamNumber)
    {
        case DMA_Stream::STREAM0: mDMA->LIFCR |= (1U << 4); break;
        case DMA_Stream::STREAM1: mDMA->LIFCR |= (1U << 10); break;
        case DMA_Stream::STREAM2: mDMA->LIFCR |= (1U << 20); break;
        case DMA_Stream::STREAM3: mDMA->LIFCR |= (1U << 26); break;
        case DMA_Stream::STREAM4: mDMA->HIFCR |= (1U << 4); break;
        case DMA_Stream::STREAM5: mDMA->HIFCR |= (1U << 10); break;
        case DMA_Stream::STREAM6: mDMA->HIFCR |= (1U << 20); break;
        case DMA_Stream::STREAM7: mDMA->HIFCR |= (1U << 26); break;
    }
}

void DMA::clearTransferErrorFlag()
{
    switch (mStreamNumber)
    {
        case DMA_Stream::STREAM0: mDMA->LIFCR |= (1U << 3); break;
        case DMA_Stream::STREAM1: mDMA->LIFCR |= (1U << 9); break;
        case DMA_Stream::STREAM2: mDMA->LIFCR |= (1U << 19); break;
        case DMA_Stream::STREAM3: mDMA->LIFCR |= (1U << 25); break;
        case DMA_Stream::STREAM4: mDMA->HIFCR |= (1U << 3); break;
        case DMA_Stream::STREAM5: mDMA->HIFCR |= (1U << 9); break;
        case DMA_Stream::STREAM6: mDMA->HIFCR |= (1U << 19); break;
        case DMA_Stream::STREAM7: mDMA->HIFCR |= (1U << 25); break;
    }
}

void DMA::clearFIFOErrorFlag()
{
    switch (mStreamNumber)
    {
        case DMA_Stream::STREAM0: mDMA->LIFCR |= (1U << 1); break;
        case DMA_Stream::STREAM1: mDMA->LIFCR |= (1U << 7); break;
        case DMA_Stream::STREAM2: mDMA->LIFCR |= (1U << 17); break;
        case DMA_Stream::STREAM3: mDMA->LIFCR |= (1U << 23); break;
        case DMA_Stream::STREAM4: mDMA->HIFCR |= (1U << 1); break;
        case DMA_Stream::STREAM5: mDMA->HIFCR |= (1U << 7); break;
        case DMA_Stream::STREAM6: mDMA->HIFCR |= (1U << 17); break;
        case DMA_Stream::STREAM7: mDMA->HIFCR |= (1U << 23); break;
    }
}

void DMA::clearDirectModeErrorFlag()
{
    switch (mStreamNumber)
    {
        case DMA_Stream::STREAM0: mDMA->LIFCR |= (1U << 2); break;
        case DMA_Stream::STREAM1: mDMA->LIFCR |= (1U << 8); break;
        case DMA_Stream::STREAM2: mDMA->LIFCR |= (1U << 18); break;
        case DMA_Stream::STREAM3: mDMA->LIFCR |= (1U << 24); break;
        case DMA_Stream::STREAM4: mDMA->HIFCR |= (1U << 2); break;
        case DMA_Stream::STREAM5: mDMA->HIFCR |= (1U << 8); break;
        case DMA_Stream::STREAM6: mDMA->HIFCR |= (1U << 18); break;
        case DMA_Stream::STREAM7: mDMA->HIFCR |= (1U << 24); break;
    }
}

void DMA::clearAllFlags()
{
    clearTransferCompleteFlag();
    clearHalfTransferFlag();
    clearTransferErrorFlag();
    clearFIFOErrorFlag();
    clearDirectModeErrorFlag();
}

void DMA::enableTransferCompleteInterrupt()
{
    mStream->CR |= (1U << 4);
}

void DMA::disableTransferCompleteInterrupt()
{
    mStream->CR &= ~(1U << 4);
}

void DMA::enableHalfTransferInterrupt()
{
    mStream->CR |= (1U << 3);
}

void DMA::disableHalfTransferInterrupt()
{
    mStream->CR &= ~(1U << 3);
}

void DMA::enableTransferErrorInterrupt()
{
    mStream->CR |= (1U << 2);
}

void DMA::disableTransferErrorInterrupt()
{
    mStream->CR &= ~(1U << 2);
}

void DMA::enableDirectModeErrorInterrupt()
{
    mStream->CR |= (1U << 1);
}

void DMA::disableDirectModeErrorInterrupt()
{
    mStream->CR &= ~(1U << 1);
}

void DMA::enableFIFOErrorInterrupt()
{
    mStream->FCR |= (1U << 7);
}

void DMA::disableFIFOErrorInterrupt()
{
    mStream->FCR &= ~(1U << 7);
}