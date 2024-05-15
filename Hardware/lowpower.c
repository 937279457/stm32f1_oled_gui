#include "lowpower.h"

void LP_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
}

void LP_Sleep(void)
{
    __WFI();
}

void LP_Stop(void)
{
    PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
    SystemInit();
}

void LP_Standby(void)
{
    PWR_EnterSTANDBYMode();
}

void LP_WakeUpOpen(void)
{
    PWR_WakeUpPinCmd(ENABLE);
}
