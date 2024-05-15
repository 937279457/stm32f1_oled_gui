#include "myflash.h"

static uint32_t MyFlash_Data[256];

uint32_t MyFlash_ReadWord(uint32_t Address)
{
    return *((__IO uint32_t*)(Address));
}

uint16_t MyFlash_ReadHalfWord(uint32_t Address)
{
    return *((__IO uint16_t*)(Address));
}

uint8_t MyFlash_ReadByte(uint32_t Address)
{
    return *((__IO uint8_t*)(Address));
}

void MyFlash_EraseAllPage(void)
{
    FLASH_Unlock();
    FLASH_EraseAllPages();
    FLASH_Lock();
}

void MyFlash_ErasePage(uint32_t Address)
{
    FLASH_Unlock();
    FLASH_ErasePage(Address);
    FLASH_Lock();
}

void MyFlash_ProgramWord(uint32_t Address, uint32_t Data)
{
    FLASH_Unlock();
    FLASH_ProgramWord(Address, Data);
    FLASH_Lock();
}

void MyFlash_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
    FLASH_Unlock();
    FLASH_ProgramHalfWord(Address, Data);
    FLASH_Lock();
}

void MyFlash_Load(void)
{
    uint16_t i;
    for(i=0;i<256;i++)
        MyFlash_Data[i] = MyFlash_ReadWord(PAGEADDRESS+i*4);
}

void MyFlash_Program(uint32_t Index, uint32_t* pData, uint16_t Count)
{
    uint16_t i;
    for(i=0;i<Count;i++)
    {
        MyFlash_Data[Index+i] = pData[i];
    }
    MyFlash_ErasePage(PAGEADDRESS);// 擦除状态下才可写
    for(i=0;i<256;i++)
        MyFlash_ProgramWord(PAGEADDRESS+i*4, MyFlash_Data[i]);
}

void MyFlash_Read(uint32_t Index, uint32_t* pData, uint16_t Count)
{
    uint16_t i;
    for(i=0;i<Count;i++)
    {
        pData[i] = MyFlash_ReadWord(PAGEADDRESS + (Index + Count)*4);
    }
}

void MyFlash_Reset(uint32_t Index)
{
    uint32_t zero = 0;
    MyFlash_Program(Index, &zero, 1);
}

void MyFlash_ResetAll(void)
{
    uint16_t i;
    MyFlash_ErasePage(PAGEADDRESS);
    for(i=0;i<256;i++)
    {
        MyFlash_Data[i] = 0;
        MyFlash_ProgramWord(PAGEADDRESS+i*4, 0);
    }
}
