#include "stm32f10x.h"
#include "delay.h"
#include "ic.h"
#include "io.h"
#include "key.h"
#include "mydma.h"
#include "myi2c.h"
#include "myspi.h"
#include "oled.h"
#include "pwm.h"
#include "serial.h"
#include "timer.h"
#include "myadc.h"
#include "myexti.h"
#include "gui.h"
#include "bkp.h"
#include "myrtc.h"
#include "lowpower.h"
#include "myflash.h"
#include "wdg.h"
#include "w25qxx.h"
#include "mpu6050.h"
#include "encoder.h"
#include "softwaretimer.h"
#include "buzzer.h"
#include "guilist.h"
#include "guimenu.h"
#include "image.h"
#include "guiadc.h"

uint32_t* padcvalue = NULL;

void Init(void)
{
    SystemInit();                                   // 系统初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 配置NVIC
    Delay_SystickStart();                           // 启动系统滴答定时器
}

void OLEDInit(void)
{
    OLED_Init(OLED_I2CInit(I2C2, ENABLE));
}

void GPIOInit(void)
{
    IO_Init(GPIOB5, GPIO_Mode_IPU);
    IO_Init(GPIOB6, GPIO_Mode_IPU);
    IO_Init(GPIOB7, GPIO_Mode_IPU);
    IO_Init(GPIOB1, GPIO_Mode_Out_PP);
}

void ADCInit(void)
{
    MyADC_InitTypeDef MyADC_InitStructure;
    MyADC_InitStructure.MyADC_ADCx = ADC1;
    MyADC_InitStructure.MyADC_ContinuousConvMode = ENABLE;
    MyADC_InitStructure.MyADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    MyADC_InitStructure.MyADC_GPIOx = GPIOA;
    MyADC_InitStructure.MyADC_NbrOfChannel = 8;
    MyADC_InitStructure.MyADC_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    MyADC_InitStructure.MyADC_ScanConvMode = ENABLE;
    MyADC_Init(&MyADC_InitStructure);
    
    MyADC_SetChannel(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
    MyADC_SetChannel(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);
    MyADC_SetChannel(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5);
    MyADC_SetChannel(ADC1, ADC_Channel_3, 4, ADC_SampleTime_239Cycles5);
    MyADC_SetChannel(ADC1, ADC_Channel_4, 5, ADC_SampleTime_239Cycles5);
    MyADC_SetChannel(ADC1, ADC_Channel_5, 6, ADC_SampleTime_239Cycles5);
    MyADC_SetChannel(ADC1, ADC_Channel_6, 7, ADC_SampleTime_239Cycles5);
    MyADC_SetChannel(ADC1, ADC_Channel_7, 8, ADC_SampleTime_239Cycles5);
    
    ADC_DMACmd(ADC1, ENABLE);
    
    MyADC_SoftwareStart(ADC1);
}

void ADCDMAInit(uint32_t* addr, uint8_t size)
{
    MYDMA_InitTypeDef MYDMA_InitStructure;
    MYDMA_InitStructure.MYDMA_BufferSize = size;
    MYDMA_InitStructure.MYDMA_DIR = DMA_DIR_PeripheralSRC;
    MYDMA_InitStructure.MYDMA_DMAy_Channelx = DMA1_Channel1;
    MYDMA_InitStructure.MYDMA_IT_Cmd = DISABLE;
    MYDMA_InitStructure.MYDMA_MemoryBaseAddr = (uint32_t)addr;
    MYDMA_InitStructure.MYDMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    MYDMA_InitStructure.MYDMA_MemoryInc = DMA_MemoryInc_Enable;
    MYDMA_InitStructure.MYDMA_Mode = DMA_Mode_Circular;
    MYDMA_InitStructure.MYDMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    MYDMA_InitStructure.MYDMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    MYDMA_InitStructure.MYDMA_PeripheralInc = DMA_PeripheralInc_Disable;
    MYDMA_Init(&MYDMA_InitStructure);
}

void SerialInit(void)
{
    Serial_InitTypeDef serial;
    serial.Serial_IT_Cmd = ENABLE;
    serial.Serial_NVIC_PPriority = 1;
    serial.Serial_NVIC_SPriority = 1;
    serial.Serial_USARTx = USART1;
    serial.Serial_USART_BaudRate = 115200;
    Serial_Init(&serial);
    Serial_SetPHead_PEnd('*', '*');
}

void paintsleepicon(void)
{
    GM_SetBuffer(0x00);
    GM_DrawBox(0, 0, 29, 29, GM_Color_Bright, GM_Color_Bright);
    GM_DrawDisc(15, 15, 10, GM_Color_Dark, GM_Color_Dark);
    GM_DrawDisc(10, 12, 7, GM_Color_Bright, GM_Color_Bright);
    GM_StoreImage(0, 0, sleep_icon, 30, 30);
    GM_SetBuffer(0x00);
}

GUIpage_t* guiaboutpageinit(GUI_Handle_t* gui)
{
    GUIpage_t* aboutpage = gui_createlistpage(15);
    gui_additemtolist(aboutpage, "    ABOUT    ", itemtype_string);
    gui_additemtolist(aboutpage, "author:", itemtype_string);
    gui_additemtolist(aboutpage, "Yanzhijian", itemtype_string);
    gui_additemtolist(aboutpage, "version:2.2.0", itemtype_string);
    gui_additemtolist(aboutpage, "build date:", itemtype_string);
    gui_additemtolist(aboutpage, "2024.4.2", itemtype_string);
    gui_additemtolist(aboutpage, "board:", itemtype_string);
    gui_additemtolist(aboutpage, "stm32f103c8t6", itemtype_string);
    gui_additemtolist(aboutpage, "ram:20KB", itemtype_string);
    gui_additemtolist(aboutpage, "rom:64KB", itemtype_string);
    gui_additemtolist(aboutpage, "screen:", itemtype_string);
    gui_additemtolist(aboutpage, "0.96inch oled", itemtype_string);
    
    return aboutpage;
}

void switchgyromode(GUI_Handle_t* gui)
{
    gui->gyroflag = !gui->gyroflag;
}

GUIpage_t* guisettingpageinit(GUI_Handle_t* gui)
{
    GUIpage_t* settingpage = gui_createlistpage(10);
    gui_additemtolist(settingpage, "   SETTING   ", itemtype_string);
    
    GUIwidget_t* white_mode = gui_additemtolist(settingpage, "white mode", itemtype_radio);
    gui_settrigger(white_mode, (void(*)(void*))gui_switchcolormode);
    
    GUIwidget_t* ani_rate = gui_additemtolist(settingpage, "ani rate", itemtype_window);
    gui_setlistitemrange(ani_rate, 0, 10);
    gui_setnumpoint(ani_rate, &gui->animationrate);
    
    GUIwidget_t* fade_rate = gui_additemtolist(settingpage, "fade rate", itemtype_window);
    gui_setlistitemrange(fade_rate, 0, 10);
    gui_setnumpoint(fade_rate, &gui->faderate);
    
    GUIwidget_t* brightness = gui_additemtolist(settingpage, "brightness", itemtype_window);
    gui_setlistitemrange(brightness, 0, 100);
    gui_setnumpoint(brightness, &gui->brightness);
    gui_settrigger(brightness, (void(*)(void*))gui_setbrightness);
    
    GUIwidget_t* jelly = gui_additemtolist(settingpage, "jelly", itemtype_radio);
    gui_settrigger(jelly, (void(*)(void*))gui_switchjellyeffect);
    
    GUIwidget_t* gyroscope = gui_additemtolist(settingpage, "gyroscope", itemtype_radio);
    gui_settrigger(gyroscope, (void(*)(void*))switchgyromode);
    
    GUIwidget_t* sound = gui_additemtolist(settingpage, "sound", itemtype_radio);
    gui_radiocmd(sound);
    gui_settrigger(sound, (void(*)(void*))gui_switchsoundstate);
    
    GUIpage_t* aboutpage = guiaboutpageinit(gui);
    aboutpage->lastpage = settingpage;
    gui_setnextpage(gui_additemtolist(settingpage, "about", itemtype_string), aboutpage);
    
    return settingpage;
}

SFTTIM_Handle_t* wifiled;
void wifiopen(void)
{
    SFTTIMCmd(wifiled, ENABLE);
    Serial_SendStr(USART1, "1");
}

GUIpage_t* guiwifipageinit(GUI_Handle_t* gui)
{
    GUIpage_t* wifipage = gui_createlistpage(10);
    gui_additemtolist(wifipage, "     WiFi    ", itemtype_string);
    gui_settrigger(gui_additemtolist(wifipage, "open wifi", itemtype_radio), (void(*)(void*))wifiopen);
    gui_additemtolist(wifipage, "wifi state:", itemtype_string);
    gui_additemtolist(wifipage, "offline", itemtype_string);
    gui_additemtolist(wifipage, "ssid:", itemtype_string);
    gui_additemtolist(wifipage, "mywifi", itemtype_string);
    gui_additemtolist(wifipage, "password:", itemtype_string);
    gui_additemtolist(wifipage, "12345678", itemtype_string);
    gui_additemtolist(wifipage, "ipv4:", itemtype_string);
    gui_additemtolist(wifipage, "0.0.0.0", itemtype_string);
    
    return wifipage;
}

GUIpage_t* guiadcpageinit(GUI_Handle_t* gui)
{
    GUIpage_t* adcpage = gui_createadcpage(8);
    gui_additemtoadc(adcpage, 0);
    gui_additemtoadc(adcpage, 1);
    gui_additemtoadc(adcpage, 2);
    gui_additemtoadc(adcpage, 3);
    gui_additemtoadc(adcpage, 4);
    gui_additemtoadc(adcpage, 5);
    gui_additemtoadc(adcpage, 6);
    gui_additemtoadc(adcpage, 7);
    
    ADCInit();
    ADCDMAInit(((GUIadc_t*)(adcpage->param))->adcvalue, 8);
    padcvalue = ((GUIadc_t*)(adcpage->param))->adcvalue;
    
    return adcpage;
}

GUIpage_t* guivoltpageinit(GUI_Handle_t* gui)
{
    GUIpage_t* voltpage = gui_createlistpage(5);
    gui_additemtolist(voltpage, "     VOLT    ", itemtype_string);
    
    GUIpage_t* adcpage = guiadcpageinit(gui);
    adcpage->lastpage = voltpage;
    gui_setnextpage(gui_additemtolist(voltpage, "adc", itemtype_string), adcpage);
    
    gui_additemtolist(voltpage, "convert", itemtype_string);
    
    return voltpage;
}

GUIpage_t* guimenupageinit(GUI_Handle_t* gui)
{
    GUIpage_t* menupage = gui_createmenupage(10);
    
    GUIpage_t* voltpage = guivoltpageinit(gui);
    voltpage->lastpage = menupage;
    gui_setnextpage(gui_additemtomenu(menupage, (char*)volt_icon, "Volt"), voltpage);
    
    gui_additemtomenu(menupage, (char*)serial_icon, "Serial");
    gui_additemtomenu(menupage, (char*)clock_icon, "Clock");
    
    GUIpage_t* wifipage = guiwifipageinit(gui);
    wifipage->lastpage = menupage;
    gui_setnextpage(gui_additemtomenu(menupage, (char*)wifi_icon, "WiFi"), wifipage);
    
    GUIpage_t* settingpage = guisettingpageinit(gui);
    settingpage->lastpage = menupage;
    gui_setnextpage(gui_additemtomenu(menupage, (char*)setting_icon, "Setting"), settingpage);
    
    gui_settrigger(gui_additemtomenu(menupage, (char*)sleep_icon, "Sleep"), (void(*)(void*))gui_switchsleepmode);
    
    gui_setstartpage(gui, menupage);
    
    return menupage;
}
