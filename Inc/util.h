/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 * Just some usefule macro definitions for the project
 */

#ifndef NUCLEOPONG_UTIL_H
#define NUCLEOPONG_UTIL_H

#define DEBUG_LOGGING


#define _CONCAT(_x, _y)                 _x##_y
#define CONCAT(_x, _y)                  _CONCAT(_x,_y)

#define SET_BIT_WAIT(_reg, _mask)       { (_reg) |= (_mask); \
                                            while (((_reg) & (_mask)) != (_mask)) ; }

#define CLR_BIT_WAIT(_reg, _mask)       { (_reg) &= ~(_mask); \
                                            while (((_reg) & (_mask)) == (_mask)) ; }

#define BTN_GPIO                        GPIOC
#define BTN_GPIO_PIN                    13U
#define LED_GPIO                        GPIOA
#define LED_GPIO_PIN                    5U
#define LCD_MOSI_GPIO                   GPIOA //GPIOB
#define LCD_MOSI_PIN                    7U //5U          // D11 (D4)
#define LCD_SCK_GPIO                    GPIOB
#define LCD_SCK_PIN                     3U          // D3
#define LCD_CS_GPIO                     GPIOB
#define LCD_CS_PIN                      6U //10U          // D10 (D6)
#define LCD_DC_GPIO                     GPIOC// GPIOB
#define LCD_DC_PIN                      7U //4U          // D9 (D5)
#define LCD_RST_GPIO                    GPIOA
#define LCD_RST_PIN                     9U //8U          // D8 (D7)
#define LCD_SPI                         SPI1
#define NUNCHUK_I2C                     I2C1


#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG_LOGGING
#define LOG(...)               { printf(__VA_ARGS__); fflush(NULL); }
#else
#define LOG(...)
#endif

#ifdef __cplusplus
}
#endif
#endif //NUCLEOPONG_UTIL_H
