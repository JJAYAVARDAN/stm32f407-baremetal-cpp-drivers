# STM32F407 Bare-Metal C++ Framework

> A modern, register-level embedded framework for the STM32F407VG Discovery Board, developed entirely from scratch using Embedded C++ without STM32 HAL or LL libraries.

![License](https://img.shields.io/badge/License-MIT-blue.svg)
![Language](https://img.shields.io/badge/Language-C++14-orange.svg)
![Platform](https://img.shields.io/badge/Platform-STM32F407-success.svg)
![Architecture](https://img.shields.io/badge/Architecture-ARM_Cortex--M4-green.svg)

---

# Project Overview

The objective of this project is to understand ARM Cortex-M4 microcontrollers at the register level by developing a complete reusable embedded software framework from scratch.

Unlike STM32Cube HAL or LL libraries, every peripheral driver in this repository directly accesses hardware registers using CMSIS definitions, providing complete control over the STM32F407 peripherals.

This repository is intended for:

- Embedded Engineers
- Firmware Developers
- Electronics Students
- ARM Cortex-M Learners
- Bare-Metal Programming Enthusiasts

---

# Features

- Register-Level Peripheral Programming
- Modern Embedded C++ Design
- Object-Oriented Driver Framework
- Modular Architecture
- No HAL
- No LL Drivers
- No Auto-generated Peripheral Code
- Layered Driver Design
- Example Applications
- PlantUML Documentation
- Professional Git Workflow
- Semantic Versioning

---

# Current Driver Support

| Peripheral | Status |
|------------|--------|
| RCC | ✅ Complete |
| GPIO | ✅ Complete |
| EXTI | ✅ Complete |
| NVIC | ✅ Complete |
| SysTick | ✅ Complete |
| USART | ✅ Complete |
| SPI | ✅ Complete |
| I2C | ✅ Complete |
| ADC | ✅ Complete |

---

# Planned Driver Support

- TIM
- DMA
- CAN
- RTC
- DAC
- FLASH
- CRC
- RNG
- IWDG
- WWDG
- Power Controller

---

# Future Driver Enhancements

After completing all peripheral drivers, every driver will be upgraded with:

- Interrupt Support
- DMA Support
- Callback Registration
- Non-Blocking APIs
- Timeout APIs
- Error Handling
- Driver State Machine
- Common Callback Framework

---

# Middleware

The framework will also include reusable middleware components.

- UART Bootloader
- UART Command Line Interface
- Ring Buffer Library
- Queue Library
- Event Manager
- Software Timer Library
- Cooperative Scheduler

---

# JayRTOS

A complete custom RTOS will be developed as part of this project.

Features include:

- Task Scheduler
- Context Switching
- PendSV
- SysTick
- Task Management
- Queues
- Semaphores
- Mutexes
- Event Groups
- Software Timers
- Memory Management

---

# Example Applications

The repository contains standalone applications demonstrating each peripheral.

Examples include:

- GPIO LED Blink
- GPIO Interrupt
- USART Echo
- SPI Loopback
- I2C EEPROM
- ADC Sensor Read
- PWM Generation
- Servo Motor Control
- CAN Communication
- RTC Clock
- DAC Wave Generation

---

# Real-World Projects

After the framework is completed, complete embedded products will be built using the custom drivers.

Projects include:

- Digital Multimeter
- Weather Station
- Data Logger
- Automotive CAN Dashboard
- Motor Controller
- Function Generator
- Digital Storage Oscilloscope
- Logic Analyzer
- Home Automation Controller
- Smart Sensor Hub

All projects will use:

- Custom Drivers
- Middleware
- JayRTOS
- Interrupts
- DMA

---

# Repository Structure

```
STM32F407_BAREMETAL_CPP_FRAMEWORK
│
├── Applications/
│   ├── GPIO
│   ├── USART
│   ├── SPI
│   ├── I2C
│   ├── ADC
│   ├── TIM
│   ├── CAN
│   └── RTOS
│
├── Drivers/
│   ├── Inc/
│   └── Src/
│
├── Startup/
│
├── CMSIS/
│
├── Docs/
│   ├── Architecture
│   ├── UML
│   ├── Sequence
│   ├── Flowcharts
│   └── PlantUML
│
├── Bootloader/
│
├── Middleware/
│
├── JayRTOS/
│
└── README.md
```

---

# Software Architecture

```
+------------------------------------------------------+
|                Embedded Applications                 |
+------------------------------------------------------+
|                     Middleware                       |
|  CLI | Ring Buffer | Queue | Event | Timers         |
+------------------------------------------------------+
|                      JayRTOS                         |
| Scheduler | Queue | Mutex | Semaphore | Memory      |
+------------------------------------------------------+
|                  Peripheral Drivers                  |
| RCC | GPIO | USART | SPI | I2C | ADC | TIM | CAN    |
| RTC | DAC | DMA | FLASH | CRC | RNG | PWR | WDG     |
+------------------------------------------------------+
|                 CMSIS Register Layer                 |
+------------------------------------------------------+
|             STM32F407VG Cortex-M4 MCU                |
+------------------------------------------------------+
```

---

# Driver Architecture

```
Application
      │
      ▼
Driver API
      │
      ▼
Peripheral Driver
      │
      ▼
Register Access Layer
      │
      ▼
STM32 Hardware
```

---

# Design Goals

- Learn ARM Cortex-M4 Architecture
- Understand Peripheral Register Programming
- Build Reusable Driver Library
- Learn Embedded C++
- Learn Interrupt Programming
- Learn DMA Programming
- Build a Bootloader
- Develop a Custom RTOS
- Build Production-Quality Embedded Applications

---

# Development Environment

IDE

- STM32CubeIDE
- Visual Studio Code

Compiler

- GNU ARM Embedded Toolchain

Debugger

- OpenOCD
- GDB

Version Control

- Git
- GitHub

Documentation

- PlantUML
- Markdown
- Doxygen

---

# Hardware

Target Board

STM32F407VG Discovery

MCU

STM32F407VG

Architecture

ARM Cortex-M4

Clock

168 MHz

---

# Coding Guidelines

- Embedded C++14
- Register-Level Programming
- Object-Oriented Design
- Modular Driver Development
- Consistent Naming Convention
- Header/Source Separation
- Reusable APIs
- Well Documented Code

---

# Documentation

The repository contains:

- Driver Documentation
- Register Maps
- Architecture Diagrams
- Flowcharts
- UML Class Diagrams
- Sequence Diagrams
- API Documentation
- Example Applications
- Release Notes

---

# Version Roadmap

## Version 1.x

Peripheral Driver Development

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
- CAN
- RTC
- DAC
- FLASH
- CRC
- RNG
- Watchdog
- Power Control

---

## Version 2.x

Driver Enhancements

- Interrupt Support
- DMA Support
- Callback Framework
- Driver State Machine
- Timeout APIs
- Non-Blocking APIs
- Error Handling

---

## Version 3.x

Middleware

- UART Bootloader
- CLI
- Queue Library
- Ring Buffer
- Software Timers
- Event Manager
- Cooperative Scheduler

---

## Version 4.x

JayRTOS

- Scheduler
- Context Switching
- Queues
- Mutexes
- Semaphores
- Event Groups
- Memory Manager

---

## Version 5.x

Embedded Projects

- Digital Multimeter
- Weather Station
- Data Logger
- CAN Dashboard
- Function Generator
- DSO
- Logic Analyzer
- Home Automation
- Smart Sensor Hub

---

## Version 6.x

Professional Framework

- Complete Documentation
- Doxygen
- UML
- Architecture Diagrams
- CI/CD
- Automated Testing
- Static Analysis
- Portfolio-Ready Embedded Framework

---

# Learning Outcomes

By completing this repository, the following concepts will be covered:

- ARM Cortex-M4 Architecture
- Embedded C++
- Register-Level Programming
- Driver Development
- Interrupt Programming
- DMA Programming
- Bootloader Development
- RTOS Development
- Middleware Development
- Embedded System Design
- Professional Software Architecture
- Embedded Debugging
- Production-Quality Firmware Development

---

# License

This project is licensed under the MIT License.

---

# Author

**Jolapuram Jayavardhan**

Embedded Software Engineer

Bluetooth Developer | Harman International

GitHub: https://github.com/JJAYAVARDAN
