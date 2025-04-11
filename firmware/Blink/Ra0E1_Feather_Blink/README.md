# Zalmotek RA0E1 Feather Blink

A simple LED blink demo for the Zalmotek RA0E1 Feather development board, powered by Renesas.

## Overview

This project demonstrates basic GPIO control on the Zalmotek RA0E1 Feather board by blinking an onboard LED. The program toggles Pin 02 on Port 01 (BSP_IO_PORT_01_PIN_02) connected to the LED at a regular interval, providing a visual indication that the microcontroller is running.

## Hardware Requirements

- Zalmotek RA0E1 Feather board (featuring Renesas R7FA0E1073CFJ)
- USB connection for programming

## Software Requirements

- e2 studio IDE
- Renesas FSP (Flexible Software Package)
- J-Link Debug Probe

## Features

- Simple LED blinking demonstration
- Configurable blink rate with delay function
- Debug output messages via SEGGER RTT

## Code Functionality

The main application:
- Initializes GPIO pin (BSP_IO_PORT_01_PIN_02) for LED control
- Toggles the LED state between LOW and HIGH every 300ms
- Outputs status messages ("LOW"/"HIGH") via debug console
- Runs in a continuous loop

## Getting Started

### Setup

1. Clone this repository
2. Open the project in e2 studio
3. Connect your Zalmotek RA0E1 Feather board via USB
4. Build the project
5. Flash the firmware to the board

### Configuration

The blink rate can be adjusted by modifying the delay value in the `hal_entry.cpp` file:

```c
delay(300); // Time in milliseconds
```

## Project Structure

- `src/hal_entry.cpp`: Main application code
- `src/common_utils.h`: Utility functions
- `src/SEGGER_RTT/`: Debug output library

## License

This project is provided for educational purposes.

## Additional Resources

- [Zalmotek Website](https://zalmotek.com)
- [Renesas RA0E1 Documentation](https://www.renesas.com/us/en/products/microcontrollers-microprocessors/ra-cortex-m-mcus/ra0e1-32-mhz-arm-cortex-m23-ultra-low-power)
- [FSP Documentation](https://www.renesas.com/us/en/software-tool/flexible-software-package-fsp) 