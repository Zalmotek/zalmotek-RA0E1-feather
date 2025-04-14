# Zalmotek RA0E1 Feather Blink

A simple LED blink demo for the Zalmotek RA0E1 Feather development board, powered by Renesas.

## Overview

This project demonstrates basic GPIO control on the Zalmotek RA0E1 Feather board by blinking an onboard LED. The program toggles Pin 02 on Port 01 (BSP_IO_PORT_01_PIN_02) connected to the LED at a regular interval, providing a visual indication that the microcontroller is running.

## Hardware Requirements

- Zalmotek RA0E1 Feather board 
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
   In e² studio go to File -> Import..., choose "Existing Projects into Workspace" and browse to the project you’ve just downloaded, then click Finish:
   
<p align="center">
  <img src="1.png" height="500">
  <img src="/2.png" height="500">
</p>

After importing your project, open the configuration.xml file to access the board configurator. Let's review some key settings that will be relevant for all your future RA0E1 Feather SoM projects. First of all, in the BSP tab, your project should have the Custom User Board and the R7FA0E1073CFJ device selected.

<p align="center">
  <img src="/3.png" height="500">
</p>

Next, in the Pins tab, you may need to adjust this for future projects, based on what peripherals you want to enable. You can also set the pins as inputs or outputs and adjust their current drive capacity. For example, the LED is connected to P102 which is set to Output Mode. You can find its configuration in the Pin Selection menu ->Ports -> P1 -> P102.

<p align="center">
  <img src="/4.png" height="500">
</p>

3. Connect your Zalmotek RA0E1 Feather board via USB
4. 4. Build the project
5. Flash the firmware to the board

To run the project, click Generate Project Content, and then you can Build the project and Debug it. In the prompt that pops up, choose Debug as Renesas GDB Hardware Debugging. Click the Resume icon to begin executing the project. Reset the board and now the USR LED should be blinking.

If you want to access the J-Link RTT terminal for the SEGGER's J-Link RTT interface, you must download the RTT Viewer (link: https://www.segger.com/downloads/jlink/). For that, open the application and configure it according to the model below.

<p align="center">
  <img src="/5.png" height="500">
  <img src="/6.png" height="500">
</p>

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
- [Zalmotek RA0E1 Website](https://zalmotek.com/products/RA0E1-Feather-SoM/)
- [FSP Documentation](https://www.renesas.com/us/en/software-tool/flexible-software-package-fsp) 
