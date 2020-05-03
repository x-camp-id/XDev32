#include <XDev32.h>

int r, g, b;
int menu = 0;

// the setup routine runs once when XDev32 starts up
void setup(){
  // Initialize the XDev32 object
  XD.begin();
}

// the loop routine runs over and over again forever
void loop() {

  XD.update();

  if(XD.Btn.isPressed()){
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

  if(menu>3){
    menu = 0;
  }

  if(menu==1){
    Serial.print("Tmp:");
    Serial.print(XD.hdc1080.Temperature());
    Serial.print(", ");
    Serial.print("Hum:");
    Serial.print(XD.hdc1080.Humidity());
    Serial.print(", ");
    Serial.print("Lux:");
    Serial.print(XD.veml6030.readLight());
    Serial.print(", ");
    Serial.println(" ");
    r = 0;
    g = 0;
    b = 0;
  }else if(menu==2){
    Serial.print("AcX:");
    Serial.print(XD.mpu6050.AcX());
    Serial.print(", ");
    Serial.print("AcY:");
    Serial.print(XD.mpu6050.AcY());
    Serial.print(", ");
    Serial.print("AcZ:");
    Serial.print(XD.mpu6050.AcZ());
    Serial.print(", ");
    Serial.println(" ");
    r = 0;
    g = 0;
    b = 0;
  }else if(menu==3){
    Serial.print("GyX:");
    Serial.print(XD.mpu6050.GyX());
    Serial.print(", ");
    Serial.print("GyY:");
    Serial.print(XD.mpu6050.GyY());
    Serial.print(", ");
    Serial.print("GyZ:");
    Serial.print(XD.mpu6050.GyZ());
    //Serial.print(", ");
    //Serial.print("Tmp:");
    //Serial.print(XD.mpu6050.GetTmp());
    Serial.println(" ");
    r = 0;
    g = 0;
    b = 0;
  }else{
    r = map(XD.mpu6050.AcX(), -32768, 32767, -100, 100);
    g = map(XD.mpu6050.AcY(), -32768, 32767, -100, 100);
    b = map(XD.mpu6050.AcZ(), -32768, 32767, -100, 100);
    if(r<0)r=0;
    if(g<0)g=0;
    if(b<0)b=0;
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
  XD.rgbled.SetLED(0,g,r,b);
  XD.rgbled.Write();
  delay(50);
}