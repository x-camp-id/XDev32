// Copyright (c) Praxistem. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "XDev32.h"

XDev32::XDev32() : isInited(0) {
}

void XDev32::begin(bool SerialEnable, bool Sim7000Enable) {
    //! Correct init once
    if (isInited)
        return;
    else
        isInited = true;

    //! UART
    if (SerialEnable) {
        Serial.flush();
        delay(50);
        Serial.println("X-Dev32 Initializing...");
    }

    if (veml6030.begin())
        Serial.println("VEML6030 Ready!");
    else
        Serial.println("Could not communicate with the VEML6030!");
    veml6030.setGain(.125);
    veml6030.setIntegTime(100);

    mpu6050.Init();
    Serial.println("MPU6050 Ready!");

    rgbled.Begin(XD_WS2812B, 1, RMT_PIXEL_RGB);
    delay(25);
    rgbled.FillLED(0, 1, 0, 0, 0);
    rgbled.Write();

    pinMode(XD_LED, OUTPUT);
    digitalWrite(XD_LED, LOW);

    if (Sim7000Enable) {
        pinMode(XD_SIM7000_RST, OUTPUT);
        // digitalWrite(XD_SIM7000_RST, LOW);
        // delay(100);
        digitalWrite(XD_SIM7000_RST, HIGH);
        delay(200);
    } else {
        pinMode(XD_SIM7000_RST, OUTPUT);
        digitalWrite(XD_SIM7000_RST, LOW);
        delay(200);
    }
}

void XDev32::update() {
    Btn.read();
    mpu6050.Get();
}

uint32_t XDev32::get_lux() {
	return veml6030.readLight();
}

float XDev32::get_temp() {
    return mpu6050.Tmp() - 10;
}

int XDev32::get_hum() {
	return 100 - (1.5 * mpu6050.Tmp());
}

float XDev32::get_gyro_x() {
	float value = mpu6050.AcY() / 16384.0;
	value = constrain(value, -1.0, 1.0);
	return asin(value) * 180.0 / PI;
}

float XDev32::get_gyro_y() {
	float value = mpu6050.AcX() / 16384.0;
	value = constrain(value, -1.0, 1.0);
	return asin(value) * 180.0 / PI;
}

float XDev32::get_gyro_z() {
	float value = mpu6050.AcZ() / 16384.0; 
	value = constrain(value, -1.0, 1.0);
	return asin(value) * 180.0 / PI;
}

int XDev32::get_led_brightness() {
	return led_brightness;
}

void XDev32::set_led_brightness(int brightness) {
	led_brightness = brightness;
	rgbled.SetLED(0, led_brightness * 255 / 100, led_brightness * 255 / 100, led_brightness * 255 / 100);
	rgbled.Write();
}