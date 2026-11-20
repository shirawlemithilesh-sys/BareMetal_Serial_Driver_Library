# Bare-Metal Serial Driver Library – STM32L476RG

A register-level peripheral driver library developed from scratch for the STM32L476RG microcontroller (ARM Cortex-M4F).

The objective of this project is to develop reusable, low-level peripheral drivers without relying on STM32 HAL peripheral APIs, while understanding the underlying hardware architecture, memory-mapped registers, communication protocols and interrupt handling.

**Target Hardware:** STM32 NUCLEO-L476RG

**Development Environment:** STM32CubeIDE 2.19.0

**Programming Language:** Embedded C

**Architecture:** ARM Cortex-M4F

---

## Project Objectives

- Develop reusable bare-metal peripheral drivers using direct register-level programming.
- Understand STM32 peripheral registers and clock configuration.
- Implement UART, SPI and I2C communication drivers.
- Develop interrupt-driven communication mechanisms.
- Implement lock-free circular ring buffers for efficient serial data handling.
- Verify peripheral timing and signal integrity using logic analyzers and digital storage oscilloscopes (DSOs).

---

## Hardware and Software Requirements

### Hardware
- STM32 NUCLEO-L476RG development board
- USB cable for ST-LINK programming and Virtual COM Port communication
- Laptop or PC

Additional test equipment, including a logic analyzer and digital storage oscilloscope, will be used during hardware validation.

### Software
- STM32CubeIDE 2.19.0
- ARM GNU Toolchain
- Git and GitHub Desktop
- Tera Term (serial terminal)

---

## Project Structure

```text
BareMetal_Serial_Driver_Library/
│
├── Inc/
│   ├── gpio.h
│   └── uart.h
│
├── Src/
│   ├── main.c
│   ├── gpio.c
│   ├── uart.c
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

---

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

**Hardware Verification:**

Successfully programmed the microcontroller using ST-LINK and verified LED operation, including execution after a complete power cycle.

### 2. USART2 UART Driver

**Status: Polling-Based Transmission and Reception Implemented and Tested**

Developed a basic USART2 driver using direct register access.

Configuration:

| Parameter | Configuration |
|---|---|
| Peripheral | USART2 |
| Operating Mode | Asynchronous |
| TX Pin | PA2 (AF7) |
| RX Pin | PA3 (AF7) |
| Configured Baud Rate | 115200 |
| Data Bits | 8 |
| Parity | None |
| Stop Bits | 1 |
| Flow Control | None |

Features:
- USART2 peripheral clock configuration.
- GPIO alternate-function configuration.
- Baud-rate register configuration.
- Character transmission using USART2_TDR.
- String transmission.
- Character reception using USART2_RDR.
- Polling-based transmission and reception.
- UART echo test.

**Hardware Verification:**

Successfully established serial communication between the STM32L476RG and a PC using the onboard ST-LINK Virtual COM Port.

Verified:
- Continuous transmission of messages from STM32 to PC.
- Reception of characters from PC to STM32.
- Character echo functionality.

**Serial Terminal:** Tera Term

**Connection:** USB through onboard ST-LINK Virtual COM Port.

Note: The initial UART implementation assumes the reset-default 4 MHz MSI system clock and an undivided APB1 clock. The baud-rate register is configured accordingly.

---

## Development Roadmap

The following features are planned for subsequent development.

### UART Enhancements
- [ ] Interrupt-driven UART reception.
- [ ] NVIC configuration and interrupt handling.
- [ ] Lock-free circular ring buffer implementation.
- [ ] Continuous high-throughput serial communication.
- [ ] UART driver testing under sustained data loads.

### SPI Driver
- [ ] SPI peripheral initialization.
- [ ] Master-mode communication.
- [ ] Data transmission and reception.
- [ ] SPI timing and signal-integrity verification.

### I2C Driver
- [ ] I2C peripheral initialization.
- [ ] Master-mode communication.
- [ ] Start/stop condition handling.
- [ ] Data transmission and reception.
- [ ] I2C signal verification.

### Hardware Validation
- [ ] Logic analyzer verification.
- [ ] Digital storage oscilloscope measurements.
- [ ] Timing accuracy analysis.
- [ ] Signal-integrity testing.

---

## Build and Flash Instructions

1. Open STM32CubeIDE.
2. Import the project into the workspace.
3. Select the Debug build configuration.
4. Build the project using `Ctrl + B`.
5. Connect the NUCLEO-L476RG through USB.
6. Configure the ST-LINK debugger.
7. Flash the generated firmware using the Run or Debug configuration.
8. Verify peripheral operation on the development board.

### UART Testing

1. Connect the NUCLEO-L476RG to the PC through USB.
2. Open Tera Term.
3. Select the ST-LINK Virtual COM Port.
4. Configure the serial connection:

   - Baud Rate: 115200
   - Data Bits: 8
   - Parity: None
   - Stop Bits: 1
   - Flow Control: None

5. Run the STM32 firmware.
6. Verify UART transmission and reception through the terminal.

---

## Project Status

This project is under active development.

GPIO and polling-based USART2 transmission and reception have been implemented and verified on the STM32 NUCLEO-L476RG.

Interrupt-driven UART, ring buffers, SPI, I2C and hardware signal validation remain planned development milestones.

---

## Author

**Mithilesh Shirawle**

Final-Year Electronics and Telecommunication Engineering Student

GitHub: [shirawlemithilesh-sys](https://github.com/shirawlemithilesh-sys)

**Target Platform:** STM32 NUCLEO-L476RG
