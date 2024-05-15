#include "guimenu.h"

GUIpage_t* gui_createmenupage(uint16_t menunum)
{
    GUIpage_t* menupage = gui_createpage(menunum);
    GUImenu_t* menu = malloc(sizeof(GUImenu_t));
    menu->menunum = 0;
    menu->position = 0;
    menu->selectedwidget = NULL;
    
    GUIpage_t* son = gui_createpage(2);
    gui_addbox(son, -30, 36, -30, 60, 0, GM_Color_Bright, GM_Color_Bright);
    gui_addbox(son, -30, 36, 6, 60, 0, GM_Color_Bright, GM_Color_Bright);
    menupage->son = son;
    
    menupage->param = menu;
    menupage->type = GUIpagetype_menu;
    
    return menupage;
}

GUIwidget_t* gui_additemtomenu(GUIpage_t* menupage, char* image, char* str)
{
    GUImenu_t* menu = menupage->param;
    GUIwidget_t* widget = gui_addimage(menupage, image, str, FontType_12x24, (SCREEN_LENGTH - 30)/2, -30, 30, 30, (SCREEN_LENGTH - strlen(str)*12)/2, SCREEN_WIDTH, GM_Color_Bright);
    
    menu->menunum++;
    
    return widget;
}

void gui_menuencodercontrol(GUI_Handle_t* gui, int encoderret)
{
    if(encoderret == 1)
        gui_sendsignal(gui, GUIsignal_scrollleft);
    else if(encoderret == -1)
        gui_sendsignal(gui, GUIsignal_scrollright);
}

void gui_menupageinit(GUI_Handle_t* gui)
{
    int i = 0;
    GUImenu_t* menu = gui->page->param;
    
    for(i = 0; i < menu->menunum; i++)
    {
        int offsetx = (i - menu->position) * 36;
        gui_syncresetposi(gui->page->widgetlist[i]);
        gui_changeposi(&gui->page->widgetlist[i]->posi, SETPOSITION_TARGET, offsetx, 30, offsetx, 30, 0);
    }
    
    if(menu->menunum > 0)
    {
        menu->selectedwidget = gui->page->widgetlist[menu->position];
        gui_changeposi(&menu->selectedwidget->son->posi, SETPOSITION_TARGET, 0, -28, 0, -28, 0);
    }
    
    gui_resetposi(&gui->page->son->widgetlist[0]->posi);
    gui_resetposi(&gui->page->son->widgetlist[1]->posi);
    gui_changeposi(&gui->page->son->widgetlist[0]->posi, SETPOSITION_TARGET, 0, 0, 36, 0, 0);
    gui_changeposi(&gui->page->son->widgetlist[1]->posi, SETPOSITION_TARGET, 0, 0, -36, 0, 0);
}

static int gui_menuenterproc(GUI_Handle_t* gui)
{
    GUImenu_t* menu = gui->page->param;
    int ret = 0;
    
    if(menu->selectedwidget->trigger != NULL)
    {
        menu->selectedwidget->trigger(gui);
        ret = 1;
    }
    else if(menu->selectedwidget->nextpage != NULL)
    {
        gui_switchpage(gui, menu->selectedwidget->nextpage);
        ret = 1;
    }
    
    return ret;
}

static int gui_menuscrollproc(GUI_Handle_t* gui, GUIsinal_e signal)
{
    GUIpage_t* menupage = gui->page;
    GUImenu_t* menu = gui->page->param;
    int dposition = 0;
    int ret = 0;
    
    if(signal == GUIsignal_scrollleft)
    {
        if(menu->position > 0)
        {
            menu->position--;
            dposition = -1;
        }
    }
    else if(signal == GUIsignal_scrollright)
    {
        if(menu->position < menu->menunum - 1)
        {
            menu->position++;
            dposition = 1;
        }
    }
    
    if(dposition != 0)
    {
        gui_resettarget(&menu->selectedwidget->son->posi);
        
        int i = 0;
        for(i = 0; i < menu->menunum; i++)
        {
            gui_changeposi(&menupage->widgetlist[i]->posi, SETPOSITION_TARGET, dposition * -36, 0, dposition * -36, 0, 0);
        }
        
        menu->selectedwidget = menupage->widgetlist[menu->position];
        
        gui_changeposi(&menu->selectedwidget->son->posi, SETPOSITION_TARGET, 0, -28, 0, -28, 0);
        
        gui_resetposi(&menupage->son->widgetlist[0]->posi);
        gui_resetposi(&menupage->son->widgetlist[1]->posi);
        gui_changeposi(&menupage->son->widgetlist[0]->posi, SETPOSITION_TARGET, 0, 0, 36, 0, 0);
        gui_changeposi(&menupage->son->widgetlist[1]->posi, SETPOSITION_TARGET, 0, 0, -36, 0, 0);
        
        ret = 1;
    }
    
    return ret;
}

int gui_menuproc(GUI_Handle_t* gui)
{
    int ret = 0;
    
    switch(gui->signal)
    {
        case GUIsignal_enter:       ret = gui_menuenterproc(gui);                           break;
        case GUIsignal_scrollleft:  ret = gui_menuscrollproc(gui, GUIsignal_scrollleft);    break;
        case GUIsignal_scrollright: ret = gui_menuscrollproc(gui, GUIsignal_scrollright);   break;
        default:break;
    }
    gui->signal = GUIsignal_null;
    
    return ret;
}
