# Zalmotek RA0E1 Feather NeoPixel

Control NeoPixel RGB LEDs with Zalmotek RA0E1 Feather board powered by Renesas RA0E1 microcontroller.

## Overview

This project demonstrates how to control NeoPixel RGB LEDs using a Zalmotek RA0E1 Feather board featuring the Renesas RA0E1 microcontroller. It utilizes Port 2 Pin 1 (D5) to drive the NeoPixel LED, creating a simple color display.

## Hardware Requirements

- Zalmotek RA0E1 Feather board 
- NeoPixel RGB LED(s)
- Jumper wires
- USB cable for programming and power

## Software Requirements

- Renesas e² studio IDE
- FSP (Flexible Software Package)
- GCC ARM Embedded compiler
- J-Link debugger

## Features

- Control individual NeoPixel RGB LEDs
- Set custom colors with RGB values
- Simple and efficient code implementation
- Compatible with various NeoPixel configurations (RGB, RGBW)

## Code Functionality

The main application:
- Initializes the NeoPixel library with proper configuration (1 pixel, Port 2 Pin 1)
- Sets up the RGB LED using the standard GRB color order at 800KHz timing
- Creates a continuous loop displaying a purple color (RGB: 35,0,160) on the NeoPixel
- Uses the Adafruit NeoPixel library ported to Renesas RA platform

## Getting Started

### Setup

1. Clone or download this repository
2. Open the project in Renesas e² studio
3. Connect your Zalmotek RA0E1 Feather board via USB
4. Build the project
5. Flash the firmware to your board

### Configuration

You can change the pin assignment and number of pixels by modifying these constants:

```c
// Which pin on the board is connected to the NeoPixels?
#define PIN (BSP_IO_PORT_02_PIN_01) // Port 02 Pin 01 (D5)

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 1
```

To change the displayed color, modify the RGB values in the setPixelColor call:

```c
// RGB values: red (35), green (0), blue (160) = purple
pixels.setPixelColor(i, pixels.Color(35,0,160));
```

## Project Structure

- `src/hal_entry.cpp`: Main application code including NeoPixel control
- `src/Adafruit_NeoPixel.h`: NeoPixel library ported to Renesas RA platform
- `src/tau_timer.h`: Timer utilities for precise timing
- `src/common_utils.h`: Common utility functions and FSP includes

## License

This project uses code from the Adafruit NeoPixel Library, which is licensed under the GNU Lesser General Public License.

## Additional Resources

- [Zalmotek Website](https://zalmotek.com)
- [Zalmotek RA0E1 Website](https://zalmotek.com/products/RA0E1-Feather-SoM/)
- [Adafruit NeoPixel Library](https://github.com/adafruit/Adafruit_NeoPixel) 
