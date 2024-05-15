#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"
#include "softwaretimer.h"

typedef struct
{
    GPIO_TypeDef* GPIOx;
    uint16_t Pin;
    uint8_t DetectLevel;
    uint8_t keyflag;
    uint8_t pressflag;
    SFTTIM_Handle_t* timer;
} Key_Handle_t;

typedef struct
{
    GPIO_TypeDef* Key_GPIOx;
    uint16_t Key_Pin;
    uint8_t Key_Delevel;
    uint8_t Key_NVIC_PPriority;
    uint8_t Key_NVIC_SPriority;
} Key_EXTIInitTypeDef;

Key_Handle_t* Key_Init(GPIO_TypeDef* GPIOx, uint16_t Pin, uint8_t DetectLevel);
uint8_t Key_Scan(Key_Handle_t* key, uint32_t hold_ms);
void Key_EXTIInit(Key_EXTIInitTypeDef* Key_InitStruct);
uint8_t Key_CheckState(GPIO_TypeDef* GPIOx, uint16_t Pin);

#endif
