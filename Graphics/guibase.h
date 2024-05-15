#ifndef __GUIBASE_H
#define __GUIBASE_H

#include "graphics_mono.h"
#include <string.h>
#include <stdlib.h>

#define SETPOSITION_START   0x01
#define SETPOSITION_CURR    0x02
#define SETPOSITION_TARGET  0x04
#define SETPOSITION_ALL     (SETPOSITION_START|SETPOSITION_CURR|SETPOSITION_TARGET)

typedef enum
{
    GUItype_null,
    GUItype_frame,
    GUItype_box,
    GUItype_string,
    GUItype_circle,
    GUItype_disc,
    GUItype_image,
    GUItype_number,
} GUItype_e;

typedef enum
{
    GUIpagetype_normal,
    GUIpagetype_list,
    GUIpagetype_menu,
    GUIpagetype_adc,
} GUIpagetype_e;

typedef enum
{
    GUIalign_right,
    GUIalign_left,
} GUIalign_e;

typedef struct
{
    float left;
    float right;
    float top;
    float bottom;
    float radius;
} GUIsize_t;

typedef struct
{
    GUIsize_t start;
    GUIsize_t curr;
    GUIsize_t target;
} GUIposi_t;

typedef struct
{
    char* str;
    uint8_t length;
    FontType_t fonttype;
    GUIalign_e align;
} GUIstring_t;

typedef struct
{
    int num;
    uint8_t digits;
    uint8_t length;
    FontType_t fonttype;
    GUIalign_e align;
} GUInumber_t;

typedef struct
{
    char* ima;
    uint8_t width;
    uint8_t height;
} GUIimage_t;

typedef struct GUIwidget_t
{
    GUIposi_t posi;
    void (*trigger)(void*);
    
    struct GUIpage_t* nextpage;
    struct GUIwidget_t* father;
    struct GUIwidget_t* son;
    
    struct GUIwidget_t* self;
    FunctionalState showcmd;
    GUItype_e type;
    GM_Color_t color;
    GM_Color_t fillcolor;
    
    uint8_t aninum;
    uint8_t aniindex;
    struct GUIanilist_t* anilist;
    
    GUIstring_t* string;
    GUInumber_t* number;
    GUIimage_t* image;
    
    void* param;
} GUIwidget_t;

typedef struct GUIpage_t
{
    GUIwidget_t** widgetlist;
    uint16_t widgetnum;
    GUIpagetype_e type;
    
    struct GUIpage_t* self;
    struct GUIpage_t* lastpage;
    struct GUIpage_t* father;
    struct GUIpage_t* son;
    
    void* param;
    
    FunctionalState showcmd;
    
} GUIpage_t;

typedef struct GUIanilist_t
{
    float* targetlist;
    uint8_t anilistindex;
} GUIanilist_t;

void gui_resetposi(GUIposi_t* posi);
void gui_resettarget(GUIposi_t* posi);
void gui_resetcurr(GUIposi_t* posi);
void gui_setposi(GUIposi_t* posi, uint8_t option, float left, float top, float right, float bottom, float radius);
void gui_changeposi(GUIposi_t* posi, uint8_t option, float left, float top, float right, float bottom, float radius);

void gui_syncresetposi(GUIwidget_t* widget);
void gui_syncresettarget(GUIwidget_t* widget);
void gui_syncresetcurr(GUIwidget_t* widget);
void gui_syncsetposi(GUIwidget_t* widget, uint8_t option, float left, float top, float right, float bottom, float radius);
void gui_syncchangeposi(GUIwidget_t* widget, uint8_t option, float left, float top, float right, float bottom, float radius);

GUIpage_t* gui_createpage(uint16_t widgetnum);
GUIwidget_t* gui_createframe(float left, float top, float right, float bottom, float radius, GM_Color_t color);
GUIwidget_t* gui_createbox(float left, float top, float right, float bottom, float radius, GM_Color_t fillcolor, GM_Color_t color);
GUIwidget_t* gui_createcircle(float x, float y, float radius, GM_Color_t color);
GUIwidget_t* gui_createdisc(float x, float y, float radius, GM_Color_t fillcolor, GM_Color_t color);
GUIwidget_t* gui_createstring(char* str, FontType_t fonttype, GUIalign_e align, float x, float y, GM_Color_t color);
GUIwidget_t* gui_createnumber(int num, FontType_t fonttype, GUIalign_e align, float x, float y, GM_Color_t color);
GUIwidget_t* gui_createimage(char* image, char* str, FontType_t fonttype, float x, float y, float width, float height, float strx, float stry, GM_Color_t color);

GUIwidget_t* gui_addframe(GUIpage_t* page, float left, float top, float right, float bottom, float radius, GM_Color_t color);
GUIwidget_t* gui_addbox(GUIpage_t* page, float left, float top, float right, float bottom, float radius, GM_Color_t fillcolor, GM_Color_t color);
GUIwidget_t* gui_addcircle(GUIpage_t* page, float x, float y, float radius, GM_Color_t color);
GUIwidget_t* gui_adddisc(GUIpage_t* page, float x, float y, float radius, GM_Color_t fillcolor, GM_Color_t color);
GUIwidget_t* gui_addstring(GUIpage_t* page, char* str, FontType_t fonttype, GUIalign_e align, float x, float y, GM_Color_t color);
GUIwidget_t* gui_addnumber(GUIpage_t* page, int num, FontType_t fonttype, GUIalign_e align, float x, float y, GM_Color_t color);
GUIwidget_t* gui_addimage(GUIpage_t* page, char* image, char* str, FontType_t fonttype, float x, float y, float width, float height, float strx, float stry, GM_Color_t color);

void gui_setnextpage(GUIwidget_t* widget, GUIpage_t* page);
void gui_settrigger(GUIwidget_t* widget, void(*trigger)(void*));
void gui_setstring(GUIstring_t* string, char* str);
void gui_setnumber(GUInumber_t* number, int num);

void gui_addanimation(GUIwidget_t* widget, float left, float top, float right, float bottom, float radius);
GUIanilist_t* gui_createanilist(uint8_t anilistnum);
void gui_addanitolist(GUIanilist_t* anilist, float left, float top, float right, float bottom, float radius);
void gui_addanilist(GUIwidget_t* widget, GUIanilist_t* anilist);

#endif
