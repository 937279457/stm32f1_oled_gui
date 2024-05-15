#include "Key.h"
#include "myexti.h"
#include "delay.h"
#include <stdlib.h>

GPIO_TypeDef* EXTILine_GPIO[16];
uint16_t EXTILine_Delevel[16];
uint8_t KeyState;

GPIO_TypeDef* Curr_GPIOx;
uint16_t Curr_Pin;

static void Key_EXTI_Scan(void)
{
    Delay_ms(10);
    switch(MYEXTI_IT_Line)
    {
        case EXTI_Line0:if(EXTILine_Delevel[0]!=GPIO_ReadInputDataBit(EXTILine_GPIO[0], GPIO_Pin_0))KeyState = 1;break;
        case EXTI_Line1:if(EXTILine_Delevel[1]!=GPIO_ReadInputDataBit(EXTILine_GPIO[1], GPIO_Pin_1))KeyState = 2;break;
        case EXTI_Line2:if(EXTILine_Delevel[2]!=GPIO_ReadInputDataBit(EXTILine_GPIO[2], GPIO_Pin_2))KeyState = 3;break;
        case EXTI_Line3:if(EXTILine_Delevel[3]!=GPIO_ReadInputDataBit(EXTILine_GPIO[3], GPIO_Pin_3))KeyState = 4;break;
        case EXTI_Line4:if(EXTILine_Delevel[4]!=GPIO_ReadInputDataBit(EXTILine_GPIO[4], GPIO_Pin_4))KeyState = 5;break;
        case EXTI_Line5:if(EXTILine_Delevel[5]!=GPIO_ReadInputDataBit(EXTILine_GPIO[5], GPIO_Pin_5))KeyState = 6;break;
        case EXTI_Line6:if(EXTILine_Delevel[6]!=GPIO_ReadInputDataBit(EXTILine_GPIO[6], GPIO_Pin_6))KeyState = 7;break;
        case EXTI_Line7:if(EXTILine_Delevel[7]!=GPIO_ReadInputDataBit(EXTILine_GPIO[7], GPIO_Pin_7))KeyState = 8;break;
        case EXTI_Line8:if(EXTILine_Delevel[8]!=GPIO_ReadInputDataBit(EXTILine_GPIO[8], GPIO_Pin_8))KeyState = 9;break;
        case EXTI_Line9:if(EXTILine_Delevel[9]!=GPIO_ReadInputDataBit(EXTILine_GPIO[9], GPIO_Pin_9))KeyState = 10;break;
        case EXTI_Line10:if(EXTILine_Delevel[10]!=GPIO_ReadInputDataBit(EXTILine_GPIO[10], GPIO_Pin_10))KeyState = 11;break;
        case EXTI_Line11:if(EXTILine_Delevel[11]!=GPIO_ReadInputDataBit(EXTILine_GPIO[11], GPIO_Pin_11))KeyState = 12;break;
        case EXTI_Line12:if(EXTILine_Delevel[12]!=GPIO_ReadInputDataBit(EXTILine_GPIO[12], GPIO_Pin_12))KeyState = 13;break;
        case EXTI_Line13:if(EXTILine_Delevel[13]!=GPIO_ReadInputDataBit(EXTILine_GPIO[13], GPIO_Pin_13))KeyState = 14;break;
        case EXTI_Line14:if(EXTILine_Delevel[14]!=GPIO_ReadInputDataBit(EXTILine_GPIO[14], GPIO_Pin_14))KeyState = 15;break;
        case EXTI_Line15:if(EXTILine_Delevel[15]!=GPIO_ReadInputDataBit(EXTILine_GPIO[15], GPIO_Pin_15))KeyState = 16;break;
    }
}

void Key_EXTIInit(Key_EXTIInitTypeDef* Key_InitStruct)
{
    uint32_t EXTI_Line;
    
    MYEXTI_InitTypeDef MYEXTI_InitStructure;
    MYEXTI_InitStructure.MYEXTI_GPIOx = Key_InitStruct->Key_GPIOx;
    MYEXTI_InitStructure.MYEXTI_NVIC_PPriority = Key_InitStruct->Key_NVIC_PPriority;
    MYEXTI_InitStructure.MYEXTI_NVIC_SPriority = Key_InitStruct->Key_NVIC_SPriority;
    MYEXTI_InitStructure.MYEXTI_Pin = Key_InitStruct->Key_Pin;
    if(Key_InitStruct->Key_Delevel)
        MYEXTI_InitStructure.MYEXTI_Trigger = EXTI_Trigger_Falling;
    else
        MYEXTI_InitStructure.MYEXTI_Trigger = EXTI_Trigger_Rising;
    MYEXTI_Init(&MYEXTI_InitStructure);
    
    switch(Key_InitStruct->Key_Pin)
    {
        case GPIO_Pin_0:EXTI_Line = EXTI_Line0;EXTILine_GPIO[0] = Key_InitStruct->Key_GPIOx;EXTILine_Delevel[0] = Key_InitStruct->Key_Delevel;break;
        case GPIO_Pin_1:EXTI_Line = EXTI_Line1;EXTILine_GPIO[1] = Key_InitStruct->Key_GPIOx;EXTILine_Delevel[1] = Key_InitStruct->Key_Delevel;break;
        case GPIO_Pin_2:EXTI_Line = EXTI_Line2;EXTILine_GPIO[2] = Key_InitStruct->Key_GPIOx;EXTILine_Delevel[2] = Key_InitStruct->Key_Delevel;break;
        case GPIO_Pin_3:EXTI_Line = EXTI_Line3;EXTILine_GPIO[3] = Key_InitStruct->Key_GPIOx;EXTILine_Delevel[3] = Key_InitStruct->Key_Delevel;break;
        case GPIO_Pin_4:EXTI_Line = EXTI_Line4;EXTILine_GPIO[4] = Key_InitStruct->Key_GPIOx;EXTILine_Delevel[4] = Key_InitStruct->Key_Delevel;break;
        case GPIO_Pin_5:EXTI_Line = EXTI_Line5;EXTILine_GPIO[5] = Key_InitStruct->Key_GPIOx;EXTILine_Delevel[5] = Key_InitStruct->Key_Delevel;break;
        case GPIO_Pin_6:EXTI_Line = EXTI_Line6;EXTILine_GPIO[6] = Key_InitStruct->Key_GPIOx;EXTILine_Delevel[6] = Key_InitStruct->Key_Delevel;break;
        case GPIO_Pin_7:EXTI_Line = EXTI_Line7;EXTILine_GPIO[7] = Key_InitStruct->Key_GPIOx;EXTILine_Delevel[7] = Key_InitStruct->Key_Delevel;break;
        case GPIO_Pin_8:EXTI_Line = EXTI_Line8;EXTILine_GPIO[8] = Key_InitStruct->Key_GPIOx;EXTILine_Delevel[8] = Key_InitStruct->Key_Delevel;break;
        case GPIO_Pin_9:EXTI_Line = EXTI_Line9;EXTILine_GPIO[9] = Key_InitStruct->Key_GPIOx;EXTILine_Delevel[9] = Key_InitStruct->Key_Delevel;break;
        case GPIO_Pin_10:EXTI_Line = EXTI_Line10;EXTILine_GPIO[10] = Key_InitStruct->Key_GPIOx;EXTILine_Delevel[10] = Key_InitStruct->Key_Delevel;break;
        case GPIO_Pin_11:EXTI_Line = EXTI_Line11;EXTILine_GPIO[11] = Key_InitStruct->Key_GPIOx;EXTILine_Delevel[11] = Key_InitStruct->Key_Delevel;break;
        case GPIO_Pin_12:EXTI_Line = EXTI_Line12;EXTILine_GPIO[12] = Key_InitStruct->Key_GPIOx;EXTILine_Delevel[12] = Key_InitStruct->Key_Delevel;break;
        case GPIO_Pin_13:EXTI_Line = EXTI_Line13;EXTILine_GPIO[13] = Key_InitStruct->Key_GPIOx;EXTILine_Delevel[13] = Key_InitStruct->Key_Delevel;break;
        case GPIO_Pin_14:EXTI_Line = EXTI_Line14;EXTILine_GPIO[14] = Key_InitStruct->Key_GPIOx;EXTILine_Delevel[14] = Key_InitStruct->Key_Delevel;break;
        case GPIO_Pin_15:EXTI_Line = EXTI_Line15;EXTILine_GPIO[15] = Key_InitStruct->Key_GPIOx;EXTILine_Delevel[15] = Key_InitStruct->Key_Delevel;break;
    }
    MYEXTI_RegisterISR(EXTI_Line, Key_EXTI_Scan);
}

uint8_t Key_CheckState(GPIO_TypeDef* GPIOx, uint16_t Pin)
{
    if(GPIOx==EXTILine_GPIO[KeyState-1]&&Pin==1<<(KeyState-1))
    {
        KeyState = 0;
        return 1;
    }
    return 0;
}

Key_Handle_t* Key_Init(GPIO_TypeDef* GPIOx, uint16_t Pin, uint8_t DetectLevel)
{
    Key_Handle_t* key = malloc(sizeof(Key_Handle_t));
    key->timer = SFTTIM_CreateTimer(1000);
    key->DetectLevel = DetectLevel;
    
    key->GPIOx = GPIOx;
    key->Pin = Pin;
    key->keyflag = 0;
    key->pressflag = 0;
    
    return key;
}

uint8_t Key_Scan(Key_Handle_t* key, uint32_t hold_ms)
{
    if(key->pressflag == 1)
    {
        if(SFTTIM_Ticking(key->timer) == 1)
        {
            key->pressflag = 0;
            SFTTIMCmd(key->timer, DISABLE);
            return 2;
        }
    }
    if(key->keyflag == 0 && GPIO_ReadInputDataBit(key->GPIOx, key->Pin) == key->DetectLevel)
    {
        key->keyflag = 1;
        Delay_ms(10);
        if(GPIO_ReadInputDataBit(key->GPIOx, key->Pin) == key->DetectLevel)
        {
            key->pressflag = 1;
            key->timer->reload = hold_ms;
            SFTTIMCmd(key->timer, ENABLE);
        }
    }
    else if(GPIO_ReadInputDataBit(key->GPIOx, key->Pin) != key->DetectLevel)
    {
        key->keyflag = 0;
        if(key->pressflag == 1)
        {
            key->pressflag = 0;
            if(SFTTIM_Ticking(key->timer) != 1)
            {
                SFTTIMCmd(key->timer, DISABLE);
                return 1;
            } 
        }
    }
    return 0;
}
