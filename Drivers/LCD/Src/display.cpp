/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "menu.h"
#include "display.h"
#include "systick.h"
#include "lcd_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

extern struct current_font cfont;

void Display_Init() {
    LCD_Init();

    LCD_clrScr();
    LCD_setColor(255, 0, 255);
    LCD_fillRect(20, 20, 220, 300);

    LCD_setColorBg(0, 0, 0);
    LCD_setColor(0, 255, 0);
    LCD_setFont(BigFont);
    LCD_print("Starting...", 40, 6);
    LCD_setXY(20, 200, 40, 220);
    LCD_fastFill();

    SysTick_Delay(250);

    LCD_clrScr();
}

void show_welcome_screen() {
    /* Welcome!
     * Display initial instructions:
     *  - make sure cursor is correctly calibrated in iPad settings
     *  - press blue btn to show menu at any point
     *  <Press blue button to continue>
     */

    WELCOME_BOX_COLOR;
    LCD_fillRect(WELCOME_BOX_POS(0,0), WELCOME_BOX_POS(WELCOME_BOX_WIDTH, WELCOME_BOX_HEIGHT));

    WELCOME_MSG_COLOR;
    LCD_setFont(BigFont);
    LCD_print("Welcome!", WELCOME_MSG_POS(0,0));
    uint8_t big_ysize = LCD_getFontY();

    LCD_setFont(SmallFont);
    LCD_printLong(WELCOME_NOTES, WELCOME_MSG_POS(-30, 5 + big_ysize));

    LCD_setFont(SmallFont);
    BEGIN_BUTTON_COLOR;
    LCD_print("<Press button to begin>", BEGIN_BUTTON_POS(0,0));
}

void hide_welcome_screen() {
    LCD_clrScr();
}

void show_menu() {
    menu main_menu("Main Menu");
    main_menu.show();
    while (true) {
        printf("enter direction\r\n");
        fflush(NULL);
        char dir;
        scanf("%c", &dir);
        fflush(NULL);
        switch (dir) {
            case 'w':
                main_menu.move_cursor(menu::UP);
                break;
            case 'a':
                main_menu.move_cursor(menu::LEFT);
                break;
            case 's':
                main_menu.move_cursor(menu::DOWN);
                break;
            case 'd':
                main_menu.move_cursor(menu::RIGHT);
                break;
            default:
                printf("error\r\n");
        }
    }
}

#ifdef __cplusplus
}
#endif