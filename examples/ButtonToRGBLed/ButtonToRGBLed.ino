/**
 * XDev32 Demo Example
 * Demonstrates how to use Button to control RGB Led output
 *
 * Hardware: XDev32 Development Board
 * Library Dependencies:
 * - XDev32
 */

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
        delay(500);
    }
    if (menu > 2) { menu = 0;}

    if (menu == 1) {
        r = 255;
        g = 0;
        b = 0;
    } else if (menu == 2) {
        r = 0;
        g = 255;
        b = 0;
    } else {
        r = 0;
        g = 0;
        b = 255;
    }
    XD32.rgbled.SetLED(0, g, r, b);
    XD32.rgbled.Write();
}