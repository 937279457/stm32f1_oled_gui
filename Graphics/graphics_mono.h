#ifndef __GRAPHICS_MONO_H
#define __GRAPHICS_MONO_H

#include "stm32f10x.h"
#include "font.h"

#define     SCREEN_LENGTH   128
#define     SCREEN_WIDTH    64
#define     BUFFER_SIZE     (SCREEN_LENGTH * SCREEN_WIDTH / 8)

typedef enum
{
    GM_Color_Bright = 0,
    GM_Color_Dark = 1,
    GM_Color_Inverse = 2,
} GM_Color_t;

char* GM_GetBuffer(void);
void GM_SetBufferPointer(char* p);
void GM_SetBuffer(char data);
void GM_CopyBuffer(char* buf);
void GM_PasteBuffer(char* buf);

void GM_DrawPoint(int x, int y, GM_Color_t color);
int GM_ReadPoint(int x, int y);
void GM_DrawHLine(int x1, int x2, int y, GM_Color_t color);
void GM_DrawVLine(int y1, int y2, int x, GM_Color_t color);
void GM_DrawLine(int x1, int y1, int x2, int y2, GM_Color_t color);
void GM_DrawFrame(int left, int top, int right, int bottom, GM_Color_t color);
void GM_DrawRoundFrame(int left, int top, int right, int bottom, int radius, GM_Color_t color);
void GM_DrawSeparateCircle(int xc, int yc, int radius, int dwidth, int dheight, GM_Color_t color);
void GM_DrawBox(int left, int top, int right, int bottom, GM_Color_t fillcolor, GM_Color_t color);
void GM_DrawRoundBox(int left, int top, int right, int bottom, int radius, GM_Color_t fillcolor, GM_Color_t color);
void GM_DrawCircle(int xc, int yc, int radius, GM_Color_t color);
void GM_DrawDisc(int xc, int yc, int radius, GM_Color_t fillcolor, GM_Color_t color);

void GM_StoreImage(int x, int y, char* store, int height, int width);
void GM_ShowImage(int x, int y, char* image , int height, int width, GM_Color_t color);
void GM_ShowChar(int x, int y, char Char, FontType_t font, GM_Color_t color);
void GM_ShowString(int x, int y, char* String, FontType_t font, GM_Color_t color);
int GM_Pow(int index);
int GM_BegDigits(int num);
void GM_ShowNumber(int x, int y, uint32_t num, int length, int sign, FontType_t font, GM_Color_t color);
void GM_ShowFloatNum(int x, int y, float num, uint8_t frontpoint, uint8_t backpoint, uint8_t sign, FontType_t font, GM_Color_t color);

#endif
