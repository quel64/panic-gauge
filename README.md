# VIX Fear Gauge Terminal
> **Project Stasis (Tier 2)** | A real-time market volatility monitor.

This project is a dedicated hardware terminal that fetches the **VIX (Volatility Index)** via the Alpha Vantage API and renders a dynamic intraday graph on a 3.2" TFT display.
I made this project because having the value of this index I think is of great importance right now because of the recent conflicts of the world. This terminal allows you to have this "fear gauge" always close to you so if it spikes you know that something big is happening.

<img width="1919" height="1199" alt="Screenshot 2026-03-12 140706" src="https://github.com/user-attachments/assets/a9dd3328-afd6-4182-9cc1-f29a4f4c43a3" />
<img width="1919" height="1199" alt="Screenshot 2026-03-12 140719" src="https://github.com/user-attachments/assets/bd67a1dd-997f-4190-a297-951358856f2e" />

## Features
- **Live Tracking:** Real-time updates via Wi-Fi.
- **Dynamic Scaling:** The graph auto-zooms to show movement even during low volatility.
- **Dual Themes:** Swap between Dark Mode and Light Mode with a physical button.
- **Timeframe Toggle:** Switch between 1-minute and 5-minute data resolution.

## Setup & Configuration
1. **Library Install:** Install `TFT_eSPI` and `ArduinoJson` in your Arduino IDE.
2. **Driver Config:** - Navigate to `Arduino/libraries/TFT_eSPI/User_Setup.h`.
   - Ensure the driver is set to `#define ILI9341_DRIVER`.
   - Set pins: `MOSI: 4`, `SCLK: 5`, `CS: 7`, `DC: 6`, `RST: 10`.
3. **API Key:** Get a free key from [Alpha Vantage](https://www.alphavantage.co/support/#api-key).
4. **Firmware:** Open `firmware_vixterminal.ino`, enter your Wi-Fi credentials and API Key, then upload.

## Button Usage
- **Top Button (GPIO 2):** Swaps the data timeframe (1min / 5min).
- **Bottom Button (GPIO 3):** Toggles between Dark and Light UI themes.

## Design
The case features a minimalist **20-degree slant** optimized for desktop viewing. It is 3D-printed in two parts (main body and bottom plate) and requires no screws—the internal components have to be secured via some small drops of adhesive on the backplate.

# Wiring diagram
Component -	Pin	- Connects To

Power -	USB 5V - AMS1117 VIN

Power	- AMS1117 VOUT - ESP32-C3 3.3V & TFT VCC

Power	- GND	- All Common Grounds

Display	- SCK	- ESP32 GPIO 5

Display	- SDI	- ESP32 GPIO 4

Display	- CS	- ESP32 GPIO 7

Display	- DC	- ESP32 GPIO 6

Display	- RST	- ESP32 GPIO 10

Button - 1 - ESP32 GPIO 2 (Timeframe)

Button - 2 - ESP32 GPIO 3 (Theme)

# Project Bill of Materials (BOM)
| Component | Description | Price | Source |
| :--- | :--- | :--- | :--- |
| **ESP32-C3** | Main MCU (SuperMini) | $2.04 | [Link](https://shorturl.at/nSwdf) |
| **3.2" SPI TFT LCD** | ILI9341 Display | $9.32 | [Link](https://shorturl.at/U1MPf) |
| **Solderable Perfboard** | 4x6cm Prototyping Board | $2.19 | [Link](https://shorturl.at/WMAwI) |
| **AMS1117-3.3V** | Voltage Regulator | $1.26 | [Link](https://shorturl.at/bQWA9) |
| **Rubber Feet** | For bottom of the case | $1.71 | [Link](https://shorturl.at/vf0jC) |
| **10uF Capacitors** | Power stabilization | $2.63 | [Link](https://shorturl.at/ITGra) |
| **Tactile Buttons** | 6x6x5mm (Time/Theme) | $0.00 | (In stock) |
| **Resistor Kit** | 10k Ohm pull-ups | $0.00 | (In stock) |
| **TOTAL** | | **$19.15** | |
