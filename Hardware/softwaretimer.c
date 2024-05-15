#include "softwaretimer.h"
#include "delay.h"
#include <stdlib.h>

SFTTIM_Handle_t* SFTTIM_CreateTimer(uint32_t ms)
{
    SFTTIM_Handle_t* p = malloc(sizeof(SFTTIM_Handle_t));
    p->reload = ms;
    p->timerstate = DISABLE;
    p->count = p->reload;
    p->alarm = 0;
    p->once = 0;
    p->target = 0;
    return p;
}

void SFTTIM_SetReload(SFTTIM_Handle_t* timer, uint32_t ms)
{
    timer->reload = ms;
}

void SFTTIMCmd(SFTTIM_Handle_t* timer, FunctionalState Cmd)
{
    timer->timerstate = Cmd;
    timer->target = 0;
    if(Cmd == ENABLE)
    {
        timer->target = systick_count + timer->reload;
        timer->count = 0;
        timer->once = 1;;
    }
}

int SFTTIM_StartupAndTicking(SFTTIM_Handle_t* timer, uint32_t ms)
{
    if(timer->once == 0)
    {
        timer->reload = ms;
        SFTTIMCmd(timer, ENABLE);
    }
    return SFTTIM_Ticking(timer);
}

int SFTTIM_Ticking(SFTTIM_Handle_t* timer)
{
    if(timer->timerstate == DISABLE)
        return -1;
    if(systick_count >= timer->target)
    {
        timer->alarm = 1;
        timer->target += timer->reload;
        timer->count = 0;
        return 1;
    }
    timer->count = timer->target - systick_count;
    return 0;
}

uint32_t SFTTIM_MeasuringTimeSlots(SFTTIM_Handle_t* timer)
{
    SFTTIM_Ticking(timer);
    uint32_t timeslots = SFTTIM_GetTimerCount(timer);
    SFTTIMCmd(timer, ENABLE);
    return timeslots;
}

void SFTTIM_MeasuringTimeSlotsStart(SFTTIM_Handle_t* timer)
{
    if(timer->timerstate == DISABLE)
        SFTTIMCmd(timer, ENABLE);
}

int SFTTIM_MeasuringTimeSlotsGet(SFTTIM_Handle_t* timer)
{
    if(timer->timerstate == DISABLE)
        return -1;
    SFTTIM_Ticking(timer);
    uint32_t timeslots = SFTTIM_GetTimerCount(timer);
    return timeslots;
}

uint8_t SFTTIM_GetAlarmStatus(SFTTIM_Handle_t* timer)
{
    uint8_t ret = timer->alarm;
    timer->alarm = 0;
    return ret;
}

uint32_t SFTTIM_GetTimerCount(SFTTIM_Handle_t* timer)
{
    return  timer->reload - timer->count;
}
