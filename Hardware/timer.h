#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h"

extern TIM_TypeDef* TIMER_IT_TIMx;

typedef struct
{
    TIM_TypeDef* TIMER_TIMx;
    uint16_t TIMER_Period;
    uint16_t TIMER_Prescaler;
    uint8_t TIMER_NVIC_PPriority;
    uint8_t TIMER_NVIC_SPriority;
    FunctionalState TIMER_IT_Cmd;
} TIMER_InitTypeDef;

void TIMER_Init(TIMER_InitTypeDef* TIM_InitStruct);
void TIMER_Startup(TIM_TypeDef* TIMx);
int TIMER_SetFrequency(TIMER_InitTypeDef* TIMER_InitStruct, float Frequency, uint8_t mode);
float Timer_GetFrequency(TIM_TypeDef* TIMx);
void TIMER_RegisterISR(TIM_TypeDef* TIMx, void (*ISR)(void));

#endif
