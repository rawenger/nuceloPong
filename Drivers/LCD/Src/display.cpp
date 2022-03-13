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

#include "qpn_port.h"
#include "statemachine.h"

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

    SysTick_Delay(250);

    LCD_clrScr();
}

static inline constexpr uint16_t center(const char *text, const uint8_t *font) {
    return (DISP_X_SIZE - __builtin_strlen(text) * font[0]) / 2;
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
    LCD_print(WELCOME_MSG, WELCOME_MSG_POS(0,0));

    LCD_setFont(SmallFont);
    LCD_printLong(WELCOME_NOTES, WELCOME_BOX_POS(10, 15 + BigFont[0]));

    LCD_setFont(SmallFont);
    BEGIN_BUTTON_COLOR;
    LCD_print("<Press button to begin>", BEGIN_BUTTON_POS(0,0));
}

void hide_welcome_screen() {
    LCD_clrScr();
}

static void resume_action(void) {
    LOG("resume action handler\r\n");
    QActive_postISR((QActive *) &nucleoPong, RESUME);
}

static void options_action(void) {
    LOG("options action handler\r\n");
    return;
}

static void mouse_action(void) {
    LOG("mouse action handler\r\n");
    QActive_postISR((QActive *) &nucleoPong, MOUSE);
}

_Noreturn static void quit_action(void) {
    LOG("quit action handler\r\n");

    constexpr const char *big_msg = "THE END";
    constexpr const char *small_msg = "<reset>";

    COLOR_WHITE;
    LCD_fillScr();
    COLOR_BLACK;
    LCD_setColorBg(255, 255, 255);

    LCD_setFont(BigFont);
    LCD_print(big_msg, center(big_msg, BigFont), 50);

    LCD_setFont(SmallFont);
    LCD_print(small_msg, center(small_msg, SmallFont), 100);

    __disable_irq();
    while (true)  {
        // blink reset prompt
        for (volatile int delay = 0; delay < 0x800000; delay++) ;
        COLOR_WHITE;
        LCD_print(small_msg, center(small_msg, SmallFont), 100);

        for (volatile int delay = 0; delay < 0x800000; delay++) ;
        COLOR_BLACK;
        LCD_print(small_msg, center(small_msg, SmallFont), 100);
    }
}

static void restart_action(void) {
    LOG("restart action handler\r\n");
    return;
}


static menu *main_menu;

void show_menu(int active_game) {
    main_menu = new menu("Main Menu");

    auto resume = new menu::menu_item(active_game ? "Resume" : "Start", 0, MENU_ITEMS_Y);
    resume->center();
    auto options = new menu::menu_item("Options", resume);
    auto mouse = new menu::menu_item("Enter Mouse Mode", options);
    auto quit = new menu::menu_item("Quit", 20, DISP_Y_SIZE - 40 - menu::menu_item::font_dim.y);
    auto restart = new menu::menu_item("Restart", nullptr, quit);

    resume->assign_action(&resume_action);
    options->assign_action(&options_action);
    mouse->assign_action(&mouse_action);
    quit->assign_action(&quit_action);
    restart->assign_action(&restart_action);

    mouse->join_below(quit);
    // only want to join these in one direction
    restart->set_upper(mouse);

    main_menu->set_first(resume);
    main_menu->show();

}

void hide_menu() {
    main_menu->hide();
    delete main_menu;
}

#define MENU_INPUT_HOLD_DELAY           500
#define MENU_INPUT_SENSITIVITY          5
static bool on_status = false;
static uint32_t last_change;
void menu_idle() {
    int x, y;
    x = nc->get_stick_x();
    y = nc->get_stick_y();
    uint16_t abs_x, abs_y;
    abs_x = __builtin_abs(x);
    abs_y = __builtin_abs(y);

    if (abs_y > MENU_INPUT_SENSITIVITY || abs_x > MENU_INPUT_SENSITIVITY) {

        if (!on_status || SysTick_GetClk() - last_change > MENU_INPUT_HOLD_DELAY) {

            if (abs_y >= abs_x)     // move vertical
                main_menu->move_cursor((y > 0) ? menu::UP : menu::DOWN);
            else    // move horizontal
                main_menu->move_cursor((x > 0) ? menu::RIGHT : menu::LEFT);

            last_change = SysTick_GetClk();
            on_status = true;
        }

    } else {
        on_status = false;
    }

}

void menu_select() {
    main_menu->select();
}

void show_options() {
    return;
}

void show_mouse_instructions() {
    LCD_clrScr();
    return;
}

void hide_mouse_instructions() {
    return;
}

#ifdef __cplusplus
}
#endif