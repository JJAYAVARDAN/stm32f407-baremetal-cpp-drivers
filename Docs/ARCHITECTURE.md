# STM32F407VG Driver Framework - Architecture Guide

This document provides an in-depth explanation of the software architecture, design principles, and organization of the STM32F407VG Bare-Metal C++ Driver Framework.

**Quick Links:**
- [API Reference](./API_REFERENCE.md)
- [Driver Guide](./DRIVER_GUIDE.md)

---

## Table of Contents

- [1. Architectural Goals](#1-architectural-goals)
- [2. Layered Architecture](#2-layered-architecture)
  - [Hardware Layer](#hardware-layer)
  - [Driver Layer](#driver-layer)
  - [Application Layer](#application-layer)
- [3. Driver Organization](#3-driver-organization)
  - [Core vs. Peripheral Drivers](#core-vs-peripheral-drivers)
  - [Directory Structure](#directory-structure)
- [4. Driver Design Template](#4-driver-design-template)
  - [Register Definitions](#register-definitions)
  - [Driver Class](#driver-class)
  - [Public API Organization](#public-api-organization)
- [5. Design Principles & Rationale](#5-design-principles--rationale)
  - [No HAL/LL Dependency](#no-halll-dependency)
  - [C++ Encapsulation](#c-encapsulation)
  - [Static vs. Instance-Based Drivers](#static-vs-instance-based-drivers)
  - [Scalability](#scalability)

---

## 1. Architectural Goals

The framework is designed to meet the following goals:
- **Performance:** Direct register access ensures minimal overhead.
- **Clarity:** Code should be self-documenting and easy to trace back to the reference manual (RM0090).
- **Reusability:** Drivers are modular and can be easily integrated into various applications.
- **Type Safety:** C++ `enum class` is used extensively to prevent common configuration errors.
- **Maintainability:** A consistent structure across all drivers makes the framework easy to extend and debug.

## 2. Layered Architecture

The framework follows a classic three-layer embedded architecture.

```
+-----------------------+
|   Application Layer   |  (e.g., main.cpp, Led_Blink_App.cpp)
+-----------------------+
           |
           v
+-----------------------+
|     Driver Layer      |  (e.g., GPIO_Driver, I2C_Driver)
+-----------------------+
           |
           v
+-----------------------+
|    Hardware Layer     |  (STM32F407VG Memory-Mapped Registers)
+-----------------------+
```

### Hardware Layer
This is the lowest level, representing the physical memory-mapped registers of the STM32F407VG microcontroller. The framework does not abstract this layer away; instead, it provides a structured way to access it.

### Driver Layer
This is the core of the framework. It contains C++ classes that encapsulate the logic for controlling a specific hardware peripheral. Each driver provides a high-level API (e.g., `i2c.masterTransmit()`) that translates into a sequence of register writes and reads.

### Application Layer
This is the user's code. It includes the framework's umbrella header (`STM32F407_Drivers.h`) and uses the driver classes to build a specific application.

## 3. Driver Organization

### Core vs. Peripheral Drivers

The drivers are split into two logical groups:

- **Core Drivers (`Core/`)**: These manage peripherals that are part of the ARM Cortex-M4 core itself or are fundamental to the MCU's operation.
  - `RCC`: Manages clocks for the entire system.
  - `NVIC`: Manages interrupts.
  - `SysTick`: Provides the system time base.
  - `SCB`: System Control Block.

- **Peripheral Drivers (`Drivers/`)**: These manage the on-chip peripherals provided by STMicroelectronics.
  - `GPIO`, `USART`, `I2C`, `SPI`, `DMA`, `TIM`, `ADC`, etc.

### Directory Structure

The file system reflects this logical separation:

```
├── Core/
│   ├── Inc/          // Core driver headers
│   └── Src/          // Core driver sources
└── Drivers/
    ├── Inc/          // Public headers (STM32F407_Drivers.h)
    ├── ADC/
    │   ├── Inc/
    │   └── Src/
    ├── DMA/
    │   ├── Inc/
    │   └── Src/
    └── ...
```

## 4. Driver Design Template

All drivers follow a consistent design pattern for predictability and ease of use.

### Register Definitions
Each driver header defines a `struct` that mirrors the peripheral's register map as described in RM0090. A macro then provides a typed pointer to the peripheral's base address.

```cpp
// Example from GPIO_Driver.h
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    // ... other registers
} GPIO_RegDef_t;

#define GPIOA ((GPIO_RegDef_t*)GPIOA_BASE_ADDR)
```

### Driver Class
A C++ class encapsulates the peripheral's logic. Member variables store the peripheral instance and configuration.

### Public API Organization
APIs within a class are grouped by function:
- **Constructor/Initialization**: To create an object and set it up.
- **Configuration**: To set operating parameters.
- **Control**: To start, stop, or enable/disable features.
- **Status/Data**: To read status flags or transfer data.
- **Interrupt**: To manage interrupt sources and flags.

## 5. Design Principles & Rationale

### No HAL/LL Dependency
By avoiding ST's libraries, the framework remains lightweight, transparent, and gives the developer full control over the hardware. This is ideal for applications where performance and code size are critical.

### C++ Encapsulation
Using classes (e.g., `GPIO led(PORTA, 5)`) provides an intuitive, object-oriented way to manage hardware. It prevents global state and allows multiple instances of a peripheral (like `I2C1` and `I2C2`) to be managed cleanly.

### Static vs. Instance-Based Drivers
- **Static (`RCC`, `NVIC`)**: For singleton peripherals where only one "instance" exists in the MCU, a static class is used. APIs are called directly, e.g., `RCC::enableGPIOClock()`.
- **Instance-Based (`GPIO`, `I2C`)**: For peripherals with multiple instances or those managed on a per-unit basis (like a single GPIO pin), an object is instantiated, e.g., `I2C i2c1(I2C_Instance::I2C1)`.

### Scalability
The consistent driver template makes it straightforward to add new peripheral drivers. A developer can create a new folder, define the register map, implement the class, and add the header to the umbrella include without modifying the existing architecture.