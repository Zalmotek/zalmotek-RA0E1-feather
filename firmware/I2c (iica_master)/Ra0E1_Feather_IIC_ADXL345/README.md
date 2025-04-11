# Zalmotek Ra0E1 Feather I2C ADXL345

Accelerometer sensor integration project using I2C communication, built on Zalmotek hardware powered by Renesas Ra0E1 microcontroller.

## Overview

This project demonstrates I2C master communication between the Zalmotek Ra0E1 Feather board and an ADXL345 accelerometer sensor. The application continuously reads 3-axis acceleration data from the sensor and outputs formatted values through UART serial communication. The project leverages Zalmotek hardware featuring Renesas Ra0E1 technology.

## Hardware Requirements

- Zalmotek Ra0E1 Feather board 
- ADXL345 accelerometer sensor
- Connection cables
- USB cable for power and data transfer

## Software Requirements

- Renesas e² studio IDE
- Renesas FSP (Flexible Software Package)
- J-Link debugging tools
- Serial terminal application (115200 baud rate)

## Features

- I2C master communication implementation
- Real-time 3-axis accelerometer data acquisition
- UART serial output of accelerometer readings
- Error detection and status reporting
- Formatted data output for easy reading

## Code Functionality

The main application:
- Initializes I2C communication with the ADXL345 accelerometer sensor
- Configures ADXL345 power control register to enable measurements
- Polls sensor data registers to read X, Y, Z acceleration values at 1-second intervals
- Formats and displays acceleration data through serial communication
- Provides visual status indicators for I2C and computation status

## Getting Started

### Setup

1. Connect the ADXL345 sensor to the Zalmotek Ra0E1 Feather board using I2C pins
2. Connect the Zalmotek board to your computer via USB
3. Open the project in Renesas e² studio
4. Build the project using the provided configuration
5. Flash the program to the Zalmotek Ra0E1 Feather board
6. Open a serial terminal at 115200 baud rate to view sensor data

### Configuration

The ADXL345 sensor is configured in the initialize function:

```c
void init_sensor(void)
{
    // Enable measurement mode
    uint8_t measure_enable[MEASURE_PAYLOAD_SIZE] = {POWER_CTL_REG, ENABLE_BIT, RESET_VALUE};
    // Write to power control register to activate measurements
    ...
}
```

## Project Structure

- `src/hal_entry.cpp`: Main application entry point and processing loop
- `src/i2c_sensor.cpp`: ADXL345 sensor interface implementation
- `src/i2c_sensor.h`: Sensor definitions and function declarations
- `src/SerialCompatibility.cpp`: Serial communication utilities
- `src/SEGGER_RTT/`: Runtime terminal debug utilities
- `src/common_utils.h`: Common utility macros and functions

## License

This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.

## Additional Resources

- [Zalmotek Website](https://zalmotek.com)
- [Zalmotek RA0E1 Website](https://zalmotek.com/products/RA0E1-Feather-SoM/)
- [SEGGER RTT Documentation](https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/) 
