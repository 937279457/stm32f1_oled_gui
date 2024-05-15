#include "myrtc.h"
#include <time.h>
#include <stdlib.h>

void MYRTC_Init(uint32_t RCC_RTCCLKSource)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
    
    PWR_BackupAccessCmd(ENABLE);
    
    if(RCC_RTCCLKSource == RCC_RTCCLKSource_LSE)
    {
        RCC_LSEConfig(RCC_LSE_ON);
        while(!RCC_GetFlagStatus(RCC_FLAG_LSERDY));
        
        RCC_RTCCLKConfig(RCC_RTCCLKSource);
        RCC_RTCCLKCmd(ENABLE);
        
        RTC_WaitForLastTask();
        RTC_WaitForSynchro();
        
        RTC_SetPrescaler(32768 - 1);
        RTC_WaitForLastTask();
    }
    else if(RCC_RTCCLKSource == RCC_RTCCLKSource_LSI)
    {
        RCC_LSICmd(ENABLE);
        while(!RCC_GetFlagStatus(RCC_FLAG_LSIRDY));
        
        RCC_RTCCLKConfig(RCC_RTCCLKSource);
        RCC_RTCCLKCmd(ENABLE);
        
        RTC_WaitForLastTask();
        RTC_WaitForSynchro();
        
        RTC_SetPrescaler(40000 - 1);
        RTC_WaitForLastTask();
    }
    else if(RCC_RTCCLKSource == RCC_RTCCLKSource_HSE_Div128)
    {
        RCC_HSEConfig(RCC_HSE_ON);
        while(!RCC_GetFlagStatus(RCC_FLAG_HSERDY));
        
        RCC_RTCCLKConfig(RCC_RTCCLKSource);
        RCC_RTCCLKCmd(ENABLE);
        
        RTC_WaitForLastTask();
        RTC_WaitForSynchro();
        
        RTC_SetPrescaler(62500 - 1);
        RTC_WaitForLastTask();
    }
    
    RTC_SetCounter(0);
    RTC_WaitForLastTask();
}

void MYRTC_SetTime(MYRTC_Date_t* Date)
{
    time_t time_cnt;
    struct tm time_date;
    
    time_date.tm_year = Date->year - 1900;
    time_date.tm_mon = Date->mon - 1;
    time_date.tm_mday = Date->day;
    time_date.tm_hour = Date->hour;
    time_date.tm_min = Date->min;
    time_date.tm_sec = Date->sec;
    
    time_cnt = mktime(&time_date) - 8*60*60;// 北京时间，时区偏移
    
    RTC_SetCounter(time_cnt);
    RTC_WaitForLastTask();
}

MYRTC_Date_t* MYRTC_ReadTime(void)
{
    MYRTC_Date_t* MYRTC_Date = malloc(sizeof(MYRTC_Date_t));
    time_t time_cnt;
    struct tm time_date;
    
    time_cnt = RTC_GetCounter() + 8*60*60;// 北京时间，时区偏移
    time_date = *localtime(&time_cnt);
    
    MYRTC_Date->year = time_date.tm_year + 1900;
    MYRTC_Date->mon = time_date.tm_mon + 1;
    MYRTC_Date->day = time_date.tm_mday;
    MYRTC_Date->hour = time_date.tm_hour;
    MYRTC_Date->min = time_date.tm_min;
    MYRTC_Date->sec = time_date.tm_sec;
    
    return MYRTC_Date;
}

uint32_t MYRTC_GetMS(void)
{
    return ((32767 - RTC_GetDivider()) / 32767.0 * 999);
}
