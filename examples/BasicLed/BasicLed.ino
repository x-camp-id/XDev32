/**
 * XDev32 Demo Example
 * Demonstrates how to turn on / off Built-in LED of XDev32 Development Board
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
    Serial.println("LED On");
    digitalWrite(XD_LED, HIGH);
    delay(500);
    Serial.println("LED Off\n");
    digitalWrite(XD_LED, LOW);
    delay(500);
}
