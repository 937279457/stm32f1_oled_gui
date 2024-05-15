#include "wdg.h"

static uint32_t WDG_WWDG_FeedValve;

void WDG_IWDG_Init(uint32_t ReloadValue)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_16);
    IWDG_SetReload(ReloadValue);// 1000ms:t=T(LSI)*PR*(RL+1)
    IWDG_ReloadCounter();
    IWDG_Enable();
}

void WDG_IWDG_Feed(void)
{
    IWDG_ReloadCounter();
}

void WDG_WWDG_Init(uint32_t WindowValue, uint32_t FeedValue)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
    WWDG_SetPrescaler(WWDG_Prescaler_8);
    WWDG_SetWindowValue(0x40 | WindowValue);// t = T(PCLK1)*4096*PR*(WV+1)
    WWDG_Enable(0x40 | FeedValue);			// t = T(PCLK1)*4096*PR*(WV-FV)
    WDG_WWDG_FeedValve = FeedValue;
}

void WDG_WWDG_Feed(void)
{
    WWDG_SetCounter(0x40 | WDG_WWDG_FeedValve);
}
