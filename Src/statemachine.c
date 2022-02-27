/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#define AO_PONG

#include "qpn_port.h"
#include "bsp.h"
#include "statemachine.h"
#include "lcd_driver.h"
#include "cup_pong_bot.h"

typedef struct PongBotTag {               // Cup Pong bot State machine
    QActive super;
} PongBot_HSM;

/* Setup state machines */
/**********************************************************************/
static QState PongBot_initial(PongBot_HSM *me);

static QState PongBot_on(PongBot_HSM *me);

static QState PongBot_play(PongBot_HSM *me);

static QState PongBot_selectCup(PongBot_HSM *me);

static QState PongBot_pauseMenu(PongBot_HSM *me);

static QState PongBot_settings(PongBot_HSM *me);

static QState PongBot_mouse(PongBot_HSM *me);

/**********************************************************************/
// remember previous mode of tuner when bringing up mode selection screen
static QState (*prev_mode)(PongBot_HSM *);

int octave = 0; // most recently selected octave

static int spectrogram_idx = 0; // time position of spectrogram
static int freeze_spectro = 1;


/**********************************************************************/
PongBot_HSM nucleoPong;

void PongBot_ctor(void) {
    PongBot_HSM *me = &nucleoPong;
    QActive_ctor(&me->super, (QStateHandler) &PongBot_initial);
}


QState PongBot_initial(PongBot_HSM *me) {
    printf("\n\rHSM Initialization\n\r");
    return Q_TRAN(&PongBot_selectCup);
}

QState PongBot_on(PongBot_HSM *me) {
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG:
            printf("PongBot on\r\n");
            show_welcome_screen();
            return Q_HANDLED();

        case Q_EXIT_SIG:
            return Q_HANDLED();



    }

    return Q_SUPER(&QHsm_top);
}

QState PongBot_pauseMenu(PongBot_HSM *me) {

    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
//            show_octave_menu(octave);
            return Q_HANDLED();
        }

        case Q_EXIT_SIG: {
//            erase_octave_menu();
            return Q_HANDLED();
        }
    }

    return Q_SUPER(&PongBot_on);
}

QState PongBot_selectCup(PongBot_HSM *me) {
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            printf("cup select screen\n\r");

//            LCD_print("LCD!!", 10, 20);

//            draw_bounding_box();
            prev_mode = &PongBot_selectCup;
            return Q_HANDLED();
        }

        case Q_EXIT_SIG: {
//            erase_tuner();
            return Q_HANDLED();
        }

        case NO_INT: {
            Mouse_behaveAsMouse();
            return Q_HANDLED();
        }
    }

    return Q_SUPER(&PongBot_on);
}

QState PongBot_settings(PongBot_HSM *me) {
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
//            show_function_menu();
            // get input via either I2C or USART
            return Q_HANDLED();
        }

        case Q_EXIT_SIG: {
            return Q_HANDLED();
        }
    }

    return Q_SUPER(&PongBot_on);
}

QState PongBot_play(PongBot_HSM *me) {
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
//            spectrogram_idx = 0;
//            freeze_spectro = 1;
//            stream_grabber_start();
//            spectrogram_wait_screen();
//            prev_mode = &PongBot_spectrogram;
            return Q_HANDLED();
        }

        case Q_EXIT_SIG: {
//            erase_spectrogram();
            return Q_HANDLED();
        }

    }
    return Q_SUPER(&PongBot_on);
}

static QState PongBot_mouse(PongBot_HSM *me) {
    switch (Q_SIG(me)) {

    }

    return Q_SUPER(&PongBot_on);
}


