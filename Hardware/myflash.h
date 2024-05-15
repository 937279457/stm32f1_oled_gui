#ifndef __MYFLASH_H
#define __MYFLASH_H

#include "stm32f10x.h"

#define PAGEADDRESS 0x0800FC00

uint32_t MyFlash_ReadWord(uint32_t Address);
uint16_t MyFlash_ReadHalfWord(uint32_t Address);
uint8_t MyFlash_ReadByte(uint32_t Address);
void MyFlash_EraseAllPage(void);
void MyFlash_ErasePage(uint32_t Address);
void MyFlash_ProgramWord(uint32_t Address, uint32_t Data);
void MyFlash_ProgramHalfWord(uint32_t Address, uint16_t Data);
void MyFlash_Load(void);
void MyFlash_Program(uint32_t Index, uint32_t* pData, uint16_t Count);
void MyFlash_Read(uint32_t Index, uint32_t* pData, uint16_t Count);
void MyFlash_Reset(uint32_t Index);
void MyFlash_ResetAll(void);

#endif
