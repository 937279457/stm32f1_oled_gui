#ifndef __BKP_H
#define __BKP_H

#include "stm32f10x.h"

void BKP_Init(void);
void BKP_Write(uint16_t BKP_DR, uint16_t Data);
void BKP_Read(uint16_t BKP_DR);
void BKP_Clear(void);

#endif
