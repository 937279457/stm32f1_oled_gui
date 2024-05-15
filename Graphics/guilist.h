#ifndef __GUILIST_H
#define __GUILIST_H

#include "gui.h"

typedef enum
{
    itemtype_string,
    itemtype_radio,
    itemtype_number,
    itemtype_window,
} GUIlistitemtype_e;

typedef struct
{
    uint16_t listnum;
    uint16_t position;
    uint16_t boxposi;
    uint8_t winflag;
    GUIwidget_t* selectedwidget;
} GUIlist_t;

typedef struct
{
    GUIlistitemtype_e type;
    uint16_t winmin;
    uint16_t winmax;
    int* num;
    uint8_t radiostate;
} GUIlistitem_t;

GUIpage_t* gui_createlistpage(uint16_t listnum);
void gui_listpageinit(GUI_Handle_t* gui);
GUIwidget_t* gui_additemtolist(GUIpage_t* listpage, char* str, GUIlistitemtype_e type);
void gui_setlistitemrange(GUIwidget_t* widget, int min, int max);
int gui_radiocmd(GUIwidget_t* widget);
void gui_setnumpoint(GUIwidget_t* widget, int* num);
void gui_listencodercontrol(GUI_Handle_t* gui, int encoderret);
int gui_listproc(GUI_Handle_t* gui);

#endif
