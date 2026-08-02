# STM32F407VG Bare-Metal C++ Driver Framework

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
![MCU](https://img.shields.io/badge/MCU-STM32F407VG-blue)
![Core](https://img.shields.io/badge/Core-ARM%20Cortex--M4-blue)
![Language](https://img.shields.io/badge/language-C%2B%2B17-00599C)
![Style](https://img.shields.io/badge/style-Bare--Metal%20%7C%20No%20HAL-critical)

> A modern C++ bare-metal driver framework for the STM32F407VG microcontroller. The project is built around direct register access, object-oriented driver classes, and documentation derived from the STM32F407 Reference Manual (RM0090), without using STM32 HAL or LL libraries.

---

## Table of Contents

- [Project Description](#project-description)
- [Features](#features)
- [Supported MCU](#supported-mcu)
- [Reference Manual](#reference-manual)
- [Implemented Drivers](#implemented-drivers)
- [Driver Architecture](#driver-architecture)
- [Folder Structure](#folder-structure)
- [Example Applications](#example-applications)
- [Documentation](#documentation)
- [How to Use](#how-to-use)
- [Documentation](#documentation)
- [Driver Documentation](#driver-documentation)
- [Coding Style](#coding-style)
- [Project Roadmap](#project-roadmap)
- [Build Environment](#build-environment)
- [References](#references)
- [License](#license)
- [Author](#author)
- [Version History](#version-history)

---

## Project Description

This repository contains a bare-metal C++ framework for the STM32F407VG. Its purpose is to provide a compact and reusable set of peripheral drivers that interact directly with the MCU’s memory-mapped registers.

Every driver in this project is implemented from the STM32F407 Reference Manual (RM0090) and is written without STM32Cube HAL or LL dependencies. The code is organized around register-level access, C++ classes, and strongly typed enums to make the APIs explicit and easier to use in embedded applications.

---

## Features

The current workspace includes the following implemented capabilities:

- Bare-metal driver development for STM32F407VG
- Register-level programming without HAL or LL
- Modern C++ object-oriented driver classes
- Strongly typed enumerations for peripheral configuration
- Modular driver architecture split across core and peripheral layers
- Documentation-rich source files with RM0090 references and driver comments
- Example applications for GPIO, EXTI, NVIC, USART, SPI, I2C, ADC, TIM, and DMA use cases

---

## Supported MCU

- STM32F407VG

---

## Reference Manual

- STM32F407 Reference Manual (RM0090)

---

## Implemented Drivers

The following drivers are present in the current implementation:

| Driver | Status | Description |
| :--- | :--- | :--- |
| RCC | Implemented | Clock control, PLL configuration, prescalers, peripheral clock enable/disable, and clock-frequency helpers |
| GPIO | Implemented | Port and pin configuration, input/output control, alternate function selection, and basic state operations |
| EXTI | Implemented | External interrupt line mapping, trigger configuration, interrupt/event control, and pending flag handling |
| NVIC | Implemented | Interrupt enable/disable, pending/clear pending, active-state checks, and priority configuration |
| SysTick | Implemented | Tick generation, delay helpers, and software tick counting |
| USART | Implemented | USART initialization, enable/disable control, baud-rate setup, and data transmit/receive helpers |
| SPI | Implemented | SPI initialization, mode selection, data transfer, and status checks |
| I2C | Implemented | I2C master configuration, start/stop generation, addressing, and byte transfers |
| ADC | Implemented | ADC initialization, channel selection, sample-time setup, and conversion control |
| TIM | Implemented | General-purpose timer configuration, counter control, prescaler/ARR setup, PWM-related APIs, and capture/compare support |
| DMA | Implemented | DMA controller/stream selection, channel setup, direction/priority/mode configuration, transfer control, and interrupt/flag handling |

---

## Driver Architecture

The driver layer follows a consistent register-oriented structure across peripherals:

1. Base address definition for the peripheral
2. Register definition struct matching the hardware register map
3. Register access macros or typed register pointers
4. Enumerations for peripheral instances, modes, and configuration values
5. A driver class exposing constructor and functional APIs
6. Clock-related APIs where applicable
7. Initialization and configuration APIs
8. Control and status APIs
9. Interrupt-related APIs where supported

This pattern is used in the current RCC, GPIO, EXTI, NVIC, SysTick, USART, SPI, I2C, ADC, TIM, and DMA drivers.

---

## Folder Structure

The workspace is organized as follows:

```text
Drivers/
├── Applications/
│   └── Led_blink/
├── Core/
│   ├── Inc/
│   └── Src/
├── Docs/
│   └── puml/
├── Drivers/
│   ├── ADC/
│   ├── DMA/
│   ├── EXTI/
│   ├── GPIO/
│   ├── I2C/
│   ├── SPI/
│   ├── TIM/
│   └── USART/
├── Middleware/
├── Src/
├── Startup/
├── README.md
├── STM32F407VGTX_FLASH.ld
└── STM32F407VGTX_RAM.ld
```

---

## How to Use

To simplify development, the framework provides a single umbrella header file that includes all implemented drivers. This is the recommended approach for new applications.

### Recommended Method (Umbrella Header)

Simply include `STM32F407_Drivers.h` to get access to all drivers in the framework.

```cpp
#include "STM32F407_Drivers.h"

int main()
{
    // Your application code here...
    // All driver classes like RCC, GPIO, USART, etc., are available.
}
```

### Legacy Method (Individual Headers)

While not recommended for new projects, it is still possible to include each driver header individually. This was the standard method before the umbrella header was introduced.

```cpp
#include "RCC_Driver.h"
#include "GPIO_Driver.h"
#include "USART_Driver.h"
// ... and so on for every required driver.
```

---

## Documentation

This framework is documented to provide a clear understanding of its architecture, usage, and API.

| Document | Description |
| :--- | :--- |
| [**API Reference**](./Docs/API_REFERENCE.md) | A comprehensive reference for all public APIs, classes, enums, and configuration structures for every implemented driver. |
| [**Driver Guide**](./Docs/DRIVER_GUIDE.md) | A step-by-step user guide explaining the project philosophy, how to get started, and practical examples for each peripheral. |
| [**Architecture Guide**](./Docs/ARCHITECTURE.md) | An in-depth explanation of the framework's software architecture, design principles, and dependency rules. |
| [**Changelog**](./Docs/CHANGELOG.md) | A log of all notable changes, additions, and fixes for each version release. |
| [**Roadmap**](./Docs/ROADMAP.md) | An overview of the project's development roadmap, including completed features and future plans. |

---

## Example Applications

The following example applications are currently present in the workspace:

- Applications/Led_blink/ADC_Test_App.cpp
- Applications/Led_blink/Button_EXTI.cpp
- Applications/Led_blink/Button_Nvic.cpp
- Applications/Led_blink/I2C_Test_app.cpp
- Applications/Led_blink/SPI_loopBack.cpp
- Applications/Led_blink/TIM_TestApp.cpp
- Applications/Led_blink/Usart_loopback.cpp
- Applications/Led_blink/main.cpp
- Applications/Led_blink/main2.cpp
- Applications/Led_blink/main_systick.cpp

These examples cover LED control, EXTI-driven interrupts, NVIC configuration, USART loopback, SPI loopback, I2C scanning, ADC acquisition, and timer-based experimentation.

---

## Documentation

The repository includes structured driver documentation alongside the implementation:

- Register documentation and comments in header files
- RM0090 references in driver headers and source files
- API documentation through function-level comments
- State machine descriptions in the driver source files
- Diagram-based documentation under the Docs folder

The documentation style is especially detailed in the more recently added drivers, including DMA.

---

## Driver Documentation

RCC provides reset and clock-control functionality for the STM32F407VG, including system-clock configuration, prescaler setup, and peripheral clock enable/disable routines.

GPIO implements port and pin configuration, digital input/output behavior, alternate-function mapping, and basic pin state manipulation for the STM32F407’s general-purpose I/O ports.

EXTI provides mapping of GPIO inputs to external interrupt lines, trigger edge selection, and interrupt/event control APIs.

NVIC exposes the Cortex-M4 interrupt controller interface for enabling and disabling IRQs, setting priorities, and checking pending or active states.

SysTick provides a simple time base for delays and software tick counting based on the Cortex-M4 SysTick peripheral.

USART implements UART/UART-like serial communication configuration and data transfer helpers for the STM32F407’s USART instances.

SPI provides master-mode SPI setup, clocking, data transfer, and status functions for the STM32F407 SPI peripherals.

I2C provides basic master-mode I2C control for initialization, addressing, start/stop generation, and byte transfer operations.

ADC offers ADC initialization, channel selection, sampling time configuration, and conversion control for the STM32F407 ADC peripherals.

TIM implements timer configuration and control for the STM32F407 general-purpose timers, including prescaler, ARR, counter, PWM, compare, and capture support.

DMA provides stream-based memory-to-peripheral and peripheral-to-memory transfer configuration for the DMA controllers, including stream/channel selection, transfer direction, interrupt control, and status/flag management.

---

## Coding Style

The implementation follows a consistent embedded C++ style:

- Bare-metal programming with direct register access
- Modern C++ class-based driver design
- No HAL dependency
- No LL dependency
- Register-level programming throughout the stack
- Consistent driver APIs with explicit names for initialization, configuration, control, and status operations

---

## Project Roadmap

### Completed

- RCC
- GPIO
- EXTI
- NVIC
- SysTick
- USART
- SPI
- I2C
- ADC
- TIM
- DMA

### In Progress

- Ongoing documentation and RM0090-alignment refinement for the implemented driver set

### Future Work

- Additional peripheral drivers beyond the currently implemented set
- Further validation and refinement of driver behavior in real hardware test applications

---

## Build Environment

- Compiler: ARM Embedded GCC / GNU Arm toolchain
- IDE: STM32CubeIDE / Eclipse-based project environment
- Language: C++
- MCU: STM32F407VG
- Architecture: ARM Cortex-M4

---

## References

- STM32F407 Reference Manual (RM0090)
- ARM Cortex-M4 Generic User Guide
- STM32F407 Datasheet

---

## License

This project is released under the [MIT License](LICENSE).

---

## Author

Jolapuram Jayavardhan ([@JJAYAVARDAN](https://github.com/JJAYAVARDAN))

---

## Version History

No formal release tags are present in the current workspace snapshot. The repository content reflects the ongoing driver-development state described above.
