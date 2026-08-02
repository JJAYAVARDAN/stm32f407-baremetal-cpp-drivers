# Project Roadmap

This document outlines the development roadmap for the STM32F407VG Bare-Metal C++ Driver Framework.

---

## Completed (v1.0.0)

The following drivers and features are considered stable and are included in the current release.

#### Core Drivers
- ✅ **RCC** (Reset and Clock Control)
- ✅ **NVIC** (Nested Vectored Interrupt Controller)
- ✅ **SysTick** (System Timer)

#### Peripheral Drivers
- ✅ **GPIO** (General-Purpose I/O)
- ✅ **EXTI** (External Interrupts)
- ✅ **USART** (Universal Synchronous/Asynchronous Receiver/Transmitter)
- ✅ **SPI** (Serial Peripheral Interface)
- ✅ **I2C** (Inter-Integrated Circuit)
- ✅ **ADC** (Analog-to-Digital Converter)
- ✅ **TIM** (General-Purpose & Advanced Timers)
- ✅ **DMA** (Direct Memory Access)

#### Framework
- ✅ Object-oriented C++ class structure.
- ✅ Direct register access model.
- ✅ Umbrella header for easy inclusion.
- ✅ Comprehensive documentation suite.

---

## In Progress

- **Documentation Refinement:** Ongoing effort to align all driver documentation with the latest RM0090 manual and improve examples.
- **Driver Validation:** Further hardware testing and validation of all implemented drivers.

---

## Planned

- **DAC Driver:** Digital-to-Analog Converter.
- **RTC Driver:** Real-Time Clock.
- **WWDG/IWDG Drivers:** Window and Independent Watchdog timers.

---

## Future

The following drivers are being considered for future releases, depending on project needs and community interest.

- CAN (Controller Area Network)
- USB OTG (On-The-Go)
- SDIO (Secure Digital Input/Output)
- Ethernet MAC
- CRC (Cyclic Redundancy Check)
- HASH (Hash processor)
- RNG (Random Number Generator)
- FSMC (Flexible Static Memory Controller)
- DCMI (Digital Camera Interface)
- Power Controller (PWR)
- Flash Interface