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

void PongBot_throwBall() {
    pb->throw_ball();
}

int PongBot_didMiss() {
    return static_cast<int>(pb->did_miss());
}

int PongBot_getCup() {
    return pb->get_cup();
}

//#ifdef __cplusplus
//}
//#endif

/*

//#define send_report()           { USBD_HID_SendReport(&hUsbDeviceFS, m.report, sizeof m); \
//                                    SysTick_Delay(POLLING_RATE); }
//
//#define mouse_release()         {  m.report[0] = 0; m.report[1] = 0; m.report[2] = 0; m.report[3] = 0; \
//                                    send_report(); }
//
//// ball grabbing sequence at to begin the throw
//#define grab_ball()             { find_ball(); \
//                                    /*move_mouse(1, 0, 0);*/ //}
//
//// cursor release sequence for proper follow-through on the throw
//#define follow_through()        { /*SysTick_Delay(5);*/ \
//                                    m.params.buttons = 0; \
//                                    send_report(); \
//                                    mouse_release(); }
//
//
//extern USBD_HandleTypeDef hUsbDeviceFS;
//
//static int mouse_x, mouse_y;
//
//static union {
//    struct {
//        int8_t buttons;
//        int8_t x;
//        int8_t y;
//        int8_t wheel;
//        int8_t wakeup;
//    } params;
//    uint8_t report[5];
//} m = { .params = {.buttons = 0, .x = 0, .y = 0, .wheel = 0, .wakeup = 0 } };

/*
//static void find_ball();
//static void reset();
static void move_mouse(int8_t click, int8_t x, int8_t y);
//static void mouse_release();

void Mouse_behaveAsMouse() {
    m.params.x = (int8_t) Nunchuk_readJoystickX();
    m.params.y = (int8_t) (-1 * Nunchuk_readJoystickY()); // mouse inverts y-axis
    m.params.buttons = (int8_t) (Nunchuk_readCButton() + (Nunchuk_readZButton() << 1));
    send_report();
}

void Mouse_trackCursor() {
    m.params.buttons = 0;
    mouse_x += (m.params.x = (int8_t) Nunchuk_readJoystickX());
    mouse_y += (m.params.y = (int8_t) (-1 * Nunchuk_readJoystickY())); // mouse inverts y-axis
    send_report();
//     if (mouse.track_cursor())
//        QActive_post((QActive *) &nucleoPong, C_BTN);
}

// adjusts the cursor sensitivity in iPad settings to a consistent value
void Mouse_calibrateSensitivity() {
    // tell the user to make sure the bar is moved all the way down to zero before calling this function!
    while (!Nunchuk_readCButton())
        Mouse_trackCursor();
    move_mouse(1, 0, 0);
    m.params.x = 127;
    for (int i = 0; i < 3; i++) {
        send_report();
    }
    mouse_release();
}

static inline void move_mouse(int8_t click, int8_t x, int8_t y) {
    m.params.buttons = click;
    m.params.x = x;
    m.params.y = y;
    send_report();
}

//static void mouse_release() {
//    for (int i = 0; i < sizeof m; i++)
//        m.report[i] = 0;
//
//    send_report();
//    SysTick_Delay(100);
//}

static inline void reset() {
    mouse_x = mouse_y = 0;

    // travel to a known location (bottom left corner of screen)
    m.params.buttons = 0;
    m.params.x = -127;
    m.params.y = 127;
    for (int i = 0; i < 10; i++) {
        send_report();
    }

    SysTick_Delay(2 * POLLING_RATE);
}

static inline void find_ball() {
    reset();

    // go to the ball's starting point (542, -51): 127 * 4 + 36 * 1, -12 * 4 - 4 * 1
    m.params.x = 127;
    m.params.y = -12;
    for (int i = 0; i < 4; i++)
        send_report();
    m.params.x = 34;
    m.params.y = -3;
    send_report();

    SysTick_Delay(100);
}

void gotoball() {
    find_ball();
}

void launch_128() {
    grab_ball();

    m.params.y = -128;
    send_report();
    send_report();

    // experimentation
//    SysTick_Delay(REPORT_DELAY);
//     CUP0_3:
//    m.params.y = -53; // 2 * (455 - 441) * 441/256
//    send_report();

    // last 2 cups
    // y: 2 * (485 - 441) * 441/256
    // x: 2 * (CUP2_X_3 - CUP0_X_3) * 441/256
//    m.params.y = 2 * (485 - 441) * 441/256.0f;
//    m.params.x = 2 * (CUP2_X_3 - CUP0_X_3) * 441/256.0f;
//    send_report();
//    SysTick_Delay(REPORT_DELAY);

    // end experimentation
    follow_through();
}

// cup0_1 & cup0_2
// sometimes undershoots stage 2
void cup0_1__cup_0_2() {
    grab_ball();

    // move 256 distance, but 128 is the max per packet so we need to do it twice
    m.params.y = -128;
    send_report();
    send_report();

    follow_through();
}

// untested
void cup0_3() {
    grab_ball();

    m.params.y = -128;
    send_report();
    send_report();

    // experimentation
//     CUP0_3:
    m.params.y = -53; // 2 * (455 - 441) * 441/256
    send_report();

    follow_through();
}

// not yet working
void cup1_1() {
    grab_ball();

    // move -256 y distance, but 128 is the max per packet so we need to do it twice
    // move -20 x distance?
    // total we need y = 256 + (CUP1_Y_1 - 441) * 441/256
//    m.params.y = (256L + (CUP1_Y_1 - 441) * 441/256.0) / 3; // overflows
    m.params.x = -18/3;
    send_report();
    send_report();
    send_report();

    follow_through();
}


/******* reverse-engineering functions *******/
/*

// throw ball with specified y power
#define MIN_PACKET_DISTANCE         (-128)
#define MAX_PACKET_DISTANCE         (127)
void throw_y(int y_pwr) {
    grab_ball();

    int full_throws = y_pwr / MIN_PACKET_DISTANCE;
    int8_t remainder = y_pwr % MIN_PACKET_DISTANCE;

    m.params.y = MIN_PACKET_DISTANCE;
    m.params.x = 0;
    for (int throws = 0; throws < full_throws; throws++) {
        send_report();
    }
    if (remainder != 0) {
        m.params.y = remainder;
        send_report();
    }

    follow_through();
    SysTick_Delay(100);
}

// rewrite to use less discrete movements

void throw_xy(int x_pwr, int y_pwr) {

    // There's no need for x to be larger than +/- 127.
    // Likewise, y should ALWAYS be less than -128 (-256 is the distance to the closest cup)
    // This simplifies the logic here a LOT
    int full_throws = y_pwr / MIN_PACKET_DISTANCE;
    int8_t remainder_y = (int8_t) (y_pwr % MIN_PACKET_DISTANCE);
    int8_t x_throw = (int8_t) (x_pwr / full_throws);
    int8_t remainder_x = (int8_t) (x_pwr % full_throws);

    LOG("x_pwr: %d, y_pwr: %d, full_throws: %d, remainder_y: %d, x_throw: %d, remainder_x: %d\r\n",
           x_pwr, y_pwr, full_throws, remainder_y, x_throw, remainder_x);

    // timing here needs to be EXACT, so we need to ensure that zero overhead from looping is incurred
    // additionally, it seems to help if we do a warmup first by moving the cursor without actually clicking the ball
//    reset();
//    move_mouse(1, 0, 0);
//    m.params.y = MIN_PACKET_DISTANCE;
//    m.params.x = x_throw;
//    for (int throws = 0; throws < full_throws; throws++)
//        send_report();
//
//    if (remainder_y != 0 || remainder_x != 0) {
//        m.params.x = remainder_x;
//        m.params.y = remainder_y;
//        send_report();
//    }
//    follow_through();
//    SysTick_Delay(100);

    // now we run for real
//    grab_ball();
    m.params.buttons = 1;
    USBD_HID_SendReport(&hUsbDeviceFS, m.report, sizeof m);
    m.params.y = MIN_PACKET_DISTANCE;
    m.params.x = x_throw;
#pragma GCC unroll 10
    for (int throws = 0; throws < full_throws; throws++)
        send_report();

    if (remainder_y != 0 || remainder_x != 0) {
        m.params.x = remainder_x;
        m.params.y = remainder_y;
        send_report();
    }

    follow_through();
    SysTick_Delay(100);
}

void test(int y) {
    grab_ball();
    move_mouse(0, 0, (int8_t) y);
    follow_through();
}

#define FULL_THROWS     4U
void smooth_throw_xy(int x_pwr, int y_pwr) {
    // attempt to eliminate randomness by making the mouse movements smoother and more discrete

    // There's no need for x to be larger than +/- 127.
    // Likewise, y should ALWAYS be less than -128 (-256 is the distance to the closest cup)
    // This simplifies the logic here a LOT
    //y_pwr / MIN_PACKET_DISTANCE;
    int8_t y_1_3 = (int8_t) (y_pwr / FULL_THROWS);
    int8_t y_4 = (int8_t) (y_pwr % FULL_THROWS);
//    int8_t remainder_y = (int8_t) (y_pwr % MIN_PACKET_DISTANCE);
    int8_t x_1_3 = (int8_t) (x_pwr / FULL_THROWS);
    int8_t x_4 = (int8_t) (x_pwr % FULL_THROWS);
//    int8_t x_throw = (int8_t) (x_pwr / full_throws);
//    int8_t remainder_x = (int8_t) (x_pwr % full_throws);

    printf("full_throws: %d, y_pwr: %d, y_1_3: %d, y_4: %d, x_pwr: %d, x_1_3: %d, x_4: %d\r\n",
           FULL_THROWS, y_pwr, y_1_3, y_4, x_pwr, x_1_3, x_4);
    fflush(NULL);

    find_ball();
//    move_mouse(1, 0, 0);
    m.params.y = y_1_3;
    m.params.x = x_1_3;
#pragma GCC unroll 10
    for (int throws = 0; throws < FULL_THROWS; throws++)
        send_report();

    m.params.y = y_4;
    m.params.x = x_4;
    send_report();
    follow_through();

    SysTick_Delay(100);

    grab_ball();
    m.params.x = m.params.y = 0;
    m.params.buttons = 1;
    send_report();
    m.params.y = y_1_3;
    m.params.x = x_1_3;
#pragma GCC unroll 10
    for (int throws = 0; throws < FULL_THROWS; throws++)
        send_report();

    m.params.y = y_4;
    m.params.x = x_4;
    send_report();

    follow_through();

    SysTick_Delay(100);
}

// get where each ball landed from nunchuk
void get_results(int *sample_buf, int samples) {
    for (int i = 0; i < samples; i++) {
        printf("Select landing spot for throw #%d\r\n", i);

        find_ball(); // reset mouse to ball's position

        while (!Nunchuk_readCButton())
            Mouse_trackCursor();

        char response;
        printf("Confirm y=%d? (y/N) ", mouse_y);
        fflush(NULL);
        scanf("%c", &response);
        if (response == 'N' || response == 'n')
            --i;
        else
            sample_buf[i] = mouse_y;
    }
}

// take num samples by prompting for a y power & throwing, then save the results to an array
// and display them
void sample_y(int samples) {
    printf("Sampling y throw %d times\r\n", samples);
    int sample_buf[samples];
    int y_buf[samples];
    int mouse_y_dist;

    for (int i = 0; i < samples; i++) {
        printf("Enter y travel distance (negative means up): \r\n");
        scanf("%d", &mouse_y_dist);
        y_buf[i] = mouse_y_dist;
        throw_y(mouse_y_dist);
        fflush(NULL);
    }

    get_results(sample_buf, samples);

    // output the samples in CSV format
    printf("-----RESULTS-----\r\n");
    printf("power list:\r\n");
    for (int i = 0; i < samples; i++) {
        printf("%d, ", y_buf[i]);
        fflush(NULL);
    }
    printf("\r\nresult list:\r\n");
    for (int i = 0; i < samples; i++) {
        printf("%d, ", sample_buf[i]);
        fflush(NULL);
    }
}
*/
//#ifdef __cplusplus
//extern "C" {
//#endif
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