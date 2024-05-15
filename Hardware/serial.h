#ifndef __SERIAL_H
#define __SERIAL_H

#include "stm32f10x.h"
#include "io.h"

#define RXPACKET_SIZE 10

extern uint32_t rxpindex;
extern char rxpacket[];

typedef struct
{
    USART_TypeDef* Serial_USARTx;
    uint32_t Serial_USART_BaudRate;
    uint8_t Serial_NVIC_PPriority;
    uint8_t Serial_NVIC_SPriority;
    FunctionalState Serial_IT_Cmd;
} Serial_InitTypeDef;

void Serial_Init(Serial_InitTypeDef* Serial_InitStruct);
void Serial_SendData(USART_TypeDef* USARTx, uint8_t Data);
void Serial_SendArr(USART_TypeDef* USARTx, uint8_t* Arr, uint32_t Length);
void Serial_SendStr(USART_TypeDef* USARTx, char* Str);
void Serial_Send16Bit(USART_TypeDef* USARTx, uint16_t data);
void Serial_Send32Bit(USART_TypeDef* USARTx, uint32_t data);
void Serial_SendFloat(USART_TypeDef* USARTx, float data);
void Serial_SetPHead_PEnd(char PHead, char PEnd);
void Serial_RecvPacket(USART_TypeDef* USARTx);
void Serial_SetIRQHandler(USART_TypeDef* USARTx, void (* IRQHandler)(USART_TypeDef*));
void Serial_Printf(USART_TypeDef* USARTx, char* format, ...);
void Serial_Begin(uint32_t BaudRate);
void Serial_Println(char* Str);
void Serial_Echo(USART_TypeDef* USARTx);
uint8_t Serial_CheckString(char* str);

#endif
