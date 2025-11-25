# Water Treatment Plant with Snowfall and poop emoji on OLED Display 🎄❄️

This project uses an SSD1306 OLED display to create an animation of falling snowflakes, asterisks (to emulate larger snowflakes), poop emojis, and a single falling municipal logo over top of a water treatment plant. The project is implemented using an Arduino-compatible microcontroller and the Adafruit SSD1306 and GFX libraries.

## Features

- A WWTP/WRRF on an OLED screen.
- Snowflakes and glyphs falling from the top of the screen in a loop.
- Real-time animation with adjustable speed.

## Hardware Requirements

1. SSD1306 OLED Display (I2C interface)
2. Arduino-compatible microcontroller (Known to compile on the ESP32 Mini C)
3. Jumper wires and breadboard for connections

## Connections

| OLED Pin |  | ESP32 Mini C Pin |
|----------|--|-------------|
| VCC      |  | 3.3V        |
| GND      |  | GND         |
| SCL      |  | GPIO 4      |
| SDA      |  | GPIO 3      |

## Software Requirements

1. **Arduino IDE**: Download and install from the [official website](https://www.arduino.cc/en/software).
2. **Adafruit SSD1306 Library**: Install via the Arduino Library Manager.
3. **Adafruit GFX Library**: Install via the Arduino Library Manager.
