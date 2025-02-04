#include "main.h"

int main()
{
    Init(); // 系统初始化
    GPIOInit(); // gpio口初始化
    OLEDInit(); // oled屏幕初始化
    SerialInit(); // 串口初始化

    paintsleepicon(); // 绘制睡眠图标

    Encoder_Init(TIM4); // 编码器初始化

    GUI_Handle_t gui;
    gui_init(&gui); // gui初始化

    gui_setbuzzer(&gui, TIM3, OC3, 1500, 50); // 设置gui蜂鸣器

    guimenupageinit(&gui); // 菜单页初始化

    MPU6050_SoftwareInit(GPIOA, GPIO_Pin_12, GPIO_Pin_11, DISABLE); // 陀螺仪初始化
    MPU6050_Data_t data;

    SFTTIM_Handle_t* timer_return = SFTTIM_CreateTimer(200); // 陀螺仪返回操作定时器
    uint8_t returnflag = 0; // 陀螺仪返回标志

    Key_Handle_t* key = Key_Init(GPIOB, GPIO_Pin_5, 0); // 按键初始化

    wifiled = SFTTIM_CreateTimer(100); // wifi led灯定时器

    SFTTIM_Handle_t* datatimer = SFTTIM_CreateTimer(100); // 串口发送定时器

    while(1)
    {
        gui_proc(&gui);

        gui_encodercontrol(&gui, Encoder_DetectCntState(TIM4));

        int key_ret = Key_Scan(key, 500);
        if(key_ret == 1)
        {
            gui_sendsignal(&gui, GUIsignal_enter);
        }
        else if(key_ret == 2)
        {
            gui_sendsignal(&gui, GUIsignal_return);
        }

        if(gui.gyroflag == 1)
        {
            MPU6050_GetData(&data);
            if(MPU6050_DetectDown(&data, 0.4,  0.1))
            {
                gui_sendsignal(&gui, GUIsignal_scrolldown);
            }
            if(MPU6050_DetectUp(&data, 0.4,  0.1))
            {
                gui_sendsignal(&gui, GUIsignal_scrollup);
            }
            if(MPU6050_DetectLeft(&data, 0.4,  0.1))
            {
                gui_sendsignal(&gui, GUIsignal_scrollright);
            }
            if(MPU6050_DetectRight(&data, 0.4,  0.1))
            {
                gui_sendsignal(&gui, GUIsignal_scrollleft);
            }
            if(MPU6050_DetectDownShake(&data, 0.2,  0.1))
            {
                gui_sendsignal(&gui, GUIsignal_scrolldown);
            }
            if(MPU6050_DetectUpShake(&data, 0.2,  0.1))
            {
                gui_sendsignal(&gui, GUIsignal_scrollup);
            }
            if(MPU6050_DetectLeftShake(&data, 0.2,  0.1))
            {
                gui_sendsignal(&gui, GUIsignal_scrollright);
            }
            if(MPU6050_DetectRightShake(&data, 0.2,  0.1))
            {
                gui_sendsignal(&gui, GUIsignal_scrollleft);
            }

            if(SFTTIM_Ticking(timer_return) == 1)
            {
                gui_sendsignal(&gui, GUIsignal_enter);
                returnflag = 0;
                SFTTIMCmd(timer_return, DISABLE);
            }
            if(MPU6050_DetectShake(Z_N, &data, -0.8))
            {
                if(returnflag == 1)
                {
                    gui_sendsignal(&gui, GUIsignal_return);
                    returnflag = 0;
                    SFTTIMCmd(timer_return, DISABLE);
                }
                else
                {
                    SFTTIMCmd(timer_return, ENABLE);
                    returnflag = 1;
                }
            }
        }

        if(SFTTIM_Ticking(wifiled) == 1)
        {
            IO_Turn(GPIOB1);
        }
        if(Serial_CheckString("2") == 1)
        {
            wifiled->timerstate = DISABLE;
            IO_WriteBit(GPIOB1, Bit_SET);
            SFTTIMCmd(datatimer, ENABLE);
        }
        if(SFTTIM_Ticking(datatimer) == 1)
        {
            Serial_Printf(USART1, "*");
            int i;
            for(i = 0; i < 8; i++)
                Serial_SendFloat(USART1, MyADC_Convert(padcvalue[i], 3.3));
        }
    }
}
