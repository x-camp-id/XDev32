#include "MPU6050.h"
#include <Wire.h>

int16_t dAcX,dAcY,dAcZ,dTmp,dGyX,dGyY,dGyZ;

MPU6050::MPU6050(void)
{

}

void MPU6050::Init(void)
{
	Wire.begin();

    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x6B); // PWR_MGMT_1 register
    Wire.write(0); // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);

}

void MPU6050::Get(void)
{
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom((int)MPU6050_ADDR,(int)14);// request a total of 14 registers
    dAcX=Wire.read()<<8|Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    dAcY=Wire.read()<<8|Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    dAcZ=Wire.read()<<8|Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    dTmp=Wire.read()<<8|Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    dGyX=Wire.read()<<8|Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    dGyY=Wire.read()<<8|Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    dGyZ=Wire.read()<<8|Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}

int16_t MPU6050::AcX(void)
{
    return dAcX;
}

int16_t MPU6050::AcY(void)
{
    return dAcY;
}

int16_t MPU6050::AcZ(void)
{
    return dAcZ;
}

int16_t MPU6050::Tmp(void)
{
    return dTmp/340.00+36.53;
}

int16_t MPU6050::GyX(void)
{
    return dGyX;
}

int16_t MPU6050::GyY(void)
{
    return dGyY;
}

int16_t MPU6050::GyZ(void)
{
    return dGyZ;
}
