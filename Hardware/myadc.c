#include "MyADC.h"

void MyADC_Init(MyADC_InitTypeDef* MyADC_InitStruct)
{
    if(MyADC_InitStruct->MyADC_GPIOx==GPIOA)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    else if(MyADC_InitStruct->MyADC_GPIOx==GPIOB)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    if(MyADC_InitStruct->MyADC_ADCx==ADC1)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    else if(MyADC_InitStruct->MyADC_ADCx==ADC2)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = MyADC_InitStruct->MyADC_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MyADC_InitStruct->MyADC_GPIOx, &GPIO_InitStructure);
    
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_ContinuousConvMode = MyADC_InitStruct->MyADC_ContinuousConvMode;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = MyADC_InitStruct->MyADC_ExternalTrigConv;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_NbrOfChannel = MyADC_InitStruct->MyADC_NbrOfChannel;
    ADC_InitStructure.ADC_ScanConvMode = MyADC_InitStruct->MyADC_ScanConvMode;
    ADC_Init(MyADC_InitStruct->MyADC_ADCx, &ADC_InitStructure);
    
    ADC_Cmd(MyADC_InitStruct->MyADC_ADCx, ENABLE);
    
    ADC_ResetCalibration(MyADC_InitStruct->MyADC_ADCx);
    while(ADC_GetResetCalibrationStatus(MyADC_InitStruct->MyADC_ADCx));
    ADC_StartCalibration(MyADC_InitStruct->MyADC_ADCx);
    while(ADC_GetCalibrationStatus(MyADC_InitStruct->MyADC_ADCx));
}

void MyADC_SetChannel(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)
{
    ADC_RegularChannelConfig(ADCx, ADC_Channel, Rank, ADC_SampleTime);
}

void MyADC_SoftwareStart(ADC_TypeDef* ADCx)
{
    ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}

uint16_t MyADC_GetValue(ADC_TypeDef* ADCx)
{
    if((ADCx->CR2 & (1<<1))!=1)// 判断是否为连续转换
        while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC));
    return ADC_GetConversionValue(ADCx);
}

float MyADC_Convert(uint16_t OriginalValue, float MaxValue)
{
    return (float)OriginalValue/4095*MaxValue;
}
