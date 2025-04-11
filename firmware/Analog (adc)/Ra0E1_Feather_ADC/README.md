# Zalmotek RA0E1 Feather ADC

Analog-to-Digital Converter (ADC) example project for Zalmotek RA0E1 Feather board powered by Renesas microcontroller.

## Overview

This project demonstrates the ADC functionality on the Zalmotek RA0E1 Feather board featuring the Renesas RA0E1 microcontroller. It initializes the ADC in either One-shot or Sequential mode and allows users to start/stop ADC scanning and view voltage readings through JLinkRTTViewer.

## Hardware Requirements

- Zalmotek RA0E1 Feather board 
- J-Link debugger
- USB cable for power and programming

## Software Requirements

- e² studio IDE
- FSP (Flexible Software Package)
- J-Link RTT Viewer
- Git (optional, for version control)

## Features

- ADC initialization in One-shot or Sequential mode
- Start/stop ADC scanning via RTT commands
- Real-time voltage reading display
- 12-bit ADC resolution
- ADC input voltage calculation and display

## Code Functionality

The main application:
- Initializes ADC module on the Zalmotek RA0E1 Feather board
- Sets up ADC Channel 4 for analog input readings
- Provides RTT interface for user interaction (start/stop scanning)
- Converts raw ADC values to voltage readings
- Displays ADC output data through JLinkRTTViewer

## Getting Started

### Setup

1. Connect the Zalmotek RA0E1 Feather board to your computer using a USB cable
2. Open the project in e² studio IDE
3. Build the project
4. Connect the J-Link debugger to the board
5. Flash the program to the board
6. Open J-Link RTT Viewer to interact with the application

### Configuration

The ADC can be configured in either One-shot or Sequential mode through the Renesas configurator:

```c
// Example configuration in One-shot mode
const adc_d_extended_cfg_t g_adc_cfg_extend =
{
    .conversion_operation = ADC_D_CONVERSION_MODE_ONESHOT,
};
```

## Project Structure

- `src/hal_entry.c`: Main application entry point
- `src/adc_ep.c`: ADC functionality implementation
- `src/adc_ep.h`: ADC header file with definitions
- `src/common_utils.h`: Common utilities and definitions
- `src/SEGGER_RTT/`: SEGGER RTT implementation for communication

## License

BSD-3-Clause

## Additional Resources

- [Zalmotek Website](https://zalmotek.com)
- [Zalmotek RA0E1 Documentation](https://zalmotek.com/products/RA0E1-Feather-SoM/)
- [FSP Documentation](https://www.renesas.com/us/en/software-tool/flexible-software-package-fsp) 