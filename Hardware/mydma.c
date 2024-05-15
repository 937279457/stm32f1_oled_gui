#include "mydma.h"

void (*MYDMA1C1_ISR)(void);
void (*MYDMA1C2_ISR)(void);
void (*MYDMA1C3_ISR)(void);
void (*MYDMA1C4_ISR)(void);
void (*MYDMA1C5_ISR)(void);
void (*MYDMA1C6_ISR)(void);
void (*MYDMA1C7_ISR)(void);

void MYDMA_Init(MYDMA_InitTypeDef* MYDMA_InitStruct)
{
    DMA_DeInit(MYDMA_InitStruct->MYDMA_DMAy_Channelx);
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_DIR = MYDMA_InitStruct->MYDMA_DIR;
    DMA_InitStructure.DMA_PeripheralBaseAddr = MYDMA_InitStruct->MYDMA_PeripheralBaseAddr;
    DMA_InitStructure.DMA_MemoryBaseAddr = MYDMA_InitStruct->MYDMA_MemoryBaseAddr;
    DMA_InitStructure.DMA_PeripheralInc = MYDMA_InitStruct->MYDMA_PeripheralInc;
    DMA_InitStructure.DMA_MemoryInc = MYDMA_InitStruct->MYDMA_MemoryInc;
    DMA_InitStructure.DMA_PeripheralDataSize = MYDMA_InitStruct->MYDMA_PeripheralDataSize;
    DMA_InitStructure.DMA_MemoryDataSize = MYDMA_InitStruct->MYDMA_MemoryDataSize;
    DMA_InitStructure.DMA_BufferSize = MYDMA_InitStruct->MYDMA_BufferSize;
    DMA_InitStructure.DMA_Mode = MYDMA_InitStruct->MYDMA_Mode;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;// 是否使用软件触发
    DMA_Init(MYDMA_InitStruct->MYDMA_DMAy_Channelx, &DMA_InitStructure);
    
    if(MYDMA_InitStruct->MYDMA_IT_Cmd)
    {
        DMA_ITConfig(MYDMA_InitStruct->MYDMA_DMAy_Channelx, DMA_IT_TC, ENABLE);
        
        NVIC_InitTypeDef NVIC_InitStructure;
        if(MYDMA_InitStruct->MYDMA_DMAy_Channelx==DMA1_Channel1)
            NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
        else if(MYDMA_InitStruct->MYDMA_DMAy_Channelx==DMA1_Channel2)
            NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
        else if(MYDMA_InitStruct->MYDMA_DMAy_Channelx==DMA1_Channel3)
            NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
        else if(MYDMA_InitStruct->MYDMA_DMAy_Channelx==DMA1_Channel4)
            NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
        else if(MYDMA_InitStruct->MYDMA_DMAy_Channelx==DMA1_Channel5)
            NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
        else if(MYDMA_InitStruct->MYDMA_DMAy_Channelx==DMA1_Channel6)
            NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
        else if(MYDMA_InitStruct->MYDMA_DMAy_Channelx==DMA1_Channel7)
            NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = MYDMA_InitStruct->MYDMA_NVIC_PPriority;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = MYDMA_InitStruct->MYDMA_NVIC_SPriority;
        NVIC_Init(&NVIC_InitStructure);
    }
    DMA_Cmd(MYDMA_InitStruct->MYDMA_DMAy_Channelx, ENABLE);
}

void MYDMA_Transfer(DMA_Channel_TypeDef* DMAy_Channelx, uint16_t BufferSize)
{
    DMA_Cmd(DMAy_Channelx, DISABLE);
    DMA_SetCurrDataCounter(DMAy_Channelx, BufferSize);
    DMA_Cmd(DMAy_Channelx, ENABLE);
}

void MYDMA_SetIRQHandler(DMA_Channel_TypeDef* DMAy_Channelx, void (*ISR)(void))
{
    if(DMAy_Channelx==DMA1_Channel1)
        MYDMA1C1_ISR = ISR;
    else if(DMAy_Channelx==DMA1_Channel2)
        MYDMA1C2_ISR = ISR;
    else if(DMAy_Channelx==DMA1_Channel3)
        MYDMA1C3_ISR = ISR;
    else if(DMAy_Channelx==DMA1_Channel4)
        MYDMA1C4_ISR = ISR;
    else if(DMAy_Channelx==DMA1_Channel5)
        MYDMA1C5_ISR = ISR;
    else if(DMAy_Channelx==DMA1_Channel6)
        MYDMA1C6_ISR = ISR;
    else if(DMAy_Channelx==DMA1_Channel7)
        MYDMA1C7_ISR = ISR;
}

void DMA1_Channel1_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC1))
    {
        MYDMA1C1_ISR();
        DMA_ClearITPendingBit(DMA1_IT_TC1);
    }
}

void DMA1_Channel2_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC2))
    {
        MYDMA1C2_ISR();
        DMA_ClearITPendingBit(DMA1_IT_TC2);
    }
}

void DMA1_Channel3_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC3))
    {
        MYDMA1C3_ISR();
        DMA_ClearITPendingBit(DMA1_IT_TC3);
    }	
}

void DMA1_Channel4_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC4))
    {
        MYDMA1C4_ISR();
        DMA_ClearITPendingBit(DMA1_IT_TC4);
    }	
}

void DMA1_Channel5_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC5))
    {
        MYDMA1C5_ISR();
        DMA_ClearITPendingBit(DMA1_IT_TC5);
    }	
}

void DMA1_Channel6_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC6))
    {
        MYDMA1C6_ISR();
        DMA_ClearITPendingBit(DMA1_IT_TC6);
    }	
}

void DMA1_Channel7_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC7))
    {
        MYDMA1C7_ISR();
        DMA_ClearITPendingBit(DMA1_IT_TC7);
    }	
}
