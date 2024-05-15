#ifndef __IC_H
#define __IC_H

#include "stm32f10x.h"

typedef struct
{
    GPIO_TypeDef* IC_GPIOx;
    TIM_TypeDef* IC_TIMx;
    uint16_t IC_Pin;
    uint8_t IC_NVIC_PPriority;
    uint8_t IC_NVIC_SPriority;
} IC_InitTypeDef;

void IC_Init(IC_InitTypeDef* IC_InitStruct);
uint32_t IC_GetFrequency(TIM_TypeDef* TIMx);
uint32_t IC_GetDuty(TIM_TypeDef* TIMx);

#endif
