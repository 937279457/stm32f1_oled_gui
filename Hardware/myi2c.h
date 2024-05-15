#ifndef __MYI2C_H
#define __MYI2C_H

#include "stm32f10x.h"

typedef enum
{
    MYI2C_Mode_Hardware,
    MYI2C_Mode_Software
} MYI2C_ModeTypeDef;

typedef struct
{
    GPIO_TypeDef* MYI2C_GPIOx;
    I2C_TypeDef* MYI2C_I2Cx;
    uint16_t MYI2C_Pin_SCL;
    uint16_t MYI2C_Pin_SDA;
    MYI2C_ModeTypeDef MYI2C_Mode;
    uint32_t MYI2C_ClockSpeed;
} MYI2C_InitTypeDef;

void MYI2C_Init(MYI2C_InitTypeDef* MYI2C_InitStruct);
void MYI2C_Start(MYI2C_InitTypeDef* MYI2C_InitStruct);
void MYI2C_Stop(MYI2C_InitTypeDef* MYI2C_InitStruct);
int MYI2C_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT, uint16_t timeout);
void MYI2C_SendAddress(MYI2C_InitTypeDef* MYI2C_InitStruct, uint8_t Address, uint8_t I2C_Direction);
void MYI2C_SendData(MYI2C_InitTypeDef* MYI2C_InitStruct, uint8_t* pData, uint32_t Size);
void MYI2C_ReceiveData(MYI2C_InitTypeDef* MYI2C_InitStruct, uint8_t* pData, uint32_t Size);
void MYI2C_Write(MYI2C_InitTypeDef* MYI2C_InitStruct, uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t* pData, uint32_t Size);
void MYI2C_Read(MYI2C_InitTypeDef* MYI2C_InitStruct, uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t* pData, uint32_t Size);
void MYI2C_WriteCmd(MYI2C_InitTypeDef* MYI2C_InitStruct, uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t Data);
uint8_t MYI2C_ReadData(MYI2C_InitTypeDef* MYI2C_InitStruct, uint8_t DeviceAddress, uint8_t RegisterAddress);

#endif
