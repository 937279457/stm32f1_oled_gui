#include "graphics_mono.h"

static char* buffer;
static int fillleft[SCREEN_WIDTH];
static int fillright[SCREEN_WIDTH];
static int fillcneter_x = 0;
static int filltop = 0;
static int fillbottom = 0;
static int trackedgeflag = 0;

char* GM_GetBuffer(void)
{
    return buffer;
}

void GM_SetBufferPointer(char* p)
{
    buffer = p;
}

void GM_SetBuffer(char data)
{
    int i = 0;
    for(i = 0; i < BUFFER_SIZE; i++)
        buffer[i] = data;
}

void GM_CopyBuffer(char* buf)
{
    int i = 0;
    for(i = 0; i < BUFFER_SIZE; i++)
        buf[i] = buffer[i];
}

void GM_PasteBuffer(char* buf)
{
    int i = 0;
    for(i = 0; i < BUFFER_SIZE; i++)
        buffer[i] = buf[i];
}
static void GM_Fill(GM_Color_t color)
{
    int i = 0;
    for(i = filltop + 1; i < fillbottom; i++)
    {
        if(i < 0 || i >= SCREEN_WIDTH)
            continue;
        GM_DrawHLine(fillleft[i] + 1, fillright[i] - 1, i, color);
    }
    
}

static void GM_TrackEdgeEnable(int top, int bottom, int center_x)
{
    trackedgeflag = 1;
    fillcneter_x = center_x;
    filltop = top;
    fillbottom = bottom;
}

static void GM_TrackEdgeDisable(void)
{
    trackedgeflag = 0;
}

static int GM_CheckRectRange(int left, int top, int right, int bottom)
{
    if(left >= SCREEN_LENGTH)
        return 0;
    if(top >= SCREEN_WIDTH)
        return 0;
    if(right < 0)
        return 0;
    if(bottom < 0)
        return 0;
    
    return 1;
}

static int GM_CheckCircleRange(int cx, int cy, int radius)
{
    if(cx - radius >= SCREEN_LENGTH)
        return 0;
    if(cy - radius >= SCREEN_WIDTH)
        return 0;
    if(cx + radius < 0)
        return 0;
    if(cy + radius < 0)
        return 0;
    
    return 1;
}

void GM_DrawPoint(int x, int y, GM_Color_t color)
{
    if(trackedgeflag == 1) // 先记录边界再判断该点是否在屏幕内，确保边界正确
    {
        if(y >= 0 && y < SCREEN_WIDTH)
        {
            if(x <= fillcneter_x)
            {
                if(x < -1) // 边界小于-1时设置边界为-1
                {
                    fillleft[y] = -1;
                }
                else
                    fillleft[y] = x;
            }
            else
            {
                if(x > SCREEN_LENGTH) // 边界大于SCREEN_LENGTH时设置边界为SCREEN_LENGTH
                {
                    fillright[y] = SCREEN_LENGTH;
                }
                else
                fillright[y] = x;
            }
        }
    }
    
    if(x>=SCREEN_LENGTH || x<0 || y>=SCREEN_WIDTH || y<0)
        return;
    
    switch(color)
    {
        case GM_Color_Bright:   buffer[(y/8)*SCREEN_LENGTH + x] |= (1<<(y%8));  break;
        case GM_Color_Dark:     buffer[(y/8)*SCREEN_LENGTH + x] &= ~(1<<(y%8)); break;
        case GM_Color_Inverse:  buffer[(y/8)*SCREEN_LENGTH + x] ^= (1<<(y%8));  break;
    }
}

int GM_ReadPoint(int x, int y)
{
    if((buffer[x + y/8*SCREEN_LENGTH] & (1 << y%8)) > 0)
        return 1;
    return 0;
}

void GM_DrawHLine(int x1, int x2, int y, GM_Color_t color)
{
    int i = 0;
    int start = 0;
    int end = 0;
    if(x1 < x2)
    {
        start = x1;
        end = x2;
    }
    else
    {
        start = x2;
        end = x1;
    }
    for(i = start; i <= end; i++)
    {
        GM_DrawPoint(i, y, color);
    }
}

void GM_DrawVLine(int y1, int y2, int x, GM_Color_t color)
{
    int i = 0;
    int start;
    int end;
    if(y1 < y2)
    {
        start = y1;
        end = y2;
    }
    else
    {
        start = y2;
        end = y1;
    }
    for(i = start; i <= end; i++)
    {
        GM_DrawPoint(x, i, color);
    }
}

void GM_DrawLine(int x1, int y1, int x2, int y2, GM_Color_t color) // Bresenham算法
{
    int i = 0;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance; 
    int incx, incy, uRow, uCol; 
    delta_x = x2 - x1; 
    delta_y = y2 - y1; 
    uRow = x1; 
    uCol = y1; 
    if(delta_x > 0)
        incx = 1; 
    else if(delta_x == 0)
        incx = 0; 
    else
    {
        incx = -1; 
        delta_x = -delta_x;
    } 
    if(delta_y > 0)
        incy = 1; 
    else if(delta_y == 0)
        incy = 0; 
    else
    {
        incy = -1;
        delta_y = -delta_y;
    } 
    if(delta_x > delta_y)
        distance = delta_x; 
    else 
        distance = delta_y; 
    GM_DrawPoint(x1, y1, color);// 初始点单独绘制
    for(i = 0; i <= distance + 1; i++) 
    { 
        GM_DrawPoint(uRow, uCol, color);
        xerr += delta_x; 
        yerr += delta_y; 
        if(xerr > distance) 
        { 
            xerr -= distance; 
            uRow += incx;
        }
        if(yerr > distance)
        {
            yerr -= distance; 
            uCol += incy;
        }
    }
}

void GM_DrawFrame(int left, int top, int right, int bottom, GM_Color_t color)
{
    if(GM_CheckRectRange(left, top, right, bottom) == 0)
        return;
    if(right - left <= 0 || bottom - top <= 0)
        return;
    GM_DrawHLine(left, right - 1, top, color);
    GM_DrawVLine(top, bottom - 1, right, color);
    GM_DrawHLine(left + 1, right, bottom, color);
    GM_DrawVLine(top + 1, bottom, left, color);
}

void GM_DrawBox(int left, int top, int right, int bottom, GM_Color_t fillcolor, GM_Color_t color)
{
    if(GM_CheckRectRange(left, top, right, bottom) == 0)
        return;
    if(right - left <= 0 || bottom - top <= 0)
        return;
    GM_TrackEdgeEnable(top, bottom, (right + left)/2);
    GM_DrawFrame(left, top, right, bottom, color);
    GM_TrackEdgeDisable();
    if(right - left > 1)
    {
        GM_Fill(fillcolor);
    }
}

static void GM_DrawCircleUnit(int xc, int yc, int x, int y, int dwidth, int dheight, int radius, GM_Color_t color)
{
    int inx = 0;
    int iny = 0;
    int offsetx = 0;
    int offsety = 0;
    if(dwidth != 0 || dheight != 0)
    {
        if(dwidth % 2 != 0)
            inx = 1;
        if(dheight % 2 != 0)
            iny = 1;
        offsetx = dwidth / 2 - radius;
        offsety = dheight / 2 - radius;
    }
    
    /*
    +--------------> X
    |      6  7
    |    5      8
    |    4      1
    |      3  2
    v
    Y
    */
    
    GM_DrawPoint(xc + y + offsetx + inx, yc + x + offsety + iny, color);       // 1
    GM_DrawPoint(xc - x - offsetx, yc + y + offsety + iny, color);             // 3
    GM_DrawPoint(xc - y - offsetx, yc - x - offsety, color);                   // 5
    GM_DrawPoint(xc + x + offsetx + inx, yc - y - offsety, color);             // 7
    if(x != y && x != 0) // 防止绘制重复的点
    {
        GM_DrawPoint(xc + x + offsetx + inx, yc + y + offsety + iny, color);   // 2
        GM_DrawPoint(xc - y - offsetx, yc + x + offsety + iny, color);         // 4
        GM_DrawPoint(xc - x - offsetx, yc - y - offsety, color);               // 6
        GM_DrawPoint(xc + y + offsetx + inx, yc - x - offsety, color);         // 8
    }
}

void GM_DrawSeparateCircle(int xc, int yc, int radius, int dwidth, int dheight, GM_Color_t color) // Bresenham算法
{
    if(radius == 0)
        return;
    int x = 0, y = radius, d;
    d = 3 - 2 * radius;
    
    if(dwidth != 0 || dheight != 0)
        x = 1;
    
    while(x <= y)
    {
        GM_DrawCircleUnit(xc, yc, x, y, dwidth, dheight, radius, color);
        
        if (d < 0)
        {
            d = d + 4*x + 6;
        }
        else 
        {
            d = d + 4*(x - y) + 10;
            y--;
        }
        x++;
    }
}

void GM_DrawCircle(int xc, int yc, int radius, GM_Color_t color)
{
    if(GM_CheckCircleRange(xc, yc, radius) == 0)
        return;
    GM_DrawSeparateCircle(xc, yc, radius, 0, 0, color);
}

void GM_DrawDisc(int xc, int yc, int radius, GM_Color_t fillcolor, GM_Color_t color)
{
    if(GM_CheckCircleRange(xc, yc, radius) == 0)
        return;
    GM_TrackEdgeEnable(yc - radius, yc + radius, xc);
    GM_DrawCircle(xc, yc, radius, color);
    GM_TrackEdgeDisable();
    if(radius > 1)
    {
        GM_Fill(fillcolor);
    }
}

void GM_DrawRoundFrame(int left, int top, int right, int bottom, int radius, GM_Color_t color)
{
    if(GM_CheckRectRange(left, top, right, bottom) == 0)
        return;
    if(right - left <= 0 || bottom - top <= 0)
        return;
    int r = radius;
    int dx = right - left;
    int dy = bottom - top;
    if(dx <= 0 || dy <= 0)
        return;
    if(2*r > dx)
        r = dx/2;
    if(2*r > dy)
        r = dy/2;
    int xc = (right + left)/2;
    int yc = (bottom + top)/2;
    if(right + left < 0)
        xc--;
    if(bottom + top < 0)
        yc--;
    GM_DrawSeparateCircle(xc, yc, r, dx, dy, color);
    GM_DrawHLine(left + r, right - r, top, color);
    GM_DrawVLine(top + r, bottom - r, right, color);
    GM_DrawHLine(left + r, right - r, bottom, color);
    GM_DrawVLine(top + r, bottom - r, left, color);
    
}

void GM_DrawRoundBox(int left, int top, int right, int bottom, int radius, GM_Color_t fillcolor, GM_Color_t color)
{
    if(GM_CheckRectRange(left, top, right, bottom) == 0)
        return;
    if(right - left <= 0 || bottom - top <= 0)
        return;
    GM_TrackEdgeEnable(top, bottom, (right + left)/2);
    GM_DrawRoundFrame(left, top, right, bottom, radius, color);
    GM_TrackEdgeDisable();
    if(right - left > 1)
    {
        GM_Fill(fillcolor);
    }
}

void GM_ShowImage(int x, int y, char* image , int width, int height, GM_Color_t color)
{
    if(GM_CheckRectRange(x, y, x + width, y + height) == 0)
        return;
    int i = 0;
    int j = 0;
    for(i = 0; i < width; i++)
    {
        for(j = 0; j < height; j++)
        {
            if((image[i + (j/8*width)] & (1<<(j%8))) > 0)
            {
                GM_DrawPoint(x + i, y + j, color);
            }
        }
    }
}

void GM_StoreImage(int x, int y, char* store, int width, int height)
{
    int i = 0;
    int j = 0;
    for(i = 0; i < (height/8 + 1)*width; i++)
    {
        store[i] = 0x00;
    }
    for(i = 0; i < width; i++)
    {
        for(j = 0; j < height; j++)
        {
            store[i + (j/8*width)] |= (buffer[x + i + (y + j)/8*SCREEN_LENGTH] & (1 << (y + j)%8)) >> y%8;
        }
    }
}

void GM_ShowChar(int x, int y, char Char, FontType_t font, GM_Color_t color)
{
    FontInfo_t fontinfo = FONT_GetFontInfo(font);
    GM_ShowImage(x, y, &fontinfo.font[(Char - ' ') * (fontinfo.width * (fontinfo.height / 8))], fontinfo.width, fontinfo.height, color);
}

void GM_ShowString(int x, int y, char* String, FontType_t font, GM_Color_t color)
{
    int i = 0;
    FontInfo_t fontinfo = FONT_GetFontInfo(font);
    for(i=0;String[i]!=0;i++)
    {
        GM_ShowChar(x + i * fontinfo.width, y, String[i], font, color);
    }
}

int GM_Pow(int index) // 生成10的n次方
{
    int i = 1;
    if(index)
    {
        while(index--)
        {
            i *= 10;
        }
        return i;
    }
    return 1;
}

int GM_BegDigits(int num) // 计算整数的位数
{
    int i = 0;
    while(1)
    {
        num /= 10;
        i++;
        if(num == 0)
            return i;
    }
}

void GM_ShowNumber(int x, int y, uint32_t num, int length, int sign, FontType_t font, GM_Color_t color)
{
    int i = 0;
    FontInfo_t fontinfo = FONT_GetFontInfo(font);
    if(sign)
    {
        if(num>0xffff)
        {
            GM_ShowChar(x, y, '-', font, color);
            num = ~(num-1);
        }
        else
            GM_ShowChar(x, y, '+', font, color);
    }
    for(i=0;i<length;i++)
    {
        GM_ShowChar(x + (length + sign - i - 1) * fontinfo.width, y, (num % GM_Pow(i + 1)) / GM_Pow(i) + 48, font, color);
    }
}

void GM_ShowFloatNum(int x, int y, float num, uint8_t frontpoint, uint8_t backpoint, uint8_t sign, FontType_t font, GM_Color_t color)
{
    FontInfo_t fontinfo = FONT_GetFontInfo(font);
    if(sign)
    {
        if(num < 0)
        {
            num += 2*(0 - num);
            GM_ShowChar(x, y, '-', font, color);
        }
        else
        {
            GM_ShowChar(x, y, '+', font, color);
        }
    }
    uint32_t num1 = (int)num;
    uint32_t num2 = (int)(num*GM_Pow(backpoint))%GM_Pow(backpoint);
    GM_ShowNumber(x + sign * fontinfo.width, y, num1, frontpoint, 0, font, color);
    GM_ShowChar(x + (sign + frontpoint) * fontinfo.width, y, '.', font, color);
    GM_ShowNumber(x + (sign + frontpoint + 1) * fontinfo.width, y, num2, backpoint, 0, font, color);
}
