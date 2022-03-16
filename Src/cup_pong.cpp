/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

/*
 * C-compatible interface for mouse & pong_bot classes
 */


#include "cup_pong.h"
#include "pong_bot.h"

// possibly refactor these into a different file -- needed by Mouse_trackCursor()
#include "qpn_port.h"
#include "statemachine.h"

#ifdef __cplusplus
extern "C" {
#endif

void Mouse_calibrateSensitivity() {
    mouse::calibrate_sensitivity();
}

// move the mouse cursor and track its position
void Mouse_trackCursor() {
    if (mouse::track_cursor())
        // TODO: check if this needs to be the ISR version
        QActive_post/*ISR*/((QActive *) &nucleoPong, C_BTN);
}

void Mouse_behaveAsMouse() {
    mouse::behave_as_mouse();
}

void Mouse_move(int8_t click, int8_t dx, int8_t dy) {
    mouse::move(click, dx, dy);
}

void Mouse_getTrackedCoords(int *x, int *y) {
    mouse::get_tracked(*x, *y);
}

void PongBot_Init() {
    pb = new pong_bot();
}

int PongBot_throwBall() {
    return static_cast<int>(pb->throw_ball());
//    SysTick_Delay(THROW_DELAY);
//    return res;
}

void PongBot_didMiss() {
    pb->did_miss();
}

int PongBot_getCup() {
    return pb->get_cup();
}

void PongBot_reset(config *cfg) {
    pb->reset(cfg->starting_cup, cfg->random_mode);
}

int PongBot_gameOver() {
    return static_cast<int>(pb->game_over());
}

#ifdef __cplusplus
}
#endif
/*
*/