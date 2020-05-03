#ifndef __MPU6050_H
#define __MPU6050_H

#include "Arduino.h"

#define MPU6050_ADDR 0x68

class MPU6050
{
public:
	MPU6050(void);
	void Init(void);
	void Get(void);
	int16_t AcX(void);
	int16_t AcY(void);
	int16_t AcZ(void);
	int16_t Tmp(void);
	int16_t GyX(void);
	int16_t GyY(void);
	int16_t GyZ(void);
};

#endif
