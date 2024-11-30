# XDev32 Arduino Library

A comprehensive Arduino library for the XDev32 development board based on ESP32. This library provides easy-to-use interfaces for various onboard sensors and components.

## Features

- Button input handling with debounce
- RGB LED control (WS2812B)
- Environmental sensors:
  - VEML6030 ambient light sensor
  - MPU6050 6-axis motion tracking
  - Temperature and humidity monitoring
- WiFi and MQTT connectivity support
- ThingsBoard IoT platform integration

## Installation

### Arduino IDE
1. Download this repository as ZIP
2. Open Arduino IDE
3. Go to Sketch > Include Library > Add .ZIP Library
4. Select the downloaded ZIP file

### PlatformIO
Add this to your `platformio.ini`:
```ini
lib_deps = 
    x-camp-id/XDev32
```