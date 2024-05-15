#include "buzzer.h"
#include <stdlib.h>

BUZZER_Handle_t* BUZZER_CreateBuzzer(TIM_TypeDef* TIMx, PWM_OCChannel_Typedef OCChannel, float Frequency, uint32_t duration)
{
    BUZZER_Handle_t* buzzer = malloc(sizeof(BUZZER_Handle_t));
    buzzer->timer = SFTTIM_CreateTimer(duration);
    buzzer->state = DISABLE;
    buzzer->pwm = PWM_Init(TIMx, 0, 0, 0, OCChannel);
    PWM_SetFrequency(buzzer->pwm, Frequency, 1);
    
    return buzzer;
}

void BUZZER_SetFrequency(BUZZER_Handle_t* Buzzer, float Frequency)
{
    PWM_SetFrequency(Buzzer->pwm, Frequency, 1);
}

void BUZZER_SetDuration(BUZZER_Handle_t* Buzzer, uint32_t duration)
{
    Buzzer->timer->reload = duration;
}

void BUZZER_Alarm(BUZZER_Handle_t* Buzzer)
{
    if(Buzzer->state == ENABLE)
        return;
    if(Buzzer->timer->timerstate == DISABLE)
    {
        TIM_Cmd(Buzzer->pwm->TIMER_InitStruct.TIMER_TIMx, ENABLE);
        SFTTIMCmd(Buzzer->timer, ENABLE);
    }
}
