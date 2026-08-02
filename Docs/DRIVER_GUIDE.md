# STM32F407VG Driver Framework - User Guide

Welcome to the user guide for the STM32F407VG Bare-Metal C++ Driver Framework. This document explains the project's philosophy, how to use the drivers, and provides practical examples for each implemented peripheral.

**Quick Links:**
- [API Reference](./API_REFERENCE.md)
- [Architecture Guide](./ARCHITECTURE.md)

---

## Table of Contents

- [Framework Overview](#framework-overview)
  - [Project Philosophy](#project-philosophy)
  - [Folder Structure](#folder-structure)
- [Getting Started](#getting-started)
  - [Including the Framework](#including-the-framework)
  - [Typical Application Flow](#typical-application-flow)
- [Driver Usage Examples](#driver-usage-examples)
  - [Core Drivers](#core-drivers)
    - [RCC: Clock Configuration](#rcc-clock-configuration)
    - [SysTick: 1ms Time Base](#systick-1ms-time-base)
  - [Peripheral Drivers](#peripheral-drivers)
    - [GPIO: Blinking an LED](#gpio-blinking-an-led)
    - [EXTI: Button Interrupt](#exti-button-interrupt)
    - [USART: UART Loopback](#usart-uart-loopback)
    - [I2C: Master Transmit](#i2c-master-transmit)
    - [DMA: Memory-to-Memory Transfer](#dma-memory-to-memory-transfer)

---

## Framework Overview

### Project Philosophy

This framework is designed with a "close-to-the-metal" philosophy. It avoids complex abstractions and dependencies (like STM32 HAL/LL) in favor of a direct, object-oriented C++ interface to the MCU's hardware registers.

The key principles are:
- **Direct Register Access:** Drivers interact directly with memory-mapped peripheral registers, providing transparency and performance.
- **C++ Encapsulation:** C++ classes and strongly-typed enums are used to create safe, readable, and reusable APIs.
- **No Magic:** The framework does not hide hardware behavior. Its structure closely follows the logic presented in the STM32F407 reference manual (RM0090).
- **Modularity:** Each peripheral driver is self-contained, promoting clean code and easy maintenance.

### Folder Structure

The project is organized to separate core logic, peripheral drivers, and application code.

```
Drivers/
├── Core/               # Core MCU drivers (RCC, NVIC, SysTick)
│   ├── Inc/
│   └── Src/
├── Drivers/            # Peripheral drivers (GPIO, I2C, DMA, etc.)
│   ├── Inc/            # Public umbrella header
│   ├── ADC/
│   ├── DMA/
│   └── ...
├── Applications/       # Example applications
└── ...
```

---

## Getting Started

### Including the Framework

To simplify development, the framework provides a single public "umbrella" header that includes all implemented drivers. This is the recommended method for all new applications.

```cpp
#include "STM32F407_Drivers.h"

int main() {
    // All driver classes (RCC, GPIO, I2C, etc.) are now available.
    // ... your code here
}
```

### Typical Application Flow

Using any peripheral in a bare-metal environment follows a consistent sequence. The framework's APIs are designed to mirror this logical flow.

1.  **Enable Clock (RCC)**: Before a peripheral can be used, its clock must be enabled. This is always the first step.
2.  **Configure Peripheral**: Set up the peripheral's operating parameters (e.g., GPIO mode, USART baud rate). For peripherals requiring GPIO pins, this includes configuring the pin's alternate function.
3.  **Initialize Peripheral**: Write the configuration to the hardware registers.
4.  **Enable Peripheral**: Activate the peripheral to begin operation.
5.  **Use Peripheral**: Perform data transfers or other operations.

This can be visualized as:

```
Enable Clock (RCC)
        ↓
Configure GPIOs (if needed)
        ↓
Configure Peripheral (e.g., USART, I2C)
        ↓
Enable Peripheral
        ↓
Start Data Transfer / Operation
```

---

## Driver Usage Examples

### Core Drivers

#### RCC: Clock Configuration

This example configures the system to run from the 8MHz external crystal (HSE).

```cpp
#include "STM32F407_Drivers.h"

void configure_clocks() {
    // Use HSE as the system clock source
    // Note: PLL_Config is not used here but must be passed.
    RCC::setSystemClockSource(ClockSource::HSE, {});

    // Set bus prescalers (AHB=1, APB1=2, APB2=1)
    RCC::configurePrescalers(AHBPrescaler::DIV1, APBPrescaler::DIV2, APBPrescaler::DIV1);
}
```

#### SysTick: 1ms Time Base

This example sets up the SysTick timer to generate a 1ms interrupt, which is used for a simple `delayMs` function.

```cpp
#include "STM32F407_Drivers.h"

// SysTick interrupt handler
extern "C" void SysTick_Handler() {
    SysTick::incrementTick();
}

int main() {
    // Initialize SysTick for a 1000 Hz (1ms) tick
    SysTick::init(1000);
    SysTick::start();

    while(1) {
        // Use the delay function
        SysTick::delayMs(500);
    }
}
```

### Peripheral Drivers

#### GPIO: Blinking an LED

This is the "Hello, World!" of embedded systems. It configures GPIO pin PA5 as an output and toggles it.

```cpp
#include "STM32F407_Drivers.h"

int main() {
    // 1. Enable GPIOA clock
    RCC::enableGPIOClock(RCC_GPIO::GPIOA);

    // 2. Configure PA5 as an output
    GPIO led(GPIO_Port::PORTA, 5);
    led.setMode(GPIO_Mode::OUTPUT);
    led.setOutputType(GPIO_OutputType::PUSH_PULL);
    led.setSpeed(GPIO_Speed::LOW);
    led.setPull(GPIO_Pull::NONE);

    while(1) {
        // 3. Use the peripheral
        led.toggle();
        SysTick::delayMs(500); // Assumes SysTick is initialized
    }
}
```

#### EXTI: Button Interrupt

This example configures pin PC13 (often connected to a user button) to trigger an interrupt on a falling edge.

```cpp
#include "STM32F407_Drivers.h"

extern "C" void EXTI15_10_IRQHandler() {
    // Check if the interrupt is from line 13 and clear it
    if (EXTI_REG->PR & (1 << 13)) {
        EXTI_REG->PR |= (1 << 13); // Clear by writing 1
        // Handle button press...
    }
}

void setup_button_interrupt() {
    // 1. Enable clocks for GPIOC and SYSCFG
    RCC::enableGPIOClock(RCC_GPIO::GPIOC);
    RCC::enableSYSCFGClock();

    // 2. Configure PC13 as an input
    GPIO button_pin(GPIO_Port::PORTC, 13);
    button_pin.setMode(GPIO_Mode::INPUT);

    // 3. Configure EXTI line 13
    EXTI button_exti(RCC_GPIO::GPIOC, 13);
    button_exti.setTrigger(EXTI_Trigger::FALLING);
    button_exti.enableInterrupt();

    // 4. Enable the IRQ in the NVIC
    NVIC::enableIRQ(EXTI15_10_IRQn);
    NVIC::setPriority(EXTI15_10_IRQn, 5);
}
```

#### I2C: Master Transmit

This example shows how to initialize I2C1 and send two bytes to a slave device at address `0x68`.

```cpp
#include "STM32F407_Drivers.h"

void i2c_example() {
    // 1. Enable clocks for I2C1 and GPIOB
    RCC::enableI2CClock(RCC_I2C::I2C1);
    RCC::enableGPIOClock(RCC_GPIO::GPIOB);

    // 2. Configure GPIOs for I2C1 (PB6=SCL, PB7=SDA)
    GPIO scl(GPIO_Port::PORTB, 6);
    scl.setMode(GPIO_Mode::ALTERNATE_FUNCTION);
    scl.setAlternateFunction(4); // AF4 for I2C1
    scl.setOutputType(GPIO_OutputType::OPEN_DRAIN);
    scl.setPull(GPIO_Pull::PULL_UP);

    GPIO sda(GPIO_Port::PORTB, 7);
    sda.setMode(GPIO_Mode::ALTERNATE_FUNCTION);
    sda.setAlternateFunction(4); // AF4 for I2C1
    sda.setOutputType(GPIO_OutputType::OPEN_DRAIN);
    sda.setPull(GPIO_Pull::PULL_UP);

    // 3. Initialize I2C1
    I2C i2c1(I2C_Instance::I2C1);
    i2c1.init(100000, I2C_ACK::ENABLE); // 100kHz Standard Mode

    // 4. Transmit data
    uint8_t data_to_send[] = {0x01, 0x02};
    i2c1.masterTransmit(0x68, data_to_send, 2);
}
```

#### DMA: Memory-to-Memory Transfer

This example configures DMA2 Stream 0 to copy 10 bytes from a source buffer to a destination buffer.

```cpp
#include "STM32F407_Drivers.h"

uint8_t src_buffer[10] = "HelloDMA!";
uint8_t dst_buffer[10] = {0};

void dma_mem2mem_example() {
    // 1. Create DMA object
    DMA dma_m2m(DMA_Instance::DMA2, DMA_Stream::STREAM0);

    // 2. Initialize and configure the stream
    dma_m2m.init(); // Enables clock and resets stream
    dma_m2m.setChannel(DMA_Channel::CHANNEL0);
    dma_m2m.setDirection(DMA_Direction::MEMORY_TO_MEMORY);
    dma_m2m.setMemoryIncrement(DMA_IncrementMode::INCREMENT);
    dma_m2m.setPeripheralDataSize(DMA_DataSize::BYTE); // For M2M, "peripheral" is the source
    dma_m2m.setMemoryDataSize(DMA_DataSize::BYTE);
    dma_m2m.setPeripheralAddress(reinterpret_cast<uint32_t>(src_buffer));
    dma_m2m.setMemoryAddress(reinterpret_cast<uint32_t>(dst_buffer));
    dma_m2m.setTransferLength(10);

    // 3. Start the transfer
    dma_m2m.start();

    // 4. Wait for completion
    while (!dma_m2m.isTransferComplete());

    // At this point, dst_buffer contains "HelloDMA!"
}
```