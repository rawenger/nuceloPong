/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "menu.h"
#include "display.h"
#include "nunchuk_controller.h"
#include "systick.h"
#include "lcd_driver.h"
#include "util.h"

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

void resume_action(void) {
    LOG("resume action\r\n");
    return;
}

void options_action(void) {
    LOG("options action\r\n");
    return;
}

void quit_action(void) {
    LOG("quit action\r\n");
    return;
}

void restart_action(void) {
    LOG("restart action\r\n");
    return;
}

static menu *main_menu;

void show_menu() {
    main_menu = new menu("Main Menu");

    auto resume = new menu::menu_item("Resume", 0, MENU_ITEMS_Y);
    resume->center();
    auto options = new menu::menu_item("Options", resume);
    auto quit = new menu::menu_item("Quit", 20, DISP_Y_SIZE - 40 - menu::menu_item::font_dim.y);
    auto restart = new menu::menu_item("Restart", nullptr, quit);

    resume->assign_action(&resume_action);
    options->assign_action(&options_action);
    quit->assign_action(&quit_action);
    restart->assign_action(&restart_action);

    options->join_below(quit);
    // only want to join these in one direction
    restart->set_upper(options);


    main_menu->set_first(resume);
    main_menu->show();
}

void hide_menu() {
    main_menu->hide();
    delete main_menu;
}

void menu_idle() {

}

#ifdef __cplusplus
}
#endif