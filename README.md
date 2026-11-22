
# ESP32 Shot Timer

[English](README.md) | [Türkçe](README_TR.md)

An ESP32-based, portable shot timer featuring an OLED display, sound detection, and a custom-designed 3D-printed enclosure.

## Overview

This project uses an ESP32 microcontroller and a MAX4466 microphone to detect acoustic events and measure elapsed time after a starting beep.

Results are displayed on a 2.42-inch SSD1309 OLED screen in seconds and milliseconds.

The device is operated using a single START button and is designed for battery-powered use.

## Features

- ESP32 microcontroller
- 2.42-inch, 128x64 SSD1309 SPI OLED display
- MAX4466 analog microphone
- 3-second countdown
- 300 ms starting beep
- Detection of up to 10 shots
- Millisecond-resolution time display
- TIME: elapsed time from the beep to the latest detected shot
- SPLIT: elapsed time from the beep to the first detected shot
- 180 ms detection lockout
- Adjustable microphone sensitivity
- Single-button operation
- Deep Sleep power-saving mode
- 3000 mAh LiPo battery
- Custom 3D-printed enclosure
- Arduino IDE support
- No Wi-Fi required

## Hardware Components

| Quantity | Component |
|---|---|
| 1 | ESP32-WROOM-32 development board |
| 1 | 2.42-inch SSD1309 SPI OLED display |
| 1 | MAX4466 microphone module |
| 1 | FM5324 charging and boost module |
| 1 | 3.7V 3000 mAh LiPo battery |
| 1 | 5V active buzzer |
| 1 | 2N2222 NPN transistor |
| 1 | 1 kΩ resistor |
| 1 | Momentary START button |
| 1 | Custom 3D-printed enclosure |

Detailed component list (Turkish): [Bill of Materials](hardware/malzeme-listesi.md)

## ESP32 Pin Connections

| ESP32 Pin | Function |
|---|---|
| GPIO5 | OLED CS |
| GPIO16 | OLED RESET |
| GPIO17 | OLED DC |
| GPIO18 | OLED SCK |
| GPIO23 | OLED MOSI |
| GPIO25 | Buzzer control |
| GPIO27 | START button |
| GPIO34 | Microphone analog output |

The OLED and MAX4466 modules use 3.3V power. All connected modules must share a common ground.

Detailed wiring instructions (Turkish): [Hardware Connections](hardware/baglanti-semasi.md)

## How It Works

1. Press and release START to begin a 3-second countdown.
2. The buzzer sounds for 300 ms when the countdown finishes.
3. Timing begins when the buzzer is activated.
4. The microphone detects acoustic events above the configured threshold.
5. The OLED displays the latest shot time, first-shot time, and shot count.
6. Press START again to reset the results and begin another countdown.

The firmware records up to 10 detected shots.

### TIME and SPLIT

- **TIME:** Total elapsed time between the starting beep and the most recently detected shot.
- **SPLIT:** Elapsed time between the starting beep and the first detected shot. It remains unchanged after subsequent shots.

### Power Saving

Holding START for 3 seconds activates ESP32 Deep Sleep.

Automatic Deep Sleep is enabled after 5 minutes of inactivity while idle or after the maximum shot count is reached.

**Power note:** The FM5324 boost converter may disable its 5V output at low current. If the output is completely disabled, ESP32 GPIO wake-up alone cannot restore power. The final power wake-up circuit should be verified before replication.

## Arduino IDE Installation

1. Install Arduino IDE.
2. Install the ESP32 board package.
3. Select **ESP32 Dev Module** as the board.
4. Install the **U8g2** library using Arduino IDE Library Manager.
5. Open [ShotTimer.ino](firmware/ShotTimer/ShotTimer.ino) in Arduino IDE.
6. Select the correct serial port.
7. Upload the firmware to the ESP32.

## Microphone Sensitivity

The default detection threshold is:

\`\`\`cpp
int threshold = 120;
\`\`\`

Open the Arduino IDE Serial Monitor at **115200 baud**.

| Command | Function |
|---|---|
| s | Start a new measurement |
| p | Toggle microphone monitoring |
| + | Increase sensitivity |
| - | Decrease sensitivity |

Sensitivity adjustments made through the Serial Monitor are temporary and are not stored after a restart.

## 3D-Printed Enclosure

The enclosure was designed specifically for this project.

All enclosure parts are included in a single 3MF file.

**[ShotTimer.3mf](3d-models/ShotTimer.3mf)**

Open the file in Bambu Studio, OrcaSlicer, or another compatible slicer. Check the print settings before printing.

[3D Printing Documentation](3d-models/README.md) (Turkish)

## Repository Structure

\`\`\`text
ESP32-Shot-Timer/
├── README.md
├── README_TR.md
├── firmware/
│   └── ShotTimer/
│       └── ShotTimer.ino
├── hardware/
│   ├── malzeme-listesi.md
│   └── baglanti-semasi.md
└── 3d-models/
    ├── README.md
    └── ShotTimer.3mf
\`\`\`

## Technical Notes

- Timing is based on the ESP32 microsecond counter.
- Time is displayed with three decimal places.
- A 1 ms display resolution does not guarantee verified 1 ms measurement accuracy.
- Detection depends on microphone gain, threshold, and ambient noise.
- Final power-management wiring and battery runtime should be independently verified.

## License

No open-source license has been selected for this project yet.
