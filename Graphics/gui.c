#include "gui.h"

void gui_init(GUI_Handle_t* gui)
{
    gui->state = GUIstate_proc;
    gui->signal = GUIsignal_null;
    
    gui->maincolor = GM_Color_Bright;
    
    gui->page = NULL;
    
    gui->fpsflag = 1;
    gui->dmabusyflag = 0;
    gui->switchpageflag = 0;
    gui->soundflag = 1;
    gui->gyroflag = 0;
    gui->sleepflag = 0;
    
    gui->signalproc = NULL;
    gui->extrapaintfunc = NULL;
    
    gui->animationcoefficient = 1;
    gui->faderate = 5;
    gui->animationrate = 5;
    gui->brightness = 100;
    gui->fps = 0;
    
    gui->ram = malloc(sizeof(char) * BUFFER_SIZE);
    
    gui->buzzer = NULL;
}

void gui_sendsignal(GUI_Handle_t* gui, GUIsinal_e signal)
{
    gui->signal = signal;
    if(gui->sleepflag == 1)
    {
        gui_switchsleepmode(gui);
        gui->signal = GUIsignal_null;
    }
}

void gui_switchpage(GUI_Handle_t* gui, GUIpage_t* page)
{
    gui->page = page;
    gui->switchpageflag = 1;
    GM_CopyBuffer(gui->ram);
    gui->signalproc = NULL;
}

static void gui_pageinit(GUI_Handle_t* gui)
{
    if(gui->page->type == GUIpagetype_list)
    {
        gui_setsignalproc(gui, gui_listproc);
        gui_listpageinit(gui);
    }
    else if(gui->page->type == GUIpagetype_menu)
    {
        gui_setsignalproc(gui, gui_menuproc);
        gui_menupageinit(gui);
    }
    else if(gui->page->type == GUIpagetype_adc)
    {
        gui_setsignalproc(gui, gui_adcproc);
        gui_adcpageinit(gui);
    }
}

void gui_setstartpage(GUI_Handle_t* gui, GUIpage_t* page)
{
    gui->page = page;
    gui_pageinit(gui);
}

void gui_setbuzzer(GUI_Handle_t* gui, TIM_TypeDef* TIMx, PWM_OCChannel_Typedef OCChannel, float Frequency, uint32_t duration)
{
    gui->buzzer = BUZZER_CreateBuzzer(TIMx, OCChannel, Frequency, duration);
}

void gui_buzzeralarm(GUI_Handle_t* gui)
{
    if(gui->buzzer != NULL && gui->soundflag == 1)
        BUZZER_Alarm(gui->buzzer);
}

void gui_encodercontrol(GUI_Handle_t* gui, int encoderret)
{
    if(gui->page->type == GUIpagetype_list)
        gui_listencodercontrol(gui, encoderret);
    else if(gui->page->type == GUIpagetype_menu)
        gui_menuencodercontrol(gui, encoderret);
    else if(gui->page->type == GUIpagetype_adc)
        gui_adcencodercontrol(gui, encoderret);
}

void gui_switchsoundstate(GUI_Handle_t* gui)
{
    gui->soundflag = !gui->soundflag;
}

void gui_switchjellyeffect(GUI_Handle_t* gui)
{
    if(gui->animationcoefficient == 1)
    {
        gui->animationcoefficient = JELLY_EFFECT;
        gui->animationrate = 10;
    }
    else
    {
        gui->animationcoefficient = 1;
    }
}

void gui_switchcolormode(GUI_Handle_t* gui)
{
    if(gui->maincolor == GM_Color_Bright)
    {
        gui->maincolor = GM_Color_Dark;
        OLED_SetColorMode(0);
    }
    else if(gui->maincolor == GM_Color_Dark)
    {
        gui->maincolor = GM_Color_Bright;
        OLED_SetColorMode(1);
    }
}

void gui_switchsleepmode(GUI_Handle_t* gui)
{
    if(gui->sleepflag == 1)
    {
        gui->sleepflag = 0;
        OLED_SetSleep(DISABLE);
    }
    else
    {
        gui->sleepflag = 1;
        OLED_SetSleep(ENABLE);
    }
}

void gui_setbrightness(GUI_Handle_t* gui)
{
    int brightness = gui->brightness * 1.0 / 100 * 255;
    OLED_SetBrightness(brightness);
}

static void gui_display(GUI_Handle_t* gui)
{
    if(gui->dmabusyflag == 0)
    {
        
    }
    if(OLED_Display() == 1)
        gui->dmabusyflag = 0;
    else
        gui->dmabusyflag = 1;
}

static int GUI_animationfade(GUI_Handle_t* gui)
{
    int i = 0;
    static int j = 0;
    char* p = GM_GetBuffer();
    GM_PasteBuffer(gui->ram);
    if(gui->maincolor == GM_Color_Dark)
    {
        switch(j)
        {
            case 0:for (i = 0; i < BUFFER_SIZE; i++) if (i % 2 != 0) p[i] = p[i] | 0xAA; break;
            case 1:for (i = 0; i < BUFFER_SIZE; i++) if (i % 2 != 0) p[i] = p[i] | 0x00; break;
            case 2:for (i = 0; i < BUFFER_SIZE; i++) if (i % 2 == 0) p[i] = p[i] | 0x55; break;
            case 3:for (i = 0; i < BUFFER_SIZE; i++) if (i % 2 == 0) p[i] = p[i] | 0x00; break;
        }
    }
    else if(gui->maincolor == GM_Color_Bright)
    {
        switch(j)
        {
            case 0:for (i = 0; i < BUFFER_SIZE; i++) if (i % 2 != 0) p[i] = p[i] & 0xAA; break;
            case 1:for (i = 0; i < BUFFER_SIZE; i++) if (i % 2 != 0) p[i] = p[i] & 0x00; break;
            case 2:for (i = 0; i < BUFFER_SIZE; i++) if (i % 2 == 0) p[i] = p[i] & 0x55; break;
            case 3:for (i = 0; i < BUFFER_SIZE; i++) if (i % 2 == 0) p[i] = p[i] & 0x00; break;
        }
    }
    j++;
    GM_CopyBuffer(gui->ram);
    Delay_ms((10 - gui->faderate) * 10);
    if(j == 4)
    {
        j = 0;
        return 1;
    }
    return 0;
}

static int GUI_animationunit(float* position, float* target, int rate, float coefficient)
{
    if(*position != *target)
    {
        if(*position < *target)
        {
            if((*target - *position) < ANIMATION_COEFFICIENT)
                *position = *target;
            else
                *position += ((*target - *position) / rate) * coefficient;
        }
        else if(*position > *target)
        {
            if((*position - *target) < ANIMATION_COEFFICIENT)
                *position = *target;
            else
                *position -= ((*position - *target) / rate) * coefficient;
        }
    }
    else
        return 1;
    return 0;
}

static int gui_animationrun(GUI_Handle_t* gui, GUIwidget_t* widget)
{
    float rate = (10 - gui->animationrate) + 1;
    float coefficient = gui->animationcoefficient;
    
    int ret = 1;
    ret &= GUI_animationunit(&widget->posi.curr.left, &widget->posi.target.left, rate, coefficient);
    ret &= GUI_animationunit(&widget->posi.curr.right, &widget->posi.target.right, rate, coefficient);
    ret &= GUI_animationunit(&widget->posi.curr.top, &widget->posi.target.top, rate, coefficient);
    ret &= GUI_animationunit(&widget->posi.curr.bottom, &widget->posi.target.bottom, rate, coefficient);
    ret &= GUI_animationunit(&widget->posi.curr.radius, &widget->posi.target.radius, rate, coefficient);
    
    if(ret == 1)
    {
        if(widget->anilist != NULL)
        {
            ret = 0;
            gui_addanimation(widget, widget->anilist->targetlist[widget->aniindex * 5], widget->anilist->targetlist[widget->aniindex * 5 + 1], 
            widget->anilist->targetlist[widget->aniindex * 5 + 2], widget->anilist->targetlist[widget->aniindex * 5 + 3], widget->anilist->targetlist[widget->aniindex * 5 + 4]);
            widget->aniindex++;
            if(widget->aniindex >= widget->aninum)
            {
                widget->anilist = NULL;
                widget->aniindex = 0;
                widget->aninum = 0;
            }
        }
    }
    
    if(widget->son != NULL)
    {
        ret &= gui_animationrun(gui, widget->son);
    }
    
    return ret;
}

static int gui_animationproc(GUI_Handle_t* gui, GUIpage_t* page)
{
    int i = 0;
    int ret = 1;
    for(i = 0; i < page->widgetnum; i++)
    {
        ret &= gui_animationrun(gui, page->widgetlist[i]);
    }
    
    if(page->son != NULL)
    {
        ret &= gui_animationproc(gui, page->son);
    }
    
    return ret;
}

static void gui_paintunit(GUIwidget_t* widget)
{
    switch(widget->type)
    {
        case GUItype_frame:
            if(widget->showcmd == ENABLE)
                GM_DrawRoundFrame(widget->posi.curr.left, widget->posi.curr.top, widget->posi.curr.right, widget->posi.curr.bottom, widget->posi.curr.radius, widget->color);
            if(widget->son != NULL)
                gui_paintunit(widget->son);
            break;
        case GUItype_box:
            if(widget->showcmd == ENABLE)
                GM_DrawRoundBox(widget->posi.curr.left, widget->posi.curr.top, widget->posi.curr.right, widget->posi.curr.bottom, widget->posi.curr.radius, widget->fillcolor, widget->color);
            if(widget->son != NULL)
                gui_paintunit(widget->son);
            break;
        case GUItype_circle:
            if(widget->showcmd == ENABLE)
                GM_DrawCircle(widget->posi.curr.left, widget->posi.curr.top, widget->posi.curr.radius, widget->color);
            if(widget->son != NULL)
                gui_paintunit(widget->son);
            break;
        case GUItype_disc:
            if(widget->showcmd == ENABLE)
                GM_DrawDisc(widget->posi.curr.left, widget->posi.curr.top, widget->posi.curr.radius, widget->fillcolor, widget->color);
            if(widget->son != NULL)
                gui_paintunit(widget->son);
            break;
        case GUItype_image:
            if(widget->showcmd == ENABLE)
                GM_ShowImage(widget->posi.curr.left, widget->posi.curr.top, widget->image->ima, widget->image->height, widget->image->width, widget->color);
            if(widget->son != NULL)
                gui_paintunit(widget->son);
            break;
        case GUItype_string:
            if(widget->showcmd == ENABLE)
            {
                if(widget->string->align == GUIalign_right)
                    GM_ShowString(widget->posi.curr.left, widget->posi.curr.top, widget->string->str, widget->string->fonttype, widget->color);
                else
                    GM_ShowString(widget->posi.curr.left - widget->string->length, widget->posi.curr.top, widget->string->str, widget->string->fonttype, widget->color);
            }
            if(widget->son != NULL)
                gui_paintunit(widget->son);
            break;
        case GUItype_number:
            if(widget->showcmd == ENABLE)
            {
                if(widget->number->align == GUIalign_right)
                    GM_ShowNumber(widget->posi.curr.left, widget->posi.curr.top, widget->number->num, widget->number->digits, (int)widget->number->num<0, widget->number->fonttype, widget->color);
                else
                    GM_ShowNumber(widget->posi.curr.left - widget->number->length, widget->posi.curr.top, widget->number->num, widget->number->digits, (int)widget->number->num<0, widget->number->fonttype, widget->color);
            }
            if(widget->son != NULL)
                gui_paintunit(widget->son);
            break;
        default:break;
    }
}

void gui_paintproc(GUIpage_t* page)
{
    int i = 0;
    if(page->showcmd == ENABLE)
    {
        for(i = 0; i < page->widgetnum; i++)
        {
            gui_paintunit(page->widgetlist[i]);
        }
    }
    
    if(page->son != NULL)
    {
        gui_paintproc(page->son);
    }
}

void gui_pageproc(GUI_Handle_t* gui)
{
    if(gui->signalproc != NULL)
    {
        if(gui->signalproc(gui) == 1)
        {
            gui_buzzeralarm(gui);
        }
    }
    
    if(gui->switchpageflag == 1)
    {
        if(GUI_animationfade(gui) == 1)
        {
            gui->switchpageflag = 0;
            gui_setextrapaintfunc(gui, NULL);
            gui_pageinit(gui);
        }
    }
    else
    {
        GM_SetBuffer(0x00);
        gui_animationproc(gui, gui->page);
        gui_paintproc(gui->page);
        
        if(gui->extrapaintfunc != NULL)
            gui->extrapaintfunc(gui);
    }
}

void gui_setsignalproc(GUI_Handle_t* gui, int (*signalproc)(GUI_Handle_t*))
{
    gui->signalproc = signalproc;
}

void gui_setextrapaintfunc(GUI_Handle_t* gui, void (*extrapaintfunc)(void*))
{
    gui->extrapaintfunc = extrapaintfunc;
}

void gui_stopproc(GUI_Handle_t* gui)
{
    
}

void gui_proc(GUI_Handle_t* gui)
{
    if(gui->dmabusyflag == 0)
    {
        switch(gui->state)
        {
            case GUIstate_proc:    gui_pageproc(gui);      break;
            case GUIstate_stop:    gui_stopproc(gui);      break;
            default:break;
        }
    }
    gui_display(gui);
    
    if(gui->buzzer != NULL)
    {
        if(SFTTIM_Ticking(gui->buzzer->timer) == 1)
        {
            SFTTIMCmd(gui->buzzer->timer, DISABLE);
            TIM_Cmd(gui->buzzer->pwm->TIMER_InitStruct.TIMER_TIMx, DISABLE);
            gui->buzzer->state = DISABLE;
        }
    }
}
