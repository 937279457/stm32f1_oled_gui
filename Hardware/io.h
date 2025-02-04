#ifndef __IO_H
#define __IO_H

#include "stm32f10x.h"

typedef enum
{
    GPIOA0 = 1,
    GPIOA1 = 2,
    GPIOA2 = 3,
    GPIOA3 = 4,
    GPIOA4 = 5,
    GPIOA5 = 6,
    GPIOA6 = 7,
    GPIOA7 = 8,
    GPIOA8 = 9,
    GPIOA9 = 10,
    GPIOA10 = 11,
    GPIOA11 = 12,
    GPIOA12 = 13,
    GPIOA13 = 14,
    GPIOA14 = 15,
    GPIOA15 = 16,
    GPIOAAll = 17,
    GPIOB0 = 21,
    GPIOB1 = 22,
    GPIOB2 = 23,
    GPIOB3 = 24,
    GPIOB4 = 25,
    GPIOB5 = 26,
    GPIOB6 = 27,
    GPIOB7 = 28,
    GPIOB8 = 29,
    GPIOB9 = 30,
    GPIOB10 = 31,
    GPIOB11 = 32,
    GPIOB12 = 33,
    GPIOB13 = 34,
    GPIOB14 = 35,
    GPIOB15 = 36,
    GPIOBAll = 37,
    GPIOC0 = 41,
    GPIOC1 = 42,
    GPIOC2 = 43,
    GPIOC3 = 44,
    GPIOC4 = 45,
    GPIOC5 = 46,
    GPIOC6 = 47,
    GPIOC7 = 48,
    GPIOC8 = 49,
    GPIOC9 = 50,
    GPIOC10 = 51,
    GPIOC11 = 52,
    GPIOC12 = 53,
    GPIOC13 = 54,
    GPIOC14 = 55,
    GPIOC15 = 56,
    GPIOCAll = 57,
    GPIOD0 = 61,
    GPIOD1 = 62,
    GPIOD2 = 63,
    GPIOD3 = 64,
    GPIOD4 = 65,
    GPIOD5 = 66,
    GPIOD6 = 67,
    GPIOD7 = 68,
    GPIOD8 = 69,
    GPIOD9 = 70,
    GPIOD10 = 71,
    GPIOD11 = 72,
    GPIOD12 = 73,
    GPIOD13 = 74,
    GPIOD14 = 75,
    GPIOD15 = 76,
    GPIODAll = 77,
    GPIOE0 = 81,
    GPIOE1 = 82,
    GPIOE2 = 83,
    GPIOE3 = 84,
    GPIOE4 = 85,
    GPIOE5 = 86,
    GPIOE6 = 87,
    GPIOE7 = 88,
    GPIOE8 = 89,
    GPIOE9 = 90,
    GPIOE10 = 91,
    GPIOE11 = 92,
    GPIOE12 = 93,
    GPIOE13 = 94,
    GPIOE14 = 95,
    GPIOE15 = 96,
    GPIOEAll = 97,
    GPIOF0 = 101,
    GPIOF1 = 102,
    GPIOF2 = 103,
    GPIOF3 = 104,
    GPIOF4 = 105,
    GPIOF5 = 106,
    GPIOF6 = 107,
    GPIOF7 = 108,
    GPIOF8 = 109,
    GPIOF9 = 110,
    GPIOF10 = 111,
    GPIOF11 = 112,
    GPIOF12 = 113,
    GPIOF13 = 114,
    GPIOF14 = 115,
    GPIOF15 = 116,
    GPIOFAll = 117,
    GPIOG0 = 121,
    GPIOG1 = 122,
    GPIOG2 = 123,
    GPIOG3 = 124,
    GPIOG4 = 125,
    GPIOG5 = 126,
    GPIOG6 = 127,
    GPIOG7 = 128,
    GPIOG8 = 129,
    GPIOG9 = 130,
    GPIOG10 = 131,
    GPIOG11 = 132,
    GPIOG12 = 133,
    GPIOG13 = 134,
    GPIOG14 = 135,
    GPIOG15 = 136,
    GPIOGAll = 137,
} IO_TypeDef;

void IO_GPIOInit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode);
void IO_Init(IO_TypeDef GPIOxy, GPIOMode_TypeDef Mode);
void IO_Turn(IO_TypeDef GPIOxy);
void IO_WriteBit(IO_TypeDef GPIOxy, BitAction BitVal);
uint8_t IO_ReadBit(IO_TypeDef GPIOxy);
void IO_Toggle(GPIO_TypeDef* GPIOx, uint16_t Pin);

#endif
