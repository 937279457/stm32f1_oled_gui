#ifndef __GUI_H
#define __GUI_H

#include "guibase.h"
#include "oled.h"
#include "delay.h"
#include "softwaretimer.h"
#include "buzzer.h"

#define ANIMATION_COEFFICIENT   0.5
#define JELLY_EFFECT            1.8

typedef enum
{
    GUIsignal_null,
    GUIsignal_enter,
    GUIsignal_return,
    GUIsignal_scrollleft,
    GUIsignal_scrollright,
    GUIsignal_scrollup,
    GUIsignal_scrolldown,
} GUIsinal_e;

typedef enum
{
    GUIstate_proc,
    GUIstate_stop,
} GUIstate_e;

typedef enum
{
    GUIprocstate_List,
    GUIprocstate_Main,
} GUIprocstate_e;

typedef struct GUI_Handle_t
{
    GUIstate_e state;
    GUIsinal_e signal;
    GM_Color_t maincolor;
    
    GUIpage_t* page;
    
    uint8_t fpsflag;
    uint8_t dmabusyflag;
    uint8_t switchpageflag;
    uint8_t soundflag;
    uint8_t gyroflag;
    uint8_t sleepflag;
    
    int (*signalproc)(struct GUI_Handle_t*);
    void (*extrapaintfunc)(void*);
    
    float animationcoefficient;
    int animationrate;
    int faderate;
    int brightness;
    uint16_t fps;
    
    char* ram;
    
    BUZZER_Handle_t* buzzer;
    
} GUI_Handle_t;

void gui_init(GUI_Handle_t* gui);
void gui_proc(GUI_Handle_t* gui);
void gui_sendsignal(GUI_Handle_t* gui, GUIsinal_e signal);
void gui_setsignalproc(GUI_Handle_t* gui, int (*signalproc)(GUI_Handle_t*));
void gui_setextrapaintfunc(GUI_Handle_t* gui, void (*extrapaintfunc)(void*));
void gui_setstartpage(GUI_Handle_t* gui, GUIpage_t* page);
void gui_switchpage(GUI_Handle_t* gui, GUIpage_t* page);

void gui_switchsoundstate(GUI_Handle_t* gui);
void gui_switchjellyeffect(GUI_Handle_t* gui);
void gui_switchcolormode(GUI_Handle_t* gui);
void gui_switchsleepmode(GUI_Handle_t* gui);
void gui_setbrightness(GUI_Handle_t* gui);
void gui_encodercontrol(GUI_Handle_t* gui, int encoderret);

void gui_setbuzzer(GUI_Handle_t* gui, TIM_TypeDef* TIMx, PWM_OCChannel_Typedef OCChannel, float Frequency, uint32_t duration);
void gui_buzzeralarm(GUI_Handle_t* gui);

int gui_listproc(GUI_Handle_t* gui);
void gui_listpageinit(GUI_Handle_t* gui);
void gui_listencodercontrol(GUI_Handle_t* gui, int encoderret);

int gui_menuproc(GUI_Handle_t* gui);
void gui_menupageinit(GUI_Handle_t* gui);
void gui_menuencodercontrol(GUI_Handle_t* gui, int encoderret);

int gui_adcproc(GUI_Handle_t* gui);
void gui_adcpageinit(GUI_Handle_t* gui);
void gui_adcencodercontrol(GUI_Handle_t* gui, int encoderret);

#endif
