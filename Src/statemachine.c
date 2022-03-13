/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

/*
 * This file lays out most of the control flow for the application, representing it as a state
 * machine which receives signals from ISR's and internal functions.
 */


#define AO_PONG

#include "qpn_port.h"
#include "bsp.h"
#include "statemachine.h"
#include "lcd_driver.h"
#include "cup_pong.h"

typedef struct PongBotTag {               // Cup Pong bot State machine
    QActive super;
} PongBot_HSM;

/* Setup state machines */
/**********************************************************************/
static QState PongBot_initial(PongBot_HSM *me);

// state hierarchy
static QState PongBot_on(PongBot_HSM *me);
    static QState PongBot_play(PongBot_HSM *me);
        static QState PongBot_selectCup(PongBot_HSM *me);
    static QState PongBot_menu(PongBot_HSM *me);
    static QState PongBot_mouse(PongBot_HSM *me);

/**********************************************************************/
// remember previous mode of tuner when bringing up mode selection screen
static QState (*prev_mode)(PongBot_HSM *);

int octave = 0; // most recently selected octave

//static int spectrogram_idx = 0; // time position of spectrogram
//static int freeze_spectro = 1;

static struct config cfg = {
        .random_mode = 0, // whether the cups are placed randomly or not
        .mouse_mode = 0, // act as a regular mouse
};

/**********************************************************************/
PongBot_HSM nucleoPong;

void PongBot_ctor(void) {
    PongBot_HSM *me = &nucleoPong;
    QActive_ctor(&me->super, (QStateHandler) &PongBot_initial);
}


QState PongBot_initial(PongBot_HSM *me) {
    LOG("HSM Initialization\r\n");
    return Q_TRAN(&PongBot_on);
}

QState PongBot_on(PongBot_HSM *me) {
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            LOG("PongBot on\r\n");
            show_welcome_screen();
            return Q_HANDLED();
        }

        case Q_EXIT_SIG: {
            LOG("PongBot_on: EXIT_SIG\r\n");
            return Q_HANDLED();
        }

        case BTN_CLICK: {
            LOG("PongBot_on: BTN_CLICK\r\n");
            hide_welcome_screen();
            return Q_TRAN(&PongBot_menu);
        }

        case IDLE:
            return Q_HANDLED();

    }

    return Q_SUPER(&QHsm_top);
}


QState PongBot_play(PongBot_HSM *me) {
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            LOG("PongBot_play: ENTRY_SIG\r\n");
            if (cfg.random_mode) {
                reset_cursor();
                return Q_TRAN(&PongBot_selectCup);
            }
            show_game_animation();
            return Q_HANDLED();
        }

        case Q_EXIT_SIG: {
            LOG("PongBot_play: EXIT_SIG\r\n");
            hide_game_animation();
            hide_finished_screen();
            return Q_HANDLED();
        }

        case IDLE: {
//            LOG("PongBot_play: IDLE\r\n");
            PongBot_throwBall();
        }

        case FINISHED: {
            LOG("PongBot_play: FINISHED\r\n");
            show_finished_screen();
        }

    }
    return Q_SUPER(&PongBot_on);
}

QState PongBot_selectCup(PongBot_HSM *me) {
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            LOG("PongBot_selectCup: ENTRY_SIG\r\n");
            show_cup_select_instructions();
            return Q_HANDLED();
        }

        case Q_EXIT_SIG: {
            LOG("PongBot_selectCup: EXIT_SIG\r\n");
            hide_cup_select_instructions();
            return Q_HANDLED();
        }

        case IDLE: {
            track_cursor();
            return Q_HANDLED();
        }

        case C_BTN: {
            LOG("PongBot_selectCup: C_BTN\r\n");
            PongBot_throwBall();
            return Q_HANDLED();
        }
    }

    return Q_SUPER(&PongBot_play);
}

QState PongBot_menu(PongBot_HSM *me) {

    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            LOG("PongBot_menu: ENTRY_SIG\r\n");
            show_menu();
            return Q_HANDLED();
        }

        case Q_EXIT_SIG: {
            LOG("PongBot_menu: EXIT_SIG\r\n");
            hide_menu();
            return Q_HANDLED();
        }

        case BTN_CLICK: {
            LOG("PongBot_menu: BTN_CLICK\r\n");
            menu_select();
            return Q_HANDLED();
        }

        case START: {
            LOG("PongBot_menu: START\r\n");
            if (cfg.random_mode)
                return Q_TRAN(&PongBot_selectCup);
            return Q_TRAN(&PongBot_play);
        }

        case IDLE: {
            menu_idle();
            return Q_HANDLED();
        }

        // maybe also add cases here for quit/resume? this might not be necessary idk yet

        case MOUSE: {
            LOG("PongBot_menu: MOUSE\r\n");
            return Q_TRAN(&PongBot_mouse);
        }
    }

    return Q_SUPER(&PongBot_on);
}

static QState PongBot_mouse(PongBot_HSM *me) {
    switch (Q_SIG(me)) {
        behave_as_mouse();
    }

    return Q_SUPER(&PongBot_on);
}


