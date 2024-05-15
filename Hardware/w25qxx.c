#include "W25Qxx.h"
#include "MYSPI.h"

static MYSPI_InitTypeDef* pMYSPI_InitStructure;
static uint32_t W25Qxx_Data[1024];
static uint32_t W25Qxx_LastSector = 0;

void W25Qxx_Load(uint32_t Sector)
{
    W25Qxx_Read(Sector, 0, W25Qxx_Data, 1024);
}

void W25Qxx_Init(W25Qxx_InitTypeDef* W25Qxx_InitStruct)
{
    pMYSPI_InitStructure = (MYSPI_InitTypeDef*)W25Qxx_InitStruct;
    MYSPI_Init(pMYSPI_InitStructure);
    W25Qxx_Load(0);
}

void W25Qxx_WriteEnable(void)
{
    MYSPI_Start(pMYSPI_InitStructure);
    MYSPI_SwapData(pMYSPI_InitStructure, W25Qxx_WRITE_ENABLE);
    MYSPI_Stop(pMYSPI_InitStructure);
}

void W25Qxx_WaitBusy(void)
{
    uint32_t timeout = 100000;
    MYSPI_Start(pMYSPI_InitStructure);
    MYSPI_SwapData(pMYSPI_InitStructure, W25Qxx_READ_STATUS_REGISTER_1);
    while((MYSPI_SwapData(pMYSPI_InitStructure, W25Qxx_DUMMY_BYTE) & 0x01) == 0x01)
    {
        timeout--;
        if(!timeout)
            break;
    }
    MYSPI_Stop(pMYSPI_InitStructure);
}

void W25Qxx_EraseSector(uint32_t Sector)
{
    W25Qxx_WaitBusy();
    W25Qxx_WriteEnable();
    MYSPI_Start(pMYSPI_InitStructure);
    MYSPI_SwapData(pMYSPI_InitStructure, W25Qxx_SECTOR_ERASE_4KB);
    MYSPI_SwapData(pMYSPI_InitStructure, (Sector*4096) >> 16);
    MYSPI_SwapData(pMYSPI_InitStructure, (Sector*4096) >> 8);
    MYSPI_SwapData(pMYSPI_InitStructure, (Sector*4096));
    MYSPI_Stop(pMYSPI_InitStructure);
}

uint32_t W25Qxx_Swap32Bit(uint32_t Data)
{
    uint32_t ReturnValue = 0;
    ReturnValue = MYSPI_SwapData(pMYSPI_InitStructure, Data);
    ReturnValue |= ((uint32_t)MYSPI_SwapData(pMYSPI_InitStructure, Data >> 8)) << 8;
    ReturnValue |= ((uint32_t)MYSPI_SwapData(pMYSPI_InitStructure, Data >> 16)) << 16;
    ReturnValue |= ((uint32_t)MYSPI_SwapData(pMYSPI_InitStructure, Data >> 24)) << 24;
    return ReturnValue;
}

void W25Qxx_PageProgram(uint32_t Sector, uint32_t Page)
{
    uint8_t i;
    W25Qxx_WaitBusy();
    W25Qxx_WriteEnable();
    MYSPI_Start(pMYSPI_InitStructure);
    MYSPI_SwapData(pMYSPI_InitStructure, W25Qxx_PAGE_PROGRAM);
    MYSPI_SwapData(pMYSPI_InitStructure, (Sector*4096 + Page*256) >> 16);
    MYSPI_SwapData(pMYSPI_InitStructure, (Sector*4096 + Page*256) >> 8);
    MYSPI_SwapData(pMYSPI_InitStructure, (Sector*4096 + Page*256));
    for(i=0;i<64;i++)
    {
        W25Qxx_Swap32Bit(W25Qxx_Data[i + Page*64]);
    }
    MYSPI_Stop(pMYSPI_InitStructure);
}

void W25Qxx_Program(uint32_t Sector, uint32_t Index, uint32_t* pData, uint16_t Count)
{
    uint16_t i;
    if(Sector != W25Qxx_LastSector)
    {
        W25Qxx_LastSector = Sector;
        W25Qxx_Load(Sector);
    }
    for(i=0;i<Count;i++)
    {
        W25Qxx_Data[Index+i] = pData[i];
    }
    W25Qxx_EraseSector(Sector);
    for(i=0;i<16;i++)
    {
        W25Qxx_PageProgram(Sector, i);
    }
}

void W25Qxx_Read(uint32_t Sector, uint32_t Index, uint32_t* pData, uint16_t Count)
{
    uint16_t i;
    W25Qxx_WaitBusy();
    MYSPI_Start(pMYSPI_InitStructure);
    MYSPI_SwapData(pMYSPI_InitStructure, W25Qxx_READ_DATA);
    MYSPI_SwapData(pMYSPI_InitStructure, (Sector*4096 + Index*4) >> 16);
    MYSPI_SwapData(pMYSPI_InitStructure, (Sector*4096 + Index*4) >> 8);
    MYSPI_SwapData(pMYSPI_InitStructure, (Sector*4096 + Index*4));
    for(i=0;i<Count;i++)
    {
        pData[i] = W25Qxx_Swap32Bit(W25Qxx_DUMMY_BYTE);
    }
    MYSPI_Stop(pMYSPI_InitStructure);
}
