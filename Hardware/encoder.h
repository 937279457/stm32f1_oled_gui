#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h"

void Encoder_Init(TIM_TypeDef* TIMx);
int Encoder_GetCnt(TIM_TypeDef* TIMx);
int Encoder_GetCntDiv4(TIM_TypeDef* TIMx);
int Encoder_DetectCntState(TIM_TypeDef* TIMx);

#endif
