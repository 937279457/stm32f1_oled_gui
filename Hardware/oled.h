#ifndef __OLED_H
#define __OLED_H

#include "stm32f10x.h"
#include "myspi.h"
#include "myi2c.h"
#include "delay.h"
#include "mydma.h"
#include "graphics_mono.h"
#include <stdlib.h>

#define OLED_ADDRESS		0x78
#define OLED_BUFFER_SIZE 	1024

typedef enum
{
    OLED_Mode_I2C = 0,
    OLED_Mode_SPI = 1
} OLEDMode_TypeDef;

typedef struct
{
    OLEDMode_TypeDef OLED_Mode;
    uint16_t OLED_MYSPI_Pin_DC;
    uint16_t OLED_MYSPI_Pin_RST;
    MYSPI_InitTypeDef* MYSPI_ConfigStruct;
    MYI2C_InitTypeDef* MYI2C_ConfigStruct;
    FunctionalState OLED_DMACmd;
} OLED_InitTypeDef;

typedef struct
{
    uint8_t* buffer;
    OLED_InitTypeDef* initstruct;
    MYDMA_InitTypeDef* DMA;
} OLED_Handle_t;

extern OLED_Handle_t* oled;

void OLED_Init(OLED_Handle_t* oled_handle);
uint8_t OLED_Display(void);
uint8_t* OLED_GetBuffer(void);
OLED_Handle_t* OLED_SoftwareI2CInit(GPIO_TypeDef* GPIOx, uint16_t SCL, uint16_t SDA);
OLED_Handle_t* OLED_SoftwareSPIInit(GPIO_TypeDef* GPIOx, uint16_t MOSI, uint16_t SCK, uint16_t RST, uint16_t DC, uint16_t CS);
OLED_Handle_t* OLED_I2CInit(I2C_TypeDef* I2Cx, FunctionalState DMACmd);
OLED_Handle_t* OLED_SPIInit(SPI_TypeDef* SPIx, FunctionalState DMACmd, GPIO_TypeDef* GPIOx, uint16_t RST, uint16_t DC, uint16_t CS);
void OLED_SetBrightness(uint8_t brightness);
void OLED_SetColorMode(uint8_t mode);
void OLED_SetHold(FunctionalState holdCmd);
void OLED_SetSleep(FunctionalState SleepCmd);

#endif
