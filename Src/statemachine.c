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

static int active_game = 0; // whether to show start option in the menu (i.e: if this is the initial run)

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
    LOG_SIGNAL();
    return Q_TRAN(&PongBot_on);
}

QState PongBot_on(PongBot_HSM *me) {
    LOG_SIGNAL();

    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            show_welcome_screen();
            return Q_HANDLED();
        }

        case Q_EXIT_SIG: {
            return Q_HANDLED();
        }

        case BTN_CLICK: {
            hide_welcome_screen();
            return Q_TRAN(&PongBot_menu);
        }

        case IDLE:
            return Q_HANDLED();

    }

    return Q_SUPER(&QHsm_top);
}


QState PongBot_play(PongBot_HSM *me) {

    LOG_SIGNAL();

    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            active_game = 1;
            if (cfg.random_mode) {
                reset_cursor();
                return Q_TRAN(&PongBot_selectCup);
            }
            show_game_animation();
            return Q_HANDLED();
        }

        case Q_EXIT_SIG: {
            hide_game_animation();
            hide_finished_screen();
            return Q_HANDLED();
        }

        case IDLE: {
            PongBot_throwBall();
        }

        case FINISHED: {
            active_game = 0;
            show_finished_screen();
        }

    }
    return Q_SUPER(&PongBot_on);
}

QState PongBot_selectCup(PongBot_HSM *me) {

    LOG_SIGNAL();

    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            show_cup_select_instructions();
            return Q_HANDLED();
        }

        case Q_EXIT_SIG: {
            hide_cup_select_instructions();
            return Q_HANDLED();
        }

        case IDLE: {
            track_cursor();
            return Q_HANDLED();
        }

        case C_BTN: {
            //TODO: refactor to something like "perform next action" or something so the class tracks timing internally
            PongBot_throwBall();
            return Q_HANDLED();
        }
    }

    return Q_SUPER(&PongBot_play);
}

QState PongBot_menu(PongBot_HSM *me) {

    LOG_SIGNAL();

    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            show_menu(active_game);
            return Q_HANDLED();
        }

        case Q_EXIT_SIG: {
            hide_menu();
            return Q_HANDLED();
        }

        case BTN_CLICK:
        case C_BTN: {
            menu_select();
            return Q_HANDLED();
        }

        case IDLE: {
            menu_idle();
            return Q_HANDLED();
        }

        case RESUME: {
            if (cfg.random_mode)
                return Q_TRAN(&PongBot_selectCup);
            return Q_TRAN(&PongBot_play);
            // etc
        }

        case OPTIONS: {
            show_options();
        }

        case RESTART: {

        }

        case MOUSE: {
            return Q_TRAN(&PongBot_mouse);
        }
    }

    return Q_SUPER(&PongBot_on);
}

static QState PongBot_mouse(PongBot_HSM *me) {

    LOG_SIGNAL();

    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            LOG("mouse_entry\r\n");
            show_mouse_instructions();
            return Q_HANDLED();
        }

        case Q_EXIT_SIG: {
            LOG("mouse_exit\r\n");
            hide_mouse_instructions();
            return Q_HANDLED();
        }

        case IDLE: {
            Mouse_behaveAsMouse();
            return Q_HANDLED();
        }
    }

    return Q_SUPER(&PongBot_on);
}


