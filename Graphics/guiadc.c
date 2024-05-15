#include "guiadc.h"

GUIpage_t* gui_createadcpage(uint16_t adcnum)
{
    GUIpage_t* adcpage = gui_createpage(adcnum);
    GUIadc_t* adc = malloc(sizeof(GUIadc_t));
    adc->adcnum = 0;
    adc->position = 0;
    adc->boxposi = 0;
    adc->selectedwidget = NULL;
    
    int i = 0;
    for(i = 0; i < 8; i++)
        adc->adcvalue[i] = 0;
    adc->voltstr = malloc(sizeof(char) * 5);
    adc->voltstr[0] = 0 + 0x30;
    adc->voltstr[1] = '.';
    adc->voltstr[2] = 0 + 0x30;
    adc->voltstr[3] = 0 + 0x30;
    adc->voltstr[4] = 'V';
    adc->volt = 0;
    adc->stopflag = 0;
    for(i = 0; i < 85; i++)
        adc->wave[i] = 48;
    
    GUIpage_t* son = gui_createpage(5);
    gui_addbox(son, 1, 1, 14, 14, 3, GM_Color_Inverse, GM_Color_Bright);
    gui_addbox(son, 40, 1, 126, 30, 3, GM_Color_Bright, GM_Color_Bright);
    gui_addstring(son, "0.00V", FontType_12x24, GUIalign_right, 53, 4, GM_Color_Inverse);
    gui_addframe(son, 40, 33, 126, 62, 3, GM_Color_Bright);
    adcpage->son = son;
    
    adcpage->param = adc;
    adcpage->type = GUIpagetype_adc;
    
    return adcpage;
}

GUIwidget_t* gui_additemtoadc(GUIpage_t* adcpage, int channel)
{
    GUIadc_t* adc = adcpage->param;
    char* str = malloc(sizeof(char)*3);
    str[0] = 'C';
    str[1] = 'H';
    str[2] = channel + 0x30;
    GUIwidget_t* widget = gui_addstring(adcpage, str, FontType_8x16, GUIalign_right, 8, 0, GM_Color_Bright);
    
    GUIadcitem_t* itemparam = malloc(sizeof(GUIadcitem_t));
    itemparam->channel = channel;
    widget->param = itemparam;
    
    adc->adcnum++;
    
    return widget;
}

static void gui_adcwavedisplay(GUI_Handle_t* gui)
{
    if(gui->page->type != GUIpagetype_adc)
        return;
    
    GUIadc_t* adc = gui->page->param;
    
    static uint8_t count = 0;
    int i = 0;
    for(i = 0; i < count; i++)
    {
        adc->wave[i+1] = adc->wave[i];
    }
    adc->wave[0] = 60 - (int)(adc->volt * 25 / 3.3);
    
    for(i = 0; i < 84; i++)
    {
        GM_DrawLine(125-i, adc->wave[i], 125-i-1, adc->wave[i+1], GM_Color_Bright);
    }
    
    count++;
    count %= 85;
}

void gui_adcencodercontrol(GUI_Handle_t* gui, int encoderret)
{
    if(encoderret == 1)
        gui_sendsignal(gui, GUIsignal_scrollup);
    else if(encoderret == -1)
        gui_sendsignal(gui, GUIsignal_scrolldown);
}

void gui_adcpageinit(GUI_Handle_t* gui)
{
    int i = 0;
    
    GUIadc_t* adc = gui->page->param;
    
    for(i = 0; i < adc->adcnum; i++)
    {
        int offsety = (i - (adc->position - adc->boxposi)) * 16;
        
        gui_syncresetposi(gui->page->widgetlist[i]);
        gui_syncchangeposi(gui->page->widgetlist[i], SETPOSITION_TARGET, 0, offsety, 0, offsety, 0);
    }
    
    gui_syncresetposi(gui->page->son->widgetlist[0]);
    if(adc->adcnum > 0)
    {
        adc->selectedwidget = gui->page->widgetlist[adc->position];
        
        gui_syncchangeposi(gui->page->son->widgetlist[0], SETPOSITION_TARGET, 0, adc->boxposi*16, adc->selectedwidget->string->length, adc->boxposi*16, 0);
    }
    
    gui_setextrapaintfunc(gui, (void(*)(void*))gui_adcwavedisplay);
}

static int gui_adcenterproc(GUI_Handle_t* gui)
{
    int ret = 0;
    
    GUIadc_t* adc = gui->page->param;
    adc->stopflag = !adc->stopflag;
    ret = 1;
    
    return ret;
}

static int gui_adcreturnproc(GUI_Handle_t* gui)
{
    int ret = 0;
    
    if(gui->page->lastpage != NULL)
    {
        gui_switchpage(gui, gui->page->lastpage);
        ret = 1;
    }
    
    return ret;
}

static int gui_adcscrollproc(GUI_Handle_t* gui, GUIsinal_e signal)
{
    GUIpage_t* adcpage = gui->page;
    GUIadc_t* adc = gui->page->param;
    int dboxposi = 0;
    int dposition = 0;
    int ret = 0;
    
    if(signal == GUIsignal_scrollup)
    {
        if(adc->position > 0)
        {
            adc->position--;
            dposition = -1;
            if(adc->boxposi > 0)
            {
                adc->boxposi--;
                dboxposi = -1;
            }
        }
    }
    else if(signal == GUIsignal_scrolldown)
    {
        if(adc->position < adc->adcnum - 1)
        {
            adc->position++;
            dposition = 1;
            if(adc->boxposi < (SCREEN_WIDTH / 16) - 1)
            {
                adc->boxposi++;
                dboxposi = 1;
            }
        }
    }
    
    if(dposition != 0)
    {
        gui_changeposi(&adcpage->son->widgetlist[0]->posi, SETPOSITION_TARGET, 0, dboxposi * 16, adcpage->widgetlist[adc->position]->string->length - (adcpage->son->widgetlist[0]->posi.target.right - 14), dboxposi * 16, 0);
        
        if(dboxposi == 0)
        {
            int i = 0;
            for(i = 0; i < adc->adcnum; i++)
            {
                gui_syncchangeposi(adcpage->widgetlist[i], SETPOSITION_TARGET, 0, dposition * -16, 0, dposition * -16, 0);
            }
        }
        
        adc->selectedwidget = adcpage->widgetlist[adc->position];
        
        ret = 1;
    }
    
    return ret;
}

int gui_adcproc(GUI_Handle_t* gui)
{
    int ret = 0;
    
    GUIadc_t* adc = gui->page->param;
    GUIadcitem_t* item = adc->selectedwidget->param;
    
    if(adc->stopflag == 0)
    {
        // adc数据处理，必须在信号处理前执行，因为信号处理中可能会切换page
        int channel = item->channel;
        int adcvalue = adc->adcvalue[channel];
        adc->volt = adcvalue * 1.0 / 4095 * 3.3;
        int frontnum = adc->volt;
        int backnum = (int)(adc->volt * 100) % 100;
        char* voltstr = adc->voltstr;
        voltstr[0] = frontnum + 0x30;
        voltstr[2] = backnum / 10 + 0x30;
        voltstr[3] = backnum % 10 + 0x30;
        gui_setstring(gui->page->son->widgetlist[2]->string, voltstr);
    }
    
    switch(gui->signal)
    {
        case GUIsignal_enter:       ret = gui_adcenterproc(gui);                        break;
        case GUIsignal_return:      ret = gui_adcreturnproc(gui);                       break;
        case GUIsignal_scrollup:    ret = gui_adcscrollproc(gui, GUIsignal_scrollup);   break;
        case GUIsignal_scrolldown:  ret = gui_adcscrollproc(gui, GUIsignal_scrolldown); break;
        default:break;
    }
    gui->signal = GUIsignal_null;
    
    return ret;
}
