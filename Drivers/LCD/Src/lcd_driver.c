/*
 * Most of this is recycled from the LCD driver I used in 153A with the Microblaze CPU.
 * Obviously the platform-specific stuff is different (namely LCD_Init, and the LCD send routines).
 *
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "lcd_driver.h"
#include "spi.h"
#include "gpio.h"
#include "systick.h"

#define swap(type, i, j)        {type t = i; i = j; j = t;}

// Global variables
static int fch; // Foreground color upper byte
static int fcl; // Foreground color lower byte
static int bch; // Background color upper byte
static int bcl; // Background color lower byte
static int X1, X2; // horizontal drawing boundaries
static int Y1, Y2; // vertical drawing boundaries
static struct current_font cfont;

// Write command to LCD controller
void LCD_Write_COM(uint8_t VL) {
    GPIO_ClearPin(LCD_DC_GPIO, LCD_DC_PIN);
    GPIO_ClearPin(LCD_CS_GPIO, LCD_CS_PIN);
    SPI_Send_Data(LCD_SPI, &VL, 1);
    GPIO_SetPin(LCD_CS_GPIO, LCD_CS_PIN);
}

// Write 16-bit data to LCD controller
void LCD_Write_DATA16(uint8_t VH, uint8_t VL) {
    GPIO_SetPin(LCD_DC_GPIO, LCD_DC_PIN);
    GPIO_ClearPin(LCD_CS_GPIO, LCD_CS_PIN);
    uint8_t pData[2] = {VH, VL};
    SPI_Send_Data(LCD_SPI, pData, 2);
    GPIO_SetPin(LCD_CS_GPIO, LCD_CS_PIN);
}

// Write 8-bit data to LCD controller
void LCD_Write_DATA(uint8_t VL) {
    GPIO_SetPin(LCD_DC_GPIO, LCD_DC_PIN);
    GPIO_ClearPin(LCD_CS_GPIO, LCD_CS_PIN);
    SPI_Send_Data(LCD_SPI, &VL, 1);
    GPIO_SetPin(LCD_CS_GPIO, LCD_CS_PIN);
}

// Initialize LCD controller
void LCD_Init(void) {
    // enable
    GPIO_SetPin(LCD_RST_GPIO, LCD_RST_PIN);
    // spi init
    GPIO_SetPin(LCD_CS_GPIO, LCD_CS_PIN);
    // reset
    GPIO_ClearPin(LCD_RST_GPIO, LCD_RST_PIN);
    SysTick_Delay(200/*200*/);
    GPIO_ClearPin(LCD_CS_GPIO, LCD_CS_PIN);	//CS OFF
    SysTick_Delay(200/*200*/);
    GPIO_SetPin(LCD_RST_GPIO, LCD_RST_PIN);

//    GPIO_SetPin(LCD_CS_GPIO, LCD_CS_PIN);
//    GPIO_ClearPin(LCD_RST_GPIO, LCD_RST_PIN);

    // Reset
    LCD_Write_COM(0x01);
    SysTick_Delay(200); //Must wait > 5ms

    LCD_Write_COM(0xCB);
    LCD_Write_DATA(0x39);
    LCD_Write_DATA(0x2C);
    LCD_Write_DATA(0x00);
    LCD_Write_DATA(0x34);
    LCD_Write_DATA(0x02);

    LCD_Write_COM(0xCF);
    LCD_Write_DATA(0x00);
    LCD_Write_DATA(0XC1);
    LCD_Write_DATA(0X30);

    LCD_Write_COM(0xE8);
    LCD_Write_DATA(0x85);
    LCD_Write_DATA(0x00);
    LCD_Write_DATA(0x78);

    LCD_Write_COM(0xEA);
    LCD_Write_DATA(0x00);
    LCD_Write_DATA(0x00);

    LCD_Write_COM(0xED);
    LCD_Write_DATA(0x64);
    LCD_Write_DATA(0x03);
    LCD_Write_DATA(0X12);
    LCD_Write_DATA(0X81);

    LCD_Write_COM(0xF7);
    LCD_Write_DATA(0x20);

    LCD_Write_COM(0xC0);   //Power control
    LCD_Write_DATA(0x23);  //VRH[5:0]

    LCD_Write_COM(0xC1);   //Power control
    LCD_Write_DATA(0x10);  //SAP[2:0];BT[3:0]

    LCD_Write_COM(0xC5);   //VCM control
    LCD_Write_DATA(0x3e);  //Contrast
    LCD_Write_DATA(0x28);

    LCD_Write_COM(0xC7);   //VCM control2
    LCD_Write_DATA(0x86);  //--

    LCD_Write_COM(0x36);   // Memory Access Control
    LCD_Write_DATA(0x48);

    LCD_Write_COM(0x3A);
    LCD_Write_DATA(0x55);

    LCD_Write_COM(0xB1);
    LCD_Write_DATA(0x00);
    LCD_Write_DATA(0x18);

    LCD_Write_COM(0xB6);   // Display Function Control
    LCD_Write_DATA(0x08);
    LCD_Write_DATA(0x82);
    LCD_Write_DATA(0x27);


    LCD_Write_COM(0xF2); // 3GAMMA Function Disable
    LCD_Write_DATA(0x00);

    LCD_Write_COM(0x26); // Gamm Curve Selected
    LCD_Write_DATA(0x01);

    LCD_Write_COM(0xE0); // Positive Gamma Correction
    LCD_Write_DATA(0x0F);
    LCD_Write_DATA(0x31);
    LCD_Write_DATA(0x2B);
    LCD_Write_DATA(0x0C);
    LCD_Write_DATA(0x0E);
    LCD_Write_DATA(0x08);
    LCD_Write_DATA(0x4E);
    LCD_Write_DATA(0xF1);
    LCD_Write_DATA(0x37);
    LCD_Write_DATA(0x07);
    LCD_Write_DATA(0x10);
    LCD_Write_DATA(0x03);
    LCD_Write_DATA(0x0E);
    LCD_Write_DATA(0x09);
    LCD_Write_DATA(0x00);

    LCD_Write_COM(0xE1); // Negative Gamma Correction
    LCD_Write_DATA(0x00);
    LCD_Write_DATA(0x0E);
    LCD_Write_DATA(0x14);
    LCD_Write_DATA(0x03);
    LCD_Write_DATA(0x11);
    LCD_Write_DATA(0x07);
    LCD_Write_DATA(0x31);
    LCD_Write_DATA(0xC1);
    LCD_Write_DATA(0x48);
    LCD_Write_DATA(0x08);
    LCD_Write_DATA(0x0F);
    LCD_Write_DATA(0x0C);
    LCD_Write_DATA(0x31);
    LCD_Write_DATA(0x36);
    LCD_Write_DATA(0x0F);

    LCD_Write_COM(0x11);   //Exit Sleep
    SysTick_Delay(120); // wait > 10ms

    LCD_Write_COM(0x29);   //Display on
//    LCD_Write_COM(0x2c);

    // set rotation
    LCD_Write_COM(0x36);
    SysTick_Delay(1);
    LCD_Write_DATA(0x40|0x08);
}

// Set boundary for drawing
void LCD_setXY(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    X1 = x1;
    X2 = x2;
    Y1 = y1;
    Y2 = y2;

    LCD_Write_COM(0x2A);
    LCD_Write_DATA16(x1 >> 8U, x1);
    LCD_Write_DATA16(x2 >> 8U, x2);

    LCD_Write_COM(0x2B);
    LCD_Write_DATA16(y1 >> 8U, y1);
    LCD_Write_DATA16(y2 >> 8U, y2);

    LCD_Write_COM(0x2C);
}

// Set foreground RGB color for next drawing
void LCD_setColor(uint8_t r, uint8_t g, uint8_t b) {
    // 5-bit r, 6-bit g, 5-bit b
    fch = (r & 0x0F8) | g >> 5;
    fcl = (g & 0x1C) << 3 | b >> 3;
}

// Set background RGB color for next drawing
void LCD_setColorBg(uint8_t r, uint8_t g, uint8_t b) {
    // 5-bit r, 6-bit g, 5-bit b
    bch = (r & 0x0F8) | g >> 5;
    bcl = (g & 0x1C) << 3 | b >> 3;
}

// Clear display
void LCD_clrScr(void) {
    // Black screen
    LCD_setColor(0, 0, 0);
    LCD_setXY(0, 0, DISP_X_SIZE, DISP_Y_SIZE);
    LCD_fastFill();
}

// fill screen with selected color
void LCD_fillScr(void) {
    LCD_setXY(0, 0, DISP_X_SIZE, DISP_Y_SIZE);
    LCD_fastFill();
}


// Remove drawing boundary
void LCD_clrXY(void) {
    X1 = X2 = Y1 = Y2 = 0;
    LCD_setXY(0, 0, 0, 0);
}

// Draw horizontal line
void LCD_drawHLine(uint16_t x, uint16_t y, int l) {
    if (l < 0) {
        l = -l;
        x -= l;
    }

    LCD_setXY(x, y, x + l, y);
    LCD_fastFill();

    LCD_clrXY();
}

// Draw vertical line
void LCD_drawVLine(uint16_t x, uint16_t y1, uint16_t y2) {
    if (y2 < y1)
        swap(uint16_t, y1, y2);

    LCD_setXY(x, y1, x, y2);
    LCD_fastFill();
    LCD_clrXY();
}

// Quickly fill a rectangular area
void LCD_fillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    if (x1 > x2)
        swap(uint16_t, x1, x2);

    if (y1 > y2)
        swap(uint16_t, y1, y2);

    LCD_setXY(x1, y1, x2, y2);
    LCD_fastFill();

    LCD_clrXY();
}

// Fill a triangle
void LCD_fillTriangle(uint16_t x, uint16_t y, int w, int h) {
//	xil_printf("draw triangle\n\r");
    short fill_dir;
    if (h < 0) {
        fill_dir = 1;
    } else {
        fill_dir = -1;
    }
    y -= h;

    float delta = w / (fill_dir * h * -2.0);
    float l = x, u = x + w;
    int i = h;
    while (i != 0) {
        LCD_drawHLine((int) l, y + i, (int) u - l);
        l += delta;
        u -= delta;
        i += fill_dir;
    }
}

// Select the font used by print() and printChar()
void LCD_setFont(const uint8_t *font) {
    cfont.font = font;
    cfont.x_size = font[0];
    cfont.y_size = font[1];
    cfont.offset = font[2];
    cfont.numchars = font[3];
}

// Print a character
void LCD_printChar(uint8_t c, uint16_t x, uint16_t y) {
    uint8_t ch;
    int i, j, pixelIndex;

    LCD_setXY(x, y, x + cfont.x_size - 1, y + cfont.y_size - 1);

    pixelIndex = (c - cfont.offset) * (cfont.x_size >> 3) * cfont.y_size + 4;
    for (j = 0; j < (cfont.x_size >> 3) * cfont.y_size; j++) {
        ch = cfont.font[pixelIndex];
        for (i = 0; i < 8; i++) {
            if ((ch & (1 << (7 - i))) != 0)
                LCD_Write_DATA16(fch, fcl);
            else
                LCD_Write_DATA16(bch, bcl);
        }
        pixelIndex++;
    }

    LCD_clrXY();
}

// Print string
void LCD_print(const char *st, uint16_t x, uint16_t y) {
    int i = 0;

    while (*st != '\0')
        LCD_printChar(*st++, x + cfont.x_size * i++, y);
}

void LCD_printLong(const char *st, uint16_t x, uint16_t y) {
    int i = 0;

    while (*st != '\0') {
            switch (*st) {
                case '\n':
                    y += cfont.y_size;
                    // fall through to next case to return cursor to line start
                case '\r':
                    i = 0;
                    break;
                default:
                    LCD_printChar(*st, x + cfont.x_size * i++, y);
            }
            ++st;
    }
}

void LCD_fastFill() {

    uint32_t size = (X2 - X1) * (Y2 - Y1);

    GPIO_SetPin(LCD_DC_GPIO, LCD_DC_PIN);
    GPIO_ClearPin(LCD_CS_GPIO, LCD_CS_PIN);

    // max burst buffer size is 500
    uint32_t buf_size, block_size, remainder;
    if (size * 2 <= MAX_BURST) {
        buf_size = block_size = size;
        remainder = 0;
    } else {
        buf_size = MAX_BURST;
        uint32_t total_size = size * 2;
        block_size = total_size / MAX_BURST;
        remainder = total_size % MAX_BURST;
    }

    uint8_t buf[buf_size];
    for (uint32_t j = 0; j < buf_size; j += 2) {
        buf[j] = fch;
        buf[j + 1] = fcl;
    }

    for (uint32_t j = 0; j < block_size; j++)
        SPI_Send_Data(LCD_SPI, (uint8_t *) buf, buf_size);

    SPI_Send_Data(LCD_SPI, (uint8_t *) buf, remainder);

    GPIO_SetPin(LCD_CS_GPIO, LCD_CS_PIN);
}

uint8_t LCD_getFontX() {
    return cfont.x_size;
}

uint8_t LCD_getFontY() {
    return cfont.y_size;
}