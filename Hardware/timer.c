#include "timer.h"
#include <math.h>

TIM_TypeDef* TIMER_IT_TIMx;

void (*TIMER1_ISR)(void);
void (*TIMER2_ISR)(void);
void (*TIMER3_ISR)(void);
void (*TIMER4_ISR)(void);
void (*TIMER5_ISR)(void);
void (*TIMER6_ISR)(void);
void (*TIMER7_ISR)(void);
void (*TIMER8_ISR)(void);

void TIMER_Init(TIMER_InitTypeDef* TIMER_InitStruct)
{
    TIM_DeInit(TIMER_InitStruct->TIMER_TIMx);
    
    if(TIMER_InitStruct->TIMER_TIMx==TIM1)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    else if(TIMER_InitStruct->TIMER_TIMx==TIM2)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    else if(TIMER_InitStruct->TIMER_TIMx==TIM3)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    else if(TIMER_InitStruct->TIMER_TIMx==TIM4)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    else if(TIMER_InitStruct->TIMER_TIMx==TIM5)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    else if(TIMER_InitStruct->TIMER_TIMx==TIM6)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    else if(TIMER_InitStruct->TIMER_TIMx==TIM7)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    else if(TIMER_InitStruct->TIMER_TIMx==TIM8)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
    
    //TIM_InternalClockConfig(TIMER_InitStruct->TIMER_TIMx);
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = TIMER_InitStruct->TIMER_Period;
    TIM_TimeBaseInitStructure.TIM_Prescaler = TIMER_InitStruct->TIMER_Prescaler;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIMER_InitStruct->TIMER_TIMx, &TIM_TimeBaseInitStructure);
    
    TIM_ClearFlag(TIMER_InitStruct->TIMER_TIMx, TIM_IT_Update);
    
    if(TIMER_InitStruct->TIMER_IT_Cmd)
    {
        TIM_ITConfig(TIMER_InitStruct->TIMER_TIMx, TIM_IT_Update, ENABLE);
        
        NVIC_InitTypeDef NVIC_InitStructure;
        if(TIMER_InitStruct->TIMER_TIMx==TIM2)
            NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
        else if(TIMER_InitStruct->TIMER_TIMx==TIM3)
            NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
        else if(TIMER_InitStruct->TIMER_TIMx==TIM4)
            NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
//		else if(TIMER_InitStruct->TIMER_TIMx==TIM5)
//			NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
//		else if(TIMER_InitStruct->TIMER_TIMx==TIM6)
//			NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
//		else if(TIMER_InitStruct->TIMER_TIMx==TIM7)
//			NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIMER_InitStruct->TIMER_NVIC_PPriority;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIMER_InitStruct->TIMER_NVIC_SPriority;
        NVIC_Init(&NVIC_InitStructure);
    }
    
    TIM_Cmd(TIMER_InitStruct->TIMER_TIMx, DISABLE);
}

void TIMER_Startup(TIM_TypeDef* TIMx)
{
    TIM_Cmd(TIMx, ENABLE);
}

int TIMER_SetFrequency(TIMER_InitTypeDef* TIMER_InitStruct, float Frequency, uint8_t mode) // 模式0：不改变Period值 模式1：最接近设置的频率 返回不为-1：返回新的定时器重装值 返回-1：无法设置频率
{
    float period_new = 0;
    float prescaler = 0;
    
    if(Frequency > 72000000 || Frequency < 0)
        return -1;
    if(mode == 0)
    {
        period_new = TIMER_InitStruct->TIMER_Period;
        prescaler = (int)(72000000 / Frequency / (period_new + 1) - 1);
        if(prescaler < 0 || prescaler > 65536)
            return -1;
        TIM_PrescalerConfig(TIMER_InitStruct->TIMER_TIMx, prescaler, TIM_PSCReloadMode_Update);
    }
    else if(mode == 1)
    {
        period_new = 72000000 / Frequency;
        if(period_new <= 65536)
        {
            if(period_new - (int)period_new > 0.5)
                period_new = period_new + 1;
            prescaler = 1;
        }
        else
        {
            prescaler = (uint16_t)(72000000 / (Frequency * 65536)) + 1;
            if(prescaler <= 65536)
            {
                period_new = 72000000 / (Frequency * prescaler);
            }
        }
        period_new = (uint16_t)(period_new - 1);
        prescaler = prescaler - 1;
        TIM_PrescalerConfig(TIMER_InitStruct->TIMER_TIMx, prescaler, TIM_PSCReloadMode_Update);
        TIM_SetAutoreload(TIMER_InitStruct->TIMER_TIMx, period_new);
    }
    
    TIMER_InitStruct->TIMER_Period = period_new;
    TIMER_InitStruct->TIMER_Prescaler = prescaler;
    
    return period_new;
}

float Timer_GetFrequency(TIM_TypeDef* TIMx)
{
    return 72000000*1.0 / (TIMx->ARR + 1) / (TIMx->PSC + 1);
}

void TIMER_RegisterISR(TIM_TypeDef* TIMx, void (*ISR)(void))
{
    if(TIMx == TIM1)
        TIMER1_ISR = ISR;
    else if(TIMx == TIM2)
        TIMER2_ISR = ISR;
    else if(TIMx == TIM3)
        TIMER3_ISR = ISR;
    else if(TIMx == TIM4)
        TIMER4_ISR = ISR;
    else if(TIMx == TIM5)
        TIMER5_ISR = ISR;
    else if(TIMx == TIM6)
        TIMER6_ISR = ISR;
    else if(TIMx == TIM7)
        TIMER7_ISR = ISR;
    else if(TIMx == TIM8)
        TIMER8_ISR = ISR;
}

void TIM1_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Update))
    {
        TIMER_IT_TIMx = TIM1;
        TIMER1_ISR();
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}

void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update))
    {
        TIMER_IT_TIMx = TIM2;
        TIMER2_ISR();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update))
    {
        TIMER_IT_TIMx = TIM3;
        TIMER3_ISR();
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}

void TIM4_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM4, TIM_IT_Update))
    {
        TIMER_IT_TIMx = TIM4;
        TIMER4_ISR();
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
}

void TIM5_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM5, TIM_IT_Update))
    {
        TIMER_IT_TIMx = TIM5;
        TIMER5_ISR();
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    }
}

void TIM6_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM6, TIM_IT_Update))
    {
        TIMER_IT_TIMx = TIM6;
        TIMER6_ISR();
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    }
}

void TIM7_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM7, TIM_IT_Update))
    {
        TIMER_IT_TIMx = TIM7;
        TIMER7_ISR();
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    }
}

void TIM8_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM8, TIM_IT_Update))
    {
        TIMER_IT_TIMx = TIM8;
        TIMER8_ISR();
        TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
    }
}
