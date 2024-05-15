#include "guilist.h"

GUIpage_t* gui_createlistpage(uint16_t listnum)
{
    GUIpage_t* listpage = gui_createpage(listnum);
    GUIlist_t* list = malloc(sizeof(GUIlist_t));
    list->boxposi = 0;
    list->listnum = 0;
    list->position = 0;
    list->winflag = 0;
    list->selectedwidget = NULL;
    
    GUIpage_t* son = gui_createpage(2);
    gui_addbox(son, 1, 1, 14, 14, 3, GM_Color_Inverse, GM_Color_Bright);
    gui_addbox(son, SCREEN_LENGTH - 5, 0, SCREEN_LENGTH - 1, SCREEN_WIDTH - 1, 0, GM_Color_Bright, GM_Color_Bright);
    listpage->son = son;
    
    GUIpage_t* grandson = gui_createpage(5);
    int y_windowinit = SCREEN_WIDTH/2 + 16;
    gui_addbox(grandson, SCREEN_LENGTH/2 - 47, SCREEN_WIDTH/2 - 15 - y_windowinit, SCREEN_LENGTH/2 + 47, SCREEN_WIDTH/2 + 15 - y_windowinit, 3, GM_Color_Dark, GM_Color_Bright);// 0:边框
    gui_addstring(grandson, " ", FontType_6x8, GUIalign_right, SCREEN_LENGTH/2 - 42, SCREEN_WIDTH/2 - 10 - y_windowinit, GM_Color_Bright);                                          // 1:字符串
    gui_addnumber(grandson, 0, FontType_6x8, GUIalign_left, SCREEN_LENGTH/2 + 42, SCREEN_WIDTH/2 - 10 - y_windowinit, GM_Color_Bright);                                             // 2:数字
    gui_addframe(grandson, SCREEN_LENGTH/2 - 42, SCREEN_WIDTH/2 + 3 - y_windowinit, SCREEN_LENGTH/2 + 42, SCREEN_WIDTH/2 + 9 - y_windowinit, 3, GM_Color_Bright);                   // 3:进度条边框
    gui_addbox(grandson, SCREEN_LENGTH/2 - 40, SCREEN_WIDTH/2 + 5 - y_windowinit, SCREEN_LENGTH/2 + 40, SCREEN_WIDTH/2 + 7 - y_windowinit, 0, GM_Color_Bright, GM_Color_Bright);    // 4:进度条进度
    listpage->son->son = grandson;
    
    listpage->param = list;
    listpage->type = GUIpagetype_list;
    
    return listpage;
}

GUIwidget_t* gui_additemtolist(GUIpage_t* listpage, char* str, GUIlistitemtype_e type)
{
    GUIlist_t* list = listpage->param;
    GUIwidget_t* widget = gui_addstring(listpage, str, FontType_8x16, GUIalign_right, 8, 0, GM_Color_Bright);
    GUIlistitem_t* itemparam = malloc(sizeof(GUIlistitem_t));
    
    itemparam->type = type;
    itemparam->winmin = 0;
    itemparam->winmin = 1;
    itemparam->num = NULL;
    itemparam->radiostate = 0;
    
    widget->param = itemparam;
    
    if(type == itemtype_radio)
    {
        widget->son = gui_createframe(SCREEN_LENGTH - 22, widget->posi.curr.top + 1, SCREEN_LENGTH - 9, widget->posi.curr.top + 14, 3, GM_Color_Bright);
        widget->son->son = gui_createbox(SCREEN_LENGTH - 20, widget->posi.curr.top + 3, SCREEN_LENGTH - 11, widget->posi.curr.top + 12, 0, GM_Color_Bright, GM_Color_Bright);
        widget->son->son->showcmd = DISABLE;
    }
    else if(type == itemtype_number || type == itemtype_window)
    {
        widget->son = gui_createnumber(0, FontType_8x16, GUIalign_left, SCREEN_LENGTH - 9, widget->posi.curr.top, GM_Color_Bright);
    }
    
    list->listnum++;
    
    return widget;
}

void gui_setlistitemrange(GUIwidget_t* widget, int min, int max)
{
    GUIlistitem_t* item = widget->param;
    item->winmin = min;
    item->winmax = max;
}

void gui_setnumpoint(GUIwidget_t* widget, int* num)
{
    GUIlistitem_t* item = widget->param;
    item->num = num;
}

void gui_listencodercontrol(GUI_Handle_t* gui, int encoderret)
{
    if(encoderret == 1)
    {
        if(((GUIlist_t*)(gui->page->param))->winflag == 1)
            gui_sendsignal(gui, GUIsignal_scrollleft);
        else
            gui_sendsignal(gui, GUIsignal_scrollup);
    }
    else if(encoderret == -1)
    {
        if(((GUIlist_t*)(gui->page->param))->winflag == 1)
            gui_sendsignal(gui, GUIsignal_scrollright);
        else
            gui_sendsignal(gui, GUIsignal_scrolldown);
    }
}

static void gui_popwindowshow(GUIpage_t* listpage)
{
    GUIpage_t* windowpage = listpage->son->son;
    GUIlist_t* list = listpage->param;
    GUIlistitem_t* item = list->selectedwidget->param;
    char* selectedstr = list->selectedwidget->string->str;
    int selectednum = list->selectedwidget->son->number->num;
    int y_windowinit = SCREEN_WIDTH/2 + 16;
    
    if(list->winflag == 1)
        return;
    
    gui_setstring(windowpage->widgetlist[1]->string, selectedstr);
    gui_setnumber(windowpage->widgetlist[2]->number, selectednum);
    
    gui_changeposi(&windowpage->widgetlist[0]->posi, SETPOSITION_TARGET, 0, y_windowinit, 0, y_windowinit, 0);
    gui_changeposi(&windowpage->widgetlist[1]->posi, SETPOSITION_TARGET, 0, y_windowinit, 0, 0, 0);
    gui_changeposi(&windowpage->widgetlist[2]->posi, SETPOSITION_TARGET, 0, y_windowinit, 0, 0, 0);
    gui_changeposi(&windowpage->widgetlist[3]->posi, SETPOSITION_TARGET, 0, y_windowinit, 0, y_windowinit, 0);
    gui_changeposi(&windowpage->widgetlist[4]->posi, SETPOSITION_TARGET, 0, y_windowinit, 0, y_windowinit, 0);
    
    int x_progress = (selectednum - item->winmin) * 1.0 / (item->winmax - item->winmin) * 80;
    windowpage->widgetlist[4]->posi.target.right = x_progress + windowpage->widgetlist[4]->posi.target.left;
    
    list->winflag = 1;
}

static void gui_popwindowfade(GUIpage_t* listpage)
{
    int i = 0;
    GUIpage_t* windowpage = listpage->son->son;
    GUIlist_t* list = listpage->param;
    
    if(list->winflag == 0)
        return;
    
    for(i = 0; i < 5; i++)
    {
        gui_resettarget(&windowpage->widgetlist[i]->posi);
    }
    
    list->winflag = 0;
}

void gui_listpageinit(GUI_Handle_t* gui)
{
    int i = 0;
    GUIlist_t* list = gui->page->param;
    
    for(i = 0; i < list->listnum; i++)
    {
        GUIlistitem_t* item = gui->page->widgetlist[i]->param;
        
        int offsety = (i - (list->position - list->boxposi)) * 16;
        
        gui_syncresetposi(gui->page->widgetlist[i]);
        gui_syncchangeposi(gui->page->widgetlist[i], SETPOSITION_TARGET, 0, offsety, 0, offsety, 0);
        
        if(item->num != NULL && (item->type == itemtype_number || item->type == itemtype_window))
        {
            gui_setnumber(gui->page->widgetlist[i]->son->number, *item->num);
        }
    }
    
    gui_syncresetposi(gui->page->son->widgetlist[0]);
    gui_syncresetposi(gui->page->son->widgetlist[1]);
    if(list->listnum > 0)
    {
        list->selectedwidget = gui->page->widgetlist[list->position];
        
        gui_syncchangeposi(gui->page->son->widgetlist[0], SETPOSITION_TARGET, 0, list->boxposi*16, list->selectedwidget->string->length, list->boxposi*16, 0);
        gui_syncchangeposi(gui->page->son->widgetlist[1], SETPOSITION_TARGET, 0, 0, 0, -(((list->listnum - 1 - list->position)*1.0/list->listnum)*(SCREEN_WIDTH - 1)), 0);
    }
}

int gui_radiocmd(GUIwidget_t* widget)
{
    GUIlistitem_t* item = widget->param;
    if(item->radiostate == 1)
    {
        item->radiostate = 0;
        widget->son->son->showcmd = DISABLE;
    }
    else
    {
        item->radiostate = 1;
        widget->son->son->showcmd = ENABLE;
    }
    
    return item->radiostate;
}

static int gui_listenterproc(GUI_Handle_t* gui)
{
    GUIlist_t* list = gui->page->param;
    GUIlistitem_t* item = list->selectedwidget->param;
    int ret = 0;
    
    if(item->type == itemtype_window)
    {
        gui_popwindowshow(gui->page);
        ret = 1;
    }
    else if(item->type == itemtype_radio)
    {
        gui_radiocmd(list->selectedwidget);
        if(list->selectedwidget->trigger != NULL)
        {
            list->selectedwidget->trigger(gui);
        }
        ret = 1;
    }
    else if(item->type == itemtype_string)
    {
        if(list->selectedwidget->nextpage != NULL)
        {
            gui_switchpage(gui, list->selectedwidget->nextpage);
            ret = 1;
        }
    }
    
    return ret;
}

static int gui_listreturnproc(GUI_Handle_t* gui)
{
    GUIlist_t* list = gui->page->param;
    int ret = 0;
    
    if(list->winflag == 1)
    {
        gui_popwindowfade(gui->page);
        ret = 1;
    }
    else if(gui->page->lastpage != NULL)
    {
        gui_switchpage(gui, gui->page->lastpage);
        ret = 1;
    }
    
    return ret;
}

static int gui_listscrollproc(GUI_Handle_t* gui, GUIsinal_e signal)
{
    GUIpage_t* listpage = gui->page;
    GUIlist_t* list = gui->page->param;
    int dboxposi = 0;
    int dposition = 0;
    int ret = 0;
    
    if(signal == GUIsignal_scrollup)
    {
        if(list->position > 0)
        {
            list->position--;
            dposition = -1;
            if(list->boxposi > 0)
            {
                list->boxposi--;
                dboxposi = -1;
            }
        }
    }
    else if(signal == GUIsignal_scrolldown)
    {
        if(list->position < list->listnum - 1)
        {
            list->position++;
            dposition = 1;
            if(list->boxposi < (SCREEN_WIDTH / 16) - 1)
            {
                list->boxposi++;
                dboxposi = 1;
            }
        }
    }
    
    if(dposition != 0)
    {
        gui_resettarget(&listpage->son->widgetlist[1]->posi);
        gui_changeposi(&listpage->son->widgetlist[0]->posi, SETPOSITION_TARGET, 0, dboxposi * 16, listpage->widgetlist[list->position]->string->length - (listpage->son->widgetlist[0]->posi.target.right - 14), dboxposi * 16, 0);
        gui_changeposi(&listpage->son->widgetlist[1]->posi, SETPOSITION_TARGET, 0, 0, 0, -(((list->listnum - 1 - list->position)*1.0/list->listnum)*(SCREEN_WIDTH - 1)), 0);
        
        if(dboxposi == 0)
        {
            int i = 0;
            for(i = 0; i < list->listnum; i++)
            {
                gui_syncchangeposi(listpage->widgetlist[i], SETPOSITION_TARGET, 0, dposition * -16, 0, dposition * -16, 0);
            }
        }
        
        list->selectedwidget = listpage->widgetlist[list->position];
        
        ret = 1;
    }
    
    return ret;
}

static int gui_windowproc(GUI_Handle_t* gui, GUIsinal_e signal)
{
    GUIpage_t* windowpage = gui->page->son->son;
    GUIlist_t* list = gui->page->param;
    GUIlistitem_t* item = list->selectedwidget->param;
    int selectednum = list->selectedwidget->son->number->num;
    int ret = 0;
    
    if(list->winflag == 0)
        return 0;
    
    if(signal == GUIsignal_scrollleft)
    {
        if(selectednum > item->winmin)
        {
            selectednum--;
            ret = 1;
        }
    }
    else if(signal == GUIsignal_scrollright)
    {
        if(selectednum < item->winmax)
        {
            selectednum++;
            ret = 1;
        }
    }
    
    gui_setnumber(list->selectedwidget->son->number, selectednum);
    gui_setnumber(windowpage->widgetlist[2]->number, selectednum);
    if(item->num != NULL)
    {
        *item->num = selectednum;
    }
    
    if(list->selectedwidget->trigger != NULL)
    {
        list->selectedwidget->trigger(gui);
    }
    
    int x_progress = (selectednum - item->winmin) * 1.0 / (item->winmax - item->winmin) * 80;
    windowpage->widgetlist[4]->posi.target.right = x_progress + windowpage->widgetlist[4]->posi.target.left;
    
    return ret;
}

int gui_listproc(GUI_Handle_t* gui)
{
    int ret = 0;
    
    switch(gui->signal)
    {
        case GUIsignal_enter:       ret = gui_listenterproc(gui);                           break;
        case GUIsignal_return:      ret = gui_listreturnproc(gui);                          break;
        case GUIsignal_scrollup:    ret = gui_listscrollproc(gui, GUIsignal_scrollup);      break;
        case GUIsignal_scrolldown:  ret = gui_listscrollproc(gui, GUIsignal_scrolldown);    break;
        case GUIsignal_scrollleft:  ret = gui_windowproc(gui, GUIsignal_scrollleft);        break;
        case GUIsignal_scrollright: ret = gui_windowproc(gui, GUIsignal_scrollright);       break;
        default:break;
    }
    gui->signal = GUIsignal_null;
    
    return ret;
}
