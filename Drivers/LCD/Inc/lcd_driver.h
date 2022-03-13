/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */


#ifndef NUCLEOPONG_LCD_DRIVER_H
#define NUCLEOPONG_LCD_DRIVER_H

#include "stm32l4xx.h"

#define DISP_X_SIZE             240
#define DISP_Y_SIZE             320
#define MAX_BURST               500

#ifdef __cplusplus
extern "C" {
#endif

struct current_font {
    const uint8_t *font;
    uint8_t x_size;
    uint8_t y_size;
    uint8_t offset;
    uint8_t numchars;
};


#ifdef __cplusplus
}
#define SmallFont       _SmallFont
#define BigFont         _BigFont
#include "../Src/fonts.cpp"     // no this isn't a typo
extern "C" {
#else
extern const uint8_t *__SmallFont;
extern const uint8_t *__BigFont;
#define SmallFont       __SmallFont
#define BigFont         __BigFont
#endif

void LCD_Write_COM(uint8_t VL);

void LCD_Write_DATA(uint8_t VL);

void LCD_Write_DATA16(uint8_t VH, uint8_t VL);

void LCD_Init(void);

void LCD_setXY(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

void LCD_setColor(uint8_t r, uint8_t g, uint8_t b);

void LCD_setColorBg(uint8_t r, uint8_t g, uint8_t b);

void LCD_clrXY(void);

void LCD_clrScr(void);

void LCD_fillScr(void);

void LCD_fastFill();

void LCD_drawHLine(uint16_t x, uint16_t y, int l);

void LCD_drawVLine(uint16_t x, uint16_t y1, uint16_t y2);

void LCD_fillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

void LCD_fillTriangle(uint16_t x, uint16_t y, int w, int h);

void LCD_setFont(const uint8_t *font);

void LCD_printChar(uint8_t c, uint16_t x, uint16_t y);

void LCD_print(const char *st, uint16_t x, uint16_t y);

/**
 * LCD_print with support for newlines, carriage returns, and automatic
 * line breaks
 * @param st string buffer to print
 * @param x x position of upper left corner of text
 * @param y y position of upper left corner of text
 */
void LCD_printLong(const char *st, uint16_t x, uint16_t y);

/**
 * Gets the width of the current font, in px
 * @return `uint8_t`, x font size
 */
uint8_t LCD_getFontX();

/**
 * Gets the height of the current font, in px
 * @return `uint8_t`, y font size
 */
uint8_t LCD_getFontY();



#ifdef __cplusplus
}
#endif

#endif //NUCLEOPONG_LCD_DRIVER_H
