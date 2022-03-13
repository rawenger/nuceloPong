/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

/*
 * colors.h
 *
 * Parameter definitions for graphical elements of the application
 */

#ifndef NUCLEOPONG_COLORS_H
#define NUCLEOPONG_COLORS_H

#include "lcd_driver.h"

#define BG_COLOR 				        setColor(0, 0, 0)
#define TRIANGLE_COLOR 			        setColor(150, 255, 100)
#define VOLUME_COLOR 			        setColor(255, 140, 0)
#define MODE_COLOR 				        { setColorBg(200, 200, 200);	setColor(100, 0, 100); }

#define WELCOME_BG_COLOR                LCD_setColor(0, 0, 0)
#define WELCOME_BOX_COLOR               LCD_setColor(229, 255, 204)     // same as message bgColor
#define WELCOME_MSG_COLOR               { LCD_setColor(0, 0, 0); LCD_setColorBg(229, 255, 204); }
#define WELCOME_MSG_POS(x, y)           ((x) + 50), ((y) + 20)
#define WELCOME_BOX_POS(x, y)           WELCOME_MSG_POS((x) - 40, (y) - 10)
#define WELCOME_BOX_HEIGHT              (5 + 26 + 9 * 12 + 20)
#define WELCOME_BOX_WIDTH               (DISP_X_SIZE - 20)
#define BEGIN_BUTTON_COLOR              { LCD_setColor(229, 204, 255); LCD_setColorBg(0, 0, 0); }
#define BEGIN_BUTTON_POS(x, y)          (30 + (x)), (220 + (y))

#define MENU_ITEMS_FONT                 (SmallFont)
#define MENU_TITLE_FONT                 (BigFont)
#define MENU_TITLE_COLOR		        { LCD_setColorBg(0, 0, 0); LCD_setColor(229, 255, 204); }
#define MENU_UNSELECTED_COLOR	        { LCD_setColorBg(0, 0, 0); LCD_setColor(229, 255, 204); }
#define MENU_SELECTED_COLOR		        { LCD_setColorBg(229, 255, 204); LCD_setColor(0, 0, 0); }
#define MENU_TITLE_Y         	        (30) /* y-position of menu title (horizontally centered) */
#define MENU_ITEM_SPACING               (5U) // vertical space (in px) between menu items
#define MENU_ITEMS_Y         	        (MENU_TITLE_Y + 4 * MENU_ITEM_SPACING)

#endif //NUCLEOPONG_COLORS_H
