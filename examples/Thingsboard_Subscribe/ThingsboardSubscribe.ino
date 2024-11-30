/**
 * ThingsBoard XDev32 MQTT Subscribe Example
 * Demonstrates how to receive RPC commands from ThingsBoard IoT platform
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
const char* RPC_REQUEST_TOPIC = "v1/devices/me/rpc/request/+";

// Constants
const int JSON_BUFFER_SIZE = 200;

// Global objects
XDev32 XD32;
WiFiClient espClient;
PubSubClient mqttClient(espClient);

void handleRPCMessage(char* topic, byte* payload, unsigned int length) {
    // Extract request ID from topic
    String requestId = String(topic).substring(26); // extract ID after "request/"
    
    // Create response topic
    String responseTopic = "v1/devices/me/rpc/response/" + requestId;
    
    // Parse JSON payload
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload, length);
    
    if (error) {
      Serial.println("Failed to parse JSON");
      return;
    }

    // Process the message
    String method = doc["method"];
    Serial.println("Received RPC method: " + method);

    // Prepare response
    String responsePayload;
    
    if (method == "getLedValue") {
      responsePayload = String(XD32.get_led_brightness());
    } else if (method == "setLedValue") {
      int brightness = doc["params"];
      XD32.set_led_brightness(brightness);
      responsePayload = String(brightness);
    }  else {
      responsePayload = "Unsupported method";
    }

    // Send response
    mqttClient.publish(responseTopic.c_str(), responsePayload.c_str());
}

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

void connectMQTT() {
    while (!mqttClient.connected()) {
        Serial.print("Attempting MQTT connection...");

        if (mqttClient.connect("ESP32Client", DEVICE_TOKEN, NULL)) {
            Serial.println("connected");
            // Subscribe to RPC topic after connection
            mqttClient.subscribe(RPC_REQUEST_TOPIC);
            Serial.println("Subscribed to RPC requests");
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
    Serial.println("\nStarting ThingsBoard MQTT Subscribe Example");

    XD32.begin();

    if (!setupWiFi()) {
        Serial.println("Failed to connect to WiFi. System halted.");
        while (1) delay(1000);
    }

    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    mqttClient.setCallback(handleRPCMessage);
}

void loop() {
    // Update XDev32
    XD32.update();

    // Ensure MQTT connection
    if (!mqttClient.connected()) {
        connectMQTT();
    }
    mqttClient.loop();
}