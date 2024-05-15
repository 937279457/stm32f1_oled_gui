#include "encoder.h"
#include "timer.h"

void Encoder_Init(TIM_TypeDef* TIMx)
{
    // 需提前开启时钟后才能设置成编码器模式
    TIMER_InitTypeDef TIMER_InitStructure;
    TIMER_InitStructure.TIMER_IT_Cmd = DISABLE;
    TIMER_InitStructure.TIMER_Period = 65536 - 1;
    TIMER_InitStructure.TIMER_Prescaler = 1 - 1;
    TIMER_InitStructure.TIMER_TIMx = TIMx;
    TIMER_Init(&TIMER_InitStructure);
    
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICFilter = 0xf;
    TIM_ICInit(TIMx, &TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICFilter = 0xf;
    TIM_ICInit(TIMx, &TIM_ICInitStructure);	
    
    TIM_EncoderInterfaceConfig(TIMx, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
     
    TIMER_Startup(TIMER_InitStructure.TIMER_TIMx);
}

int Encoder_GetCnt(TIM_TypeDef* TIMx)
{
    return (int16_t)TIM_GetCounter(TIMx);
}

int Encoder_GetCntDiv4(TIM_TypeDef* TIMx)
{
    return Encoder_GetCnt(TIMx) / 4;
}

int Encoder_DetectCntState(TIM_TypeDef* TIMx)
{
    static int cnt = 0;
    static int cnt_last = 0;
    cnt_last = cnt;
    cnt = Encoder_GetCntDiv4(TIMx);
    if(cnt > cnt_last)
        return 1;
    else if(cnt < cnt_last)
        return -1;
    return 0;
}
