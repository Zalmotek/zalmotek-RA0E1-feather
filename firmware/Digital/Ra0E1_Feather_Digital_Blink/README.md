# Zalmotek RA0E1 Feather Digital Blink

A digital pin control example project for the Zalmotek RA0E1 Feather board powered by Renesas RA0E1 microcontroller.

## Overview

This project demonstrates basic digital output control by sequentially toggling pins D5, D6, D9, D10, D11, D12, and D13 on the Zalmotek RA0E1 Feather board. Each pin is turned on and off in sequence with a 50ms delay, creating a visible LED chase effect when LEDs are connected to these pins.

## Hardware Requirements

- Zalmotek RA0E1 Feather board (featuring Renesas RA0E1 microcontroller)
- LEDs (optional, for visual feedback)
- Jumper wires (if using external LEDs)
- USB cable for programming and power

## Software Requirements

- e² studio IDE
- Renesas FSP (Flexible Software Package)
- J-Link debugger software
- GCC ARM compiler

## Features

- Sequential control of 7 digital output pins
- Configurable delay between state changes
- Debug output via SEGGER RTT
- Simple implementation demonstrating Renesas GPIO control

## Code Functionality

The main application:
- Initializes the GPIO pins (D5, D6, D9, D10, D11, D12, D13) as outputs
- Creates a continuous loop that sequentially activates each pin
- Sets each pin LOW, waits 50ms, then sets it HIGH and waits another 50ms
- Outputs debug information via SEGGER RTT
- Demonstrates proper pin configuration for the RA0E1 microcontroller

## Getting Started

### Setup

1. Clone or download this repository
2. Open e² studio IDE
3. Import the project into your workspace
4. Connect your Zalmotek RA0E1 Feather board via USB
5. Build and flash the project to your board

### Configuration

You can modify the delay between pin state changes:

```c
// Change the delay value (in milliseconds)
delay(50);  // Currently set to 50ms
```

To change which pins are controlled, modify the pins array:

```c
const bsp_io_port_pin_t pins[] = {
    BSP_IO_PORT_02_PIN_00, // D5
    BSP_IO_PORT_01_PIN_08, // D6
    // Add or remove pins as needed
};
```

## Project Structure

- `src/hal_entry.cpp`: Main application code with pin control logic
- `src/common_utils.h`: Utility functions for the application
- `src/SEGGER_RTT/`: SEGGER Real-Time Terminal implementation for debug output
- `ra_cfg.txt`: FSP configuration settings

## License

[License information]

## Additional Resources

- [Zalmotek Website](https://zalmotek.com)
- [Zalmotek RA0E1 Website](https://zalmotek.com/products/RA0E1-Feather-SoM/)
- [SEGGER RTT Documentation](https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/) 