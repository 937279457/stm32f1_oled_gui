#include "bkp.h"

void BKP_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
    
    PWR_BackupAccessCmd(ENABLE);
}

void BKP_Write(uint16_t BKP_DR, uint16_t Data)
{
    BKP_WriteBackupRegister(BKP_DR, Data);
}

void BKP_Read(uint16_t BKP_DR)
{
    BKP_ReadBackupRegister(BKP_DR);
}

void BKP_Clear(void)
{
    BKP_DeInit();
}
