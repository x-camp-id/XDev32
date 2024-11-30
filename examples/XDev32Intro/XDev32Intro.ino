/**
 * This is Introduction Code for XDev32
 * Make sure to always import XDev32 Library
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

    // Display a message to Serial Monitor every 1 sec
    Serial.println("Hello World");
    delay(1000);
    
}