#ifndef __WDG_H
#define __WDG_H

#include "stm32f10x.h"

void WDG_IWDG_Init(uint32_t ReloadValue);
void WDG_IWDG_Feed(void);
void WDG_WWDG_Init(uint32_t WindowValue, uint32_t FeedValue);
void WDG_WWDG_Feed(void);

#endif
