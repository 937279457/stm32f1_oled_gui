#ifndef __BUZZER_H
#define __BUZZER_H

#include "stm32f10x.h"
#include "softwaretimer.h"
#include "pwm.h"

typedef struct
{
    PWM_InitTypeDef* pwm;
    SFTTIM_Handle_t* timer;
    FunctionalState state;
} BUZZER_Handle_t;

BUZZER_Handle_t* BUZZER_CreateBuzzer(TIM_TypeDef* TIMx, PWM_OCChannel_Typedef OCChannel, float Frequency, uint32_t duration);
void BUZZER_SetFrequency(BUZZER_Handle_t* Buzzer, float Frequency);
void BUZZER_SetDuration(BUZZER_Handle_t* Buzzer, uint32_t duration);
void BUZZER_Alarm(BUZZER_Handle_t* Buzzer);

#endif
