/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

/*
 * C-compatible interface for mouse & pong_bot classes
 */


#include "cup_pong.h"
#include "pong_bot.h"

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

void PongBot_throwBall() {
    pb->throw_ball();
}

int PongBot_didMiss() {
    return static_cast<int>(pb->did_miss());
}

int PongBot_getCup() {
    return pb->get_cup();
}

// take num samples by prompting for an x,y power & throwing, then save the results to an array
// and display them
void sample_xy(int samples) {
    printf("Sampling y throw %d times\r\n", samples);
//    int sample_buf[samples];
//    int y_buf[samples];
    int mouse_x_dist, mouse_y_dist;

    for (int i = 0; i < samples; i++) {
        printf("Enter <x,y> travel distance: \r\n");
        scanf("%d %d", &mouse_x_dist, &mouse_y_dist);
        fflush(NULL);
//        y_buf[i] = mouse_y_dist;
        pb->throw_xy(mouse_x_dist, mouse_y_dist);
    }

//    get_results(sample_buf, samples);
//
//    // output the samples in CSV format
//    printf("-----RESULTS-----\r\n");
//    printf("power list:\r\n");
//    for (int i = 0; i < samples; i++) {
//        printf("%d, ", y_buf[i]);
//        fflush(NULL);
//    }
//    printf("\r\nresult list:\r\n");
//    for (int i = 0; i < samples; i++) {
//        printf("%d, ", sample_buf[i]);
//        fflush(NULL);
//    }
}
#ifdef __cplusplus
}
#endif
/*
*/