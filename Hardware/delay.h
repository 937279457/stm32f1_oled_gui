#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

#define SYSTICK_RELOAD 9000

extern volatile uint64_t systick_count;

void Delay_SystickStart(void);
void Delay_s(uint32_t s);
void Delay_ms(uint32_t ms);

#endif
