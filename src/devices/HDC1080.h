#ifndef __HDC1080_H
#define __HDC1080_H

#include "Arduino.h"


#define HDC1080_ADDR 0x40

typedef enum 
{
	TEMPERATURE = 0x00,
	HUMIDITY = 0x01,
	CONFIGURATION = 0x02
}HDC1080_REGISTER;

/* Define temperature resolution */
typedef enum
{
	Temperature_Resolution_14_bit = 0,
	Temperature_Resolution_11_bit = 1
}Temp_Res;

/* Define humidity resolution */
typedef enum
{
	Humidity_Resolution_14_bit = 0,
	Humidity_Resolution_11_bit = 1,
	Humidity_Resolution_8_bit =2
}Humi_Res;

class HDC1080
{
public:
	HDC1080(void);
	void Init(Temp_Res Temperature_Resolution_x_bit, Humi_Res Humidity_Resolution_x_bit);
	void Get(void);
	float Temperature(void);
	unsigned char Humidity(void);
private:
	void Read(float* tmp, unsigned char* hum);
};


#endif
