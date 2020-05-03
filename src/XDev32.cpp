// Copyright (c) Praxistem. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "XDev32.h"

XDev32::XDev32():isInited(0) {

}

void XDev32::begin(bool SerialEnable, bool Sim7000Enable){
	
	//! Correct init once
	if (isInited) return;
	else isInited = true;

	//! UART
	if (SerialEnable) {
		Serial.begin(115200);
		Serial.flush();
		delay(50);
		Serial.println("X-Dev32 Initializing...");
	}

    hdc1080.Init(Temperature_Resolution_14_bit,Humidity_Resolution_14_bit);
    Serial.println("HDC1080 Ready!"); 

    if(veml6030.begin())
    Serial.println("VEML6030 Ready!"); 
    else
    Serial.println("Could not communicate with the VEML6030!");
	veml6030.setGain(.125);
	veml6030.setIntegTime(100);

    mpu6050.Init();
    Serial.println("MPU6050 Ready!"); 

	rgbled.Begin(XD_WS2812B, 1, RMT_PIXEL_RGB);
	delay(25);
	rgbled.FillLED(0,1,0,0,0);
    rgbled.Write();

	pinMode(XD_LED, OUTPUT);
	digitalWrite(XD_LED, LOW);

	if (Sim7000Enable) {
		pinMode(XD_SIM7000_RST, OUTPUT);
  	    //digitalWrite(XD_SIM7000_RST, LOW);
  		//delay(100);
  		digitalWrite(XD_SIM7000_RST, HIGH);
  		delay(200);
	}else{
		pinMode(XD_SIM7000_RST, OUTPUT);
  	    digitalWrite(XD_SIM7000_RST, LOW);
  	    delay(200);
	}

}

void XDev32::update() {
	XD.Btn.read();
	XD.hdc1080.Get();
	XD.mpu6050.Get();
}

XDev32 XD;