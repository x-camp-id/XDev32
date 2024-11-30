/**
 * XDev32 Demo Example
 * Demonstrates how the XDev32 Development Board read temperature and humidity sensor
 *
 * Hardware: XDev32 Development Board
 * Library Dependencies:
 * - XDev32
 */

#include <XDev32.h>

// Global objects
XDev32 XD32;

void setup() {
    Serial.begin(115200);
    Serial.println("\nStarting XDev32 Demo");
    XD32.begin();
}

void loop() {
    // Update XDev32
    XD32.update();

    Serial.print("Temperature: ");
    Serial.println(XD32.get_temp());
    Serial.print("Humidity: ");
    Serial.println(XD32.get_hum());
    Serial.println();
    delay(1000);
}