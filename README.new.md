# STM32F407VG C++ Driver Framework

## 1. Project Overview

This project provides a bare-metal C++ driver framework for the STM32F407VG microcontroller. The drivers are written in C++ with a class-based approach for each peripheral, promoting code reusability and a clean, object-oriented interface.

The framework is designed for direct register-level programming, offering fine-grained control over the hardware without the use of vendor-provided Hardware Abstraction Layers (HAL) or Low-Layer (LL) libraries. This approach is ideal for developers who want to gain a deep understanding of the STM32 architecture and write highly optimized, efficient firmware.

## 2. Supported Hardware

-   **Microcontroller:** STM32F407VG
-   **Core:** ARM Cortex-M4
-   **Reference Manual:** RM0090
-   **Development Board:** This framework is generic and can be adapted to any custom board using the STM32F407VG MCU. Examples are often based on a development board with accessible pins for peripherals.

## 3. Software Architecture

The software is structured into three main layers:

-   **Core Layer:** Provides drivers for essential microcontroller components like the Reset and Clock Control (RCC), Nested Vectored Interrupt Controller (NVIC), and System Tick (SysTick). These are fundamental for any application.
-   **Driver Layer:** Contains peripheral drivers such as GPIO, USART, SPI, I2C, ADC, and TIM. Each peripheral is encapsulated in a C++ class, exposing a high-level API for configuration and operation.
-   **Application Layer:** This is where the user's application code resides. It utilizes the driver and core layer APIs to implement the desired functionality.

### Architecture Diagram

```
+---------------------+
|   Application Layer |
+---------------------+
|    Driver Layer     |
| (GPIO, USART, etc.) |
+---------------------+
|      Core Layer     |
|  (RCC, NVIC, etc.)  |
+---------------------+
|       Hardware      |
+---------------------+
```

## 4. Driver Status Table

| Peripheral | Status      | Description                                      |
| :--------- | :---------- | :----------------------------------------------- |
| RCC        | Implemented | Reset and Clock Control.                         |
| GPIO       | Implemented | General Purpose Input/Output.                    |
| EXTI       | Implemented | External Interrupts.                             |
| NVIC       | Implemented | Nested Vectored Interrupt Controller.            |
| SysTick    | Implemented | System Tick Timer.                               |
| USART      | Implemented | Universal Synchronous/Asynchronous Receiver/Transmitter. |
| SPI        | Implemented | Serial Peripheral Interface.                     |
| I2C        | Implemented | Inter-Integrated Circuit.                        |
| ADC        | Implemented | Analog-to-Digital Converter.                     |
| TIM        | Implemented | General Purpose Timers.                          |

## 5. API Documentation

### RCC Driver

**Header:** `Drivers/Core/Inc/RCC_Driver.h`

**Class:** `RCC`

The `RCC` class provides static methods to control the clock sources for various peripherals.

**Enumerations:**

-   `RCC_GPIO`: Selects the GPIO port (PORTA, PORTB, etc.).
-   `RCC_USART`: Selects the USART peripheral (USART1, USART2, etc.).
-   `RCC_SPI`: Selects the SPI peripheral (SPI1, SPI2, etc.).
-   `RCC_I2C`: Selects the I2C peripheral (I2C1, I2C2, etc.).
-   `RCC_ADC`: Selects the ADC peripheral (ADC1, ADC2, etc.).
-   `RCC_TIM`: Selects the Timer peripheral (TIM2, TIM3, etc.).

**APIs:**

-   `enableGPIOClock(RCC_GPIO port)`
    -   **Purpose:** Enables the clock for the specified GPIO port.
    -   **Parameters:** `port` - The GPIO port to enable.
    -   **Return:** None.

-   `disableGPIOClock(RCC_GPIO port)`
    -   **Purpose:** Disables the clock for the specified GPIO port.
    -   **Parameters:** `port` - The GPIO port to disable.
    -   **Return:** None.

-   `resetGPIO(RCC_GPIO port)`
    -   **Purpose:** Resets the specified GPIO port.
    -   **Parameters:** `port` - The GPIO port to reset.
    -   **Return:** None.

-   `enableUSARTClock(RCC_USART usart)`
    -   **Purpose:** Enables the clock for the specified USART peripheral.
    -   **Parameters:** `usart` - The USART peripheral to enable.
    -   **Return:** None.

-   `disableUSARTClock(RCC_USART usart)`
    -   **Purpose:** Disables the clock for the specified USART peripheral.
    -   **Parameters:** `usart` - The USART peripheral to disable.
    -   **Return:** None.

-   `enableSPIClock(RCC_SPI spi)`
    -   **Purpose:** Enables the clock for the specified SPI peripheral.
    -   **Parameters:** `spi` - The SPI peripheral to enable.
    -   **Return:** None.

-   `disableSPIClock(RCC_SPI spi)`
    -   **Purpose:** Disables the clock for the specified SPI peripheral.
    -   **Parameters:** `spi` - The SPI peripheral to disable.
    -   **Return:** None.

-   `enableI2CClock(RCC_I2C i2c)`
    -   **Purpose:** Enables the clock for the specified I2C peripheral.
    -   **Parameters:** `i2c` - The I2C peripheral to enable.
    -   **Return:** None.

-   `disableI2CClock(RCC_I2C i2c)`
    -   **Purpose:** Disables the clock for the specified I2C peripheral.
    -   **Parameters:** `i2c` - The I2C peripheral to disable.
    -   **Return:** None.

-   `enableADCClock(RCC_ADC adc)`
    -   **Purpose:** Enables the clock for the specified ADC peripheral.
    -   **Parameters:** `adc` - The ADC peripheral to enable.
    -   **Return:** None.

-   `disableADCClock(RCC_ADC adc)`
    -   **Purpose:** Disables the clock for the specified ADC peripheral.
    -   **Parameters:** `adc` - The ADC peripheral to disable.
    -   **Return:** None.

-   `enableTIMClock(RCC_TIM tim)`
    -   **Purpose:** Enables the clock for the specified Timer peripheral.
    -   **Parameters:** `tim` - The Timer peripheral to enable.
    -   **Return:** None.

-   `disableTIMClock(RCC_TIM tim)`
    -   **Purpose:** Disables the clock for the specified Timer peripheral.
    -   **Parameters:** `tim` - The Timer peripheral to disable.
    -   **Return:** None.

-   `resetTIMClock(RCC_TIM tim)`
    -   **Purpose:** Resets the specified Timer peripheral.
    -   **Parameters:** `tim` - The Timer peripheral to reset.
    -   **Return:** None.

-   `isTIMClockEnabled(RCC_TIM tim)`
    -   **Purpose:** Checks if the clock for the specified Timer peripheral is enabled.
    -   **Parameters:** `tim` - The Timer peripheral to check.
    -   **Return:** `true` if the clock is enabled, `false` otherwise.

### NVIC Driver

**Header:** `Drivers/Core/Inc/NVIC_Driver.h`

**Class:** `NVIC`

The `NVIC` class provides static methods to configure the Nested Vectored Interrupt Controller.

**APIs:**

-   `enableIRQ(uint8_t irq)`
    -   **Purpose:** Enables the specified interrupt request (IRQ).
    -   **Parameters:** `irq` - The IRQ number to enable.
    -   **Return:** None.

-   `disableIRQ(uint8_t irq)`
    -   **Purpose:** Disables the specified IRQ.
    -   **Parameters:** `irq` - The IRQ number to disable.
    -   **Return:** None.

-   `setPendingIRQ(uint8_t irq)`
    -   **Purpose:** Sets the pending status of the specified IRQ.
    -   **Parameters:** `irq` - The IRQ number to set as pending.
    -   **Return:** None.

-   `clearPendingIRQ(uint8_t irq)`
    -   **Purpose:** Clears the pending status of the specified IRQ.
    -   **Parameters:** `irq` - The IRQ number to clear from pending.
    -   **Return:** None.

-   `getActiveIRQ(uint8_t irq)`
    -   **Purpose:** Checks if the specified IRQ is currently active.
    -   **Parameters:** `irq` - The IRQ number to check.
    -   **Return:** `true` if the IRQ is active, `false` otherwise.

-   `setPriority(uint8_t irq, uint8_t priority)`
    -   **Purpose:** Sets the priority of the specified IRQ.
    -   **Parameters:**
        -   `irq`: The IRQ number.
        -   `priority`: The priority level to set.
    -   **Return:** None.

-   `isIRQEnabled(uint8_t irq)`
    -   **Purpose:** Checks if the specified IRQ is enabled.
    -   **Parameters:** `irq` - The IRQ number to check.
    -   **Return:** `true` if the IRQ is enabled, `false` otherwise.

### SysTick Driver

**Header:** `Drivers/Core/Inc/SysTick_Driver.h`

**Class:** `SysTick`

The `SysTick` class provides a simple, interrupt-driven timer for creating delays and managing time-based events.

**APIs:**

-   `init(uint32_t tickFreq)`
    -   **Purpose:** Initializes the SysTick timer with a specified frequency.
    -   **Parameters:** `tickFreq` - The desired tick frequency in Hz.
    -   **Return:** None.

-   `start()`
    -   **Purpose:** Starts the SysTick timer.
    -   **Return:** None.

-   `stop()`
    -   **Purpose:** Stops the SysTick timer.
    -   **Return:** None.

-   `delayMs(uint32_t ms)`
    -   **Purpose:** Creates a blocking delay in milliseconds.
    -   **Parameters:** `ms` - The delay duration in milliseconds.
    -   **Return:** None.

-   `delayUs(uint32_t us)`
    -   **Purpose:** Creates a blocking delay in microseconds.
    -   **Parameters:** `us` - The delay duration in microseconds.
    -   **Return:** None.

-   `getTick()`
    -   **Purpose:** Gets the current tick count since the timer was started.
    -   **Return:** The current tick count.

-   `resetTick()`
    -   **Purpose:** Resets the tick count to zero.
    -   **Return:** None.

-   `incrementTick()`
    -   **Purpose:** Increments the internal tick count. This is typically called from the `SysTick_Handler` interrupt service routine.
    -   **Return:** None.

### GPIO Driver

**Header:** `Drivers/Drivers/GPIO/Inc/Gpio_Driver.h`

**Class:** `GPIO`

The `GPIO` class provides an interface to control the General Purpose Input/Output pins.

**Constructor:**

-   `GPIO(GPIO_Port port, uint8_t pin)`
    -   **Purpose:** Constructs a GPIO object for a specific pin.
    -   **Parameters:**
        -   `port`: The GPIO port (e.g., `GPIO_Port::PORTA`).
        -   `pin`: The pin number (0-15).

**Enumerations:**

-   `GPIO_Port`: Specifies the GPIO port (PORTA, PORTB, etc.).
-   `GPIO_Mode`: Pin mode (INPUT, OUTPUT, ALTERNATE_FUNCTION, ANALOG).
-   `GPIO_OutputType`: Output type (PUSH_PULL, OPEN_DRAIN).
-   `GPIO_Speed`: Pin speed (LOW, MEDIUM, HIGH, VERY_HIGH).
-   `GPIO_Pull`: Pull-up/pull-down configuration (NO_PULL, PULL_UP, PULL_DOWN).
-   `GPIO_State`: Pin state (LOW, HIGH).

**APIs:**

-   `clockEnable()`
    -   **Purpose:** Enables the clock for the GPIO port associated with the object.
    -   **Return:** None.

-   `setMode(GPIO_Mode mode)`
    -   **Purpose:** Sets the mode of the GPIO pin.
    -   **Parameters:** `mode` - The desired pin mode.
    -   **Return:** None.

-   `setOutputType(GPIO_OutputType type)`
    -   **Purpose:** Sets the output type of the GPIO pin.
    -   **Parameters:** `type` - The desired output type.
    -   **Return:** None.

-   `setSpeed(GPIO_Speed speed)`
    -   **Purpose:** Sets the speed of the GPIO pin.
    -   **Parameters:** `speed` - The desired pin speed.
    -   **Return:** None.

-   `setPull(GPIO_Pull pull)`
    -   **Purpose:** Configures the pull-up/pull-down resistor for the GPIO pin.
    -   **Parameters:** `pull` - The desired pull configuration.
    -   **Return:** None.

-   `write(GPIO_State state)`
    -   **Purpose:** Writes a high or low state to the GPIO pin.
    -   **Parameters:** `state` - The state to write (`GPIO_State::HIGH` or `GPIO_State::LOW`).
    -   **Return:** None.

-   `read()`
    -   **Purpose:** Reads the current state of the GPIO pin.
    -   **Return:** The current pin state (`GPIO_State::HIGH` or `GPIO_State::LOW`).

-   `toggle()`
    -   **Purpose:** Toggles the current state of the GPIO pin.
    -   **Return:** None.

-   `setAlternateFunction(uint8_t Af)`
    -   **Purpose:** Configures the alternate function for the GPIO pin.
    -   **Parameters:** `Af` - The alternate function number (0-15).
    -   **Return:** None.

-   `lock()`
    -   **Purpose:** Locks the configuration of the GPIO pin.
    -   **Return:** None.

-   `write_port(uint16_t value)`
    -   **Purpose:** Writes a 16-bit value to the entire GPIO port's Output Data Register (ODR).
    -   **Parameters:** `value` - The 16-bit value to write.
    -   **Return:** None.

-   `readport()`
    -   **Purpose:** Reads the 16-bit value from the entire GPIO port's Input Data Register (IDR).
    -   **Return:** The 16-bit value of the port.

-   `deInit()`
    -   **Purpose:** Resets the GPIO port to its default state.
    -   **Return:** None.

### EXTI Driver

**Header:** `Drivers/Drivers/EXTI/Inc/EXTI_Driver.h`

**Class:** `EXTI`

The `EXTI` class configures external interrupts for GPIO pins.

**Constructor:**

-   `EXTI(RCC_GPIO port, uint8_t pin)`
    -   **Purpose:** Constructs an EXTI object for a specific GPIO pin.
    -   **Parameters:**
        -   `port`: The GPIO port to be used for the external interrupt.
        -   `pin`: The pin number (0-15).

**Enumerations:**

-   `EXTI_Trigger`: Specifies the trigger condition for the interrupt (RISING, FALLING, RISING_FALLING).

**APIs:**

-   `SYSCFGClock()`
    -   **Purpose:** Enables the clock for the SYSCFG peripheral, which is required for EXTI.
    -   **Return:** None.

-   `configureEXTILine()`
    -   **Purpose:** Configures the EXTI line mapping to the GPIO pin.
    -   **Return:** None.

-   `setTrigger(EXTI_Trigger trigger)`
    -   **Purpose:** Sets the trigger type for the EXTI line.
    -   **Parameters:** `trigger` - The desired trigger condition.
    -   **Return:** None.

-   `enableInterrupt()`
    -   **Purpose:** Enables the interrupt for the configured EXTI line.
    -   **Return:** None.

-   `disableInterrupt()`
    -   **Purpose:** Disables the interrupt for the configured EXTI line.
    -   **Return:** None.

-   `enableEvent()`
    -   **Purpose:** Enables the event for the configured EXTI line.
    -   **Return:** None.

-   `disableEvent()`
    -   **Purpose:** Disables the event for the configured EXTI line.
    -   **Return:** None.

-   `softwareInterrupt()`
    -   **Purpose:** Triggers the EXTI interrupt manually through software.
    -   **Return:** None.

-   `isPending()`
    -   **Purpose:** Checks if an interrupt is pending on the EXTI line.
    -   **Return:** `true` if an interrupt is pending, `false` otherwise.

-   `clearPending()`
    -   **Purpose:** Clears the pending interrupt flag for the EXTI line.
    -   **Return:** None.

-   `configureTrigger(EXTI_Trigger trigger)`
    -   **Purpose:** Configures the trigger for the EXTI.
    -   **Parameters:** `trigger` - The desired trigger condition.
    -   **Return:** None.

### USART Driver

**Header:** `Drivers/Drivers/USART/Inc/USART_Driver.h`

**Class:** `USART`

The `USART` class provides an interface for the Universal Synchronous/Asynchronous Receiver/Transmitter peripheral.

**Constructor:**

-   `USART(USART_Instance instance)`
    -   **Purpose:** Constructs a USART object for a specific USART/UART instance.
    -   **Parameters:** `instance` - The USART/UART peripheral to use (e.g., `USART_Instance::USART1`).

**Enumerations:**

-   `USART_Instance`: Specifies the USART/UART peripheral (USART1, USART2, etc.).
-   `USART_WordLength`: Data word length (8 or 9 bits).
-   `USART_StopBits`: Number of stop bits (1, 0.5, 2, 1.5).
-   `USART_Parity`: Parity mode (NONE, EVEN, ODD).
-   `USART_Mode`: Communication mode (RX, TX, or TX_RX).

**APIs:**

-   `enableClock()`
    -   **Purpose:** Enables the clock for the selected USART peripheral.
    -   **Return:** None.

-   `init(uint32_t baud, USART_Mode mode, USART_WordLength wordLength, USART_StopBits stop_bits, USART_Parity parity)`
    -   **Purpose:** Initializes the USART peripheral with the specified parameters.
    -   **Parameters:**
        -   `baud`: The desired baud rate.
        -   `mode`: The communication mode (TX, RX, or both).
        -   `wordLength`: The data word length.
        -   `stop_bits`: The number of stop bits.
        -   `parity`: The parity mode.
    -   **Return:** None.

-   `enable()`
    -   **Purpose:** Enables the USART peripheral.
    -   **Return:** None.

-   `disable()`
    -   **Purpose:** Disables the USART peripheral.
    -   **Return:** None.

-   `transmit(uint8_t data)`
    -   **Purpose:** Transmits a single byte of data.
    -   **Parameters:** `data` - The byte to transmit.
    -   **Return:** None.

-   `receive()`
    -   **Purpose:** Receives a single byte of data.
    -   **Return:** The received byte.

-   `txString(const char *str)`
    -   **Purpose:** Transmits a null-terminated string.
    -   **Parameters:** `str` - A pointer to the string to transmit.
    -   **Return:** None.

### SPI Driver

**Header:** `Drivers/Drivers/SPI/Inc/SPI_Driver.h`

**Class:** `SPI`

The `SPI` class provides an interface for the Serial Peripheral Interface.

**Constructor:**

-   `SPI(SPI_Instance instance)`
    -   **Purpose:** Constructs an SPI object for a specific SPI instance.
    -   **Parameters:** `instance` - The SPI peripheral to use (e.g., `SPI_Instance::SPI1`).

**Enumerations:**

-   `SPI_Instance`: Specifies the SPI peripheral (SPI1, SPI2, etc.).
-   `SPI_Mode`: SPI mode (MASTER, SLAVE).
-   `SPI_ClockPolarity`: Clock polarity (LOW, HIGH).
-   `SPI_ClockPhase`: Clock phase (FIRST_EDGE, SECOND_EDGE).
-   `SPI_DataSize`: Data frame format (8-bit, 16-bit).
-   `SPI_BaudRate`: Baud rate prescaler (DIV2 to DIV256).
-   `SPI_SSM`: Software slave management (ENABLE, DISABLE).

**APIs:**

-   `enableClock()`
    -   **Purpose:** Enables the clock for the selected SPI peripheral.
    -   **Return:** None.

-   `init(SPI_Mode mode, SPI_BaudRate baudRate, SPI_ClockPolarity polarity, SPI_ClockPhase phase, SPI_DataSize dataSize, SPI_SSM ssm)`
    -   **Purpose:** Initializes the SPI peripheral.
    -   **Parameters:**
        -   `mode`: Master or slave mode.
        -   `baudRate`: Clock speed.
        -   `polarity`: Clock polarity.
        -   `phase`: Clock phase.
        -   `dataSize`: 8-bit or 16-bit data.
        -   `ssm`: Software slave management.
    -   **Return:** None.

-   `enable()`
    -   **Purpose:** Enables the SPI peripheral.
    -   **Return:** None.

-   `disable()`
    -   **Purpose:** Disables the SPI peripheral.
    -   **Return:** None.

-   `transmit(uint8_t data)`
    -   **Purpose:** Transmits a single 8-bit data frame.
    -   **Parameters:** `data` - The byte to transmit.
    -   **Return:** None.

-   `transmit16(uint16_t data)`
    -   **Purpose:** Transmits a single 16-bit data frame.
    -   **Parameters:** `data` - The 16-bit value to transmit.
    -   **Return:** None.

-   `transmitBuffer(const uint8_t* buffer, uint32_t length)`
    -   **Purpose:** Transmits a buffer of 8-bit data.
    -   **Parameters:**
        -   `buffer`: Pointer to the data buffer.
        -   `length`: Number of bytes to transmit.
    -   **Return:** None.

-   `receive()`
    -   **Purpose:** Receives a single 8-bit data frame.
    -   **Return:** The received byte.

-   `receive16()`
    -   **Purpose:** Receives a single 16-bit data frame.
    -   **Return:** The received 16-bit value.

-   `receiveBuffer(uint8_t* buffer, uint32_t length)`
    -   **Purpose:** Receives a buffer of 8-bit data.
    -   **Parameters:**
        -   `buffer`: Pointer to the destination buffer.
        -   `length`: Number of bytes to receive.
    -   **Return:** None.

-   `isTXE()`
    -   **Purpose:** Checks if the transmit buffer is empty.
    -   **Return:** `true` if TXE flag is set, `false` otherwise.

-   `isRXNE()`
    -   **Purpose:** Checks if the receive buffer is not empty.
    -   **Return:** `true` if RXNE flag is set, `false` otherwise.

-   `isBusy()`
    -   **Purpose:** Checks if the SPI peripheral is busy.
    -   **Return:** `true` if BSY flag is set, `false` otherwise.

### I2C Driver

**Header:** `Drivers/Drivers/I2C/Inc/I2C_Driver.h`

**Class:** `I2C`

The `I2C` class provides an interface for the Inter-Integrated Circuit peripheral.

**Constructor:**

-   `I2C(I2C_Instance instance)`
    -   **Purpose:** Constructs an I2C object for a specific I2C instance.
    -   **Parameters:** `instance` - The I2C peripheral to use (e.g., `I2C_Instance::I2C1`).

**Enumerations:**

-   `I2C_Instance`: Specifies the I2C peripheral (I2C1, I2C2, etc.).
-   `I2C_DutyCycle`: I2C clock duty cycle in fast mode.
-   `I2C_ACK`: Acknowledge enable/disable.

**APIs:**

-   `enableClock()`
    -   **Purpose:** Enables the clock for the selected I2C peripheral.
    -   **Return:** None.

-   `init(uint32_t clockSpeed, I2C_ACK ack)`
    -   **Purpose:** Initializes the I2C peripheral.
    -   **Parameters:**
        -   `clockSpeed`: The desired I2C clock speed in Hz.
        -   `ack`: Enable or disable acknowledgment.
    -   **Return:** None.

-   `enable()`
    -   **Purpose:** Enables the I2C peripheral.
    -   **Return:** None.

-   `disable()`
    -   **Purpose:** Disables the I2C peripheral.
    -   **Return:** None.

-   `generateStart()`
    -   **Purpose:** Generates the I2C START condition.
    -   **Return:** None.

-   `generateStop()`
    -   **Purpose:** Generates the I2C STOP condition.
    -   **Return:** None.

-   `sendAddress(uint8_t address, bool read)`
    -   **Purpose:** Sends the slave address.
    -   **Parameters:**
        -   `address`: The 7-bit slave address.
        -   `read`: `true` for a read operation, `false` for a write operation.
    -   **Return:** None.

-   `sendByte(uint8_t data)`
    -   **Purpose:** Transmits a single byte of data.
    -   **Parameters:** `data` - The byte to transmit.
    -   **Return:** None.

-   `receiveByte()`
    -   **Purpose:** Receives a single byte of data.
    -   **Return:** The received byte.

-   `masterTransmit(uint8_t slaveAddress, const uint8_t* buffer, uint32_t length)`
    -   **Purpose:** Transmits a buffer of data to a slave device.
    -   **Parameters:**
        -   `slaveAddress`: The 7-bit address of the slave.
        -   `buffer`: Pointer to the data buffer.
        -   `length`: Number of bytes to transmit.
    -   **Return:** None.

-   `masterReceive(uint8_t slaveAddress, uint8_t* buffer, uint32_t length)`
    -   **Purpose:** Receives a buffer of data from a slave device.
    -   **Parameters:**
        -   `slaveAddress`: The 7-bit address of the slave.
        -   `buffer`: Pointer to the destination buffer.
        -   `length`: Number of bytes to receive.
    -   **Return:** None.

-   `isStartGenerated()`
    -   **Purpose:** Checks if a START condition has been generated.
    -   **Return:** `true` if SB flag is set, `false` otherwise.

-   `isAddressSent()`
    -   **Purpose:** Checks if the address has been sent.
    -   **Return:** `true` if ADDR flag is set, `false` otherwise.

-   `isTXE()`
    -   **Purpose:** Checks if the data register is empty.
    -   **Return:** `true` if TXE flag is set, `false` otherwise.

-   `isRXNE()`
    -   **Purpose:** Checks if the data register is not empty.
    -   **Return:** `true` if RXNE flag is set, `false` otherwise.

-   `isTransferFinished()`
    -   **Purpose:** Checks if the byte transfer is finished.
    -   **Return:** `true` if BTF flag is set, `false` otherwise.

-   `isBusy()`
    -   **Purpose:** Checks if the I2C bus is busy.
    -   **Return:** `true` if BUSY flag is set, `false` otherwise.

### ADC Driver

**Header:** `Drivers/Drivers/ADC/Inc/ADC_Driver.h`

**Class:** `ADC`

The `ADC` class provides an interface for the Analog-to-Digital Converter.

**Constructor:**

-   `ADC(ADC_Instance instance)`
    -   **Purpose:** Constructs an ADC object for a specific ADC instance.
    -   **Parameters:** `instance` - The ADC peripheral to use (e.g., `ADC_Instance::ADC1`).

**Enumerations:**

-   `ADC_Instance`: Specifies the ADC peripheral (ADC1, ADC2, ADC3).
-   `ADC_Resolution`: ADC conversion resolution (12, 10, 8, or 6 bits).
-   `ADC_Alignment`: Data alignment (RIGHT or LEFT).
-   `ADC_MODE`: Conversion mode (SINGLE, CONTINUOUS).
-   `ADC_Channel`: ADC input channel (CH0 to CH18).
-   `ADC_SampleTime`: Channel sample time (3 to 480 cycles).

**APIs:**

-   `enableClock()`
    -   **Purpose:** Enables the clock for the selected ADC peripheral.
    -   **Return:** None.

-   `init(ADC_Resolution resolution, ADC_Alignment alignment, ADC_MODE mode)`
    -   **Purpose:** Initializes the ADC peripheral.
    -   **Parameters:**
        -   `resolution`: The ADC conversion resolution.
        -   `alignment`: The data alignment.
        -   `mode`: The conversion mode.
    -   **Return:** None.

-   `enable()`
    -   **Purpose:** Enables the ADC peripheral.
    -   **Return:** None.

-   `disable()`
    -   **Purpose:** Disables the ADC peripheral.
    -   **Return:** None.

-   `selectChannel(ADC_Channel channel)`
    -   **Purpose:** Selects a channel for conversion.
    -   **Parameters:** `channel` - The ADC channel to select.
    -   **Return:** None.

-   `setSampleTime(ADC_Channel channel, ADC_SampleTime sampletime)`
    -   **Purpose:** Sets the sample time for a specific channel.
    -   **Parameters:**
        -   `channel`: The ADC channel.
        -   `sampletime`: The sample time for the channel.
    -   **Return:** None.

-   `startConversion()`
    -   **Purpose:** Starts the ADC conversion.
    -   **Return:** None.

-   `conversionComplete()`
    -   **Purpose:** Checks if the ADC conversion is complete.
    -   **Return:** `true` if the conversion is complete, `false` otherwise.

-   `read()`
    -   **Purpose:** Reads the converted ADC value.
    -   **Return:** The 16-bit converted value.

-   `stopConversion()`
    -   **Purpose:** Stops the ADC conversion.
    -   **Return:** None.

### Timer (TIM) Driver

**Header:** `Drivers/Drivers/TIM/Inc/TIM_Driver.h`

**Class:** `TIM`

The `TIM` class provides an interface for the general-purpose timers.

**Constructor:**

-   `TIM(TIM_Instance instance)`
    -   **Purpose:** Constructs a TIM object for a specific timer instance.
    -   **Parameters:** `instance` - The timer peripheral to use (e.g., `TIM_Instance::TIM2`).

**Enumerations:**

-   `TIM_Instance`: Specifies the timer peripheral (TIM2, TIM3, etc.).
-   `TIM_CounterMode`: Timer counting mode (UP, DOWN, UP_DOWN).
-   `TIM_Channel`: Timer channel (CHANNEL_1 to CHANNEL_4).
-   `TIM_OCMode`: Output Compare mode.
-   `TIM_ICPolatity`: Input Capture polarity (RISING, FALLING, BOTH).

**APIs:**

-   `enableClock()`
    -   **Purpose:** Enables the clock for the selected timer peripheral.
    -   **Return:** None.

-   `init(TIM_CounterMode Mode)`
    -   **Purpose:** Initializes the timer in a specific counting mode.
    -   **Parameters:** `Mode` - The counting direction.
    -   **Return:** None.

-   `setPrescaler(uint16_t prescaler)`
    -   **Purpose:** Sets the timer prescaler value.
    -   **Parameters:** `prescaler` - The 16-bit prescaler value.
    -   **Return:** None.

-   `setAutoReload(uint32_t arr)`
    -   **Purpose:** Sets the auto-reload register value.
    -   **Parameters:** `arr` - The 32-bit auto-reload value.
    -   **Return:** None.

-   `setCounter(uint32_t value)`
    -   **Purpose:** Sets the current counter value.
    -   **Parameters:** `value` - The 32-bit counter value.
    -   **Return:** None.

-   `getCounter()`
    -   **Purpose:** Gets the current counter value.
    -   **Return:** The 32-bit counter value.

-   `start()`
    -   **Purpose:** Starts the timer.
    -   **Return:** None.

-   `stop()`
    -   **Purpose:** Stops the timer.
    -   **Return:** None.

-   `reset()`
    -   **Purpose:** Resets the timer counter.
    -   **Return:** None.

-   `isUpdateEvent()`
    -   **Purpose:** Checks if an update event (overflow/underflow) has occurred.
    -   **Return:** `true` if an update event has occurred, `false` otherwise.

-   `clearUpdateEvent()`
    -   **Purpose:** Clears the update event flag.
    -   **Return:** None.

-   `pwmInit(TIM_Channel channel, TIM_OCMode mode)`
    -   **Purpose:** Initializes a timer channel for PWM generation.
    -   **Parameters:**
        -   `channel`: The timer channel to configure.
        -   `mode`: The PWM mode (e.g., `TIM_OCMode::PWM_MODE_1`).
    -   **Return:** None.

-   `setDutyCycle(TIM_Channel channel, uint16_t dutyCycle)`
    -   **Purpose:** Sets the PWM duty cycle for a channel.
    -   **Parameters:**
        -   `channel`: The timer channel.
        -   `dutyCycle`: The duty cycle value (0-1000 for 0-100%).
    -   **Return:** None.

-   `setCompareValue(TIM_Channel channel, uint32_t value)`
    -   **Purpose:** Sets the capture/compare register value for a channel.
    -   **Parameters:**
        -   `channel`: The timer channel.
        -   `value`: The 32-bit compare value.
    -   **Return:** None.

-   `inputCaptureInit(TIM_Channel channel, TIM_ICPolatity polarity)`
    -   **Purpose:** Initializes a timer channel for input capture mode.
    -   **Parameters:**
        -   `channel`: The timer channel.
        -   `polarity`: The edge polarity to capture.
    -   **Return:** None.

-   `captureValue(TIM_Channel channel)`
    -   **Purpose:** Reads the captured value from a timer channel.
    -   **Return:** The 32-bit captured value.

-   `enableInterrupt()`
    -   **Purpose:** Enables the timer's update interrupt.
    -   **Return:** None.

-   `disableInterrupt()`
    -   **Purpose:** Disables the timer's update interrupt.
    -   **Return:** None.

