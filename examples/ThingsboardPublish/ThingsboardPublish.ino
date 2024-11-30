/**
 * ThingsBoard XDev32 MQTT Example
 * Demonstrates how to send telemetry data to ThingsBoard IoT platform
 * using MQTT protocol
 *
 * Hardware: XDev32 Development Board
 * Library Dependencies:
 * - ArduinoJson
 * - PubSubClient
 * - WiFi
 * - XDev32
 */

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <Wire.h>
#include <XDev32.h>

// WiFi credentials
const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASS = "YOUR_PASS";

// MQTT configuration
const char* MQTT_SERVER = "thingsboard.cloud";
const int MQTT_PORT = 1883;
const char* DEVICE_TOKEN = "YOUR_DEVICE_TOKEN";

// MQTT topics
const char* TELEMETRY_TOPIC = "v1/devices/me/telemetry";

// Constants
const unsigned long PUBLISH_INTERVAL = 2000;  // Publish every 1 second
const int JSON_BUFFER_SIZE = 200;

// Global objects
XDev32 XD32;
WiFiClient espClient;
PubSubClient mqttClient(espClient);
unsigned long lastPublishTime = 0;

/**
 * Connects to WiFi network
 * Returns true if connection successful
 */
bool setupWiFi() {
    Serial.print("Connecting to WiFi");
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected");
        Serial.println("IP address: " + WiFi.localIP().toString());
        return true;
    }

    Serial.println("\nWiFi connection failed");
    return false;
}

/**
 * Connects to MQTT broker
 * Retries connection if failed
 */
void connectMQTT() {
    while (!mqttClient.connected()) {
        Serial.print("Attempting MQTT connection...");

        if (mqttClient.connect("ESP32Client", DEVICE_TOKEN, NULL)) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" retrying in 2 seconds");
            delay(2000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("\nStarting ThingsBoard MQTT Example");

    XD32.begin();

    if (!setupWiFi()) {
        Serial.println("Failed to connect to WiFi. System halted.");
        while (1) delay(1000);
    }

    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
}

void loop() {
    // Update XDev32
    XD32.update();

    // Ensure MQTT connection
    if (!mqttClient.connected()) {
        connectMQTT();
    }
    mqttClient.loop();

    // Publish data at regular intervals
    unsigned long currentTime = millis();
    if (currentTime - lastPublishTime >= PUBLISH_INTERVAL) {
        // Create JSON payload
        StaticJsonDocument<JSON_BUFFER_SIZE> doc;
        doc["Lux"] = XD32.get_lux();
        doc["Temperature"] = XD32.get_temp();
        doc["Humidity"] = XD32.get_hum();
        doc["GyroX"] = XD32.get_gyro_x();
        doc["GyroY"] = XD32.get_gyro_y();
        doc["GyroZ"] = XD32.get_gyro_z();

        char jsonBuffer[512];
        serializeJson(doc, jsonBuffer);

        // Publish to MQTT
        if (mqttClient.publish(TELEMETRY_TOPIC, jsonBuffer)) {
            Serial.println("Published: " + String(jsonBuffer));
        } else {
            Serial.println("Failed to publish");
        }

        lastPublishTime = currentTime;
    }
}