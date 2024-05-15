#include "ic.h"
#include "timer.h"

static void IC_ChangePrescaler(void)
{
    if(TIMER_IT_TIMx==TIM1)
    {
        if(!TIM_GetITStatus(TIM1, TIM_IT_Trigger))
        {
            TIM1->PSC++;	
        }
        else
        {
            if(TIM1->CCMR1==0&&TIM1->PSC>0)
                TIM1->PSC--;
            TIM_ClearITPendingBit(TIM1, TIM_IT_Trigger);
        }
    }
    else if(TIMER_IT_TIMx==TIM2)
    {
        if(!TIM_GetITStatus(TIM2, TIM_IT_Trigger))
        {
            TIM2->PSC++;	
        }
        else
        {
            if(TIM2->CCMR1==0&&TIM2->PSC>0)
                TIM2->PSC--;
            TIM_ClearITPendingBit(TIM2, TIM_IT_Trigger);
        }
    }
    else if(TIMER_IT_TIMx==TIM3)
    {
        if(!TIM_GetITStatus(TIM3, TIM_IT_Trigger))
        {
            TIM3->PSC++;	
        }
        else
        {
            if(TIM3->CCMR1==0&&TIM3->PSC>0)
                TIM3->PSC--;
            TIM_ClearITPendingBit(TIM3, TIM_IT_Trigger);
        }
    }
    else if(TIMER_IT_TIMx==TIM4)
    {
        if(!TIM_GetITStatus(TIM4, TIM_IT_Trigger))
        {
            TIM4->PSC++;	
        }
        else
        {
            if(TIM4->CCMR1==0&&TIM4->PSC>0)
                TIM4->PSC--;
            TIM_ClearITPendingBit(TIM4, TIM_IT_Trigger);
        }
    }
    else if(TIMER_IT_TIMx==TIM5)
    {
        if(!TIM_GetITStatus(TIM5, TIM_IT_Trigger))
        {
            TIM5->PSC++;	
        }
        else
        {
            if(TIM5->CCMR1==0&&TIM5->PSC>0)
                TIM5->PSC--;
            TIM_ClearITPendingBit(TIM5, TIM_IT_Trigger);
        }
    }
    else if(TIMER_IT_TIMx==TIM8)
    {
        if(!TIM_GetITStatus(TIM8, TIM_IT_Trigger))
        {
            TIM8->PSC++;	
        }
        else
        {
            if(TIM8->CCMR1==0&&TIM8->PSC>0)
                TIM8->PSC--;
            TIM_ClearITPendingBit(TIM8, TIM_IT_Trigger);
        }
    }
}

void IC_Init(IC_InitTypeDef* IC_InitStruct)
{
    TIMER_InitTypeDef TIMER_InitStructure;
    TIMER_InitStructure.TIMER_IT_Cmd = ENABLE;
    TIMER_InitStructure.TIMER_NVIC_PPriority = IC_InitStruct->IC_NVIC_PPriority;
    TIMER_InitStructure.TIMER_NVIC_SPriority = IC_InitStruct->IC_NVIC_SPriority;
    TIMER_InitStructure.TIMER_Period = 65536-1;
    TIMER_InitStructure.TIMER_Prescaler = 1-1;
    TIMER_InitStructure.TIMER_TIMx = IC_InitStruct->IC_TIMx;
    TIMER_Init(&TIMER_InitStructure);
    
    TIMER_RegisterISR(IC_InitStruct->IC_TIMx, IC_ChangePrescaler);
    
    TIM_ITConfig(IC_InitStruct->IC_TIMx, TIM_IT_Trigger, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = IC_InitStruct->IC_Pin;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(IC_InitStruct->IC_GPIOx, &GPIO_InitStruct);
    
    TIM_ICInitTypeDef TIM_ICInitStruct;
    TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStruct.TIM_ICFilter = 0x0;
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInit(IC_InitStruct->IC_TIMx, &TIM_ICInitStruct);
    
    TIM_PWMIConfig(IC_InitStruct->IC_TIMx, &TIM_ICInitStruct);
    
    TIM_SelectInputTrigger(IC_InitStruct->IC_TIMx, TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(IC_InitStruct->IC_TIMx, TIM_SlaveMode_Reset);

    TIM_Cmd(IC_InitStruct->IC_TIMx, ENABLE);
}

uint32_t IC_GetFrequency(TIM_TypeDef* TIMx)
{
    return 72000000/(TIMx->PSC+1)/TIMx->CCR1;
}

uint32_t IC_GetDuty(TIM_TypeDef* TIMx)
{
    return 100*(TIMx->CCR2+1)/TIMx->CCR1;
}
