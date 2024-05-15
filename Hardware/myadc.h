#ifndef __MYADC_H
#define __MYADC_H

#include "stm32f10x.h"

typedef struct
{
    ADC_TypeDef* MyADC_ADCx;
    GPIO_TypeDef* MyADC_GPIOx;
    uint16_t MyADC_Pin;
    uint8_t MyADC_NbrOfChannel;
    uint32_t MyADC_ExternalTrigConv;
    FunctionalState MyADC_ContinuousConvMode;
    FunctionalState MyADC_ScanConvMode;
} MyADC_InitTypeDef;

void MyADC_Init(MyADC_InitTypeDef* MyADC_InitStruct);
void MyADC_SetChannel(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);
void MyADC_SoftwareStart(ADC_TypeDef* ADCx);
uint16_t MyADC_GetValue(ADC_TypeDef* ADCx);
float MyADC_Convert(uint16_t OriginalValue, float MaxValue);
    
#endif
