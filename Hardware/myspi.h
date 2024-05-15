#ifndef __MYSPI_H
#define __MYSPI_H

#include "stm32f10x.h"

typedef enum
{
    MYSPI_Mode_Software = 1,
    MYSPI_Mode_Hardware = 0
} MYSPIMode_TypeDef;

typedef struct
{
    GPIO_TypeDef* MYSPI_GPIOx;
    SPI_TypeDef* MYSPI_SPIx;
    uint16_t MYSPI_Pin_CS;
    uint16_t MYSPI_Pin_MOSI;
    uint16_t MYSPI_Pin_SCK;
    uint16_t MYSPI_Pin_MISO;
    MYSPIMode_TypeDef MYSPI_Mode;
} MYSPI_InitTypeDef;

void MYSPI_Init(MYSPI_InitTypeDef* MYSPI_InitStruct);
void MYSPI_Start(MYSPI_InitTypeDef* MYSPI_InitStruct);
void MYSPI_Stop(MYSPI_InitTypeDef* MYSPI_InitStruct);
int MYSPI_GetFlagStatus(SPI_TypeDef* SPIx, uint16_t SPI_I2S_FLAG, uint16_t timeout);
uint8_t MYSPI_SwapData(MYSPI_InitTypeDef* MYSPI_InitStruct, uint8_t Data);
void MYSPI_SendData(MYSPI_InitTypeDef* MYSPI_InitStruct, uint8_t* pData, uint32_t Times);

#endif
