#ifndef __GUIMENU_H
#define __GUIMENU_H

#include "gui.h"

typedef struct
{
    uint16_t menunum;
    uint16_t position;
    GUIwidget_t* selectedwidget;
} GUImenu_t;

GUIpage_t* gui_createmenupage(uint16_t menunum);
GUIwidget_t* gui_additemtomenu(GUIpage_t* menupage, char* image, char* str);
void gui_menupageinit(GUI_Handle_t* gui);
int gui_menuproc(GUI_Handle_t* gui);
void gui_menuencodercontrol(GUI_Handle_t* gui, int encoderret);

#endif
