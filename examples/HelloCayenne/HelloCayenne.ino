#define TINY_GSM_DEBUG Serial
#define CAYENNE_PRINT Serial
#define TINY_GSM_MODEM_SIM7000

#define USE_GSM  //! Uncomment will use SIM7000 for GSM communication

#ifdef USE_GSM
#include <CayenneMQTTGSM.h>
#else
#include <CayenneMQTTESP32.h>
#endif
#include <XDev32.h>

#define TEMPERATURE_VIRTUAL_CHANNEL    1
#define HUMIDITY_VIRTUAL_CHANNEL       2
#define LIGHTSENSOR_VIRTUAL_CHANNEL    3
#define USERLED_VIRTUAL_CHANNEL        4
#define RGBLEDR_VIRTUAL_CHANNEL        5
#define RGBLEDG_VIRTUAL_CHANNEL        6
#define RGBLEDB_VIRTUAL_CHANNEL        7

#define UART_BAUD   115200

HardwareSerial  gsmSerial(1);

#ifdef USE_GSM
// GSM connection info.
char apn[] = "internet"; // Access point name. Leave empty if it is not needed.
char gprsLogin[] = ""; // GPRS username. Leave empty if it is not needed.
char gprsPassword[] = ""; // GPRS password. Leave empty if it is not needed.
char pin[] = ""; // SIM pin number. Leave empty if it is not needed.
#else
// WiFi network info.
char ssid[] = "Hotspot";
char wifiPassword[] = "Secure123";
#endif

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "3bd2a710-8d07-11ea-b767-3f1a8f1211ba";
char password[] = "da6a6647c2ccecea767f061a1763ebf2d7cceeae";
char clientID[] = "4f61d300-8d07-11ea-93bf-d33a96695544";

char r,g,b;
int wait = 3000;

void setup()
{
#ifdef USE_GSM
    XD.begin(true,true);
    gsmSerial.begin(UART_BAUD, SERIAL_8N1, XD_SIM7000_TXD, XD_SIM7000_RXD);

    pinMode(XD_SIM7000_PWR, OUTPUT);

    //Launch SIM7000
    digitalWrite(XD_SIM7000_PWR, LOW);
    delay(300);
    digitalWrite(XD_SIM7000_PWR, HIGH);
    delay(200);
    gsmSerial.println("AT");
    Serial.println("SIM7000 Initializing...");
    //Wait for the SIM7000 communication to be normal, and will quit when receiving any byte
    while(wait){
        if (gsmSerial.available()) {
            String r = gsmSerial.readStringUntil('\n');
            //Serial.println(r);
            r.trim();
            if(r == "+CFUN: 1")
            {
              Serial.println("SIM7000 Ready!");
              break;
            }
        }
        delay(10);
        wait--;
    }
#else
    XD.begin(true,false);
#endif

#ifdef USE_GSM
    Cayenne.begin(username, password, clientID, gsmSerial, apn, gprsLogin, gprsPassword, pin);
#else
    Cayenne.begin(username, password, clientID, ssid, wifiPassword);
#endif
}

void loop()
{
    XD.update();
    Cayenne.loop();
}

CAYENNE_IN_DEFAULT()
{
    CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
}

CAYENNE_OUT_DEFAULT()
{
    CAYENNE_LOG("Sending Data..");
}

CAYENNE_IN(USERLED_VIRTUAL_CHANNEL)
{
    digitalWrite(XD_LED, getValue.asInt());
}

CAYENNE_IN(RGBLEDR_VIRTUAL_CHANNEL)
{
    r = getValue.asInt();
    XD.rgbled.SetLED(0,g,r,b);
    XD.rgbled.Write();
}

CAYENNE_IN(RGBLEDG_VIRTUAL_CHANNEL)
{
    g = getValue.asInt();
    XD.rgbled.SetLED(0,g,r,b);
    XD.rgbled.Write();
}

CAYENNE_IN(RGBLEDB_VIRTUAL_CHANNEL)
{
    b = getValue.asInt();
    XD.rgbled.SetLED(0,g,r,b);
    XD.rgbled.Write();
}

CAYENNE_OUT(TEMPERATURE_VIRTUAL_CHANNEL)
{
        float temperature = XD.hdc1080.Temperature();

        //Serial.print("Temperature = ");
        //Serial.print(temperature);
        //Serial.println(" *C");

        Cayenne.celsiusWrite(TEMPERATURE_VIRTUAL_CHANNEL, temperature);
}

CAYENNE_OUT(HUMIDITY_VIRTUAL_CHANNEL)
{
        unsigned char humidity = XD.hdc1080.Humidity();

        //Serial.print("Humidity = ");
        //Serial.print(humidity );
        //Serial.println(" %");

        Cayenne.virtualWrite(HUMIDITY_VIRTUAL_CHANNEL, humidity, TYPE_RELATIVE_HUMIDITY, UNIT_PERCENT);
}

CAYENNE_OUT(LIGHTSENSOR_VIRTUAL_CHANNEL)
{
        long luminosity = XD.veml6030.readLight();

        //Serial.print("Luminosity = ");
        //Serial.print(luminosity);
        //Serial.println(" Lux");

        Cayenne.luxWrite(LIGHTSENSOR_VIRTUAL_CHANNEL, luminosity);
}

//CAYENNE_OUT(USERLED_VIRTUAL_CHANNEL)
//{
//        char val = digitalRead(XD_LED);
//        Cayenne.virtualWrite(USERLED_VIRTUAL_CHANNEL, val, TYPE_DIGITAL_SENSOR, UNIT_DIGITAL);
//}