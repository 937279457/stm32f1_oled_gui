#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"
#include "timer.h"

typedef enum
{
    OC1,
    OC2,
    OC3,
    OC4
} PWM_OCChannel_Typedef;

typedef struct
{
    GPIO_TypeDef* PWM_GPIOx;
    uint16_t PWM_Pin;
    PWM_OCChannel_Typedef PWM_OCChannel;
    uint16_t PWM_Pulse;
    TIMER_InitTypeDef TIMER_InitStruct;
} PWM_InitTypeDef;

PWM_InitTypeDef* PWM_Init(TIM_TypeDef* TIMx, uint16_t Period, uint16_t Prescaler, uint16_t Pulse, PWM_OCChannel_Typedef OCChannel);
void PWM_Startup(TIM_TypeDef* TIMx);
void PWM_SetDuty(PWM_InitTypeDef* PWM_InitStruct, float Duty);
int PWM_SetFrequency(PWM_InitTypeDef* PWM_InitStruct, float Frequency, uint8_t mode);

#endif
