# Zalmotek Ra0E1 Feather TF-Luna UART

A UART-based distance sensor integration project using Zalmotek Ra0E1 Feather board powered by Renesas RA0E1 microcontroller.

## Overview

This project demonstrates how to interface a TF-Luna LiDAR sensor with the Zalmotek Ra0E1 Feather board using UART communication. The system reads distance and signal strength measurements from the sensor and displays them through debug output. Utilizing the Renesas RA0E1 microcontroller's UART capabilities, the project showcases effective sensor integration.

## Hardware Requirements

- Zalmotek Ra0E1 Feather board 
- TF-Luna LiDAR sensor
- Connection wires
- USB cable for programming and power

## Software Requirements

- Renesas e² studio IDE
- FSP (Flexible Software Package)
- GCC ARM compiler
- J-Link debugger software

## Features

- Real-time distance measurement using TF-Luna LiDAR sensor
- UART communication protocol implementation
- Signal strength monitoring
- Debug output via SEGGER RTT

## Code Functionality

The main application:
- Initializes the UART interface for communication with the TF-Luna sensor
- Reads 9-byte data packets from the sensor via UART
- Parses distance (in cm) and signal strength from the received data
- Displays readings through debug console
- Operates in a continuous polling loop with 10ms intervals

## Getting Started

### Setup

1. Connect the TF-Luna sensor to the Ra0E1 Feather board's UART pins
2. Open the project in e² studio IDE
3. Build the project using the GCC ARM compiler
4. Program the Ra0E1 Feather board using J-Link
5. Monitor output via SEGGER RTT Viewer

### Configuration

The UART communication is configured as follows:

```c
// UART configuration in FSP configurator
// Baud rate: 115200 bps
// Data bits: 8
// Parity: None
// Stop bits: 1
// Flow control: None
```

## Project Structure

- `src/`: Main source code directory
- `src/hal_entry.cpp`: Main application entry point and sensor reading logic
- `src/SerialCompatibility.h/.cpp`: UART communication helper functions
- `src/SEGGER_RTT/`: Debug output implementation
- `src/common_utils.h`: Common utility functions and macros

## License

Copyright © 2023 Zalmotek

## Additional Resources

- [Zalmotek Website](https://zalmotek.com)
- [Zalmotek RA0E1 Website](https://zalmotek.com/products/RA0E1-Feather-SoM/)
- [SEGGER RTT Documentation](https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/) 