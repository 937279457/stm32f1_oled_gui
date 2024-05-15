#include "myspi.h"
#include "softwaretimer.h"
#include "delay.h"
#include "io.h"

#define TIMEOUT 5

static SFTTIM_Handle_t MYSPI_Timer;

int MYSPI_GetFlagStatus(SPI_TypeDef* SPIx, uint16_t SPI_I2S_FLAG, uint16_t timeout) // 返回1：超时 返回0：未超时
{
    while(1)
    {
        if(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG) == 1)
        {
            MYSPI_Timer.timerstate = DISABLE;
            return 0;
        }
        if(MYSPI_Timer.timerstate == DISABLE)
        {
            MYSPI_Timer.timerstate = ENABLE;
            MYSPI_Timer.target = systick_count + timeout;
        }
        if(systick_count >= MYSPI_Timer.target)
        {
            MYSPI_Timer.timerstate = DISABLE;
            return 1;
        }
    }
}

static void MYSPI_W_CS(MYSPI_InitTypeDef* MYSPI_InitStruct, uint8_t Level)
{
    //GPIO_WriteBit(MYSPI_InitStruct->MYSPI_GPIOx, MYSPI_InitStruct->MYSPI_Pin_CS, (BitAction)Level);
    if(Level != Bit_RESET)
        MYSPI_InitStruct->MYSPI_GPIOx->BSRR = MYSPI_InitStruct->MYSPI_Pin_CS;
    else
        MYSPI_InitStruct->MYSPI_GPIOx->BRR = MYSPI_InitStruct->MYSPI_Pin_CS;
}

static void MYSPI_W_SCK(MYSPI_InitTypeDef* MYSPI_InitStruct, uint8_t Level)
{
    //GPIO_WriteBit(MYSPI_InitStruct->MYSPI_GPIOx, MYSPI_InitStruct->MYSPI_Pin_SCK, (BitAction)Level);
    if(Level != Bit_RESET)
        MYSPI_InitStruct->MYSPI_GPIOx->BSRR = MYSPI_InitStruct->MYSPI_Pin_SCK;
    else
        MYSPI_InitStruct->MYSPI_GPIOx->BRR = MYSPI_InitStruct->MYSPI_Pin_SCK;
}

static void MYSPI_W_MOSI(MYSPI_InitTypeDef* MYSPI_InitStruct, uint8_t Level)
{
    //GPIO_WriteBit(MYSPI_InitStruct->MYSPI_GPIOx, MYSPI_InitStruct->MYSPI_Pin_MOSI, (BitAction)Level);
    if(Level != Bit_RESET)
        MYSPI_InitStruct->MYSPI_GPIOx->BSRR = MYSPI_InitStruct->MYSPI_Pin_MOSI;
    else
        MYSPI_InitStruct->MYSPI_GPIOx->BRR = MYSPI_InitStruct->MYSPI_Pin_MOSI;
}

static uint8_t MYSPI_R_MISO(MYSPI_InitTypeDef* MYSPI_InitStruct)
{
    //return GPIO_ReadInputDataBit(MYSPI_InitStruct->MYSPI_GPIOx, MYSPI_InitStruct->MYSPI_Pin_MISO);
    if((MYSPI_InitStruct->MYSPI_GPIOx->IDR & MYSPI_InitStruct->MYSPI_Pin_MISO) != (uint32_t)Bit_RESET)
        return (uint8_t)Bit_SET;
    else
        return (uint8_t)Bit_RESET;
}

void MYSPI_Init(MYSPI_InitTypeDef* MYSPI_InitStruct)
{
    if(MYSPI_InitStruct->MYSPI_Mode)
    {
        IO_GPIOInit(MYSPI_InitStruct->MYSPI_GPIOx, MYSPI_InitStruct->MYSPI_Pin_CS | MYSPI_InitStruct->MYSPI_Pin_SCK | MYSPI_InitStruct->MYSPI_Pin_MOSI, GPIO_Mode_Out_PP);
        IO_GPIOInit(MYSPI_InitStruct->MYSPI_GPIOx, MYSPI_InitStruct->MYSPI_Pin_MISO, GPIO_Mode_IPU);
    
        MYSPI_W_CS(MYSPI_InitStruct, 1);
        MYSPI_W_SCK(MYSPI_InitStruct, 0);
    }
    else
    {
        SPI_I2S_DeInit(MYSPI_InitStruct->MYSPI_SPIx);
        if(MYSPI_InitStruct->MYSPI_SPIx==SPI1)// PA5-SCK PA6-MISO PA7-MOSI
        {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
            IO_GPIOInit(GPIOA, MYSPI_InitStruct->MYSPI_Pin_CS, GPIO_Mode_Out_PP);
            IO_GPIOInit(GPIOA, GPIO_Pin_5 | GPIO_Pin_7, GPIO_Mode_AF_PP);
            IO_GPIOInit(GPIOA, GPIO_Pin_6, GPIO_Mode_IPU);
        }
        else if(MYSPI_InitStruct->MYSPI_SPIx==SPI2)// PB13-SCK PB14-MISO PB15-MOSI
        {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
            IO_GPIOInit(GPIOB, MYSPI_InitStruct->MYSPI_Pin_CS, GPIO_Mode_Out_PP);
            IO_GPIOInit(GPIOB, GPIO_Pin_13 | GPIO_Pin_15, GPIO_Mode_AF_PP);
            IO_GPIOInit(GPIOB, GPIO_Pin_14, GPIO_Mode_IPU);
        }
        SPI_InitTypeDef SPI_InitStructure;
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CRCPolynomial = 7;
        SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
        SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
        SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
        SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
        SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
        SPI_Init(MYSPI_InitStruct->MYSPI_SPIx, &SPI_InitStructure);
    
        SPI_Cmd(MYSPI_InitStruct->MYSPI_SPIx, ENABLE);
    
        MYSPI_W_CS(MYSPI_InitStruct, 1);// 默认不选中从机
    }
}

void MYSPI_Start(MYSPI_InitTypeDef* MYSPI_InitStruct)
{
    MYSPI_W_CS(MYSPI_InitStruct, 0);
}

void MYSPI_Stop(MYSPI_InitTypeDef* MYSPI_InitStruct)
{
    MYSPI_W_CS(MYSPI_InitStruct, 1);
}

uint8_t MYSPI_SwapData(MYSPI_InitTypeDef* MYSPI_InitStruct, uint8_t Data)
{
    uint8_t i;
    uint8_t RecvData = 0x00;
    if(MYSPI_InitStruct->MYSPI_Mode)
    {
        for (i=0;i<8;i++)
        {
            MYSPI_W_MOSI(MYSPI_InitStruct, Data & (0x80>>i));
            MYSPI_W_SCK(MYSPI_InitStruct, 1);
            if (MYSPI_R_MISO(MYSPI_InitStruct)==1){RecvData |= (0x80>>i);}
            MYSPI_W_SCK(MYSPI_InitStruct, 0);
        }
        return Data;
    }
    else
    {
        MYSPI_GetFlagStatus(MYSPI_InitStruct->MYSPI_SPIx, SPI_I2S_FLAG_TXE, TIMEOUT);
//		while(!SPI_I2S_GetFlagStatus(MYSPI_InitStruct->MYSPI_SPIx, SPI_I2S_FLAG_TXE));
        SPI_I2S_SendData(MYSPI_InitStruct->MYSPI_SPIx, Data);
        MYSPI_GetFlagStatus(MYSPI_InitStruct->MYSPI_SPIx, SPI_I2S_FLAG_RXNE, TIMEOUT);
//		while(!SPI_I2S_GetFlagStatus(MYSPI_InitStruct->MYSPI_SPIx, SPI_I2S_FLAG_RXNE));
        return SPI_I2S_ReceiveData(MYSPI_InitStruct->MYSPI_SPIx);
    }
}

void MYSPI_SendData(MYSPI_InitTypeDef* MYSPI_InitStruct, uint8_t* pData, uint32_t Times)
{
    uint8_t i;
    uint32_t j;
    if(MYSPI_InitStruct->MYSPI_Mode)
    {
        for(j=0;j<Times;j++)
        {
            for (i=0;i<8;i++)
            {		
                MYSPI_W_MOSI(MYSPI_InitStruct, *(pData+j) & (0x80>>i));
                MYSPI_W_SCK(MYSPI_InitStruct, 1);
                MYSPI_W_SCK(MYSPI_InitStruct, 0);
            }
        }
    }
    else
    {
        for(j=0;j<Times;j++)
        {
            MYSPI_GetFlagStatus(MYSPI_InitStruct->MYSPI_SPIx, SPI_I2S_FLAG_TXE, TIMEOUT);
//			while(!SPI_I2S_GetFlagStatus(MYSPI_InitStruct->MYSPI_SPIx, SPI_I2S_FLAG_TXE));
            SPI_I2S_SendData(MYSPI_InitStruct->MYSPI_SPIx, *(pData+j));
        }
    }
}
