# STM32F407VG Bare-Metal C++ Driver Framework - API Reference

This document provides a detailed API reference for the drivers implemented in the STM32F407VG Bare-Metal C++ Driver Framework.

For a practical guide on how to use these APIs, please see the [**Driver Guide**](./DRIVER_GUIDE.md).

---

## Table of Contents

- [Core Drivers](#core-drivers)
  - [RCC (Reset and Clock Control)](#rcc-reset-and-clock-control)
  - [NVIC (Nested Vectored Interrupt Controller)](#nvic-nested-vectored-interrupt-controller)
  - [SysTick (System Timer)](#systick-system-timer)
  - [SCB (System Control Block)](#scb-system-control-block)
- [Peripheral Drivers](#peripheral-drivers)
  - [GPIO (General-Purpose I/O)](#gpio-general-purpose-io)
  - [EXTI (External Interrupt/Event Controller)](#exti-external-interruptevent-controller)
  - [USART (Universal Synchronous/Asynchronous Receiver/Transmitter)](#usart-universal-synchronousasynchronous-receivertransmitter)
  - [SPI (Serial Peripheral Interface)](#spi-serial-peripheral-interface)
  - [I2C (Inter-Integrated Circuit)](#i2c-inter-integrated-circuit)
  - [ADC (Analog-to-Digital Converter)](#adc-analog-to-digital-converter)
  - [TIM (Timers)](#tim-timers)
  - [DMA (Direct Memory Access)](#dma-direct-memory-access)

---

## Core Drivers

### RCC (Reset and Clock Control)

**Driver Overview:** The RCC driver provides a static class for managing the STM32F407's Reset and Clock Control unit. It is responsible for configuring the system clock (SYSCLK), bus prescalers (AHB, APB1, APB2), and enabling/disabling/resetting peripheral clocks.

**Class:** `RCC`

**RM0090 Reference:** Chapter 7

#### Public APIs

All APIs in the `RCC` driver are static and can be called directly.

##### Clock Configuration APIs

- **Prototype:** `static void setSystemClockSource(ClockSource source, PLL_Config pll_config)`
  - **Description:** Sets the main system clock (SYSCLK) source to HSI, HSE, or PLL. If PLL is chosen, the `pll_config` struct must be provided.
  - **Parameters:** `source` (the desired clock source), `pll_config` (PLL configuration parameters).
  - **Registers:** `RCC->CR`, `RCC->PLLCFGR`, `RCC->CFGR`.

- **Prototype:** `static void configurePrescalers(AHBPrescaler ahb, APBPrescaler apb1, APBPrescaler apb2)`
  - **Description:** Configures the division factors (prescalers) for the AHB, APB1, and APB2 buses.
  - **Parameters:** `ahb` (AHB prescaler), `apb1` (APB1 prescaler), `apb2` (APB2 prescaler).
  - **Register:** `RCC->CFGR`.

##### Frequency Getter APIs

- **Prototype:** `static uint32_t getSystemClockFreq()`
  - **Description:** Returns the currently configured system clock (SYSCLK) frequency in Hz.
- **Prototype:** `static uint32_t getAHBClockFreq()`
  - **Description:** Returns the AHB bus clock (HCLK) frequency in Hz.
- **Prototype:** `static uint32_t getAPB1ClockFreq()`
  - **Description:** Returns the APB1 peripheral bus clock (PCLK1) frequency in Hz.
- **Prototype:** `static uint32_t getAPB2ClockFreq()`
  - **Description:** Returns the APB2 peripheral bus clock (PCLK2) frequency in Hz.
- **Prototype:** `static uint32_t getUSARTClock(RCC_USART usart)`
  - **Description:** Returns the clock frequency supplied to a specific USART peripheral.
- **Prototype:** `static uint32_t getSPIClock(RCC_SPI spi)`
  - **Description:** Returns the clock frequency supplied to a specific SPI peripheral.
- **Prototype:** `static uint32_t getI2CClock(RCC_I2C i2c)`
  - **Description:** Returns the clock frequency supplied to a specific I2C peripheral.
- **Prototype:** `static uint32_t getTimerClockFreq(TIM_Instance tim)`
  - **Description:** Returns the effective clock frequency for a timer, accounting for the APB prescaler doubling rule.

##### Peripheral Clock & Reset APIs

- **GPIO:** `enableGPIOClock`, `disableGPIOClock`, `resetGPIO`
- **USART:** `enableUSARTClock`, `disableUSARTClock`
- **I2C:** `enableI2CClock`, `disableI2CClock`
- **SPI:** `enableSPIClock`, `disableSPIClock`
- **ADC:** `enableADCClock`, `disableADCClock`
- **TIM:** `enableTIMClock`, `disableTIMClock`, `resetTIMClock`, `isTIMClockEnabled`
- **DMA:** `enableDMAClock`, `disableDMAClock`
- **SYSCFG:** `enableSYSCFGClock`

---

### NVIC (Nested Vectored Interrupt Controller)

**Driver Overview:** The NVIC driver provides a static class to configure the Nested Vectored Interrupt Controller. It allows enabling/disabling IRQs, setting interrupt priorities, and managing pending flags.

**Class:** `NVIC`

**RM0090 Reference:** ARM Cortex-M4 Generic User Guide

#### Public APIs

All APIs in the `NVIC` driver are static and can be called directly.

##### Interrupt Control APIs

- **Prototype:** `static void enableIRQ(uint8_t irq)`
  - **Description:** Enables a specific interrupt in the NVIC.
  - **Register:** `NVIC->ISERx`.
- **Prototype:** `static void disableIRQ(uint8_t irq)`
  - **Description:** Disables a specific interrupt in the NVIC.
  - **Register:** `NVIC->ICERx`.
- **Prototype:** `static void setPendingIRQ(uint8_t irq)`
  - **Description:** Sets the pending flag for an interrupt, forcing it to be serviced.
  - **Register:** `NVIC->ISPRx`.
- **Prototype:** `static void clearPendingIRQ(uint8_t irq)`
  - **Description:** Clears the pending flag for an interrupt.
  - **Register:** `NVIC->ICPRx`.

##### Status & Configuration APIs

- **Prototype:** `static bool getActiveIRQ(uint8_t irq)`
  - **Description:** Checks if a specific interrupt is currently active (being serviced).
  - **Register:** `NVIC->IABRx`.
- **Prototype:** `static bool isIRQEnabled(uint8_t irq)`
  - **Description:** Checks if a specific interrupt is enabled.
  - **Register:** `NVIC->ISERx`.
- **Prototype:** `static void setPriority(uint8_t irq, uint8_t priority)`
  - **Description:** Sets the priority for a specific interrupt.
  - **Register:** `NVIC->IPRx`.

---

### SysTick (System Timer)

**Driver Overview:** The SysTick driver configures the 24-bit Cortex-M4 system timer. It is primarily used to generate a periodic interrupt for a time base (e.g., 1ms ticks) and to provide blocking delay functions.

**Class:** `SysTick`

**RM0090 Reference:** ARM Cortex-M4 Generic User Guide

#### Public APIs

All APIs in the `SysTick` driver are static and can be called directly.

##### Initialization & Control APIs

- **Prototype:** `static void init(uint32_t tickFreq)`
  - **Description:** Initializes the SysTick timer to generate interrupts at a specified frequency. For a 1ms tick, `tickFreq` should be 1000.
  - **Registers:** `SysTick->LOAD`, `SysTick->VAL`, `SysTick->CTRL`.
- **Prototype:** `static void start()`
  - **Description:** Enables the SysTick counter and its interrupt.
  - **Register:** `SysTick->CTRL`.
- **Prototype:** `static void stop()`
  - **Description:** Disables the SysTick counter.
  - **Register:** `SysTick->CTRL`.

##### Helper APIs

- **Prototype:** `static void delayMs(uint32_t ms)`
  - **Description:** Provides a blocking delay in milliseconds. Requires `SysTick::init()` to be called first.
- **Prototype:** `static void delayUs(uint32_t us)`
  - **Description:** Provides a blocking delay in microseconds. Requires `SysTick::init()` to be called first.
- **Prototype:** `static uint32_t getTick()`
  - **Description:** Returns the global tick counter value.
- **Prototype:** `static void incrementTick()`
  - **Description:** Increments the global tick counter. This should be called from the `SysTick_Handler` ISR.

---

### SCB (System Control Block)

**Driver Overview:** Manages system-level configuration, exceptions, and faults.

**Class:** `SCB`

**RM0090 Reference:** ARM Cortex-M4 Generic User Guide

*Note: The `SCB_Driver.cpp` file is currently a skeleton with documentation but no implemented APIs. This section will be updated as the driver is implemented.*

---

## Peripheral Drivers

### GPIO (General-Purpose I/O)

**Driver Overview:** The GPIO driver provides an object-oriented interface to configure and control individual GPIO pins. Each `GPIO` object represents a single pin.

**Class:** `GPIO`

**RM0090 Reference:** Chapter 8

#### Constructor & Initialization

- **Prototype:** `GPIO(GPIO_Port port, uint8_t pin)`
  - **Description:** Creates a GPIO object for a specific pin on a specific port.
- **Prototype:** `void clockEnable()`
  - **Description:** Enables the clock for the associated GPIO port via the RCC.
  - **Register:** `RCC->AHB1ENR`.
- **Prototype:** `void deInit()`
  - **Description:** Resets the GPIO port registers to their default values via the RCC.
  - **Register:** `RCC->AHB1RSTR`.

#### Configuration APIs

- **Prototype:** `void setMode(GPIO_Mode mode)`
  - **Description:** Configures the pin's operating mode (Input, Output, Alternate Function, Analog).
  - **Register:** `GPIOx->MODER`.
- **Prototype:** `void setOutputType(GPIO_OutputType type)`
  - **Description:** Configures the output type (Push-Pull or Open-Drain).
  - **Register:** `GPIOx->OTYPER`.
- **Prototype:** `void setSpeed(GPIO_Speed speed)`
  - **Description:** Configures the pin's output speed.
  - **Register:** `GPIOx->OSPEEDR`.
- **Prototype:** `void setPull(GPIO_Pull pull)`
  - **Description:** Configures the internal pull-up/pull-down resistor.
  - **Register:** `GPIOx->PUPDR`.
- **Prototype:** `void setAlternateFunction(uint8_t af)`
  - **Description:** Configures the alternate function mapping for the pin (AF0-AF15).
  - **Registers:** `GPIOx->AFR[0]`, `GPIOx->AFR[1]`.

##### I/O Control APIs

- **Prototype:** `void write(GPIO_State state)`
  - **Description:** Sets or clears the pin's output state.
  - **Register:** `GPIOx->BSRR`.
- **Prototype:** `GPIO_State read()`
  - **Description:** Reads the pin's input state.
  - **Register:** `GPIOx->IDR`.
- **Prototype:** `void toggle()`
  - **Description:** Toggles the pin's output state.
  - **Register:** `GPIOx->ODR`.

#### Typical Usage

```cpp
// Configure PA5 as a push-pull output
GPIO led(GPIO_Port::PORTA, 5);
led.clockEnable();
led.setMode(GPIO_Mode::OUTPUT);
led.setOutputType(GPIO_OutputType::PUSH_PULL);
led.setSpeed(GPIO_Speed::LOW);
led.setPull(GPIO_Pull::NONE);

// Control the pin
led.write(GPIO_State::HIGH);
```

---

### EXTI (External Interrupt/Event Controller)

**Driver Overview:** The EXTI driver configures external/event interrupts for a specific GPIO pin. It maps a pin to an EXTI line and configures the trigger source (rising, falling, or both).

**Class:** `EXTI`

**RM0090 Reference:** Chapter 12

#### Constructor & Initialization

- **Prototype:** `EXTI(RCC_GPIO port, uint8_t pin)`
  - **Description:** Creates an EXTI object for a specific GPIO pin. This automatically enables the `SYSCFG` clock and configures the `SYSCFG_EXTICR` register to map the pin to the EXTI line.
  - **Registers:** `RCC->APB2ENR`, `SYSCFG->EXTICR[]`.

#### Configuration APIs

- **Prototype:** `void setTrigger(EXTI_Trigger trigger)`
  - **Description:** Configures the trigger edge for the interrupt (Rising, Falling, or Both).
  - **Registers:** `EXTI->RTSR`, `EXTI->FTSR`.

##### Interrupt & Event Control APIs

- **Prototype:** `void enableInterrupt()`
  - **Description:** Enables the interrupt request for the configured EXTI line.
  - **Register:** `EXTI->IMR`.
- **Prototype:** `void disableInterrupt()`
  - **Description:** Disables the interrupt request for the configured EXTI line.
  - **Register:** `EXTI->IMR`.
- **Prototype:** `void enableEvent()`
  - **Description:** Enables the event request for the configured EXTI line.
  - **Register:** `EXTI->EMR`.
- **Prototype:** `void disableEvent()`
  - **Description:** Disables the event request for the configured EXTI line.
  - **Register:** `EXTI->EMR`.

##### Status & Flag APIs

- **Prototype:** `bool isPending()`
  - **Description:** Checks if an interrupt is pending on the EXTI line.
  - **Register:** `EXTI->PR`.
- **Prototype:** `void clearPending()`
  - **Description:** Clears the pending flag for the EXTI line by writing a '1' to the corresponding bit.
  - **Register:** `EXTI->PR`.

#### Typical Usage

```cpp
// Configure PC13 for a falling edge interrupt
RCC::enableGPIOClock(RCC_GPIO::GPIOC);
GPIO button_pin(GPIO_Port::PORTC, 13);
button_pin.setMode(GPIO_Mode::INPUT);
button_pin.setPull(GPIO_Pull::NONE);

EXTI button_exti(RCC_GPIO::GPIOC, 13);
button_exti.setTrigger(EXTI_Trigger::FALLING);
button_exti.enableInterrupt();

// Enable the corresponding IRQ in the NVIC
NVIC::enableIRQ(EXTI15_10_IRQn);
NVIC::setPriority(EXTI15_10_IRQn, 5);
```

---

### I2C (Inter-Integrated Circuit)

**Purpose:** Provides master-mode I2C communication.

**Class:** `I2C`

**RM0090 Reference:** Chapter 27

#### Constructor

- `I2C(I2C_Instance instance)`: Creates an I2C object for a specific I2C peripheral (I2C1, I2C2, or I2C3).

#### Public APIs

##### Initialization & Control

- `void init(uint32_t clockSpeed, I2C_ACK ack)`: Initializes the I2C peripheral with a specified clock speed and ACK setting.
- `void enableClock()`: Enables the clock for the I2C peripheral.
- `void enable()`: Enables the I2C peripheral.
- `void disable()`: Disables the I2C peripheral.
- `void generateStart()`: Generates an I2C START condition.
- `void generateStop()`: Generates an I2C STOP condition.

##### Data Transfer (Master Polling)

- `void sendAddress(uint8_t address, bool read)`: Sends a 7-bit slave address with the R/W bit.
- `void sendByte(uint8_t data)`: Transmits a single byte.
- `uint8_t receiveByte()`: Receives a single byte.
- `void masterTransmit(uint8_t slaveAddress, const uint8_t* buffer, uint32_t length)`: Performs a complete master transmission.
- `void masterReceive(uint8_t slaveAddress, uint8_t* buffer, uint32_t length)`: Performs a complete master reception.

##### Status Flags

- `bool isStartGenerated()`: Checks if a START condition has been generated (SB flag).
- `bool isAddressSent()`: Checks if the address has been sent (ADDR flag).
- `bool isTXE()`: Checks if the transmit data register is empty (TXE flag).
- `bool isRXNE()`: Checks if the receive data register is not empty (RXNE flag).
- `bool isTransferFinished()`: Checks if the byte transfer is finished (BTF flag).
- `bool isBusy()`: Checks if the I2C bus is busy (BUSY flag).

#### Typical Usage

```cpp
I2C i2c1(I2C_Instance::I2C1);
i2c1.init(100000, I2C_ACK::ENABLE); // 100kHz, ACK enabled

uint8_t data_to_send[] = {0x01, 0x02};
i2c1.masterTransmit(0x68, data_to_send, 2);
```

---

### DMA (Direct Memory Access)

**Purpose:** Manages memory-to-peripheral, peripheral-to-memory, and memory-to-memory data transfers without CPU intervention.

**Class:** `DMA`

**RM0090 Reference:** Chapter 10

#### Constructor

- `DMA(DMA_Instance dma, DMA_Stream stream)`: Creates a DMA object for a specific stream on a specific DMA controller.

#### Public APIs

##### Initialization & Control

- `void init()`: Enables the DMA clock, disables the stream, and resets its registers.
- `void enableClock()`: Enables the clock for the associated DMA controller.
- `void start()`: Enables the DMA stream to start the transfer.
- `void stop()`: Disables the DMA stream.

##### Configuration

- `void setChannel(DMA_Channel channel)`: Selects the channel for the stream.
- `void setDirection(DMA_Direction direction)`: Configures the transfer direction.
- `void setPriority(DMA_Priority priority)`: Sets the stream's priority level.
- `void setMode(DMA_Mode mode)`: Configures the transfer mode (Normal or Circular).
- `void setMemoryIncrement(DMA_IncrementMode mode)`: Enables or disables memory address increment.
- `void setPeripheralDataSize(DMA_DataSize size)`: Sets the data width for the peripheral.
- `void setMemoryDataSize(DMA_DataSize size)`: Sets the data width for the memory.
- `void setPeripheralAddress(uint32_t address)`: Sets the peripheral's data register address.
- `void setMemoryAddress(uint32_t address)`: Sets the memory buffer's base address.
- `void setTransferLength(uint16_t length)`: Sets the number of data items to transfer.
- `void setFIFOThreshold(DMA_FIFOThreshold threshold)`: Configures the FIFO threshold level.

##### Status Flags

- `bool isEnabled()`: Checks if the stream is currently enabled.
- `bool isTransferComplete()`: Checks for the Transfer Complete flag.
- `bool isHalfTransfer()`: Checks for the Half Transfer flag.
- `bool isTransferError()`: Checks for a Transfer Error.
- `bool isFIFOError()`: Checks for a FIFO Error.
- `bool isDirectModeError()`: Checks for a Direct Mode Error.

##### Flag Clearing

- `void clearTransferCompleteFlag()`: Clears the Transfer Complete flag.
- `void clearHalfTransferFlag()`: Clears the Half Transfer flag.
- `void clearTransferErrorFlag()`: Clears the Transfer Error flag.
- `void clearFIFOErrorFlag()`: Clears the FIFO Error flag.
- `void clearDirectModeErrorFlag()`: Clears the Direct Mode Error flag.
- `void clearAllFlags()`: Clears all flags for the stream.

##### Interrupt Control

- `void enableTransferCompleteInterrupt()`: Enables the Transfer Complete interrupt.
- `void disableTransferCompleteInterrupt()`: Disables the Transfer Complete interrupt.
- `void enableHalfTransferInterrupt()`: Enables the Half Transfer interrupt.
- `void disableHalfTransferInterrupt()`: Disables the Half Transfer interrupt.
- `void enableTransferErrorInterrupt()`: Enables the Transfer Error interrupt.
- `void disableTransferErrorInterrupt()`: Disables the Transfer Error interrupt.
- `void enableDirectModeErrorInterrupt()`: Enables the Direct Mode Error interrupt.
- `void disableDirectModeErrorInterrupt()`: Disables the Direct Mode Error interrupt.
- `void enableFIFOErrorInterrupt()`: Enables the FIFO Error interrupt.
- `void disableFIFOErrorInterrupt()`: Disables the FIFO Error interrupt.

#### Typical Usage

```cpp
// Configure DMA1 Stream 5, Channel 4 for USART1 RX
DMA dma_usart1_rx(DMA_Instance::DMA2, DMA_Stream::STREAM5);
dma_usart1_rx.init();
dma_usart1_rx.setChannel(DMA_Channel::CHANNEL4);
dma_usart1_rx.setDirection(DMA_Direction::PERIPHERAL_TO_MEMORY);
dma_usart1_rx.setPeripheralAddress(reinterpret_cast<uint32_t>(&(USART1_REG->DR)));
dma_usart1_rx.setMemoryAddress(reinterpret_cast<uint32_t>(rx_buffer));
dma_usart1_rx.setTransferLength(10);
dma_usart1_rx.setMemoryIncrement(DMA_IncrementMode::INCREMENT);
dma_usart1_rx.setMode(DMA_Mode::NORMAL);

dma_usart1_rx.start();
```