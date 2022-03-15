/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#ifndef NUCLEOPONG_CUP_PONG_H
#define NUCLEOPONG_CUP_PONG_H

#include <stdint.h>

#define THROW_DELAY         3000U // time between ball throws

#ifdef __cplusplus
extern "C" {
#endif

// adjusts the cursor sensitivity in iPad settings to a consistent value
void Mouse_calibrateSensitivity();

// move the mouse cursor and track its position
void Mouse_trackCursor();

void Mouse_behaveAsMouse();

void Mouse_move(int8_t click, int8_t dx, int8_t dy);

void Mouse_getTrackedCoords(int *x, int *y);

void PongBot_Init();

int PongBot_throwBall();

int PongBot_didMiss();

int PongBot_getCup();

#ifdef __cplusplus
}
#endif

#endif //NUCLEOPONG_CUP_PONG_H
