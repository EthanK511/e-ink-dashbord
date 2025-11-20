# ESP32 + Waveshare 3.7" E-Paper Information Display  
Low-power time, date, weather, and to-do dashboard using the Waveshare 3.7-inch 4-color e-paper and an ESP32 development board.

---

## Overview
This project creates a minimalist, ultra-low-power information display that updates every 15 minutes (or any interval you choose).  
The ESP32 wakes from deep sleep, fetches time and weather, renders text using the Waveshare `Paint` API, updates the e-paper panel, and returns to deep sleep.

The display is clean, readable, and optimized for the limited power draw of E-Ink technology.

---

## Hardware

### Required Components
- **ESP32 Development Board**  
  (AITRIP CP2102 Type-C ESP-WROOM-32 or equivalent)
- **Waveshare 3.7" 4-Color E-Paper Display (416×240)**  
  SPI interface, four-color (Black/White/Red/Yellow)
- **Jumper wires**
- **Optional:** Li-ion battery + TP4056 charger for portable use

---

## Wiring

| E-Paper Pin | ESP32 Pin | Function |
|-------------|-----------|----------|
| VCC         | 3V3       | Power input |
| GND         | GND       | Ground |
| DIN (MOSI)  | GPIO 23   | SPI MOSI |
| CLK (SCK)   | GPIO 18   | SPI clock |
| CS          | GPIO 5    | Chip select |
| DC          | GPIO 17   | Data/command |
| RST         | GPIO 16   | Reset line |
| BUSY        | GPIO 4    | Busy status |
| **PWR**     | **3V3**   | Display power enable (tie HIGH) |

### Optional
If you want software control of the display’s power:
- Connect **PWR → GPIO 15**  
- Set HIGH before drawing, LOW before deep sleep.

---

## Software Setup

### 1. Arduino IDE Libraries

Install the following:

- **Waveshare e-Paper Library**  
  Download from: https://github.com/waveshareteam/e-Paper  
  Copy `lib/e-Paper/Arduino/` to `Documents/Arduino/libraries/`.

- **ArduinoJson** (via Library Manager)
- **NTPClient** (via Library Manager)

Built-in libraries used:
- `WiFi.h`
- `HTTPClient.h`

---

## Folder Structure

