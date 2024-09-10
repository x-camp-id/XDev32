#include <XDev32.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

//#define USE_GSM  //! Uncomment will use SIM7000
//#define USE_NB  //! Uncomment will use NB network

#ifdef USE_GSM
#define TINY_GSM_MODEM_SIM7000
#define UART_BAUD      115200
#include <TinyGsmClient.h>
#else
#include <WiFi.h>
#endif

#define MQTT_HOST      "mqtt.flexiot.xl.co.id"
#define MQTT_PORT      1883
#define MQTT_USERNAME  "xcamp-xdev32-1_4439"
#define MQTT_PASSWORD  "1588879557_4439"
#define MQTT_SERIAL    "5282310950434754"

#define MQTT_INTERVAL  1000

String CLIENT_ID = "XDev32-V1";
String PUB_TOPIC = "xcamp/xdev32/1/common"; //Publishing Topic
String SUB_TOPIC = "+/" + String(MQTT_SERIAL) + "/xcamp/xdev32/1/sub"; //Subscription Topic
String RES_TOPIC = "/" + String(MQTT_SERIAL) + "/xcamp/xdev32/1/pub";  //Subscription Respose Topic

#ifdef USE_GSM
#ifdef USE_NB
const char *GSM_APN = "internet";
#else
const char *GSM_APN = "internet";
#endif
const char *GSM_USER = " ";
const char *GSM_PASS = " ";
#else
const char *WIFI_SSID = "Hotspot";
const char *WIFI_PASS = "Secure123";
#endif
char getmsg[100];
char setmsg[100];
char rLed,gLed,bLed;

#ifdef USE_GSM
HardwareSerial SerialSIM(1);
TinyGsm modem(SerialSIM);
TinyGsmClient client(modem);
#else
WiFiClient client;
#endif
PubSubClient mqtt(client);

double round2(double value) {
   return (int)(value * 100 + 0.5) / 100.0;
}

void setup() {
#ifdef USE_GSM
  XD.begin(true,true);
#else
  XD.begin(true,false);
#endif
  pinMode(XD_LED, OUTPUT);
}

void loop(){
  XD.update();
  if (mqtt.connected()) {
    mqtt.loop();
  }else {
#ifdef USE_GSM
    connectGSM();
#else
    connectWIFI();
#endif
    Serial.println(F("Connecting MQTT..."));
    delay(500);
    ConnectToMQTT();
    delay(500);
  }
  send_message();
  delay(MQTT_INTERVAL);
}

// Connecting to MQTT
boolean ConnectToMQTT(){
  Serial.print(F("ClientID :"));
  Serial.println(CLIENT_ID);

  if (!mqtt.connect(CLIENT_ID.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.println(F("MQTT fail"));
    return false;
  }

  Serial.println(F("MQTT Connected."));
  mqtt.subscribe(SUB_TOPIC.c_str());
  Serial.print(F("Subscription Topic :"));
  Serial.println(SUB_TOPIC);
  return mqtt.connected();
}

void callBack(char *t, byte *payload, unsigned int l) {
  Serial.println(F("Action message is Received."));

  for (unsigned int i = 0; i < l; i++){
    getmsg[i] = (char)payload[i];
  }
  
  Serial.print(F("Message : "));
  Serial.println(getmsg);

  DynamicJsonDocument action(256);
  deserializeJson(action, getmsg);
    
  if (strcmp(action["action"], "ledwOn") == 0) {
    digitalWrite(XD_LED, HIGH);
  } else if (strcmp(action["action"], "ledwOff") == 0) {
    digitalWrite(XD_LED, LOW);
  } else if (strcmp(action["action"], "ledrAdj") == 0) {
    rLed = (int)action["param"]["rpower"];
    XD.rgbled.SetLED(0,gLed,rLed,bLed);
    XD.rgbled.Write();
  }else if (strcmp(action["action"], "ledgAdj") == 0) {
    gLed = (int)action["param"]["gpower"];
    XD.rgbled.SetLED(0,gLed,rLed,bLed);
    XD.rgbled.Write();
  }else if (strcmp(action["action"], "ledbAdj") == 0) {
    bLed = (int)action["param"]["bpower"];
    XD.rgbled.SetLED(0,gLed,rLed,bLed);
    XD.rgbled.Write();
  } else {
    Serial.println("Command is not recognized");
  }

  delay(10);
  memset(getmsg, 0, sizeof(getmsg)); 
}

void send_message()
{
  String message = "";

  DynamicJsonDocument root(1024);
  root["eventName"] = "sensors";
  root["status"] = "none";
  root["temp"] = serialized(String(XD.hdc1080.Temperature(),2));
  root["humi"] = XD.hdc1080.Humidity();
  root["lumi"] = XD.veml6030.readLight();
  root["mac"] = MQTT_SERIAL;

  serializeJson(root, message);
  
  if (!mqtt.publish(PUB_TOPIC.c_str(), message.c_str())){
    Serial.println(F("Publish Event Failed."));
  }else{
    Serial.println(F("Event Sent."));
    Serial.print(F("Published Message :"));
    Serial.println(message);
  }
}

#ifdef USE_GSM
void connectGSM()
{
  digitalWrite(XD_SIM7000_PWR, LOW);
  delay(300);
  digitalWrite(XD_SIM7000_PWR, HIGH);
  delay(200);

  SerialSIM.begin(UART_BAUD, SERIAL_8N1, XD_SIM7000_TXD, XD_SIM7000_RXD);
#ifdef USE_NB
  modem.setNetworkMode(38);
  modem.setPreferredMode(2);
#else
  modem.setNetworkMode(13);
#endif
  modem.getModemName();
  
  modem.restart();
  delay(1000);
  Serial.println(F("Finding Network..."));
  if (!modem.waitForNetwork()) {
    Serial.println(F("Network Fail"));
    while (true);
  }else {
    Serial.println(F("Network identified."));
    Serial.print(F("Signal Strength : "));
    Serial.println(modem.getSignalQuality());
    if (!modem.gprsConnect(GSM_APN, GSM_USER, GSM_PASS)) {
      Serial.println(F("GPRS Fail"));  
    }else {
      Serial.println(F("GPRS Connected."));
      mqtt.setServer(MQTT_HOST, MQTT_PORT);
      mqtt.setCallback(callBack);
    }
  }
}
#else

void connectWIFI()
{
  delay(100);
  Serial.print("\nConnecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print("-");
  }
  Serial.print("\nConnected to ");
  Serial.println(WIFI_SSID);
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(callBack);
}
#endif
