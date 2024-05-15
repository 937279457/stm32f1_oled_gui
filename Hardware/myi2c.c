#include "myi2c.h"
#include "softwaretimer.h"
#include "delay.h"
#include "io.h"

#define TIMEOUT 5

static SFTTIM_Handle_t MYI2C_Timer;

int MYI2C_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT, uint16_t timeout)   // 返回1：超时 返回0：未超时
{
    while(1)
    {
        if(I2C_CheckEvent(I2Cx, I2C_EVENT) == 1)
        {
            MYI2C_Timer.timerstate = DISABLE;
            return 0;
        }
        if(MYI2C_Timer.timerstate == DISABLE)
        {
            MYI2C_Timer.timerstate = ENABLE;
            MYI2C_Timer.target = systick_count + timeout;
        }
        if(systick_count >= MYI2C_Timer.target)
        {
            MYI2C_Timer.timerstate = DISABLE;
            return 1;
        }
    }
}

static void MYI2C_WriteSCL(MYI2C_InitTypeDef* MYI2C_InitStruct, uint8_t Level)
{
    //GPIO_WriteBit(MYI2C_InitStruct->MYI2C_GPIOx, MYI2C_InitStruct->MYI2C_Pin_SCL, (BitAction)Level);
    if(Level != Bit_RESET)
        MYI2C_InitStruct->MYI2C_GPIOx->BSRR = MYI2C_InitStruct->MYI2C_Pin_SCL;
    else
        MYI2C_InitStruct->MYI2C_GPIOx->BRR = MYI2C_InitStruct->MYI2C_Pin_SCL;
}

static void MYI2C_WriteSDA(MYI2C_InitTypeDef* MYI2C_InitStruct, uint8_t Level)
{
    //GPIO_WriteBit(MYI2C_InitStruct->MYI2C_GPIOx, MYI2C_InitStruct->MYI2C_Pin_SDA, (BitAction)Level);
    if(Level != Bit_RESET)
        MYI2C_InitStruct->MYI2C_GPIOx->BSRR = MYI2C_InitStruct->MYI2C_Pin_SDA;
    else
        MYI2C_InitStruct->MYI2C_GPIOx->BRR = MYI2C_InitStruct->MYI2C_Pin_SDA;
}

static uint8_t MYI2C_ReadSDA(MYI2C_InitTypeDef* MYI2C_InitStruct)
{
    //return GPIO_ReadInputDataBit(MYI2C_InitStruct->MYI2C_GPIOx, MYI2C_InitStruct->MYI2C_Pin_SDA);
    if((MYI2C_InitStruct->MYI2C_GPIOx->IDR & MYI2C_InitStruct->MYI2C_Pin_SDA) != (uint32_t)Bit_RESET)
        return (uint8_t)Bit_SET;
    else
        return (uint8_t)Bit_RESET;
}

static void MYI2C_SendAck(MYI2C_InitTypeDef* MYI2C_InitStruct, uint8_t Ack)// 高电平：非应答 低电平：应答
{
    MYI2C_WriteSDA(MYI2C_InitStruct, Ack);
    MYI2C_WriteSCL(MYI2C_InitStruct, 1);
    MYI2C_WriteSCL(MYI2C_InitStruct, 0);
}

static uint8_t MYI2C_ReceiveAck(MYI2C_InitTypeDef* MYI2C_InitStruct)
{
    uint8_t Ack;
    MYI2C_WriteSDA(MYI2C_InitStruct, 1);// 主机切换为输入模式
    MYI2C_WriteSCL(MYI2C_InitStruct, 1);
    Ack = MYI2C_ReadSDA(MYI2C_InitStruct);
    MYI2C_WriteSCL(MYI2C_InitStruct, 0);
    return Ack;
}

void MYI2C_Init(MYI2C_InitTypeDef* MYI2C_InitStruct)
{
    if(MYI2C_InitStruct->MYI2C_Mode)
    {
        IO_GPIOInit(MYI2C_InitStruct->MYI2C_GPIOx, MYI2C_InitStruct->MYI2C_Pin_SCL | MYI2C_InitStruct->MYI2C_Pin_SDA, GPIO_Mode_Out_OD);
        
        GPIO_SetBits(MYI2C_InitStruct->MYI2C_GPIOx, MYI2C_InitStruct->MYI2C_Pin_SCL | MYI2C_InitStruct->MYI2C_Pin_SDA);
    }
    else
    {
        I2C_DeInit(MYI2C_InitStruct->MYI2C_I2Cx);
        if(MYI2C_InitStruct->MYI2C_I2Cx==I2C1)// PB6:SCL PB7:SDA
        {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
            IO_GPIOInit(GPIOB, GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_AF_OD);
        }
        else if(MYI2C_InitStruct->MYI2C_I2Cx==I2C2)// PB10:SCL PB11:SDA
        {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
            IO_GPIOInit(GPIOB, GPIO_Pin_10 | GPIO_Pin_11, GPIO_Mode_AF_OD);
        }
        I2C_InitTypeDef I2C_InitStructure;
        I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
        I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
        I2C_InitStructure.I2C_ClockSpeed = MYI2C_InitStruct->MYI2C_ClockSpeed;
        if(MYI2C_InitStruct->MYI2C_ClockSpeed >= 100000)
            I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
        else
            I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_16_9;
        I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
        I2C_InitStructure.I2C_OwnAddress1 = 0x00;
        I2C_Init(MYI2C_InitStruct->MYI2C_I2Cx, &I2C_InitStructure);
        
        I2C_Cmd(MYI2C_InitStruct->MYI2C_I2Cx, ENABLE);
    }
}

void MYI2C_Start(MYI2C_InitTypeDef* MYI2C_InitStruct)
{
    if(MYI2C_InitStruct->MYI2C_Mode == MYI2C_Mode_Software)
    {
        MYI2C_WriteSDA(MYI2C_InitStruct, 1);// 兼容起始条件
        MYI2C_WriteSCL(MYI2C_InitStruct, 1);
        MYI2C_WriteSDA(MYI2C_InitStruct, 0);
        MYI2C_WriteSCL(MYI2C_InitStruct, 0);
    }
    else
    {
        I2C_GenerateSTART(MYI2C_InitStruct->MYI2C_I2Cx, ENABLE);
        MYI2C_CheckEvent(MYI2C_InitStruct->MYI2C_I2Cx, I2C_EVENT_MASTER_MODE_SELECT, TIMEOUT);
//		while(!I2C_CheckEvent(MYI2C_InitStruct->MYI2C_I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
    }
}

void MYI2C_Stop(MYI2C_InitTypeDef* MYI2C_InitStruct)
{
    if(MYI2C_InitStruct->MYI2C_Mode == MYI2C_Mode_Software)
    {
        MYI2C_WriteSDA(MYI2C_InitStruct, 0);
        MYI2C_WriteSCL(MYI2C_InitStruct, 1);
        MYI2C_WriteSDA(MYI2C_InitStruct, 1);
    }
    else
    {
        I2C_GenerateSTOP(MYI2C_InitStruct->MYI2C_I2Cx, ENABLE);
    }
}

void MYI2C_SendAddress(MYI2C_InitTypeDef* MYI2C_InitStruct, uint8_t Address, uint8_t I2C_Direction)
{
    uint8_t i;
    if(MYI2C_InitStruct->MYI2C_Mode == MYI2C_Mode_Software)
    {
        if(I2C_Direction == I2C_Direction_Receiver)
            Address |= 0x01;
        for(i=0;i<8;i++)
        {
            MYI2C_WriteSDA(MYI2C_InitStruct, Address & (0x80>>i));
            MYI2C_WriteSCL(MYI2C_InitStruct, 1);
            MYI2C_WriteSCL(MYI2C_InitStruct, 0);
        }
        MYI2C_ReceiveAck(MYI2C_InitStruct);// 不处理从机应答
    }
    else
    {
        if(I2C_Direction == I2C_Direction_Transmitter)
        {
            I2C_Send7bitAddress(MYI2C_InitStruct->MYI2C_I2Cx, Address, I2C_Direction_Transmitter);
            MYI2C_CheckEvent(MYI2C_InitStruct->MYI2C_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED, TIMEOUT);
//			while(!I2C_CheckEvent(MYI2C_InitStruct->MYI2C_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
        }
        else
        {
            I2C_Send7bitAddress(MYI2C_InitStruct->MYI2C_I2Cx, Address, I2C_Direction_Receiver);
            MYI2C_CheckEvent(MYI2C_InitStruct->MYI2C_I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED, TIMEOUT);
//			while(!I2C_CheckEvent(MYI2C_InitStruct->MYI2C_I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
        }
    }
}

void MYI2C_SendData(MYI2C_InitTypeDef* MYI2C_InitStruct, uint8_t* pData, uint32_t Size)
{
    uint8_t i;
    uint32_t j;
    if(MYI2C_InitStruct->MYI2C_Mode == MYI2C_Mode_Software)
    {
        for(j=0;j<Size;j++)
        {
            for(i=0;i<8;i++)
            {
                MYI2C_WriteSDA(MYI2C_InitStruct, pData[j] & (0x80>>i));
                MYI2C_WriteSCL(MYI2C_InitStruct, 1);
                MYI2C_WriteSCL(MYI2C_InitStruct, 0);
            }
            MYI2C_ReceiveAck(MYI2C_InitStruct);// 不处理从机应答
        }
    }
    else
    {
        for(j=0;j<Size;j++)
        {
            I2C_SendData(MYI2C_InitStruct->MYI2C_I2Cx, pData[j]);
            if(j+1 != Size)
                MYI2C_CheckEvent(MYI2C_InitStruct->MYI2C_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED, TIMEOUT);
//				while(!I2C_CheckEvent(MYI2C_InitStruct->MYI2C_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
            else
                MYI2C_CheckEvent(MYI2C_InitStruct->MYI2C_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED, TIMEOUT);
//				while(!I2C_CheckEvent(MYI2C_InitStruct->MYI2C_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
        }
    }
}

void MYI2C_ReceiveData(MYI2C_InitTypeDef* MYI2C_InitStruct, uint8_t* pData, uint32_t Size)
{
    uint8_t i, RecvData;
    uint32_t j;
    if(MYI2C_InitStruct->MYI2C_Mode)
    {
        for(j=0;j<Size;j++)
        {
            RecvData = 0x00;
            MYI2C_WriteSDA(MYI2C_InitStruct, 1);// 主机切换为输入模式
            for(i=0;i<8;i++)
            {
                MYI2C_WriteSCL(MYI2C_InitStruct, 1);
                if(MYI2C_ReadSDA(MYI2C_InitStruct) == 1){RecvData |= (0x80>>i);}
                MYI2C_WriteSCL(MYI2C_InitStruct, 0);
            }
            if(j+1 != Size)
            {
                MYI2C_SendAck(MYI2C_InitStruct, 0);// 发送应答
            }
            pData[j] = RecvData;
        }
        MYI2C_SendAck(MYI2C_InitStruct, 1);// 发送非应答
    }
    else
    {
        for(j=0;j<Size;j++)
        {
            if(j+1 == Size)
            {
                I2C_AcknowledgeConfig(MYI2C_InitStruct->MYI2C_I2Cx, DISABLE);
                I2C_GenerateSTOP(MYI2C_InitStruct->MYI2C_I2Cx, ENABLE);
            }
            MYI2C_CheckEvent(MYI2C_InitStruct->MYI2C_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED, TIMEOUT);
//			while(!I2C_CheckEvent(MYI2C_InitStruct->MYI2C_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));
            pData[j] = I2C_ReceiveData(MYI2C_InitStruct->MYI2C_I2Cx);
        }
        I2C_AcknowledgeConfig(MYI2C_InitStruct->MYI2C_I2Cx, ENABLE);
    }
}

void MYI2C_Write(MYI2C_InitTypeDef* MYI2C_InitStruct, uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t* pData, uint32_t Size)
{
    MYI2C_Start(MYI2C_InitStruct);
    MYI2C_SendAddress(MYI2C_InitStruct, DeviceAddress, I2C_Direction_Transmitter);
    MYI2C_SendData(MYI2C_InitStruct, &RegisterAddress, 1);
    MYI2C_SendData(MYI2C_InitStruct, pData, Size);
    MYI2C_Stop(MYI2C_InitStruct);
}

void MYI2C_Read(MYI2C_InitTypeDef* MYI2C_InitStruct, uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t* pData, uint32_t Size)
{
    MYI2C_Start(MYI2C_InitStruct);
    MYI2C_SendAddress(MYI2C_InitStruct, DeviceAddress, I2C_Direction_Transmitter);
    MYI2C_SendData(MYI2C_InitStruct, &RegisterAddress, 1);
    MYI2C_Start(MYI2C_InitStruct);
    MYI2C_SendAddress(MYI2C_InitStruct, DeviceAddress, I2C_Direction_Receiver);
    MYI2C_ReceiveData(MYI2C_InitStruct, pData, Size);
    if(MYI2C_InitStruct->MYI2C_Mode == MYI2C_Mode_Software)
        MYI2C_Stop(MYI2C_InitStruct);
}

void MYI2C_WriteCmd(MYI2C_InitTypeDef* MYI2C_InitStruct, uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t Data)
{
    MYI2C_Write(MYI2C_InitStruct, DeviceAddress, RegisterAddress, &Data, 1);
}

uint8_t MYI2C_ReadData(MYI2C_InitTypeDef* MYI2C_InitStruct, uint8_t DeviceAddress, uint8_t RegisterAddress)
{
    uint8_t data = 0x00;
    MYI2C_Read(MYI2C_InitStruct, DeviceAddress, RegisterAddress, &data, 1);
    return data;
}
