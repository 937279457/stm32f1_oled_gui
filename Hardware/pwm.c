#include "pwm.h"
#include "io.h"
#include <stdlib.h>

PWM_InitTypeDef* PWM_Init(TIM_TypeDef* TIMx, uint16_t Period, uint16_t Prescaler, uint16_t Pulse, PWM_OCChannel_Typedef OCChannel)
{
    PWM_InitTypeDef* p = malloc(sizeof(PWM_InitTypeDef));
    p->PWM_OCChannel = OCChannel;
    p->PWM_Pulse = Pulse;
    p->TIMER_InitStruct.TIMER_TIMx = TIMx;
    p->TIMER_InitStruct.TIMER_Period = Period;
    p->TIMER_InitStruct.TIMER_Prescaler = Prescaler;
    
    if(TIMx == TIM1)
    {
        switch(OCChannel)
        {
            case OC1: p->PWM_GPIOx = GPIOA; p->PWM_Pin = GPIO_Pin_8; break;
            case OC2: p->PWM_GPIOx = GPIOA; p->PWM_Pin = GPIO_Pin_9; break;
            case OC3: p->PWM_GPIOx = GPIOA; p->PWM_Pin = GPIO_Pin_10; break;
            case OC4: p->PWM_GPIOx = GPIOA; p->PWM_Pin = GPIO_Pin_11; break;
        }
    }
    else if(TIMx == TIM2)
    {
        switch(OCChannel)
        {
            case OC1: p->PWM_GPIOx = GPIOA; p->PWM_Pin = GPIO_Pin_0; break;
            case OC2: p->PWM_GPIOx = GPIOA; p->PWM_Pin = GPIO_Pin_1; break;
            case OC3: p->PWM_GPIOx = GPIOA; p->PWM_Pin = GPIO_Pin_2; break;
            case OC4: p->PWM_GPIOx = GPIOA; p->PWM_Pin = GPIO_Pin_3; break;
        }
    }
    else if(TIMx == TIM3)
    {
        switch(OCChannel)
        {
            case OC1: p->PWM_GPIOx = GPIOA; p->PWM_Pin = GPIO_Pin_6; break;
            case OC2: p->PWM_GPIOx = GPIOA; p->PWM_Pin = GPIO_Pin_7; break;
            case OC3: p->PWM_GPIOx = GPIOB; p->PWM_Pin = GPIO_Pin_0; break;
            case OC4: p->PWM_GPIOx = GPIOB; p->PWM_Pin = GPIO_Pin_1; break;
        }
    }
    else if(TIMx == TIM4)
    {
        switch(OCChannel)
        {
            case OC1: p->PWM_GPIOx = GPIOB; p->PWM_Pin = GPIO_Pin_6; break;
            case OC2: p->PWM_GPIOx = GPIOB; p->PWM_Pin = GPIO_Pin_7; break;
            case OC3: p->PWM_GPIOx = GPIOB; p->PWM_Pin = GPIO_Pin_8; break;
            case OC4: p->PWM_GPIOx = GPIOB; p->PWM_Pin = GPIO_Pin_9; break;
        }
    }
    
    IO_GPIOInit(p->PWM_GPIOx, p->PWM_Pin, GPIO_Mode_AF_PP);
    
    TIMER_Init(&p->TIMER_InitStruct);
    
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = Pulse;
    
    switch(OCChannel)
    {
        case OC1: TIM_OC1Init(TIMx, &TIM_OCInitStructure); break;
        case OC2: TIM_OC2Init(TIMx, &TIM_OCInitStructure); break;
        case OC3: TIM_OC3Init(TIMx, &TIM_OCInitStructure); break;
        case OC4: TIM_OC4Init(TIMx, &TIM_OCInitStructure); break;
    }
    
    if(TIMx == TIM1 || TIMx == TIM8)
        TIM_CtrlPWMOutputs(TIMx, ENABLE);
    
    return p;
}

void PWM_Startup(TIM_TypeDef* TIMx)
{
    TIM_Cmd(TIMx, ENABLE);
}

void PWM_SetDuty(PWM_InitTypeDef* PWM_InitStruct, float Duty)
{
    PWM_InitStruct->PWM_Pulse = (PWM_InitStruct->TIMER_InitStruct.TIMER_Period + 1) * (Duty / 100);
    if(PWM_InitStruct->PWM_OCChannel == OC1)
        TIM_SetCompare1(PWM_InitStruct->TIMER_InitStruct.TIMER_TIMx, PWM_InitStruct->PWM_Pulse);
    else if(PWM_InitStruct->PWM_OCChannel == OC2)
        TIM_SetCompare2(PWM_InitStruct->TIMER_InitStruct.TIMER_TIMx, PWM_InitStruct->PWM_Pulse);
    else if(PWM_InitStruct->PWM_OCChannel == OC3)
        TIM_SetCompare3(PWM_InitStruct->TIMER_InitStruct.TIMER_TIMx, PWM_InitStruct->PWM_Pulse);
    else if(PWM_InitStruct->PWM_OCChannel == OC4)
        TIM_SetCompare4(PWM_InitStruct->TIMER_InitStruct.TIMER_TIMx, PWM_InitStruct->PWM_Pulse);
}

int PWM_SetFrequency(PWM_InitTypeDef* PWM_InitStruct, float Frequency, uint8_t mode)
{
    int ret = TIMER_SetFrequency(&PWM_InitStruct->TIMER_InitStruct, Frequency, mode);
    if(ret >= 0)
        PWM_SetDuty(PWM_InitStruct, 50);
    return ret;
}
