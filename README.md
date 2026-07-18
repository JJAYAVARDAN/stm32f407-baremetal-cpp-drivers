# STM32F407 Bare-Metal C++ Drivers

> Production-quality bare-metal C++ peripheral driver library for the STM32F407VG Discovery board using direct register programming. Built from scratch without STM32 HAL or LL libraries.

---

# Project Overview

This repository contains a reusable, object-oriented C++ driver library for the STM32F407VG Discovery board.

The objective of this project is to understand the STM32F4 architecture at the register level by implementing every peripheral driver from scratch using the STM32 Reference Manual.

## Features

- Register-Level Programming
- Bare-Metal Development
- Object-Oriented C++ Drivers
- No HAL
- No LL
- Reusable Driver Architecture
- Detailed Documentation
- Practical Example Applications
- PlantUML Architecture Diagrams

---

# Hardware

- STM32F407VG Discovery
- ARM Cortex-M4
- ST-Link V2

---

# Toolchain

- STM32CubeIDE
- GCC ARM Embedded
- OpenOCD
- Git
- GitHub

---

# Driver Status

| Peripheral | Status |
|------------|--------|
| GPIO | 🚧 In Progress |
| RCC | ⏳ Planned |
| EXTI | ⏳ Planned |
| SysTick | ⏳ Planned |
| USART | ⏳ Planned |
| SPI | ⏳ Planned |
| I2C | ⏳ Planned |
| CAN | ⏳ Planned |
| TIM | ⏳ Planned |
| DMA | ⏳ Planned |
| FLASH | ⏳ Planned |
| WWDG | ⏳ Planned |
| IWDG | ⏳ Planned |

---

# Repository Structure

```
stm32f407-baremetal-cpp-drivers
│
├── Drivers
│   ├── Inc
│   └── Src
│
├── Applications
│
├── Docs
│   ├── GPIO.md
│   ├── RCC.md
│   ├── USART.md
│   ├── SPI.md
│   ├── I2C.md
│   ├── CAN.md
│   └── PUML
│
├── Images
│
├── Startup
│
├── Linker
│
├── README.md
│
└── LICENSE
```

---

# Documentation

Each peripheral contains

- Theory
- Register Description
- Initialization Flow
- Driver API
- Example
- PlantUML Design

---

# PlantUML Diagrams

The `Docs/PUML` directory contains architecture and design diagrams.

Current diagrams:

- GPIO Driver Class Diagram
- GPIO Initialization Flow
- RCC Clock Tree
- EXTI Interrupt Flow
- USART Driver Flow
- SPI Communication
- I2C State Machine
- CAN Message Flow
- Driver Layer Architecture
- Complete Project Architecture

---

# Development Roadmap

## Phase 1

- [ ] GPIO
- [ ] RCC
- [ ] SysTick

## Phase 2

- [ ] EXTI
- [ ] USART
- [ ] SPI

## Phase 3

- [ ] I2C
- [ ] CAN
- [ ] TIM

## Phase 4

- [ ] DMA
- [ ] FLASH
- [ ] Watchdog

---

# Design Goals

- Clean C++ API
- Register-Level Programming
- Reusable Drivers
- Production-Style Code
- Extensive Documentation
- Interview Ready

---

# Example

```cpp
GPIO led(GPIOD,12);

led.setMode(GPIO_Mode::OUTPUT);

while(true)
{
    led.toggle();
}
```

---

# Future Work

- UART Bootloader Integration
- RTOS Compatibility
- Unit Testing
- Continuous Integration
- Additional STM32F4 Support

---

# License

MIT License

---

# Author

Jolapuram Jayavardhan

Embedded Software Engineer

GitHub:
https://github.com/JJAYAVARDAN
