#ifndef __MYEXTI_H
#define __MYEXTI_H

#include "stm32f10x.h"

extern uint32_t MYEXTI_IT_Line;

typedef struct
{
    GPIO_TypeDef* MYEXTI_GPIOx;
    uint16_t MYEXTI_Pin;
    EXTITrigger_TypeDef MYEXTI_Trigger;
    uint8_t MYEXTI_NVIC_PPriority;
    uint8_t MYEXTI_NVIC_SPriority;
} MYEXTI_InitTypeDef;
    
void MYEXTI_Init(MYEXTI_InitTypeDef* MYEXTI_InitStruct);
void MYEXTI_RegisterISR(uint32_t EXTI_Line, void (*ISR)(void));

#endif
