# Zalmotek Ra0E1 Feather USB Serial

A UART communication example for Zalmotek Ra0E1 Feather board powered by Renesas RA0E1 microcontroller.

## Overview

This project demonstrates basic UART serial communication on the Zalmotek Ra0E1 Feather board featuring Renesas RA0E1 microcontroller. It implements a simple LED blink pattern with serial output messages, utilizing pins P102 for LED control and the USB serial interface for communication.

## Hardware Requirements

- Zalmotek Ra0E1 Feather board 
- USB cable for power and communication
- Optional: LEDs for visual output

## Software Requirements

- Renesas e² studio IDE
- Renesas FSP (Flexible Software Package)
- J-Link debugger software
- Serial terminal application (e.g., PuTTY, TeraTerm)

## Features

- Arduino-like Serial API compatibility layer
- Dual UART channel support (Serial and Serial1)
- LED blink pattern with status messages
- USB serial communication at 115200 baud

## Code Functionality

The main application:
- Initializes UART communication at 115200 baud rate
- Toggles the LED connected to pin P102 (BSP_IO_PORT_01_PIN_02)
- Outputs "LOW" and "HIGH" messages via Serial during LED state changes
- Implements timing delays of 300ms for LOW state and 1000ms for HIGH state
- Provides Arduino-compatible Serial API functions through a compatibility layer

## Getting Started

### Setup

1. Clone or download this repository
2. Open the project in Renesas e² studio
3. Connect the Ra0E1 Feather board to your computer via USB
4. Build and flash the project to the board
5. Open a serial terminal application and connect to the board's COM port at 115200 baud

### Configuration

The project uses Serial over USB for communication:

```c
Serial.begin(115200);
while (true) {
    R_BSP_PinWrite(BSP_IO_PORT_01_PIN_02, BSP_IO_LEVEL_LOW);
    Serial.println((uint8_t*)("LOW\n"));
    delay(300);

    R_BSP_PinWrite(BSP_IO_PORT_01_PIN_02, BSP_IO_LEVEL_HIGH);
    Serial.println((uint8_t*)("HIGH\n"));
    delay(1000);
}
```

## Project Structure

- `src/hal_entry.cpp`: Main application entry point with LED blink and serial output logic
- `src/SerialCompatibility.h`: Header defining the Arduino-compatible Serial API
- `src/SerialCompatibility.cpp`: Implementation of the Serial compatibility layer
- `script/`: Build and configuration scripts
- `.settings/`: Project settings

## License

[License information]

## Additional Resources

- [Zalmotek Website](https://zalmotek.com)
- [Zalmotek RA0E1 Website](https://zalmotek.com/products/RA0E1-Feather-SoM/)
- [SEGGER RTT Documentation](https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/) 