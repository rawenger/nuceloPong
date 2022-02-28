/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#ifndef NUCLEOPONG_STATEMACHINE_H
#define NUCLEOPONG_STATEMACHINE_H

#ifdef __cplusplus
extern "C" {
#endif

enum PongBotSignals {
    BTN_CLICK = Q_USER_SIG,	// left twist
    C_BTN,
    IDLE,
    FINISHED,
    START,
    MOUSE
};


extern struct PongBotTag nucleoPong;

void PongBot_ctor(void);


#ifdef __cplusplus
}
#endif


#endif //NUCLEOPONG_STATEMACHINE_H
