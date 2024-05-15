#ifndef __GUIADC_H
#define __GUIADC_H

#include "gui.h"

typedef struct
{
    uint16_t adcnum;
    uint16_t position;
    uint16_t boxposi;
    GUIwidget_t* selectedwidget;
    
    uint32_t adcvalue[8];
    char* voltstr;
    float volt;
    uint8_t stopflag;
    uint8_t wave[85];
    
} GUIadc_t;

typedef struct
{
    int channel;
} GUIadcitem_t;

GUIpage_t* gui_createadcpage(uint16_t adcnum);
GUIwidget_t* gui_additemtoadc(GUIpage_t* adcpage, int channel);
void gui_adcencodercontrol(GUI_Handle_t* gui, int encoderret);
void gui_adcpageinit(GUI_Handle_t* gui);
int gui_adcproc(GUI_Handle_t* gui);

#endif
