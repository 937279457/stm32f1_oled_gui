#include "serial.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define PRINTF_SIZE 100

char rxpflag;
uint32_t rxpindex;
char rxpacket[RXPACKET_SIZE];
static char phead = 0;
static char pend = 0;

void (* Serial1_Handler)(USART_TypeDef* USARTx) = NULL;
void (* Serial2_Handler)(USART_TypeDef* USARTx) = NULL;
void (* Serial3_Handler)(USART_TypeDef* USARTx) = NULL;

void Serial_Init(Serial_InitTypeDef* Serial_InitStruct)
{
    if(Serial_InitStruct->Serial_USARTx==USART1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
        IO_GPIOInit(GPIOA, GPIO_Pin_9, GPIO_Mode_AF_PP);
        IO_GPIOInit(GPIOA, GPIO_Pin_10, GPIO_Mode_IPU);
    }
    else if(Serial_InitStruct->Serial_USARTx==USART2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        IO_GPIOInit(GPIOA, GPIO_Pin_2, GPIO_Mode_AF_PP);
        IO_GPIOInit(GPIOA, GPIO_Pin_3, GPIO_Mode_IPU);
    }
    else if(Serial_InitStruct->Serial_USARTx==USART3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        IO_GPIOInit(GPIOB, GPIO_Pin_10, GPIO_Mode_AF_PP);
        IO_GPIOInit(GPIOB, GPIO_Pin_11, GPIO_Mode_IPU);
    }
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = Serial_InitStruct->Serial_USART_BaudRate;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(Serial_InitStruct->Serial_USARTx, &USART_InitStructure);
    if(Serial_InitStruct->Serial_IT_Cmd)
    {
        USART_ITConfig(Serial_InitStruct->Serial_USARTx, USART_IT_RXNE, ENABLE);
        
        NVIC_InitTypeDef NVIC_InitStructure;
        if(Serial_InitStruct->Serial_USARTx==USART1)
            NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        else if(Serial_InitStruct->Serial_USARTx==USART2)
            NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
        else if(Serial_InitStruct->Serial_USARTx==USART3)
            NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Serial_InitStruct->Serial_NVIC_PPriority;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = Serial_InitStruct->Serial_NVIC_SPriority;
        NVIC_Init(&NVIC_InitStructure);
        Serial_SetIRQHandler(Serial_InitStruct->Serial_USARTx, Serial_Echo);
    }
    USART_Cmd(Serial_InitStruct->Serial_USARTx, ENABLE);
}

void Serial_SetIRQHandler(USART_TypeDef* USARTx, void (* IRQHandler)(USART_TypeDef*))
{
    if(USARTx==USART1)
        Serial1_Handler=IRQHandler;
    else if(USARTx==USART2)
        Serial2_Handler=IRQHandler;
    else if(USARTx==USART3)
        Serial3_Handler=IRQHandler;
}

void Serial_SendData(USART_TypeDef* USARTx, uint8_t Data)
{
    USART_SendData(USARTx, Data);
    while(!USART_GetFlagStatus(USARTx, USART_FLAG_TXE));
}

void Serial_SendArr(USART_TypeDef* USARTx, uint8_t* Arr, uint32_t Length)
{
    uint32_t i;
    for(i=0;i<Length;i++)
    {
        Serial_SendData(USARTx, Arr[i]);
    }
}

void Serial_SendStr(USART_TypeDef* USARTx, char* Str)
{
    uint8_t i;
    for(i=0;Str[i]!=0;i++)
    {
        Serial_SendData(USARTx, Str[i]);
    }
}

void Serial_Send16Bit(USART_TypeDef* USARTx, uint16_t data)
{
    Serial_SendData(USARTx, data);
    Serial_SendData(USARTx, data>>8);
}

void Serial_Send32Bit(USART_TypeDef* USARTx, uint32_t data)
{
    Serial_SendData(USARTx, data);
    Serial_SendData(USARTx, data>>8);
    Serial_SendData(USARTx, data>>16);
    Serial_SendData(USARTx, data>>24);
}

void Serial_SendFloat(USART_TypeDef* USARTx, float data)
{
    char hex[4];
    *(float*)hex = data;
    Serial_SendData(USARTx, hex[0]);
    Serial_SendData(USARTx, hex[1]);
    Serial_SendData(USARTx, hex[2]);
    Serial_SendData(USARTx, hex[3]);
}

void Serial_Printf(USART_TypeDef* USARTx, char* format, ...)// 该函数不能在中断中调用
{
    char str[PRINTF_SIZE];
    va_list arg;
    va_start(arg, format);
    vsprintf(str, format, arg);
    va_end(arg);
    Serial_SendStr(USARTx, str);
}

uint8_t Serial_CheckString(char* str)
{
    if(rxpflag)
    {
        if(strcmp(rxpacket, str) == 0)
        {
            rxpflag = 0;
            return 1;
        }
        else
            return 0;
    }
    else
        return 0;
}

void Serial_SetPHead_PEnd(char PHead, char PEnd)
{
    phead = PHead;
    pend = PEnd;
}

void Serial_RecvPacket(USART_TypeDef* USARTx)
{
    static uint8_t rxdata;
    static uint8_t rxstate = 0;
    rxdata = USART_ReceiveData(USARTx);
    if(rxstate == 0)
    {
        if(rxdata == phead && rxpflag == 0)
        {
            rxstate = 1;
            rxpflag = 1;
            rxpindex = 0;
            return;
        }
    }
    if(rxstate == 1)
    {
        if(rxdata == pend)
        {
            rxstate = 0;
        }
        else
        {
            rxpacket[rxpindex] = rxdata;
            rxpindex++;
        }
    }
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE))
    {
        if(Serial1_Handler != NULL)
            Serial1_Handler(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE))
    {
        if(Serial2_Handler != NULL)
            Serial2_Handler(USART2);
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}

void USART3_IRQHandler(void)
{
    if(USART_GetITStatus(USART3, USART_IT_RXNE))
    {
        if(Serial3_Handler != NULL)
            Serial3_Handler(USART3);
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}

void Serial_Begin(uint32_t BaudRate)
{
    Serial_InitTypeDef Serial_InitStructure;
    Serial_InitStructure.Serial_IT_Cmd = DISABLE;
    Serial_InitStructure.Serial_USARTx = USART1;
    Serial_InitStructure.Serial_USART_BaudRate = BaudRate;
    Serial_Init(&Serial_InitStructure);
}

void Serial_Println(char* Str)
{
    Serial_SendStr(USART1, Str);
    Serial_SendStr(USART1, "\r\n");
}

void Serial_Echo(USART_TypeDef* USARTx)
{
    Serial_SendData(USARTx, USART_ReceiveData(USARTx));
}
