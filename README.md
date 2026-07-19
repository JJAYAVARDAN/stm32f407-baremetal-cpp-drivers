# STM32F407 Bare-Metal C++ Drivers

> Production-quality bare-metal C++ peripheral driver library for the STM32F407VG Discovery board using direct register programming. Built from scratch without STM32 HAL or LL libraries.

**Current version: v0.6.0**

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
| GPIO | ✅ Implemented |
| RCC | ✅ Implemented |
| EXTI | ✅ Implemented |
| NVIC | ✅ Implemented |
| SysTick | ✅ Implemented |
| USART | ✅ Implemented |
| SPI | ✅ Implemented |
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
│   ├── Drivers
│   │   ├── Inc     → GPIO, RCC, EXTI, NVIC, SysTick, USART headers
│   │   └── Src     → GPIO, RCC, EXTI, NVIC, SysTick, USART sources
│   ├── Applications → example apps (LED blink, button+EXTI, USART loopback, etc.)
│   ├── Src         → main.cpp (placeholder entry point)
│   ├── Startup     → startup_stm32f407vgtx.s (vector table)
│   ├── STM32F407VGTX_FLASH.ld
│   └── STM32F407VGTX_RAM.ld
│
├── Docs
│   └── puml        → GPIO class diagram, project architecture diagram
│
├── README.md
│
└── LICENSE
```

> **Note:** `Drivers/Applications/` contains several standalone example files that each define `main()` (`main.cpp`, `main2.cpp`, `main_systick.cpp`, `Button_EXTI.cpp`, `Button_Nvic.cpp`, `Usart_loopback.cpp`). These are meant to be swapped in one at a time to demonstrate a specific driver/feature. Only the active example should be included in the build — the rest are intentionally excluded from build (CubeIDE: right-click → Resource Configurations → Exclude from Build) to avoid `multiple definition of main`.

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

The `Docs/puml` directory contains architecture and design diagrams.

Current diagrams:

- GPIO Driver Class Diagram
- Complete Project Architecture

Planned:

- GPIO Initialization Flow
- RCC Clock Tree
- EXTI Interrupt Flow
- USART Driver Flow
- SPI Communication
- I2C State Machine
- CAN Message Flow
- Driver Layer Architecture

---

# Development Roadmap

## Phase 1

- [x] GPIO
- [x] RCC
- [x] SysTick

## Phase 2

- [x] EXTI
- [x] NVIC
- [x] USART
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
GPIO led(GPIO_Port::PORTD, 12);

led.setMode(GPIO_Mode::OUTPUT);

while(true)
{
    led.toggle();
}
```

---

# Known Issues (v0.6.0)

- ~~`USART_Driver.h` had a misplaced `#endif` that left most of the header outside the include guard.~~ **Fixed.**
- ~~GPIO constructor calls in example apps passed the `GPIOx` register-pointer macro instead of `GPIO_Port::PORTx`.~~ **Fixed.**
- `EXTI::setTrigger()` is declared in the header but not implemented; use `configureTrigger()` instead.
- `SysTick::delayMs()` assumes a 1 kHz tick rate; timing will be wrong if `init()` is called with a different `tickFreq`.
- `SysTick::delayUs()` is a stub (TODO).
- `USART::transmit()` intentionally skips the TC (transmission-complete) wait — fine for back-to-back polling, not yet documented inline.

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
