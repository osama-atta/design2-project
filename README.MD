# Embedded Microcontroller Design Project

This project involves the design and implementation of two microcontroller-based systems: the AT89LP51 board and the STM32 board. Below is a summary of the key details from the reports for each board. For more information, please check out each board's respective report.

---

## AT89LP51 Board

The AT89LP51 board is designed for interfacing with peripherals such as an LCD, 7-segment displays, and sensors. Key features include:

- **Microcontroller**: AT89LP51
- **Devices on the Board**:
  - TFT LCD (ILI9341) with 320x240 resolution
  - 7-segment display
  - Temperature sensor
  - Light sensor (photodiode)
  - Keypad
  - LEDs
  - UART interface
  - External RAM
  - External clock
- **Functionalities**:
  - RAM testing
  - Memory data dump, move, count, edit, and find operations 
  - Temperature and light measurements
- **Programming**: 
  - Written in C with inline assembly for performance-critical operations.

 - [View the AT89LP51 Board Report (PDF)](./documentation/at89lp51-board-report.pdf)
---

## STM32 Board

The STM32 board is based on the STM32L031K6T7 microcontroller interfacing with peripherals such as a camera, GPS module and WIFI module. Key features include:

- **Microcontroller**: STM32L031K6T7
- **Devices**:
  - Proximity Sensor
  - Camera
  - SRAM
  - USB to UART
  - GPS Module
  - WIFI Module
- **Functionalities**:
  - Take a picture using the camera and displaying it on the AT89LP51 board over WIFI
  - Read location from GPS
  - Read from proximity sensor
  - Ram Check
- **Programming**:
  - Written in embedded C
  - STM32CubeIDE for project setup and code generation
  - STM32CubeMX for peripheral configuration
- [View the STM32 Board Report (PDF)](./documentation/stm32-board-report.pdf)
