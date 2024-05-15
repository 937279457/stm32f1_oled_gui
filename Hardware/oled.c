#include "OLED.h"

OLED_Handle_t* oled;

static uint8_t OLEDCmd  = 0x00;
static uint8_t OLEDData = 0x40;
static uint8_t DMABusyflag = 0;
static uint8_t TCflag = 0;      // 传输完成标志

static uint8_t OLED_GDDRAM[OLED_BUFFER_SIZE];

void OLED_WR_Cmd(OLED_InitTypeDef* OLED_InitStruct, uint8_t Cmd)
{
    if(OLED_InitStruct->OLED_Mode)
    {
        GPIO_ResetBits(OLED_InitStruct->MYSPI_ConfigStruct->MYSPI_GPIOx, OLED_InitStruct->OLED_MYSPI_Pin_DC);
        MYSPI_Start(OLED_InitStruct->MYSPI_ConfigStruct);
        MYSPI_SwapData(OLED_InitStruct->MYSPI_ConfigStruct, Cmd);
        MYSPI_Stop(OLED_InitStruct->MYSPI_ConfigStruct);
    }
    else
    {
        MYI2C_Write(OLED_InitStruct->MYI2C_ConfigStruct, OLED_ADDRESS, OLEDCmd, &Cmd, 1);
    }
}

void OLED_WR_Data(OLED_InitTypeDef* OLED_InitStruct, uint8_t* pData)
{
    if(OLED_InitStruct->OLED_Mode)
    {
        GPIO_SetBits(OLED_InitStruct->MYSPI_ConfigStruct->MYSPI_GPIOx, OLED_InitStruct->OLED_MYSPI_Pin_DC);
        MYSPI_Start(OLED_InitStruct->MYSPI_ConfigStruct);
        SPI_I2S_DMACmd(OLED_InitStruct->MYSPI_ConfigStruct->MYSPI_SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
        MYSPI_SendData(OLED_InitStruct->MYSPI_ConfigStruct, pData, OLED_BUFFER_SIZE);
        MYSPI_Stop(OLED_InitStruct->MYSPI_ConfigStruct);
    }
    else
    {
        MYI2C_Write(OLED_InitStruct->MYI2C_ConfigStruct, OLED_ADDRESS, OLEDData, pData, OLED_BUFFER_SIZE);
    }
}

uint8_t* OLED_GetBuffer(void)
{
    return oled->buffer;
}

static void OLED_Reset(OLED_InitTypeDef* OLED_InitStruct)
{
    GPIO_SetBits(OLED_InitStruct->MYSPI_ConfigStruct->MYSPI_GPIOx, OLED_InitStruct->OLED_MYSPI_Pin_RST);
    Delay_ms(100);
    GPIO_ResetBits(OLED_InitStruct->MYSPI_ConfigStruct->MYSPI_GPIOx, OLED_InitStruct->OLED_MYSPI_Pin_RST);
    Delay_ms(100);
    GPIO_SetBits(OLED_InitStruct->MYSPI_ConfigStruct->MYSPI_GPIOx, OLED_InitStruct->OLED_MYSPI_Pin_RST); 
}

static void OLED_DMAShutdown(void)
{
    while(DMABusyflag != 0);
    // DMA传输完成后必须检查I2C通信是否传输完成
    if(TCflag == 0)
    {
        if(oled->initstruct->OLED_Mode)
            MYSPI_GetFlagStatus(oled->initstruct->MYSPI_ConfigStruct->MYSPI_SPIx, SPI_I2S_FLAG_TXE, 5);
        else
            MYI2C_CheckEvent(oled->initstruct->MYI2C_ConfigStruct->MYI2C_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED, 5);
        TCflag = 1;
    }
}

static OLED_Handle_t* OLED_CreateScreen(OLED_InitTypeDef* OLED_InitStruct)
{
    OLED_Handle_t* screen = malloc(sizeof(OLED_Handle_t));
    screen->buffer = malloc(OLED_BUFFER_SIZE);
    screen->initstruct = OLED_InitStruct;
    screen->DMA = NULL;
    return screen;
}

static void OLED_DMAISRHandler(void)
{
    DMABusyflag = 0;
}

static OLED_Handle_t* OLED_BaseInit(OLED_InitTypeDef* OLED_InitStruct)
{
    OLED_Handle_t* screen = OLED_CreateScreen(OLED_InitStruct);
    if(OLED_InitStruct->OLED_Mode)
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Pin = OLED_InitStruct->OLED_MYSPI_Pin_DC | OLED_InitStruct->OLED_MYSPI_Pin_RST;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(OLED_InitStruct->MYSPI_ConfigStruct->MYSPI_GPIOx, &GPIO_InitStructure);
        
        GPIO_WriteBit(OLED_InitStruct->MYSPI_ConfigStruct->MYSPI_GPIOx, OLED_InitStruct->MYSPI_ConfigStruct->MYSPI_Pin_CS, Bit_SET);
        GPIO_WriteBit(OLED_InitStruct->MYSPI_ConfigStruct->MYSPI_GPIOx, OLED_InitStruct->MYSPI_ConfigStruct->MYSPI_Pin_SCK, Bit_RESET);
        
        OLED_Reset(OLED_InitStruct);
    }
    else
        Delay_ms(100);
            
    OLED_WR_Cmd(OLED_InitStruct, 0xAE); // 关闭显示
    OLED_WR_Cmd(OLED_InitStruct, 0x81); // 设置对比度（0x00~0xFF）
    OLED_WR_Cmd(OLED_InitStruct, 0xFF);
    OLED_WR_Cmd(OLED_InitStruct, 0xA4); // 启用输出GDDRAM中的内容 0xA5:忽略GDDRAM中的内容
    OLED_WR_Cmd(OLED_InitStruct, 0xA6); // 正常显示 0xA6:1亮0灭 0xA7:0亮1灭
    OLED_WR_Cmd(OLED_InitStruct, 0x20); // 设置寻址模式 0x00:水平寻址 0x01:垂直寻址 0x02:页寻址（默认模式）
    OLED_WR_Cmd(OLED_InitStruct, 0x00);
    OLED_WR_Cmd(OLED_InitStruct, 0x21); // 设置列起始地址（0~127）
    OLED_WR_Cmd(OLED_InitStruct, 0x00);
    OLED_WR_Cmd(OLED_InitStruct, 0x7F);
    OLED_WR_Cmd(OLED_InitStruct, 0x22); // 设置行起始地址（0~7）
    OLED_WR_Cmd(OLED_InitStruct, 0x00);
    OLED_WR_Cmd(OLED_InitStruct, 0x07);
    OLED_WR_Cmd(OLED_InitStruct, 0x40); // 设置显示开始行
//    水平寻址下使用无效，该命令仅对页寻址模式有效
//    OLED_WR_Cmd(OLED_InitStruct, 0xB0 | 0);                 // 设置Y位置
//    OLED_WR_Cmd(OLED_InitStruct, 0x10 | ((0 & 0xF0) >> 4)); // 设置X位置高4位
//    OLED_WR_Cmd(OLED_InitStruct, 0x00 | (0 & 0x0F));        // 设置X位置低4位
    OLED_WR_Cmd(OLED_InitStruct, 0xA1); // 设置左右方向 0xA0:左右反置
    OLED_WR_Cmd(OLED_InitStruct, 0xC8); // 设置上下方向 0xC0:上下反置
    OLED_WR_Cmd(OLED_InitStruct, 0xA8); // 设置多路复用率（复用率：屏幕显示的结束行数）
    OLED_WR_Cmd(OLED_InitStruct, 0x3F);
    OLED_WR_Cmd(OLED_InitStruct, 0XD3); // 设置显示偏移量（偏移量：屏幕向上移动的行数）
    OLED_WR_Cmd(OLED_InitStruct, 0X00); 
    OLED_WR_Cmd(OLED_InitStruct, 0xDA); // 设置COM引脚硬件配置
    OLED_WR_Cmd(OLED_InitStruct, 0x12);
    OLED_WR_Cmd(OLED_InitStruct, 0xD5); // 设置显示时钟分频比例和振荡器频率
    OLED_WR_Cmd(OLED_InitStruct, 0x80);
    OLED_WR_Cmd(OLED_InitStruct, 0xD9); // 设置预充电周期
    OLED_WR_Cmd(OLED_InitStruct, 0XF1);  
    OLED_WR_Cmd(OLED_InitStruct, 0xDB); // 设置VCOMH取消选择级别
    OLED_WR_Cmd(OLED_InitStruct, 0x30);
    OLED_WR_Cmd(OLED_InitStruct, 0x8D); // 启用电荷泵
    OLED_WR_Cmd(OLED_InitStruct, 0x14);  
    OLED_WR_Cmd(OLED_InitStruct, 0xAF); // 开启显示
    
    if(OLED_InitStruct->MYI2C_ConfigStruct->MYI2C_Mode == MYI2C_Mode_Software||
            OLED_InitStruct->MYSPI_ConfigStruct->MYSPI_Mode == MYSPI_Mode_Software)
    {
        OLED_InitStruct->OLED_DMACmd = DISABLE;
    }
    
    if(OLED_InitStruct->OLED_DMACmd)
    {
        static MYDMA_InitTypeDef MYDMA_InitStructure;
        MYDMA_InitStructure.MYDMA_BufferSize = OLED_BUFFER_SIZE;
        MYDMA_InitStructure.MYDMA_DIR = DMA_DIR_PeripheralDST;
        if(OLED_InitStruct->OLED_Mode)
        {
            if(OLED_InitStruct->MYSPI_ConfigStruct->MYSPI_SPIx==SPI1)
                MYDMA_InitStructure.MYDMA_DMAy_Channelx = DMA1_Channel3;
            if(OLED_InitStruct->MYSPI_ConfigStruct->MYSPI_SPIx==SPI2)
                MYDMA_InitStructure.MYDMA_DMAy_Channelx = DMA1_Channel5;
            MYDMA_InitStructure.MYDMA_PeripheralBaseAddr = (uint32_t)&OLED_InitStruct->MYSPI_ConfigStruct->MYSPI_SPIx->DR;
            
            GPIO_SetBits(OLED_InitStruct->MYSPI_ConfigStruct->MYSPI_GPIOx, OLED_InitStruct->OLED_MYSPI_Pin_DC);
            MYSPI_Start(OLED_InitStruct->MYSPI_ConfigStruct);
            SPI_I2S_DMACmd(OLED_InitStruct->MYSPI_ConfigStruct->MYSPI_SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
        }
        else
        {
            MYI2C_Start(OLED_InitStruct->MYI2C_ConfigStruct);
            MYI2C_SendAddress(OLED_InitStruct->MYI2C_ConfigStruct, OLED_ADDRESS, I2C_Direction_Transmitter);
            MYI2C_SendData(OLED_InitStruct->MYI2C_ConfigStruct, &OLEDData, 1);
            
            if(OLED_InitStruct->MYI2C_ConfigStruct->MYI2C_I2Cx==I2C1)
                MYDMA_InitStructure.MYDMA_DMAy_Channelx = DMA1_Channel6;
            if(OLED_InitStruct->MYI2C_ConfigStruct->MYI2C_I2Cx==I2C2)
                MYDMA_InitStructure.MYDMA_DMAy_Channelx = DMA1_Channel4;
            MYDMA_InitStructure.MYDMA_PeripheralBaseAddr = (uint32_t)&OLED_InitStruct->MYI2C_ConfigStruct->MYI2C_I2Cx->DR;
            
            I2C_DMACmd(OLED_InitStruct->MYI2C_ConfigStruct->MYI2C_I2Cx, ENABLE);
        }
        MYDMA_InitStructure.MYDMA_IT_Cmd = ENABLE;
        MYDMA_InitStructure.MYDMA_MemoryBaseAddr = (uint32_t)OLED_GDDRAM;
        MYDMA_InitStructure.MYDMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
        MYDMA_InitStructure.MYDMA_MemoryInc = DMA_MemoryInc_Enable;
        MYDMA_InitStructure.MYDMA_Mode = DMA_Mode_Normal;
        MYDMA_InitStructure.MYDMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        MYDMA_InitStructure.MYDMA_PeripheralInc = DMA_PeripheralInc_Disable;
        MYDMA_InitStructure.MYDMA_NVIC_PPriority = 2;
        MYDMA_InitStructure.MYDMA_NVIC_SPriority = 2;
        
        MYDMA_SetIRQHandler(MYDMA_InitStructure.MYDMA_DMAy_Channelx, OLED_DMAISRHandler);
        DMABusyflag = 1;
        TCflag = 0;
        
        MYDMA_Init(&MYDMA_InitStructure);
        
        screen->DMA = &MYDMA_InitStructure;
    }
    
    int i;
    for(i = 0; i < OLED_BUFFER_SIZE; i++)
    {
        screen->buffer[i] = 0x00;
    }
    
    return screen;
}

OLED_Handle_t* OLED_SoftwareI2CInit(GPIO_TypeDef* GPIOx, uint16_t SCL, uint16_t SDA)
{
    MYI2C_InitTypeDef* MYI2C_InitStructure = malloc(sizeof(MYI2C_InitTypeDef));
    MYI2C_InitStructure->MYI2C_Mode = MYI2C_Mode_Software;
    MYI2C_InitStructure->MYI2C_Pin_SCL = SCL;
    MYI2C_InitStructure->MYI2C_Pin_SDA = SDA;
    MYI2C_InitStructure->MYI2C_GPIOx = GPIOx;
    MYI2C_Init(MYI2C_InitStructure);
    
    OLED_InitTypeDef* OLED_InitStructure = malloc(sizeof(OLED_InitTypeDef));
    OLED_InitStructure->MYI2C_ConfigStruct = MYI2C_InitStructure;
    OLED_InitStructure->OLED_Mode = OLED_Mode_I2C;
    OLED_InitStructure->OLED_DMACmd = DISABLE;
    return OLED_BaseInit(OLED_InitStructure);
}

OLED_Handle_t* OLED_SoftwareSPIInit(GPIO_TypeDef* GPIOx, uint16_t MOSI, uint16_t SCK, uint16_t RST, uint16_t DC, uint16_t CS)
{
    MYSPI_InitTypeDef* MYSPI_InitStructure = malloc(sizeof(MYSPI_InitTypeDef));
    MYSPI_InitStructure->MYSPI_GPIOx = GPIOx;
    MYSPI_InitStructure->MYSPI_Mode = MYSPI_Mode_Software;
    MYSPI_InitStructure->MYSPI_Pin_CS = CS;
    MYSPI_InitStructure->MYSPI_Pin_MOSI = MOSI;
    MYSPI_InitStructure->MYSPI_Pin_SCK = SCK;
    MYSPI_Init(MYSPI_InitStructure);
    
    OLED_InitTypeDef* OLED_InitStructure = malloc(sizeof(OLED_InitTypeDef));
    OLED_InitStructure->MYSPI_ConfigStruct = MYSPI_InitStructure;
    OLED_InitStructure->OLED_Mode = OLED_Mode_SPI;
    OLED_InitStructure->OLED_MYSPI_Pin_DC = DC;
    OLED_InitStructure->OLED_MYSPI_Pin_RST = RST;
    OLED_InitStructure->OLED_DMACmd = DISABLE;
    return OLED_BaseInit(OLED_InitStructure);
}

OLED_Handle_t* OLED_I2CInit(I2C_TypeDef* I2Cx, FunctionalState DMACmd)
{
    MYI2C_InitTypeDef* MYI2C_InitStructure = malloc(sizeof(MYI2C_InitTypeDef));
    MYI2C_InitStructure->MYI2C_I2Cx = I2Cx;
    MYI2C_InitStructure->MYI2C_Mode = MYI2C_Mode_Hardware;
    MYI2C_InitStructure->MYI2C_ClockSpeed = 1000000; // 经过测试，设置为该时钟频率时可大幅提升屏幕帧率
    MYI2C_Init(MYI2C_InitStructure);
    
    OLED_InitTypeDef* OLED_InitStructure = malloc(sizeof(OLED_InitTypeDef));
    OLED_InitStructure->MYI2C_ConfigStruct = MYI2C_InitStructure;
    OLED_InitStructure->OLED_Mode = OLED_Mode_I2C;
    OLED_InitStructure->OLED_DMACmd = DMACmd;
    return OLED_BaseInit(OLED_InitStructure);
}

OLED_Handle_t* OLED_SPIInit(SPI_TypeDef* SPIx, FunctionalState DMACmd, GPIO_TypeDef* GPIOx, uint16_t RST, uint16_t DC, uint16_t CS)
{
    MYSPI_InitTypeDef* MYSPI_InitStructure = malloc(sizeof(MYSPI_InitTypeDef));
    MYSPI_InitStructure->MYSPI_GPIOx = GPIOx;
    MYSPI_InitStructure->MYSPI_Mode = MYSPI_Mode_Hardware;
    MYSPI_InitStructure->MYSPI_Pin_CS = CS;
    MYSPI_InitStructure->MYSPI_SPIx = SPIx;
    MYSPI_Init(MYSPI_InitStructure);
    
    OLED_InitTypeDef* OLED_InitStructure = malloc(sizeof(OLED_InitTypeDef));
    OLED_InitStructure->MYSPI_ConfigStruct = MYSPI_InitStructure;
    OLED_InitStructure->OLED_Mode = OLED_Mode_SPI;
    OLED_InitStructure->OLED_MYSPI_Pin_DC = DC;
    OLED_InitStructure->OLED_MYSPI_Pin_RST = RST;
    OLED_InitStructure->OLED_DMACmd = DMACmd;
    return OLED_BaseInit(OLED_InitStructure);
}

void OLED_Init(OLED_Handle_t* oled_handle)
{
    oled = oled_handle;
    GM_SetBufferPointer((char*)oled->buffer);
}

uint8_t OLED_Display(void)
{
    uint16_t i;
    if(oled->initstruct->OLED_DMACmd != ENABLE)
    {
        OLED_WR_Data(oled->initstruct, oled->buffer);
    }
    else
    {
        if(DMABusyflag == 0)
        {
            for(i=0;i<OLED_BUFFER_SIZE;i++)
                OLED_GDDRAM[i] = oled->buffer[i];
            if(oled->initstruct->OLED_Mode)
            {
                GPIO_SetBits(oled->initstruct->MYSPI_ConfigStruct->MYSPI_GPIOx, oled->initstruct->OLED_MYSPI_Pin_DC);
                MYSPI_Start(oled->initstruct->MYSPI_ConfigStruct);
            }
            else
            {
                MYI2C_Start(oled->initstruct->MYI2C_ConfigStruct);
                MYI2C_SendAddress(oled->initstruct->MYI2C_ConfigStruct, OLED_ADDRESS, I2C_Direction_Transmitter);
                MYI2C_SendData(oled->initstruct->MYI2C_ConfigStruct, &OLEDData, 1);
            }
            DMABusyflag = 1;
            TCflag = 0;
            MYDMA_Transfer(oled->DMA->MYDMA_DMAy_Channelx, OLED_BUFFER_SIZE);
        }
        else
            return 0;
    }
    return 1;
}

void OLED_SetBrightness(uint8_t brightness)
{
    if(oled->initstruct->OLED_DMACmd == ENABLE)
    {
        OLED_DMAShutdown();
        OLED_WR_Cmd(oled->initstruct, 0x81);
        OLED_WR_Cmd(oled->initstruct, brightness);
    }
    else
    {
        OLED_WR_Cmd(oled->initstruct, 0x81);
        OLED_WR_Cmd(oled->initstruct, brightness);
    }
}

void OLED_SetColorMode(uint8_t mode)
{
    uint8_t cmd = 0x00;
    if(mode == 1)
        cmd = 0xA6;
    else
        cmd = 0xA7;
    if(oled->initstruct->OLED_DMACmd == ENABLE)
    {
        OLED_DMAShutdown();
        OLED_WR_Cmd(oled->initstruct, cmd);
    }
    else
    {
        OLED_WR_Cmd(oled->initstruct, cmd);
    }
}

void OLED_SetHold(FunctionalState HoldCmd)
{
    uint8_t cmd = 0x00;
    if(HoldCmd == ENABLE)
        cmd = 0xA5;
    else
        cmd = 0xA4;
    if(oled->initstruct->OLED_DMACmd == ENABLE)
    {
        OLED_DMAShutdown();
        OLED_WR_Cmd(oled->initstruct, cmd);
    }
    else
    {
        OLED_WR_Cmd(oled->initstruct, cmd);
    }
}

void OLED_SetSleep(FunctionalState SleepCmd)
{
    uint8_t cmd1 = 0x00;
    uint8_t cmd2 = 0x00;
    if(SleepCmd == ENABLE)
    {
        cmd1 = 0xAE;
        cmd2 = 0x10;
    }
    else
    {
        cmd1 = 0xAF;
        cmd2 = 0x14;
    }
    if(oled->initstruct->OLED_DMACmd == ENABLE)
    {
        OLED_DMAShutdown();
        OLED_WR_Cmd(oled->initstruct, 0x8D);
        OLED_WR_Cmd(oled->initstruct, cmd2);
        OLED_WR_Cmd(oled->initstruct, cmd1);
    }
    else
    {
        OLED_WR_Cmd(oled->initstruct, 0x8D);
        OLED_WR_Cmd(oled->initstruct, cmd2);
        OLED_WR_Cmd(oled->initstruct, cmd1);
    }
}
