#ifndef __W25QXX_H
#define __W25QXX_H

#include "stm32f10x.h"

#define W25Qxx_WRITE_ENABLE							0x06
#define W25Qxx_WRITE_DISABLE						0x04
#define W25Qxx_READ_STATUS_REGISTER_1				0x05
#define W25Qxx_READ_STATUS_REGISTER_2				0x35
#define W25Qxx_WRITE_STATUS_REGISTER				0x01
#define W25Qxx_PAGE_PROGRAM							0x02
#define W25Qxx_QUAD_PAGE_PROGRAM					0x32
#define W25Qxx_BLOCK_ERASE_64KB						0xD8
#define W25Qxx_BLOCK_ERASE_32KB						0x52
#define W25Qxx_SECTOR_ERASE_4KB						0x20
#define W25Qxx_CHIP_ERASE							0xC7
#define W25Qxx_ERASE_SUSPEND						0x75
#define W25Qxx_ERASE_RESUME							0x7A
#define W25Qxx_POWER_DOWN							0xB9
#define W25Qxx_HIGH_PERFORMANCE_MODE				0xA3
#define W25Qxx_CONTINUOUS_READ_MODE_RESET			0xFF
#define W25Qxx_RELEASE_POWER_DOWN_HPM_DEVICE_ID		0xAB
#define W25Qxx_MANUFACTURER_DEVICE_ID				0x90
#define W25Qxx_READ_UNIQUE_ID						0x4B
#define W25Qxx_JEDEC_ID								0x9F
#define W25Qxx_READ_DATA							0x03
#define W25Qxx_FAST_READ							0x0B
#define W25Qxx_FAST_READ_DUAL_OUTPUT				0x3B
#define W25Qxx_FAST_READ_DUAL_IO					0xBB
#define W25Qxx_FAST_READ_QUAD_OUTPUT				0x6B
#define W25Qxx_FAST_READ_QUAD_IO					0xEB
#define W25Qxx_OCTAL_WORD_READ_QUAD_IO				0xE3

#define W25Qxx_DUMMY_BYTE							0xFF 

typedef enum
{
    W25Qxx_MYSPI_Mode_Software = 1,
    W25Qxx_MYSPI_Mode_Hardware = 0
} W25Qxx_MYSPIMode_TypeDef;

typedef struct
{
    GPIO_TypeDef* W25Qxx_GPIOx;
    SPI_TypeDef* W25Qxx_SPIx;
    uint16_t W25Qxx_Pin_CS;
    uint16_t W25Qxx_Pin_MOSI;
    uint16_t W25Qxx_Pin_SCK;
    uint16_t W25Qxx_Pin_MISO;
    W25Qxx_MYSPIMode_TypeDef W25Qxx_MYSPI_Mode;
} W25Qxx_InitTypeDef;

void W25Qxx_Init(W25Qxx_InitTypeDef* W25Qxx_InitStruct);
void W25Qxx_Program(uint32_t Sector, uint32_t Index, uint32_t* pData, uint16_t Count);
void W25Qxx_Read(uint32_t Sector, uint32_t Index, uint32_t* pData, uint16_t Count);

#endif
