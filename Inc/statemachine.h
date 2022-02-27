/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

/*
 * This file lays out most of the control flow for the application, representing it as a state
 * machine which receives signals from the `main` function.
 */

#ifndef NUCLEOPONG_STATEMACHINE_H
#define NUCLEOPONG_STATEMACHINE_H

#ifdef __cplusplus
extern "C" {
#endif

enum PongBotSignals {
    BTN_CLICK = Q_USER_SIG,	// left twist
//    ENCODER_DOWN, 				// right twist
//    ENCODER_CLICK,
    BTN_CLICK_UP,
    BTN_CLICK_LEFT,
    BTN_CLICK_RIGHT,
    BTN_CLICK_DOWN,
    BTN_CLICK_CENTER,
    NO_INT
};


extern struct PongBotTag nucleoPong;

void PongBot_ctor(void);


#ifdef __cplusplus
}
#endif


#endif //NUCLEOPONG_STATEMACHINE_H
