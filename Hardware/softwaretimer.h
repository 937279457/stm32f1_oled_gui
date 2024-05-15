#ifndef __SOFTWARETIMER_H
#define __SOFTWARETIMER_H

#include "stm32f10x.h"

typedef struct
{
    uint64_t target;
    uint32_t reload;
    uint32_t count;
    uint8_t alarm;
    uint8_t once;
    FunctionalState timerstate;
} SFTTIM_Handle_t;

SFTTIM_Handle_t* SFTTIM_CreateTimer(uint32_t ms);
int SFTTIM_StartupAndTicking(SFTTIM_Handle_t* timer, uint32_t ms);
int SFTTIM_Ticking(SFTTIM_Handle_t* timer);
void SFTTIMCmd(SFTTIM_Handle_t* timer, FunctionalState Cmd);
uint32_t SFTTIM_MeasuringTimeSlots(SFTTIM_Handle_t* timer);
void SFTTIM_MeasuringTimeSlotsStart(SFTTIM_Handle_t* timer);
int SFTTIM_MeasuringTimeSlotsGet(SFTTIM_Handle_t* timer);
void SFTTIM_SetReload(SFTTIM_Handle_t* timer, uint32_t ms);
uint8_t SFTTIM_GetAlarmStatus(SFTTIM_Handle_t* timer);
uint32_t SFTTIM_GetTimerCount(SFTTIM_Handle_t* timer);

#endif
