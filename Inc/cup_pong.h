/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#ifndef NUCLEOPONG_CUP_PONG_H
#define NUCLEOPONG_CUP_PONG_H

#include <stdint.h>

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

void PongBot_throwBall();

int PongBot_didMiss();

int PongBot_getCup();

//void launch_128();

//void cup0_1__cup_0_2();
//void cup1_1();

//void cup0_3();

/******* Reverse-Engineering Functions *******/

/** The main idea behind this process is to simply throw the ball with a given mouse power while screen recording,
 * then go through and select the position each ball landed at from the recording. This data can
 * then be put through a regression to determine a mathematical model for the throws.
 */

// run the sampling routine a specified number of times with x=0
//void sample_y(int samples);

// run the sampling routine a specified number of times
void sample_xy(int samples);

//void test(int y);

#ifdef __cplusplus
}
#endif

#endif //NUCLEOPONG_CUP_PONG_H
