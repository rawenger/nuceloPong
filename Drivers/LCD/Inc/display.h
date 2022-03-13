/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */


#ifndef NUCLEOPONG_DISPLAY_H
#define NUCLEOPONG_DISPLAY_H

#include "colors.h"


#define WELCOME_NOTES       "A few notes before you get\n" \
                            "started:\n" \
                            " - Make sure the mouse\n" \
                            "   cursor is correctly\n" \
                            "   calibrated in iPad\n"  \
                            "   settings.\n"               \
                            " - Press the blue button\n" \
                            "   on the nucleo to show\n" \
                            "   the main menu."

#ifdef __cplusplus
extern "C" {
#endif

struct config {
    int random_mode;
    int mouse_mode;
};

struct point {
    uint16_t x;
    uint16_t y;
};

void Display_Init();

void show_welcome_screen();
void hide_welcome_screen();

void show_menu();
void hide_menu();
void menu_idle();
void menu_select();

#ifdef __cplusplus
}
#endif

#endif //NUCLEOPONG_DISPLAY_H
