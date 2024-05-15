#include "io.h"

typedef struct
{
    GPIO_TypeDef* IO_GPIOx;
    uint16_t IO_Pin;
} IO_GPIO_t;

static void FindGPIO(uint8_t GPIOxy, IO_GPIO_t* p)
{
    switch((GPIOxy)%20)
    {
        case 1:p->IO_Pin = GPIO_Pin_0;break;
        case 2:p->IO_Pin = GPIO_Pin_1;break;
        case 3:p->IO_Pin = GPIO_Pin_2;break;
        case 4:p->IO_Pin = GPIO_Pin_3;break;
        case 5:p->IO_Pin = GPIO_Pin_4;break;
        case 6:p->IO_Pin = GPIO_Pin_5;break;
        case 7:p->IO_Pin = GPIO_Pin_6;break;
        case 8:p->IO_Pin = GPIO_Pin_7;break;
        case 9:p->IO_Pin = GPIO_Pin_8;break;
        case 10:p->IO_Pin = GPIO_Pin_9;break;
        case 11:p->IO_Pin = GPIO_Pin_10;break;
        case 12:p->IO_Pin = GPIO_Pin_11;break;
        case 13:p->IO_Pin = GPIO_Pin_12;break;
        case 14:p->IO_Pin = GPIO_Pin_13;break;
        case 15:p->IO_Pin = GPIO_Pin_14;break;
        case 16:p->IO_Pin = GPIO_Pin_15;break;
    }
    switch((GPIOxy)/20)
    {
        case 0:p->IO_GPIOx = GPIOA;break;
        case 1:p->IO_GPIOx = GPIOB;break;
        case 2:p->IO_GPIOx = GPIOC;break;
        case 3:p->IO_GPIOx = GPIOD;break;
        case 4:p->IO_GPIOx = GPIOE;break;
        case 5:p->IO_GPIOx = GPIOF;break;
        case 6:p->IO_GPIOx = GPIOG;break;
    }
}

void IO_GPIOInit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode)
{
    if(GPIOx==GPIOA)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    else if(GPIOx == GPIOB)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    else if(GPIOx == GPIOC)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    else if(GPIOx == GPIOD)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    else if(GPIOx ==GPIOE)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    else if(GPIOx == GPIOF)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    else if(GPIOx == GPIOG)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}

void IO_Init(IO_TypeDef GPIOxy, GPIOMode_TypeDef Mode)
{
    IO_GPIO_t io;
    FindGPIO((uint8_t)GPIOxy, &io);
    IO_GPIOInit(io.IO_GPIOx, io.IO_Pin, Mode);
}

void IO_Turn(IO_TypeDef GPIOxy)
{
    IO_GPIO_t io;
    FindGPIO((uint8_t)GPIOxy, &io);
    GPIO_WriteBit(io.IO_GPIOx, io.IO_Pin, (BitAction)!GPIO_ReadInputDataBit(io.IO_GPIOx, io.IO_Pin));
}

void IO_WriteBit(IO_TypeDef GPIOxy, BitAction BitVal)
{
    IO_GPIO_t io;
    FindGPIO((uint8_t)GPIOxy, &io);
    GPIO_WriteBit(io.IO_GPIOx, io.IO_Pin, BitVal);
}

uint8_t IO_ReadBit(IO_TypeDef GPIOxy)
{
    IO_GPIO_t io;
    FindGPIO((uint8_t)GPIOxy, &io);
    return GPIO_ReadInputDataBit(io.IO_GPIOx, io.IO_Pin);
}

// 高性能IO口电平翻转函数
void IO_Toggle(GPIO_TypeDef* GPIOx, uint16_t Pin)
{ 
    if((GPIOx->IDR & Pin) != (uint32_t)Bit_RESET)
        GPIOx->BRR = Pin; 
    else
        GPIOx->BSRR = Pin;
}
