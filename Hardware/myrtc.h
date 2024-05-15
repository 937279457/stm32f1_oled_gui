#ifndef __MYRTC_H
#define __MYRTC_H

#include "stm32f10x.h"

typedef struct
{
    uint16_t year;
    uint8_t mon;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
} MYRTC_Date_t;

void MYRTC_Init(uint32_t RCC_RTCCLKSource);
void MYRTC_SetTime(MYRTC_Date_t* MyRTC_Date);
MYRTC_Date_t* MYRTC_ReadTime(void);
uint32_t MYRTC_GetMS(void);

#endif
