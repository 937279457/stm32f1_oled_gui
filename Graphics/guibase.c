#include "guibase.h"

static void gui_posiinit(GUIposi_t* posi)
{
    posi->start.bottom = 0;
    posi->start.left = 0;
    posi->start.radius = 0;
    posi->start.right = 0;
    posi->start.top = 0;
    posi->curr = posi->target = posi->start;
}

static void gui_widgetinit(GUIwidget_t* widget)
{
    gui_posiinit(&widget->posi);
    widget->nextpage = NULL;
    widget->param = NULL;
    
    widget->father = NULL;
    widget->son = NULL;

    widget->self = widget;
    widget->showcmd = ENABLE;
    widget->type = GUItype_null;
    widget->color = GM_Color_Bright;
    widget->fillcolor = GM_Color_Bright;
    
    widget->aninum = 0;
    widget->aniindex = 0;
    widget->anilist = NULL;
    
    widget->string = NULL;
    widget->number = NULL;
    widget->image = NULL;
    
    widget->trigger = NULL;
}

void gui_resetposi(GUIposi_t* posi)
{
    posi->curr = posi->target = posi->start;
}

void gui_syncresetposi(GUIwidget_t* widget)
{
    gui_resetposi(&widget->posi);
    if(widget->son != NULL)
    {
        gui_syncresetposi(widget->son);
    }
}

void gui_resettarget(GUIposi_t* posi)
{
    posi->target = posi->start;
}

void gui_syncresettarget(GUIwidget_t* widget)
{
    gui_resettarget(&widget->posi);
    if(widget->son != NULL)
    {
        gui_syncresettarget(widget->son);
    }
}

void gui_resetcurr(GUIposi_t* posi)
{
    posi->curr = posi->start;
}

void gui_syncresetcurr(GUIwidget_t* widget)
{
    gui_resetcurr(&widget->posi);
    if(widget->son != NULL)
    {
        gui_syncresetcurr(widget->son);
    }
}

void gui_setposi(GUIposi_t* posi, uint8_t option, float left, float top, float right, float bottom, float radius)
{
    if(option & SETPOSITION_START)
    {
        posi->start.left    = left;
        posi->start.right   = right;
        posi->start.top     = top;
        posi->start.bottom  = bottom;
        posi->start.radius  = radius;
    }
    if(option & SETPOSITION_CURR)
    {
        posi->curr.left     = left;
        posi->curr.right    = right;
        posi->curr.top      = top;
        posi->curr.bottom   = bottom;
        posi->curr.radius   = radius;
    }
    if(option & SETPOSITION_TARGET)
    {
        posi->target.left   = left;
        posi->target.right  = right;
        posi->target.top    = top;
        posi->target.bottom = bottom;
        posi->target.radius = radius;
    }
}

void gui_syncsetposi(GUIwidget_t* widget, uint8_t option, float left, float top, float right, float bottom, float radius)
{
    gui_setposi(&widget->posi, option, left, top, right, bottom, radius);
    if(widget->son != NULL)
    {
        gui_syncsetposi(widget->son, option, left, top, right, bottom, radius);
    }
}

void gui_changeposi(GUIposi_t* posi, uint8_t option, float left, float top, float right, float bottom, float radius)
{
    if(option & SETPOSITION_START)
    {
        posi->start.left    += left;
        posi->start.right   += right;
        posi->start.top     += top;
        posi->start.bottom  += bottom;
        posi->start.radius  += radius;
    }
    if(option & SETPOSITION_CURR)
    {
        posi->curr.left     += left;
        posi->curr.right    += right;
        posi->curr.top      += top;
        posi->curr.bottom   += bottom;
        posi->curr.radius   += radius;
    }
    if(option & SETPOSITION_TARGET)
    {
        posi->target.left   += left;
        posi->target.right  += right;
        posi->target.top    += top;
        posi->target.bottom += bottom;
        posi->target.radius += radius;
    }
}

void gui_syncchangeposi(GUIwidget_t* widget, uint8_t option, float left, float top, float right, float bottom, float radius)
{
    gui_changeposi(&widget->posi, option, left, top, right, bottom, radius);
    if(widget->son != NULL)
    {
        gui_syncchangeposi(widget->son, option, left, top, right, bottom, radius);
    }
}

static void gui_addwidget(GUIpage_t* page, GUIwidget_t* widget)
{
    page->widgetlist[page->widgetnum] = widget;
    page->widgetnum++;
}

GUIpage_t* gui_createpage(uint16_t widgetnum)
{
    GUIpage_t* page = malloc(sizeof(GUIpage_t));
    page->widgetlist = malloc(widgetnum * sizeof(GUIwidget_t*));
    page->widgetnum = 0;
    
    page->type = GUIpagetype_normal;
    
    page->self = page;
    page->lastpage = NULL;
    page->father = NULL;
    page->son = NULL;
    
    page->param = NULL;
    
    page->showcmd = ENABLE;
    
    return page;
}

GUIwidget_t* gui_createframe(float left, float top, float right, float bottom, float radius, GM_Color_t color)
{
    GUIwidget_t* widget = malloc(sizeof(GUIwidget_t));
    gui_widgetinit(widget);
    widget->color = color;
    gui_setposi(&widget->posi, SETPOSITION_ALL, left, top, right, bottom, radius);
    widget->type = GUItype_frame;
    
    return widget;
}

GUIwidget_t* gui_createbox(float left, float top, float right, float bottom, float radius, GM_Color_t fillcolor, GM_Color_t color)
{
    GUIwidget_t* widget = malloc(sizeof(GUIwidget_t));
    gui_widgetinit(widget);
    widget->color = color;
    widget->fillcolor = fillcolor;
    gui_setposi(&widget->posi, SETPOSITION_ALL, left, top, right, bottom, radius);
    widget->type = GUItype_box;
    
    return widget;
}

GUIwidget_t* gui_createcircle(float x, float y, float radius, GM_Color_t color)
{
    GUIwidget_t* widget = malloc(sizeof(GUIwidget_t));
    gui_widgetinit(widget);
    widget->color = color;
    gui_setposi(&widget->posi, SETPOSITION_ALL, x, y, 0, 0, radius);
    widget->type = GUItype_circle;
    
    return widget;
}

GUIwidget_t* gui_createdisc(float x, float y, float radius, GM_Color_t fillcolor, GM_Color_t color)
{
    GUIwidget_t* widget = malloc(sizeof(GUIwidget_t));
    gui_widgetinit(widget);
    widget->color = color;
    widget->fillcolor = fillcolor;
    gui_setposi(&widget->posi, SETPOSITION_ALL, x, y, 0, 0, radius);
    widget->type = GUItype_disc;
    
    return widget;
}

GUIwidget_t* gui_createstring(char* str, FontType_t fonttype, GUIalign_e align, float x, float y, GM_Color_t color)
{
    GUIwidget_t* widget = malloc(sizeof(GUIwidget_t));
    gui_widgetinit(widget);
    widget->color = color;
    gui_setposi(&widget->posi, SETPOSITION_ALL, x, y, 0, 0, 0);
    widget->type = GUItype_string;
    
    widget->string = malloc(sizeof(GUIstring_t));
    widget->string->fonttype = fonttype;
    widget->string->str = str;
    widget->string->length = strlen(str) * FONT_GetFontInfo(fonttype).width;
    widget->string->align = align;
    
    return widget;
}

GUIwidget_t* gui_createnumber(int num, FontType_t fonttype, GUIalign_e align, float x, float y, GM_Color_t color)
{
    GUIwidget_t* widget = malloc(sizeof(GUIwidget_t));
    gui_widgetinit(widget);
    widget->color = color;
    gui_setposi(&widget->posi, SETPOSITION_ALL, x, y, 0, 0, 0);
    widget->type = GUItype_number;
    
    widget->number = malloc(sizeof(GUInumber_t));
    widget->number->num = num;
    widget->number->fonttype = fonttype;
    widget->number->digits = GM_BegDigits(num);
    widget->number->length = widget->number->digits * FONT_GetFontInfo(fonttype).width;
    widget->number->align = align;
    
    return widget;
}

GUIwidget_t* gui_createimage(char* image, char* str, FontType_t fonttype, float x, float y, float width, float height, float strx, float stry, GM_Color_t color)
{
    GUIwidget_t* widget = malloc(sizeof(GUIwidget_t));
    gui_widgetinit(widget);
    widget->color = color;
    gui_setposi(&widget->posi, SETPOSITION_ALL, x, y, 0, 0, 0);
    widget->type = GUItype_image;
    
    widget->image = malloc(sizeof(GUIimage_t));
    widget->image->ima = image;
    widget->image->height = height;
    widget->image->width = width;
    
    GUIwidget_t* son = malloc(sizeof(GUIwidget_t));
    gui_widgetinit(son);
    son->color = color;
    gui_setposi(&son->posi, SETPOSITION_ALL, strx, stry, 0, 0, 0);
    son->type = GUItype_string;
    
    son->string = malloc(sizeof(GUIstring_t));
    son->string->fonttype = fonttype;
    son->string->str = str;
    son->string->length = strlen(str) * FONT_GetFontInfo(fonttype).width;
    son->string->align = GUIalign_right;
    
    widget->son = son;
    
    return widget;
}

GUIwidget_t* gui_addframe(GUIpage_t* page, float left, float top, float right, float bottom, float radius, GM_Color_t color)
{
    GUIwidget_t* widget = gui_createframe(left, top, right, bottom, radius, color);
    gui_addwidget(page, widget);
    
    return widget;
}

GUIwidget_t* gui_addbox(GUIpage_t* page, float left, float top, float right, float bottom, float radius, GM_Color_t fillcolor, GM_Color_t color)
{
    GUIwidget_t* widget = gui_createbox(left, top, right, bottom, radius, fillcolor, color);
    gui_addwidget(page, widget);
    
    return widget;
}

GUIwidget_t* gui_addcircle(GUIpage_t* page, float x, float y, float radius, GM_Color_t color)
{
    GUIwidget_t* widget = gui_createcircle(x, y, radius, color);
    gui_addwidget(page, widget);
    
    return widget;
}

GUIwidget_t* gui_adddisc(GUIpage_t* page, float x, float y, float radius, GM_Color_t fillcolor, GM_Color_t color)
{
    GUIwidget_t* widget = gui_createdisc(x, y, radius, fillcolor, color);
    gui_addwidget(page, widget);
    
    return widget;
}

GUIwidget_t* gui_addstring(GUIpage_t* page, char* str, FontType_t fonttype, GUIalign_e align, float x, float y, GM_Color_t color)
{
    GUIwidget_t* widget = gui_createstring(str, fonttype, align, x, y, color);
    gui_addwidget(page, widget);
    
    return widget;
}

GUIwidget_t* gui_addnumber(GUIpage_t* page, int num, FontType_t fonttype, GUIalign_e align, float x, float y, GM_Color_t color)
{
    GUIwidget_t* widget = gui_createnumber(num, fonttype, align, x, y, color);
    gui_addwidget(page, widget);
    
    return widget;
}

GUIwidget_t* gui_addimage(GUIpage_t* page, char* image, char* str, FontType_t fonttype, float x, float y, float width, float height, float strx, float stry, GM_Color_t color)
{
    GUIwidget_t* widget = gui_createimage(image, str, fonttype, x, y, width, height, strx, stry, color);
    gui_addwidget(page, widget);
    
    return widget;
}

void gui_setnextpage(GUIwidget_t* widget, GUIpage_t* page)
{
    widget->nextpage = page;
}

void gui_settrigger(GUIwidget_t* widget, void(*trigger)(void*))
{
    widget->trigger = trigger;
}

void gui_setstring(GUIstring_t* string, char* str)
{
    string->str = str;
    string->length = strlen(str) * FONT_GetFontInfo(string->fonttype).width;
}

void gui_setnumber(GUInumber_t* number, int num)
{
    number->num = num;
    number->digits = GM_BegDigits(num);
    number->length = number->digits * FONT_GetFontInfo(number->fonttype).width;
}

void gui_addanimation(GUIwidget_t* widget, float left, float top, float right, float bottom, float radius)
{
    gui_setposi(&widget->posi, SETPOSITION_TARGET, widget->posi.target.left + left, widget->posi.target.top + top, widget->posi.target.right + right, widget->posi.target.bottom + bottom, widget->posi.target.radius + radius);
}

GUIanilist_t* gui_createanilist(uint8_t anilistnum)
{
    GUIanilist_t* anilist = malloc(sizeof(GUIanilist_t));
    anilist->targetlist = malloc(anilistnum * 5 * sizeof(float));
    anilist->anilistindex = 0;
    
    return anilist;
}

void gui_addanitolist(GUIanilist_t* anilist, float left, float top, float right, float bottom, float radius)
{
    anilist->targetlist[anilist->anilistindex * 5] = left;
    anilist->targetlist[anilist->anilistindex * 5 + 1] = top;
    anilist->targetlist[anilist->anilistindex * 5 + 2] = right;
    anilist->targetlist[anilist->anilistindex * 5 + 3] = bottom;
    anilist->targetlist[anilist->anilistindex * 5 + 4] = radius;
    anilist->anilistindex++;
}

void gui_addanilist(GUIwidget_t* widget, GUIanilist_t* anilist)
{
    widget->anilist = anilist;
    widget->aninum = anilist->anilistindex;
}
