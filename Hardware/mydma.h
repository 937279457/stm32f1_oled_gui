#ifndef __MYDMA_H
#define __MYDMA_H

#include "stm32f10x.h"

typedef struct
{
    DMA_Channel_TypeDef* MYDMA_DMAy_Channelx;
    uint32_t MYDMA_PeripheralBaseAddr;
    uint32_t MYDMA_MemoryBaseAddr;
    uint32_t MYDMA_PeripheralInc;
    uint32_t MYDMA_MemoryInc;
    uint32_t MYDMA_PeripheralDataSize;
    uint32_t MYDMA_MemoryDataSize;
    uint32_t MYDMA_DIR;
    uint32_t MYDMA_Mode;
    uint32_t MYDMA_BufferSize;
    uint8_t MYDMA_NVIC_PPriority;
    uint8_t MYDMA_NVIC_SPriority;
    FunctionalState MYDMA_IT_Cmd;
} MYDMA_InitTypeDef;

void MYDMA_Init(MYDMA_InitTypeDef* MYDMA_InitStruct);
void MYDMA_Transfer(DMA_Channel_TypeDef* DMAy_Channelx, uint16_t BufferSize);
void MYDMA_SetIRQHandler(DMA_Channel_TypeDef* DMAy_Channelx, void (*IRQHandler)(void));

#endif
