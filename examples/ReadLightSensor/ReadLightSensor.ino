/**
 * XDev32 Demo Example
 * Demonstrates how the XDev32 Development Board read light sensor in lux unit
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

    Serial.print("Lux: ");
    Serial.println(XD32.get_lux());
    Serial.println();
    delay(1000);
}