#ifndef __MPU6050_H
#define __MPU6050_H

#include "stm32f10x.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include <math.h>

#define MPU6050_ADDRESS		    0xD0

#define	MPU6050_SMPLRT_DIV		0x19
#define	MPU6050_CONFIG			0x1A
#define	MPU6050_GYRO_CONFIG		0x1B
#define	MPU6050_ACCEL_CONFIG	0x1C

#define	MPU6050_ACCEL_XOUT_H	0x3B
#define	MPU6050_ACCEL_XOUT_L	0x3C
#define	MPU6050_ACCEL_YOUT_H	0x3D
#define	MPU6050_ACCEL_YOUT_L	0x3E
#define	MPU6050_ACCEL_ZOUT_H	0x3F
#define	MPU6050_ACCEL_ZOUT_L	0x40
#define	MPU6050_TEMP_OUT_H		0x41
#define	MPU6050_TEMP_OUT_L		0x42
#define	MPU6050_GYRO_XOUT_H		0x43
#define	MPU6050_GYRO_XOUT_L		0x44
#define	MPU6050_GYRO_YOUT_H		0x45
#define	MPU6050_GYRO_YOUT_L		0x46
#define	MPU6050_GYRO_ZOUT_H		0x47
#define	MPU6050_GYRO_ZOUT_L		0x48

#define	MPU6050_PWR_MGMT_1		0x6B
#define	MPU6050_PWR_MGMT_2		0x6C
#define	MPU6050_WHO_AM_I		0x75

#define ACCRANGE                4
#define GRYORANGE               500

typedef enum
{
    X_P, 
    X_N, 
    Y_P, 
    Y_N,
    Z_P,
    Z_N,
} MPU6050_Direction_e;

typedef struct
{
    float AccX; 
    float AccY; 
    float AccZ; 
    float GyroX;
    float GyroY;
    float GyroZ;
} MPU6050_Data_t;

typedef struct
{
    int16_t AccX; 
    int16_t AccY; 
    int16_t AccZ; 
    int16_t GyroX;
    int16_t GyroY;
    int16_t GyroZ;
} MPU6050_RawData_t;

typedef struct
{
    float pitch;
    float roll;
    float yaw;
} MPU6050_CapeEuler_t;

int MPU6050_DMP_GetCapeEuler(MPU6050_CapeEuler_t* cape_euler);
int MPU6050_SoftwareInit(GPIO_TypeDef* GPIOx, uint16_t SCL, uint16_t SDA, FunctionalState DMPCmd);
int MPU6050_I2CInit(I2C_TypeDef* I2Cx, FunctionalState DMPCmd);
void MPU6050_GetRawData(MPU6050_RawData_t* data);
void MPU6050_GetData(MPU6050_Data_t* data);
int MPU6050_DetectRotation(MPU6050_Direction_e direction, MPU6050_Data_t* data, float threshold);
int MPU6050_DetectShake(MPU6050_Direction_e direction, MPU6050_Data_t* data, float threshold);
int MPU6050_DetectDirection(MPU6050_Direction_e direction, MPU6050_Data_t* data, float threshold1, float threshold2);
int MPU6050_DetectUp(MPU6050_Data_t* data, float threshold_x, float threshold_y);
int MPU6050_DetectDown(MPU6050_Data_t* data, float threshold_x, float threshold_y);
int MPU6050_DetectRight(MPU6050_Data_t* data, float threshold_y, float threshold_x);
int MPU6050_DetectLeft(MPU6050_Data_t* data, float threshold_y, float threshold_x);
int MPU6050_DetectUpShake(MPU6050_Data_t* data, float threshold_x, float threshold_y);
int MPU6050_DetectDownShake(MPU6050_Data_t* data, float threshold_x, float threshold_y);
int MPU6050_DetectRightShake(MPU6050_Data_t* data, float threshold_y, float threshold_x);
int MPU6050_DetectLeftShake(MPU6050_Data_t* data, float threshold_y, float threshold_x);

#endif
