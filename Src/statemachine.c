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
    static QState PongBot_finished(PongBot_HSM *me);
    static QState PongBot_menu(PongBot_HSM *me);
    static QState PongBot_mouse(PongBot_HSM *me);

/**********************************************************************/
// remember previous mode of tuner when bringing up mode selection screen
static QState (*prev_mode)(PongBot_HSM *);

static int active_game = 0; // whether to show start option in the menu (i.e: if this is the initial run)
static int turn = 1;        // whether it is the bot's turn or not

static struct config cfg = {
        .random_mode = 0, // whether the cups are placed randomly or not
        .mouse_tracking_speed = 0, // act as a regular mouse
};

/**********************************************************************/
PongBot_HSM nucleoPong;

void PongBot_ctor(void) {
    PongBot_HSM *me = &nucleoPong;
    QActive_ctor(&me->super, (QStateHandler) &PongBot_initial);
}


static QState PongBot_initial(PongBot_HSM *me) {
    LOG_SIGNAL();
    return Q_TRAN(&PongBot_on);
}

static QState PongBot_on(PongBot_HSM *me) {
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

        case IDLE: {
            return Q_HANDLED();
        }
    }

    return Q_SUPER(&QHsm_top);
}


static QState PongBot_play(PongBot_HSM *me) {

    LOG_SIGNAL();

    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {

            if (!active_game)
                PongBot_reset();
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

            if (active_game && turn) {

                turn = PongBot_throwBall();

                // blocks, queueing calls to show main menu
                if (turn && !prompt_for_success())
                    PongBot_didMiss();
                else if (PongBot_gameOver())
                    return Q_TRAN(&PongBot_finished);

                if (!turn) {
                    // doesn't trigger when resuming from main menu
                    prompt_turn();
                }

            } else if (!turn && Nunchuk_readCButton()) {
                hide_prompt_turn();
                turn = 1;
            }

            return Q_HANDLED();
        }
    }
    return Q_SUPER(&PongBot_on);
}

static QState PongBot_selectCup(PongBot_HSM *me) {

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
            PongBot_throwBall();
            return Q_HANDLED();
        }
    }

    return Q_SUPER(&PongBot_play);
}

static QState PongBot_finished(PongBot_HSM *me) {

    LOG_SIGNAL();

    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            active_game = 0;
            show_finished_screen();
            return Q_HANDLED();
        }

        case Q_EXIT_SIG: {
            hide_finished_screen();
            return Q_HANDLED();
        }
    }

    return Q_SUPER(&PongBot_on);
}

static QState PongBot_menu(PongBot_HSM *me) {

    LOG_SIGNAL();

    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            show_menu(active_game);
            return Q_HANDLED();
        }

        case Q_EXIT_SIG: {
            hide_menu();
            hide_options();
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
        }

        case OPTIONS: {
//            show_options(&cfg);
            return Q_HANDLED();
        }

        case RESTART: {
            PongBot_reset();
            return Q_TRAN(&PongBot_play);

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
            // TODO: set config (tracking speed, etc)
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


