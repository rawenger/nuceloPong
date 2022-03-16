/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include <util.h>
#include "pong_bot.h"
#include "nunchuk_controller.h"

//constexpr mouse::mouse() {}
mouse::report_type mouse::report{0, 0, 0, 0};

void mouse::reset()  {
//    mouse::mouse_x = mouse::mouse_y = 0;
    // TODO: figure out why this^ causes linker error

    report = {0, -127, 127, 0};

    for (int i = 0; i < 10; i++)
        send_report();

    report_delay();
    report_delay();
}

void mouse::find_ball() {
    reset();

    // go to the ball's starting point (542, -51): 127 * 4 + 36 * 1, -12 * 4 - 4 * 1
    x = 127;
    y = -12;
    for (int i = 0; i < 4; i++)
        send_report();
    x = 34;
    y = -3;
    send_report();

    SysTick_Delay(100);
}

void mouse::move(int8_t click, int8_t dx, int8_t dy) {
    buttons = click;
    x = dx;
    y = dy;
    send_report();
}

void mouse::behave_as_mouse() {
    buttons = static_cast<int8_t>(nc->get_c() | (nc->get_z() << 1U));
    x = static_cast<int8_t>(nc->get_stick_x());
    y = static_cast<int8_t>(-1 * nc->get_stick_y());
    wheel = 0;
    send_report();
}

bool mouse::track_cursor() {
    buttons = 0;
    mouse_x += (x = static_cast<int8_t>(nc->get_stick_x()));
    mouse_y += (y = static_cast<int8_t>(-1 * nc->get_stick_y())); // mouse inverts y-axis
    send_report();
    return nc->get_c();
}

void mouse::calibrate_sensitivity() {
    // tell the user to make sure the bar is moved all the way down to zero before calling this function!
    while (!nc->get_c())
        track_cursor();

    move(1, 0, 0);
    x = 127;

    for (int i = 0; i < 3; i++)
        send_report();

    release();
}

pong_bot *pb;

bool pong_bot::throw_ball() {

    CPP_LOG("throwing at cup " << cup << ", throw_1: " << throw_1 << ", missed: " << missed);

    if (cup > 9) {
        return false;
    }

    if (throw_1 && missed) {
        missed = false;
        return false;
    }

    const auto y_1_3 = static_cast<int8_t>(cups[cup].y / FULL_THROWS);
    const auto y_4 = static_cast<int8_t>(cups[cup].y % FULL_THROWS);
    const auto x_1_3 = static_cast<int8_t>(cups[cup].x / FULL_THROWS);
    const auto x_4 = static_cast<int8_t>(cups[cup].x % FULL_THROWS);

//    printf("full_throws: %d, y_pwr: %d, y_1_3: %d, y_4: %d, x_pwr: %d, x_1_3: %d, x_4: %d\r\n",
//           FULL_THROWS, y_pwr, y_1_3, y_4, x_pwr, x_1_3, x_4);
//    fflush(NULL);

    mouse::find_ball();
//    move_mouse(1, 0, 0);
    mouse::y = y_1_3;
    mouse::x = x_1_3;
#pragma GCC unroll 10
    for (int throws = 0; throws < FULL_THROWS; throws++)
        mouse::send_report();

    mouse::y = y_4;
    mouse::x = x_4;
    mouse::send_report();
    mouse::follow_through();

    SysTick_Delay(100);

    mouse::grab_ball();
    mouse::buttons = 1;
    mouse::x = mouse::y = 0;
    mouse::send_report();
    mouse::y = y_1_3;
    mouse::x = x_1_3;

#pragma GCC unroll 10
    for (int throws = 0; throws < FULL_THROWS; throws++)
        mouse::send_report();

    mouse::y = y_4;
    mouse::x = x_4;
    mouse::send_report();

    mouse::follow_through();
    ++cup;

    SysTick_Delay(100);


    throw_1 = !throw_1;

    return true;

}

void pong_bot::did_miss() {
    if (cup)
        --cup;

    missed = true;
}

void pong_bot::throw_xy(int x_pwr, int y_pwr) {
    const auto y_1_3 = static_cast<int8_t>(y_pwr / FULL_THROWS);
    const auto y_4 = static_cast<int8_t>(y_pwr % FULL_THROWS);
    const auto x_1_3 = static_cast<int8_t>(x_pwr / FULL_THROWS);
    const auto x_4 = static_cast<int8_t>(x_pwr % FULL_THROWS);

//    printf("full_throws: %d, y_pwr: %d, y_1_3: %d, y_4: %d, x_pwr: %d, x_1_3: %d, x_4: %d\r\n",
//           FULL_THROWS, y_pwr, y_1_3, y_4, x_pwr, x_1_3, x_4);
//    fflush(NULL);

    mouse::release();
    mouse::find_ball();
//    move_mouse(1, 0, 0);
    mouse::y = y_1_3;
    mouse::x = x_1_3;
#pragma GCC unroll 10
    for (int throws = 0; throws < FULL_THROWS; throws++)
        mouse::send_report();

    mouse::y = y_4;
    mouse::x = x_4;
    mouse::send_report();
    mouse::follow_through();

    SysTick_Delay(100);

    mouse::grab_ball();
    mouse::buttons = 1;
    mouse::x = mouse::y = 0;
    mouse::send_report();
    mouse::y = y_1_3;
    mouse::x = x_1_3;

#pragma GCC unroll 10
    for (int throws = 0; throws < FULL_THROWS; throws++)
        mouse::send_report();

    mouse::y = y_4;
    mouse::x = x_4;
    mouse::send_report();

    mouse::follow_through();
    ++cup;

    SysTick_Delay(100);
}

void pong_bot::reset(int starting_cup, bool rand) {
    cup = starting_cup;
    random_mode = rand;
    throw_1 = true;
    missed = false;
}
