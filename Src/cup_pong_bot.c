/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "cup_pong_bot.h"
#include "nunchuk.h"
#include "usbd_hid.h"
#include "usb_device.h"
#include "systick.h"

#define send_report()           USBD_HID_SendReport(&hUsbDeviceFS, m.buf, sizeof m);

extern USBD_HandleTypeDef hUsbDeviceFS;

static int mouse_x, mouse_y;

static union {
    struct {
        int8_t buttons;
        int8_t x;
        int8_t y;
        int8_t wheel;
        int8_t wakeup;
    } params;
    uint8_t buf[5];
} m = { .params = {.buttons = 0, .x = 0, .y = 0, .wheel = 0, .wakeup = 0 } };


static void findBall();
static void reset();
static void move_mouse(int8_t click, int8_t x, int8_t y);
static void mouse_release();

void Mouse_behaveAsMouse() {
    m.params.x = (int8_t) Nunchuk_readJoystickX();
    m.params.y = (int8_t) (-1 * Nunchuk_readJoystickY()); // mouse inverts y-axis
    m.params.buttons = (int8_t) (Nunchuk_readCButton() + (Nunchuk_readZButton() << 1));
    send_report();
}

void Mouse_trackCursor() {
    move_mouse(0,
                mouse_x += ((int8_t) Nunchuk_readJoystickX()),
                mouse_y += ((int8_t) (-1 * Nunchuk_readJoystickY()))); // mouse inverts y-axis
}

// adjusts the cursor sensitivity in iPad settings to a consistent value
void Mouse_calibrateSensitivity() {
    // tell the user to make sure the bar is moved all the way down to zero before calling this function!
    move_mouse(1, 0, 0);
    m.params.x = 127;
    SysTick_Delay(REPORT_DELAY);
    for (int i = 0; i < 3; i++) {
        send_report();
        SysTick_Delay(REPORT_DELAY);
    }
    mouse_release();
}

static void move_mouse(int8_t click, int8_t x, int8_t y) {
    m.params.buttons = click;
    m.params.x = x;
    m.params.y = y;
    send_report();
}

static void mouse_release() {
    for (int i = 0; i < sizeof m; i++)
        m.buf[i] = 0;

    send_report();
}

static void reset() {
    mouse_x = mouse_y = 0;

    // travel to a known location (bottom left corner of screen)
    m.params.buttons = 0;
    m.params.x = -127;
    m.params.y = 127;
    for (int i = 0; i < 10; i++) {
        send_report();
        SysTick_Delay(REPORT_DELAY);
    }

}

void find_ball() {
    reset();

    // go to the ball's starting point (542, -51): 127 * 4 + 36 * 1, -12 * 4 - 4 * 1
    m.params.x = 127;
    m.params.y = -12;
    for (int i = 0; i < 4; i++) {
        send_report();
        SysTick_Delay(REPORT_DELAY);
    }
    m.params.x = 36;
    m.params.y = -4;
    send_report();
    SysTick_Delay(REPORT_DELAY);
}