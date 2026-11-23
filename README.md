# Bare-Metal Serial Driver Library – STM32L476RG

A register-level peripheral driver library developed from scratch for the STM32L476RG microcontroller (ARM Cortex-M4F).

The objective of this project is to develop reusable, low-level peripheral drivers without relying on STM32 HAL peripheral APIs, while understanding the underlying hardware architecture, memory-mapped registers, communication protocols and interrupt handling.

## Project Overview

- **Target Hardware:** STM32 NUCLEO-L476RG
- **Development Environment:** STM32CubeIDE 1.19.0
- **Programming Language:** Embedded C
- **Architecture:** ARM Cortex-M4F
- **Programming Approach:** Bare-metal, register-level programming

## Project Objectives

- Develop reusable bare-metal peripheral drivers using direct register-level programming.
- Understand STM32 peripheral registers and clock configuration.
- Implement UART, SPI and I²C communication drivers.
- Develop interrupt-driven communication mechanisms.
- Implement circular ring buffers for efficient serial data handling.
- Validate peripheral functionality through hardware testing.

## Hardware and Software Requirements

### Hardware
- STM32 NUCLEO-L476RG development board
- USB cable for ST-LINK programming and Virtual COM Port communication
- PC or laptop
- Jumper wires for SPI loopback testing
- RG1602A 16×2 LCD with PCF8574T I²C backpack for I²C testing

### Software
- STM32CubeIDE 2.19.0
- ARM GNU Toolchain
- Git and GitHub Desktop
- Tera Term (serial terminal)

## Project Structure

```text
BareMetal_Serial_Driver_Library/
│
├── Inc/
│   ├── gpio.h
│   ├── uart.h
│   ├── spi.h
│   └── i2c.h
│
├── Src/
│   ├── main.c
│   ├── gpio.c
│   ├── uart.c
│   ├── spi.c
│   ├── i2c.c
│   ├── syscalls.c
│   └── sysmem.c
│
├── Startup/
│   └── startup_stm32l476rgtx.s
│
├── .settings/
├── .gitignore
├── .gitattributes
├── .project
├── .cproject
│
├── STM32L476RGTX_FLASH.ld
└── STM32L476RGTX_RAM.ld
```

## Implemented Drivers

### 1. GPIO Driver

**Status: Implemented and Hardware Tested**

Developed a register-level GPIO driver for the STM32L476RG.

Features:
- GPIO peripheral clock configuration through RCC registers.
- GPIO pin mode configuration using GPIOx_MODER.
- Direct output control using GPIOx_BSRR.
- Configured PA5 to control the onboard green user LED (LD2).
- Implemented LED blinking using a software delay.

Hardware Verification:
- Successfully programmed the microcontroller using ST-LINK.
- Verified LED operation, including execution after a complete power cycle.

### 2. USART2 UART Driver

**Status: Implemented and Hardware Tested**

Developed a bare-metal USART2 driver using direct register access.

Configuration:

| Parameter | Configuration |
|---|---|
| Peripheral | USART2 |
| Operating Mode | Asynchronous |
| TX Pin | PA2 (AF7) |
| RX Pin | PA3 (AF7) |
| Baud Rate | 115200 |
| Data Bits | 8 |
| Parity | None |
| Stop Bits | 1 |
| Flow Control | None |

Features:
- USART2 peripheral clock configuration.
- GPIO alternate-function configuration.
- Baud-rate register configuration.
- Character and string transmission using USART2_TDR.
- Interrupt-driven UART reception.
- Circular RX ring buffer implementation.
- Buffer overflow monitoring.
- Polling-based UART transmission.
- Character echo functionality.

Hardware Verification:
- Established serial communication between STM32L476RG and PC using the onboard ST-LINK Virtual COM Port.
- Verified continuous transmission of messages from STM32 to PC.
- Verified interrupt-driven character reception and echo functionality.
- Verified RX overflow monitoring functionality.

Serial Terminal: Tera Term

Connection: USB through onboard ST-LINK Virtual COM Port.

Clock Configuration Note: The initial UART implementation assumes the reset-default 4 MHz MSI system clock and an undivided APB1 clock.

### 3. SPI1 Driver

**Status: Implemented and Hardware Tested**

Developed a register-level SPI1 master driver supporting full-duplex communication.

Configuration:

| Parameter | Configuration |
|---|---|
| Peripheral | SPI1 |
| Operating Mode | Master |
| Clock Prescaler | PCLK2 / 16 |
| Data Frame | 8-bit |
| Communication | Full-duplex |
| Clock Polarity | CPOL = 0 |
| Clock Phase | CPHA = 0 |
| NSS Management | Software |

Features:
- SPI peripheral clock configuration through RCC registers.
- GPIO alternate-function configuration.
- SPI master-mode initialization.
- Full-duplex data transmission and reception.
- TXE, RXNE and BSY status flag handling.

Hardware Verification:
- Implemented SPI loopback testing by connecting MOSI directly to MISO.
- Transmitted test byte 0xA5.
- Successfully received the same byte.
- Verified received decimal value: 165.
- Displayed test results through the UART terminal.

### 4. I²C1 Driver

**Status: Implemented and Hardware Tested**

Developed a register-level I²C1 master driver for communication with external I²C slave devices.

Configuration:

| Parameter | Configuration |
|---|---|
| Peripheral | I2C1 |
| Operating Mode | Master |
| SCL Pin | PB8 |
| SDA Pin | PB9 |
| Clock Speed | 100 kHz target |
| Addressing | 7-bit |

Features:
- I²C peripheral clock configuration.
- GPIO alternate-function configuration.
- Open-drain pin configuration.
- I²C timing register configuration.
- START and STOP condition generation through peripheral control.
- Slave addressing and byte transmission.
- ACK/NACK detection.
- Timeout-based error handling.
- I²C slave-device address scanning.

Hardware Verification:
- Connected an RG1602A LCD with PCF8574T I²C backpack.
- Successfully detected the I²C slave device.
- Verified acknowledgement at address 0x27 (decimal 39).
- Displayed I²C scanning results through the UART terminal.

Note: LCD character display functionality and a complete reusable LCD driver are not yet implemented.

## Development Roadmap

The following enhancements are planned for future development.

### UART Enhancements
- Sustained high-throughput communication testing.
- Detailed buffer performance and overflow analysis.
- Additional error-handling mechanisms.

### SPI Enhancements
- Communication with external SPI slave devices.
- Additional SPI configuration modes.
- Logic analyser-based timing verification.

### I²C Enhancements
- Complete reusable I²C transaction handling.
- LCD initialization and character-display driver.
- Additional error handling and bus recovery.

### Hardware Validation
- Logic analyser verification.
- Digital storage oscilloscope measurements.
- Timing accuracy analysis.
- Signal-integrity testing.

## Build and Flash Instructions

1. Open STM32CubeIDE.
2. Import the project into the workspace.
3. Select the Debug build configuration.
4. Build the project using Ctrl + B.
5. Connect the NUCLEO-L476RG through USB.
6. Configure the ST-LINK debugger.
7. Flash the generated firmware using the Run or Debug configuration.
8. Verify peripheral operation on the development board.

## UART Testing

1. Connect the NUCLEO-L476RG to the PC through USB.
2. Open Tera Term.
3. Select the ST-LINK Virtual COM Port.
4. Configure the serial connection:

| Parameter | Configuration |
|---|---|
| Baud Rate | 115200 |
| Data Bits | 8 |
| Parity | None |
| Stop Bits | 1 |
| Flow Control | None |

5. Run the STM32 firmware.
6. Verify UART transmission and interrupt-driven reception through the terminal.

## Project Status

This project is under active development.

The GPIO, USART2, SPI1 and I²C1 peripheral drivers have been implemented using register-level programming and tested on the STM32 NUCLEO-L476RG.

Hardware validation includes UART terminal communication, SPI loopback testing and I²C slave-device detection.

Future development will focus on extended protocol functionality, performance testing and hardware-level signal verification.

## Author

**Mithilesh Shirawle**

Final-Year Electronics and Telecommunication Engineering Student

GitHub: shirawlemithilesh-sys

Target Platform: STM32 NUCLEO-L476RG
