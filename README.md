# STM32F407 Bare-Metal C++ Driver Framework

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
![Platform](https://img.shields.io/badge/platform-STM32F407VG-blue)
![Language](https://img.shields.io/badge/language-C%2B%2B-00599C)
![Style](https://img.shields.io/badge/style-Bare--Metal%20%7C%20No%20HAL-critical)

> A register-level, object-oriented C++ peripheral driver framework for the STM32F407VG microcontroller — built entirely from the STM32F407 Reference Manual (RM0090), with no dependency on STM32Cube HAL or LL libraries.

---

## Table of Contents

- [1. Project Overview](#1-project-overview)
- [2. Features](#2-features)
- [3. Architecture](#3-architecture)
- [4. Project Folder Structure](#4-project-folder-structure)
- [5. Driver Design Approach](#5-driver-design-approach)
- [License](#license)
- [Author](#author)

---

## 1. Project Overview

This project is a **bare-metal embedded C++ driver framework** developed for the **STM32F407VG** microcontroller. The objective is to understand and implement low-level peripheral drivers **without relying on STM32 HAL libraries**, working directly against the hardware at the register level.

The framework is built around the following principles:

- **Direct register-level programming** — every driver interacts with hardware registers directly, giving full control and transparency over peripheral behavior, with no abstraction overhead from vendor libraries.
- **STM32F407 Reference Manual (RM0090) based implementation** — all driver logic is derived from and validated against the official reference manual, ensuring register-accurate, correct behavior.
- **C++ object-oriented driver architecture** — peripherals are modeled as C++ classes, using encapsulation and type safety to produce driver code that is modular, predictable, and easy to extend.
- **Reusable peripheral drivers** — each driver is designed as a self-contained, portable unit that can be dropped into other STM32F4-based projects with minimal changes.
- **Hardware abstraction layer approach** — a clear boundary is maintained between application logic and hardware-specific details, so application code stays readable and hardware changes stay contained to the driver layer.

This project serves both as a production-oriented driver library and as a deep, hands-on study of the STM32F4 architecture.

---

## 2. Features

### Implemented Drivers

**Core System Drivers**

| Driver | Description |
|---|---|
| **RCC** | Reset and Clock Control — clock source selection, PLL configuration, peripheral clock gating |
| **NVIC** | Nested Vectored Interrupt Controller — interrupt enable/priority/vector management |
| **SysTick** | System timer for blocking delays and time-base generation |
| **SCB** | System Control Block — core exception and system-level configuration |

**Peripheral Drivers**

| Driver | Description |
|---|---|
| **GPIO** | General Purpose Input/Output |
| **USART** | Universal Synchronous/Asynchronous Receiver Transmitter |
| **SPI** | Serial Peripheral Interface |
| **I2C** | Inter-Integrated Circuit |
| **ADC** | Analog-to-Digital Converter |
| **TIM** | General-purpose and advanced-control Timers |
| **DMA** | Direct Memory Access |
| **CAN** | Controller Area Network |
| **RTC** | Real-Time Clock |
| **DAC** | Digital-to-Analog Converter |
| **FLASH** | Flash memory interface control |
| **CRC** | Cyclic Redundancy Check |
| **RNG** | Random Number Generator |
| **PWR** | Power Control |

---

## 3. Architecture

The framework follows a **layered architecture** to enforce separation of concerns and keep every layer independently testable and replaceable:

```
Application Layer
        |
Middleware Layer
        |
Peripheral Driver Layer
        |
Core System Layer
        |
STM32F407 Registers
```

| Layer | Responsibility |
|---|---|
| **Application Layer** | User/firmware logic built on top of the drivers |
| **Middleware Layer** | Cross-cutting reusable components (logging, buffering, protocols) |
| **Peripheral Driver Layer** | Object-oriented drivers for multi-instance peripherals |
| **Core System Layer** | Static drivers for single-instance system resources |
| **STM32F407 Registers** | The physical memory-mapped hardware registers |

### Design Rationale

**Core drivers use static APIs**, because the microcontroller exposes only **one instance** of these resources — there is nothing to differentiate between multiple objects, so a static interface avoids unnecessary instantiation overhead.

Examples: `RCC`, `NVIC`, `SysTick`, `SCB`

**Peripheral drivers use object-oriented classes**, because the microcontroller exposes **multiple instances** of these peripherals. Each object binds to one specific hardware instance, allowing independent configuration and operation without instances interfering with one another.

Examples: `GPIOA` / `GPIOB` ..., `USART1` / `USART2` ..., `TIM2` / `TIM3` ..., `ADC1` / `ADC2` ...

---

## 4. Project Folder Structure

```
STM32F407-BareMetal-CPP
│
├── Core
│   ├── Inc                 // Header files for Core System Drivers (RCC, NVIC, SysTick, SCB)
│   └── Src                 // Source files for Core System Drivers (RCC, NVIC, SysTick, SCB)
│
├── Drivers
│   │
│   ├── GPIO
│   │   ├── Inc             // GPIO Driver headers
│   │   └── Src             // GPIO Driver sources
│   │
│   ├── ADC
│   │   ├── Inc             // ADC Driver headers
│   │   └── Src             // ADC Driver sources
│   │
│   ├── TIM
│   │   ├── Inc             // TIM Driver headers
│   │   └── Src             // TIM Driver sources
│   │
│   ├── USART
│   │   ├── Inc             // USART Driver headers
│   │   └── Src             // USART Driver sources
│   │
│   ├── SPI
│   │   ├── Inc             // SPI Driver headers
│   │   └── Src             // SPI Driver sources
│   │
│   └── I2C
│       ├── Inc             // I2C Driver headers
│       └── Src             // I2C Driver sources
│
├── Applications             // Example applications and hardware tests
│
├── Startup                  // Startup files (startup_stm32f407vgtx.s)
│
└── Docs                     // Documentation, diagrams, and design notes
```

---

## 5. Driver Design Approach

The framework applies two distinct C++ design patterns, chosen deliberately based on the hardware topology of each peripheral.

### Static Classes

For drivers representing unique, single-instance hardware components, **static classes** are used. This gives direct access to functionality without instantiating an object, reflecting the peripheral's global, singleton nature on the microcontroller.

**Example: RCC Driver**

```cpp
RCC::enableGPIOClock(RCC_GPIO::PORTD);
RCC::enableTIMClock(RCC_TIM::TIM2);
```

### Object-Oriented Classes

For drivers controlling peripherals with multiple hardware instances, **object-oriented classes** are used instead. Each object corresponds to one specific hardware instance (e.g. `GPIOA`, `USART1`), allowing every instance to be configured and operated independently of the others.

**Example: GPIO Driver**

```cpp
GPIO gpioPortA(GPIO_Port::PORTA);
gpioPortA.pinMode(GPIO_Pin::PIN_0, GPIO_Mode::OUTPUT);
gpioPortA.digitalWrite(GPIO_Pin::PIN_0, GPIO_State::HIGH);
```

**Example: USART Driver**

```cpp
USART usart2(USART_Instance::USART2);
usart2.init(115200);
usart2.transmit("Hello, STM32!\r\n");
```

---

## License

Released under the [MIT License](LICENSE).

## Author

**Jolapuram Jayavardhan**
Embedded Software Engineer
GitHub: [@JJAYAVARDAN](https://github.com/JJAYAVARDAN)