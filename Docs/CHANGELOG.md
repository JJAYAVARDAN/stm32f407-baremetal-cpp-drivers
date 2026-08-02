# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to Semantic Versioning.

---

## [Unreleased]

### Added
- Umbrella header `STM32F407_Drivers.h` for convenient single-include access to all drivers.
- Comprehensive documentation suite including API Reference, Driver Guide, Architecture Guide, Changelog, and Roadmap.

---

## [1.0.0] - 2024-07-30

### Added
- **Initial Framework Release**
- **Core Drivers:**
  - `RCC`: Clock management, including system clock, prescalers, and peripheral clocks.
  - `NVIC`: Interrupt controller management.
  - `SysTick`: System timer for time base and delays.
  - `SCB`: System Control Block driver (skeleton).
- **Peripheral Drivers:**
  - `GPIO`: Pin configuration and I/O control.
  - `EXTI`: External interrupt configuration.
  - `USART`: Asynchronous serial communication.
  - `SPI`: Serial Peripheral Interface master mode.
  - `I2C`: Inter-Integrated Circuit master mode.
  - `ADC`: Analog-to-Digital Converter for single-channel conversions.
  - `TIM`: General-purpose and advanced timers, including basic and PWM modes.
  - `DMA`: Direct Memory Access for memory-to-peripheral transfers.
- **Example Applications:** Test applications for all implemented drivers.