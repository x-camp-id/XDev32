/**
 * XDev32 Demo Example
 * Demonstrates basic functionality of XDev32 Development Board including:
 * - Button input
 * - RGB LED control
 * - Temperature, humidity and light sensors
 * - Gyroscope readings
 *
 * Hardware: XDev32 Development Board
 * Library Dependencies:
 * - XDev32
 */

#include <Arduino.h>
#include <XDev32.h>

// Global objects
XDev32 XD32;

int r, g, b;
int menu = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("\nStarting XDev32 Demo");
    XD32.begin();
}

void loop() {
    // Update XDev32
    XD32.update();

    if (XD32.Btn.isPressed()) {
        menu++;
        digitalWrite(XD_LED, HIGH);
        delay(250);
        digitalWrite(XD_LED, LOW);
        delay(250);
        digitalWrite(XD_LED, HIGH);
        delay(250);
        digitalWrite(XD_LED, LOW);
        delay(250);
    }

    if (menu > 2) {
        menu = 0;
    }

    if (menu == 1) {
        Serial.print("Tmp:");
        Serial.print(XD32.get_temp());
        Serial.print(", ");
        Serial.print("Hum:");
        Serial.print(XD32.get_hum());
        Serial.print(", ");
        Serial.print("Lux:");
        Serial.print(XD32.get_lux());
        Serial.print(", ");
        Serial.println(" ");
        r = 0;
        g = 0;
        b = 0;
    } else if (menu == 2) {
        Serial.print("GyX:");
        Serial.print(XD32.get_gyro_x());
        Serial.print(", ");
        Serial.print("GyY:");
        Serial.print(XD32.get_gyro_y());
        Serial.print(", ");
        Serial.print("GyZ:");
        Serial.print(XD32.get_gyro_z());
        Serial.println(" ");
        r = 0;
        g = 0;
        b = 0;
    } else {
        r = map(XD32.get_gyro_x(), -32768, 32767, -100, 100);
        g = map(XD32.get_gyro_y(), -32768, 32767, -100, 100);
        b = map(XD32.get_gyro_z(), -32768, 32767, -100, 100);
        if (r < 0) r = 0;
        if (g < 0) g = 0;
        if (b < 0) b = 0;
        Serial.print("B:");
        Serial.print(b);
        Serial.print(", ");
        Serial.print("R:");
        Serial.print(r);
        Serial.print(", ");
        Serial.print("G:");
        Serial.print(g);
        Serial.println(" ");
    }
    XD32.rgbled.SetLED(0, g, r, b);
    XD32.rgbled.Write();
    delay(50);
}