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

#define MENU_INPUT_HOLD_DELAY           500
#define MENU_INPUT_SENSITIVITY          5

// joystick stuff
static bool joystick_moved_status = false;
static uint32_t last_change;

extern struct current_font cfont;
static menu *main_menu{nullptr};
static menu *options_menu{nullptr};
static menu *active_menu;
static struct config temp_cfg;

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

static void resume_action(menuitem itm) {
    LOG("resume action handler\r\n");
    QActive_postISR((QActive *) &nucleoPong, RESUME);
}

static void options_action(menuitem itm) {
    LOG("options action handler\r\n");
    QActive_postISR((QActive *) &nucleoPong, OPTIONS);
}

static void mouse_action(menuitem itm) {
    LOG("mouse action handler\r\n");
    QActive_postISR((QActive *) &nucleoPong, MOUSE);
}

_Noreturn static void quit_action(menuitem itm) {
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

static void restart_action(menuitem itm) {
    LOG("restart action handler\r\n");
    QActive_postISR((QActive *) &nucleoPong, RESTART);
}

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

    active_menu = main_menu;
    active_menu->show();
}

void hide_menu() {
    if (main_menu && !main_menu->is_hidden())
        main_menu->hide();
    delete main_menu;
    main_menu = nullptr;
}

void menu_idle() {
    if (active_menu->is_hidden())
        active_menu->show();

    int x, y;
    if (nc->get_c()) {
        active_menu->select();
        return;
    }
    x = nc->get_stick_x();
    y = nc->get_stick_y();
    uint16_t abs_x, abs_y;
    abs_x = __builtin_abs(x);
    abs_y = __builtin_abs(y);

    if (abs_y > MENU_INPUT_SENSITIVITY || abs_x > MENU_INPUT_SENSITIVITY) {

        if (!joystick_moved_status || SysTick_GetClk() - last_change > MENU_INPUT_HOLD_DELAY) {

            if (abs_y >= abs_x)     // move vertical
                active_menu->move_cursor((y > 0) ? menu::UP : menu::DOWN);
            else    // move horizontal
                active_menu->move_cursor((x > 0) ? menu::RIGHT : menu::LEFT);

            last_change = SysTick_GetClk();
            joystick_moved_status = true;
        }

    } else {
        joystick_moved_status = false;
    }

}

void menu_select() {
    active_menu->select();
}

static void toggle_random_mode(menuitem itm) {
    int val = !(itm->get_value());
        temp_cfg.random_mode = val;
        itm->update_value(val);
}

static void tracking_speed_up(menuitem itm) {
    if (itm->get_value() == 9)
        return;
    temp_cfg.mouse_tracking_speed = itm->get_value() + 1;
    itm->update_value(temp_cfg.mouse_tracking_speed);
}

static void tracking_speed_down(menuitem itm) {
    if (itm->get_value() == 1)
        return;
    temp_cfg.mouse_tracking_speed = itm->get_value() - 1;
    itm->update_value(temp_cfg.mouse_tracking_speed);
}

//TODO: make an action handler that calls delete on the options menu... hopefully doesn't delete itself (yikes)
void show_options(struct config *cfg) {
    temp_cfg = *cfg;

    main_menu->hide();

    options_menu = new menu("Options");

    auto random_mode = new menu::menu_item("Random mode", 40, MENU_ITEMS_Y);
    auto tracking_speed = new menu::menu_item("Mouse tracking speed", random_mode);
//    auto mouse = new menu::menu_item("Enter Mouse Mode", tracking_speed);
    auto save = new menu::menu_item("Save", 20, DISP_Y_SIZE - 40 - menu::menu_item::font_dim.y);
    auto cancel = new menu::menu_item("Cancel", nullptr, save);

    random_mode->default_value(cfg->random_mode);
    random_mode->assign_action(&toggle_random_mode, menu::LEFT);
    random_mode->assign_action(&toggle_random_mode, menu::CLICK);
    random_mode->assign_action(&toggle_random_mode, menu::RIGHT);

    tracking_speed->default_value(cfg->mouse_tracking_speed);
    tracking_speed->assign_action(&tracking_speed_up, menu::RIGHT);
    tracking_speed->assign_action(&tracking_speed_down, menu::LEFT);

    save->assign_action([cfg] (menuitem) {
        *cfg = temp_cfg;
        options_menu->hide();
        active_menu = main_menu;
    });

    cancel->assign_action([] (menuitem) {
        options_menu->hide();
        active_menu = main_menu;
//        delete options_menu;
    });

    tracking_speed->join_below(save);
    // only want to join these in one direction
    cancel->set_upper(tracking_speed);

    options_menu->set_first(random_mode);

    active_menu = options_menu;
    active_menu->show();
}

void hide_options() {
    if (options_menu && !options_menu->is_hidden())
        options_menu->hide();
    delete options_menu;
    options_menu = nullptr;
}

int prompt_for_success() {
    constexpr const char *prompt_msg = "did you make it? (c/z)";
    constexpr size_t len = __builtin_strlen(prompt_msg);

    LCD_print(prompt_msg, 0, 0);

    bool ret;
    while (true) {
        if (nc->get_c()) {
            ret = true;
            break;
        } else if (nc->get_z()) {
            ret = false;
            break;
        }
    }

    for (int i = 0; i < len; i++)
        LCD_printChar(' ', i * cfont.x_size, 0);

    return ret;
}


void show_mouse_instructions() {
    LCD_clrScr();
}

void hide_mouse_instructions() {
    return;
}

#ifdef __cplusplus
}
#endif