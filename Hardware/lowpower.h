#ifndef __LOWPOWER_H
#define __LOWPOWER_H

#include "stm32f10x.h"

void LP_Init(void);
void LP_Sleep(void);
void LP_Stop(void);
void LP_Standby(void);
void LP_WakeUpOpen(void);

#endif
