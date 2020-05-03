#include "HDC1080.h"
#include <Wire.h>

float temperature;
unsigned char humidity;

HDC1080::HDC1080(void)
{

}

void HDC1080::Init(Temp_Res Temperature_Resolution_x_bit, Humi_Res Humidity_Resolution_x_bit)
{
	/* Temperature and Humidity are acquired in sequence, Temperature first
	 * Default:   Temperature resolution = 14 bit,
	 *            Humidity resolution = 14 bit
	 */

	/* Set the acquisition mode to measure both temperature and humidity by setting Bit[12] to 1 */
	unsigned int config_reg_value=0x3000; //heater enable
	unsigned char data_send[2];

	if(Temperature_Resolution_x_bit == Temperature_Resolution_11_bit)
	{
		config_reg_value |= (1<<10); //11 bit
	}

	switch(Humidity_Resolution_x_bit)
	{
	case Humidity_Resolution_11_bit:
		config_reg_value|= (1<<8);
		break;
	case Humidity_Resolution_8_bit:
		config_reg_value|= (1<<9);
		break;
	}

	data_send[0]= (config_reg_value>>8);
	data_send[1]= (config_reg_value&0x00ff);

	Wire.begin();

	Wire.beginTransmission(HDC1080_ADDR);
	Wire.write(CONFIGURATION);
	Wire.write(data_send[0]);
	Wire.write(data_send[1]);
	Wire.endTransmission();

}

void HDC1080::Get(void)
{
	Read(&temperature,&humidity);
}

float HDC1080::Temperature(void)
{
    return temperature;
}

unsigned char HDC1080::Humidity(void)
{
    return humidity;
}

void HDC1080::Read(float* tmp, unsigned char* hum)
{
	unsigned char receive_data[4];
	unsigned int temp=0,humi=0;
	
	Wire.beginTransmission(HDC1080_ADDR);
	Wire.write(TEMPERATURE);
	Wire.endTransmission();
	
	delay(14);

	Wire.requestFrom((int)HDC1080_ADDR,(int)4);

	for(unsigned char i=0;i<4;i++)
	{
		receive_data[i] = Wire.read();
	}

	temp=((receive_data[0]<<8)|receive_data[1]);
	humi=((receive_data[2]<<8)|receive_data[3]);

	*tmp=((temp/65536.0)*165.0)-40.0;
	*hum=(uint8_t)((humi/65536.0)*100.0);
}