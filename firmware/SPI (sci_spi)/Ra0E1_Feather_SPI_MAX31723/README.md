# Zalmotek RA0E1 Feather SPI Temperature Sensor

A temperature sensor integration using the MAX31723 and SPI communication with Zalmotek RA0E1 Feather board powered by Renesas RA0E1 microcontroller.

## Overview

This project demonstrates SPI communication between a Zalmotek RA0E1 Feather board and the MAX31723 high-precision temperature sensor. The application continuously reads temperature data from the sensor using the Renesas RA0E1 microcontroller's SPI peripheral and displays the values through serial communication.

## Hardware Requirements

- Zalmotek RA0E1 Feather board
- MAX31723 temperature sensor
- Connecting wires
- USB cable for power and communication

## Software Requirements

- Renesas e² studio IDE
- Renesas FSP (Flexible Software Package)
- Renesas RA0E1 board support package
- Serial terminal application (115200 baud rate)

## Features

- SPI communication with MAX31723 temperature sensor
- 12-bit temperature resolution configuration
- Continuous temperature monitoring
- Serial output of temperature readings
- Error detection and status reporting

## Code Functionality

The main application:
- Initializes the RA0E1 Feather's SPI peripheral (using port 4, pin 7 as chip select)
- Configures the MAX31723 sensor with 12-bit resolution
- Reads temperature values at 1-second intervals
- Validates communication status and data integrity
- Outputs formatted temperature readings via serial (115200 baud)
- Provides visual status indicators for SPI communication

## Getting Started

### Setup

1. Connect the MAX31723 sensor to the RA0E1 Feather board using SPI connections
2. Install Renesas e² studio IDE and required software packages
3. Import the project into e² studio
4. Build the project
5. Flash the compiled program to the RA0E1 Feather board
6. Open a serial terminal (115200 baud) to view temperature readings

### Configuration

The MAX31723 sensor is configured for 12-bit resolution:

```c
/* Value for configuration register write. Set resolution as 12-bits*/
const uint8_t config_sensor[3] =
{
 0x80, 0x06
};
```

## Project Structure

- `src/hal_entry.cpp`: Main application entry point and program loop
- `src/spi_max31723.cpp`: MAX31723 sensor interface implementation
- `src/spi_max31723.h`: Header file with sensor definitions and declarations
- `src/SerialCompatibility.cpp/.h`: Serial communication wrapper
- `src/SEGGER_RTT/`: SEGGER RTT debugging interface

## License

Copyright © 2023 Zalmotek. All rights reserved.

## Additional Resources

- [Zalmotek Website](https://zalmotek.com)
- [Zalmotek RA0E1 Website](https://zalmotek.com/products/RA0E1-Feather-SoM/)
- [SEGGER RTT Documentation](https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/) 
