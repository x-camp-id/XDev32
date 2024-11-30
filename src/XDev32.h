//! Copyright (c) Praxistem. All rights reserved.
#ifndef _XDEV32_H_
#define _XDEV32_H_

#if defined(ESP32)

#include <Arduino.h>
#include <Wire.h>

#include "devices/Button.h"
#include "devices/HDC1080.h"
#include "devices/MPU6050.h"
#include "devices/RMT_Pixel.h"
#include "devices/VEML6030.h"

#define XD_LED 32
#define XD_BUTTON 35
#define XD_SIM7000_PWR 5
#define XD_SIM7000_RST 15
#define XD_SIM7000_TXD 16
#define XD_SIM7000_RXD 17
#define XD_SIM7000_RI 18
#define XD_SIM7000_DTR 19
#define XD_WS2812B 14

#define USE_SERIAL Serial

#define DEBOUNCE_MS 10

class XDev32 {
   public:
    XDev32();
    void begin(bool SerialEnable = true, bool Sim7000Enable = false);
    void update();

    Button Btn = Button(XD_BUTTON, true, DEBOUNCE_MS);

    /* ver 1.2 board is not working */
    // HDC1080 hdc1080;

    VEML6030 veml6030 = VEML6030(altAddr);

    MPU6050 mpu6050;

    RMTPixelType rgbled;

   private:
    bool isInited;
};

extern XDev32 XD;
#define xd XD

#else
#error “This library only supports boards with ESP32 processor.”
#endif

#endif