#ifndef __FONT_H
#define __FONT_H

typedef enum
{
    FontType_6x8,
    FontType_8x16,
    FontType_12x24,
    FontType_16x32,
} FontType_t;

typedef struct
{
    int width;
    int height;
    char* font;
} FontInfo_t;

FontInfo_t FONT_GetFontInfo(FontType_t fonttype);

#endif
