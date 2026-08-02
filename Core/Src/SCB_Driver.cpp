/*
 * SCB_Driver.cpp
 *
 * STM32F407 Bare-Metal SCB Driver
 *
 * Created on: 29-Jul-2026
 * Author: jolap
 *
 * Purpose:
 *   Implementation of the ARM Cortex-M4 System Control Block driver for
 *   exception, fault, and system-configuration management.
 *
 * State Machine:
 *   System reset -> config -> exception entry -> handler dispatch -> return.
 *
 * RM0090 Reference:
 *   ARM Cortex-M4 Generic User Guide - System Control Block (SCB)
 */

/*********************************************************************
 *                      SCB Driver Design
 *
 * The System Control Block (SCB) is a core peripheral of the
 * ARM Cortex-M4 processor responsible for managing system-level
 * configuration, exception handling, processor status, and fault
 * management. Unlike peripheral modules such as GPIO or USART, the
 * SCB directly controls processor behavior and exception execution.
 *
 * The SCB provides software control over:
 *
 *      • Vector Table Relocation
 *      • Processor Reset
 *      • System Exception Configuration
 *      • Fault Status Monitoring
 *      • Fault Address Reporting
 *      • Interrupt Priority Grouping
 *      • Sleep Configuration
 *
 * The SCB contains several important registers:
 *
 *      • CPUID   - CPU Identification Register
 *      • ICSR    - Interrupt Control and State Register
 *      • VTOR    - Vector Table Offset Register
 *      • AIRCR   - Application Interrupt and Reset Control Register
 *      • SCR     - System Control Register
 *      • CCR     - Configuration and Control Register
 *      • SHPR1   - System Handler Priority Register 1
 *      • SHPR2   - System Handler Priority Register 2
 *      • SHPR3   - System Handler Priority Register 3
 *      • SHCSR   - System Handler Control and State Register
 *      • CFSR    - Configurable Fault Status Register
 *      • HFSR    - HardFault Status Register
 *      • DFSR    - Debug Fault Status Register
 *      • MMFAR   - MemManage Fault Address Register
 *      • BFAR    - BusFault Address Register
 *      • AFSR    - Auxiliary Fault Status Register
 *
 * The SCB driver provides software access to processor-level
 * configuration while abstracting direct register manipulation from
 * the application layer.
 *
 *********************************************************************
 *              System Configuration State Machine
 *
 *              System Reset
 *                     │
 *                     ▼
 *            Cortex-M4 Starts
 *                     │
 *                     ▼
 *         Read CPU Identification
 *                     │
 *                     ▼
 *      Configure Vector Table Base
 *                     │
 *                     ▼
 *    Configure Priority Grouping
 *                     │
 *                     ▼
 *    Configure Sleep Behaviour
 *                     │
 *                     ▼
 *      Processor Ready
 *                     │
 *                     ▼
 *     Execute Application
 *
 *********************************************************************
 *              Exception Handling State Machine
 *
 *          Exception Generated
 *                    │
 *                    ▼
 *          Cortex Saves Context
 *                    │
 *                    ▼
 *         SCB Determines Exception
 *                    │
 *                    ▼
 *        Load Exception Vector
 *                    │
 *                    ▼
 *      Execute Exception Handler
 *                    │
 *                    ▼
 *     Update Fault Status Registers
 *                    │
 *                    ▼
 *        Return From Exception
 *                    │
 *                    ▼
 *      Resume Program Execution
 *
 *********************************************************************
 *                 Fault Handling State Machine
 *
 *           Fault Condition
 *                  │
 *                  ▼
 *      Hardware Detects Fault
 *                  │
 *                  ▼
 *      Update Fault Status Bits
 *                  │
 *                  ▼
 *     Store Fault Address (if any)
 *                  │
 *                  ▼
 *     Generate Fault Exception
 *                  │
 *                  ▼
 *      Execute Fault Handler
 *                  │
 *                  ▼
 *      Application Recovery
 *                  │
 *                  ▼
 *        Continue or Reset
 *
 *********************************************************************
 *                  Driver Responsibilities
 *
 * • Read processor identification.
 * • Configure vector table location.
 * • Configure interrupt priority grouping.
 * • Generate software system reset.
 * • Configure sleep modes.
 * • Configure deep sleep mode.
 * • Read processor fault status.
 * • Read fault addresses.
 * • Configure system exception priorities.
 * • Monitor processor state.
 *
 * ARM Cortex-M4 Reference:
 * Chapter 4 - System Control Block (SCB)
 *
 *********************************************************************/