# VIX Fear Gauge Terminal
> **Project Stasis (Tier 2)** | Built by quel64

A minimalist hardware terminal that visualizes market volatility (VIX) in real-time. 

### Tech Stack
- **Hardware:** ESP32-C3, 3.2" ILI9341 TFT Display
- **Firmware:** C++ (Arduino/TFT_eSPI)
- **Data:** Alpha Vantage API
- **Case:** Custom 3D-printed "L-Slant" enclosure

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
