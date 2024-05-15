#include "myexti.h"

uint32_t MYEXTI_IT_Line;

void (*MYEXTI0_ISR)(void);
void (*MYEXTI1_ISR)(void);
void (*MYEXTI2_ISR)(void);
void (*MYEXTI3_ISR)(void);
void (*MYEXTI4_ISR)(void);
void (*MYEXTI5_ISR)(void);
void (*MYEXTI6_ISR)(void);
void (*MYEXTI7_ISR)(void);
void (*MYEXTI8_ISR)(void);
void (*MYEXTI9_ISR)(void);
void (*MYEXTI10_ISR)(void);
void (*MYEXTI11_ISR)(void);
void (*MYEXTI12_ISR)(void);
void (*MYEXTI13_ISR)(void);
void (*MYEXTI14_ISR)(void);
void (*MYEXTI15_ISR)(void);

void MYEXTI_LineConfig(uint8_t GPIO_PortSource, uint16_t MYEXTI_Pin)
{
    static uint8_t GPIO_PinSource;
    switch(MYEXTI_Pin)
    {
        case GPIO_Pin_0:GPIO_PinSource = GPIO_PinSource0;break;
        case GPIO_Pin_1:GPIO_PinSource = GPIO_PinSource1;break;
        case GPIO_Pin_2:GPIO_PinSource = GPIO_PinSource2;break;
        case GPIO_Pin_3:GPIO_PinSource = GPIO_PinSource3;break;
        case GPIO_Pin_4:GPIO_PinSource = GPIO_PinSource4;break;
        case GPIO_Pin_5:GPIO_PinSource = GPIO_PinSource5;break;
        case GPIO_Pin_6:GPIO_PinSource = GPIO_PinSource6;break;
        case GPIO_Pin_7:GPIO_PinSource = GPIO_PinSource7;break;
        case GPIO_Pin_8:GPIO_PinSource = GPIO_PinSource8;break;
        case GPIO_Pin_9:GPIO_PinSource = GPIO_PinSource9;break;
        case GPIO_Pin_10:GPIO_PinSource = GPIO_PinSource10;break;
        case GPIO_Pin_11:GPIO_PinSource = GPIO_PinSource11;break;
        case GPIO_Pin_12:GPIO_PinSource = GPIO_PinSource12;break;
        case GPIO_Pin_13:GPIO_PinSource = GPIO_PinSource13;break;
        case GPIO_Pin_14:GPIO_PinSource = GPIO_PinSource14;break;
        case GPIO_Pin_15:GPIO_PinSource = GPIO_PinSource15;break;
    }
    GPIO_EXTILineConfig(GPIO_PortSource, GPIO_PinSource);
}

void MYEXTI_Init(MYEXTI_InitTypeDef* MYEXTI_InitStruct)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    if(MYEXTI_InitStruct->MYEXTI_GPIOx==GPIOA)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        MYEXTI_LineConfig(GPIO_PortSourceGPIOA, MYEXTI_InitStruct->MYEXTI_Pin);
    }
    else if(MYEXTI_InitStruct->MYEXTI_GPIOx==GPIOB)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        MYEXTI_LineConfig(GPIO_PortSourceGPIOB, MYEXTI_InitStruct->MYEXTI_Pin);
    }
    else if(MYEXTI_InitStruct->MYEXTI_GPIOx==GPIOC)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
        MYEXTI_LineConfig(GPIO_PortSourceGPIOC, MYEXTI_InitStruct->MYEXTI_Pin);
    }
    else if(MYEXTI_InitStruct->MYEXTI_GPIOx==GPIOD)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
        MYEXTI_LineConfig(GPIO_PortSourceGPIOD, MYEXTI_InitStruct->MYEXTI_Pin);
    }
    else if(MYEXTI_InitStruct->MYEXTI_GPIOx==GPIOE)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
        MYEXTI_LineConfig(GPIO_PortSourceGPIOE, MYEXTI_InitStruct->MYEXTI_Pin);
    }
    else if(MYEXTI_InitStruct->MYEXTI_GPIOx==GPIOF)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
        MYEXTI_LineConfig(GPIO_PortSourceGPIOF, MYEXTI_InitStruct->MYEXTI_Pin);
    }
    else if(MYEXTI_InitStruct->MYEXTI_GPIOx==GPIOG)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
        MYEXTI_LineConfig(GPIO_PortSourceGPIOG, MYEXTI_InitStruct->MYEXTI_Pin);
    }
    
    GPIO_InitTypeDef GPIO_InitStructure;
    if(MYEXTI_InitStruct->MYEXTI_Trigger==EXTI_Trigger_Rising)
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    else if(MYEXTI_InitStruct->MYEXTI_Trigger==EXTI_Trigger_Falling)
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    else if(MYEXTI_InitStruct->MYEXTI_Trigger==EXTI_Trigger_Rising_Falling)
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = MYEXTI_InitStruct->MYEXTI_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MYEXTI_InitStruct->MYEXTI_GPIOx, &GPIO_InitStructure);
    
    EXTI_InitTypeDef EXTI_InitStructure;
    switch(MYEXTI_InitStruct->MYEXTI_Pin)
    {
        case GPIO_Pin_0: EXTI_InitStructure.EXTI_Line = EXTI_Line0; break;
        case GPIO_Pin_1: EXTI_InitStructure.EXTI_Line = EXTI_Line1; break;
        case GPIO_Pin_2: EXTI_InitStructure.EXTI_Line = EXTI_Line2; break;
        case GPIO_Pin_3: EXTI_InitStructure.EXTI_Line = EXTI_Line3; break;
        case GPIO_Pin_4: EXTI_InitStructure.EXTI_Line = EXTI_Line4; break;
        case GPIO_Pin_5: EXTI_InitStructure.EXTI_Line = EXTI_Line5; break;
        case GPIO_Pin_6: EXTI_InitStructure.EXTI_Line = EXTI_Line6; break;
        case GPIO_Pin_7: EXTI_InitStructure.EXTI_Line = EXTI_Line7; break;
        case GPIO_Pin_8: EXTI_InitStructure.EXTI_Line = EXTI_Line8; break;
        case GPIO_Pin_9: EXTI_InitStructure.EXTI_Line = EXTI_Line9; break;
        case GPIO_Pin_10: EXTI_InitStructure.EXTI_Line = EXTI_Line10; break;
        case GPIO_Pin_11: EXTI_InitStructure.EXTI_Line = EXTI_Line11; break;
        case GPIO_Pin_12: EXTI_InitStructure.EXTI_Line = EXTI_Line12; break;
        case GPIO_Pin_13: EXTI_InitStructure.EXTI_Line = EXTI_Line13; break;
        case GPIO_Pin_14: EXTI_InitStructure.EXTI_Line = EXTI_Line14; break;
        case GPIO_Pin_15: EXTI_InitStructure.EXTI_Line = EXTI_Line15; break;
    }
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = MYEXTI_InitStruct->MYEXTI_Trigger;
    EXTI_Init(&EXTI_InitStructure);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    switch(MYEXTI_InitStruct->MYEXTI_Pin)
    {
        case GPIO_Pin_0: NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; break;
        case GPIO_Pin_1: NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn; break;
        case GPIO_Pin_2: NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn; break;
        case GPIO_Pin_3: NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn; break;
        case GPIO_Pin_4: NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn; break;
        case GPIO_Pin_5: 
        case GPIO_Pin_6: 
        case GPIO_Pin_7: 
        case GPIO_Pin_8: 
        case GPIO_Pin_9: NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; break;
        case GPIO_Pin_10: 
        case GPIO_Pin_11: 
        case GPIO_Pin_12: 
        case GPIO_Pin_13: 
        case GPIO_Pin_14: 
        case GPIO_Pin_15: NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; break;
    }
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = MYEXTI_InitStruct->MYEXTI_NVIC_PPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = MYEXTI_InitStruct->MYEXTI_NVIC_SPriority;
    NVIC_Init(&NVIC_InitStructure);
}

void MYEXTI_RegisterISR(uint32_t EXTI_Line, void (*ISR)(void))
{
    switch(EXTI_Line)
    {
        case EXTI_Line0: MYEXTI0_ISR = ISR; break;
        case EXTI_Line1: MYEXTI1_ISR = ISR; break;
        case EXTI_Line2: MYEXTI2_ISR = ISR; break;
        case EXTI_Line3: MYEXTI3_ISR = ISR; break;
        case EXTI_Line4: MYEXTI4_ISR = ISR; break;
        case EXTI_Line5: MYEXTI5_ISR = ISR; break;
        case EXTI_Line6: MYEXTI6_ISR = ISR; break;
        case EXTI_Line7: MYEXTI7_ISR = ISR; break;
        case EXTI_Line8: MYEXTI8_ISR = ISR; break;
        case EXTI_Line9: MYEXTI9_ISR = ISR; break;
        case EXTI_Line10: MYEXTI10_ISR = ISR; break;
        case EXTI_Line11: MYEXTI11_ISR = ISR; break;
        case EXTI_Line12: MYEXTI12_ISR = ISR; break;
        case EXTI_Line13: MYEXTI13_ISR = ISR; break;
        case EXTI_Line14: MYEXTI14_ISR = ISR; break;
        case EXTI_Line15: MYEXTI15_ISR = ISR; break;
    }
}

void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0))
    {
        MYEXTI_IT_Line = EXTI_Line0;
        MYEXTI0_ISR();
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1))
    {
        MYEXTI_IT_Line = EXTI_Line1;
        MYEXTI1_ISR();
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

void EXTI2_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line2))
    {
        MYEXTI_IT_Line = EXTI_Line2;
        MYEXTI2_ISR();
        EXTI_ClearITPendingBit(EXTI_Line2);
    }
}

void EXTI3_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line3))
    {
        MYEXTI_IT_Line = EXTI_Line3;
        MYEXTI3_ISR();
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}

void EXTI4_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line4))
    {
        MYEXTI_IT_Line = EXTI_Line4;
        MYEXTI4_ISR();
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
}

void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line5))
    {
        MYEXTI_IT_Line = EXTI_Line5;
        MYEXTI5_ISR();
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
    else if(EXTI_GetITStatus(EXTI_Line6))
    {
        MYEXTI_IT_Line = EXTI_Line6;
        MYEXTI6_ISR();
        EXTI_ClearITPendingBit(EXTI_Line6);
    }
    else if(EXTI_GetITStatus(EXTI_Line7))
    {
        MYEXTI_IT_Line = EXTI_Line7;
        MYEXTI7_ISR();
        EXTI_ClearITPendingBit(EXTI_Line7);
    }
    else if(EXTI_GetITStatus(EXTI_Line8))
    {
        MYEXTI_IT_Line = EXTI_Line8;
        MYEXTI8_ISR();
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
    else if(EXTI_GetITStatus(EXTI_Line9))
    {
        MYEXTI_IT_Line = EXTI_Line9;
        MYEXTI9_ISR();
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
}

void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line10))
    {
        MYEXTI_IT_Line = EXTI_Line10;
        MYEXTI10_ISR();
        EXTI_ClearITPendingBit(EXTI_Line10);
    }
    else if(EXTI_GetITStatus(EXTI_Line11))
    {
        MYEXTI_IT_Line = EXTI_Line11;
        MYEXTI11_ISR();
        EXTI_ClearITPendingBit(EXTI_Line11);
    }
    else if(EXTI_GetITStatus(EXTI_Line12))
    {
        MYEXTI_IT_Line = EXTI_Line12;
        MYEXTI12_ISR();
        EXTI_ClearITPendingBit(EXTI_Line12);
    }
    else if(EXTI_GetITStatus(EXTI_Line13))
    {
        MYEXTI_IT_Line = EXTI_Line13;
        MYEXTI13_ISR();
        EXTI_ClearITPendingBit(EXTI_Line13);
    }
    else if(EXTI_GetITStatus(EXTI_Line14))
    {
        MYEXTI_IT_Line = EXTI_Line14;
        MYEXTI14_ISR();
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
    else if(EXTI_GetITStatus(EXTI_Line15))
    {
        MYEXTI_IT_Line = EXTI_Line15;
        MYEXTI15_ISR();
        EXTI_ClearITPendingBit(EXTI_Line15);
    }
}
