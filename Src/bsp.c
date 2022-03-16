/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "bsp.h"
#include "qpn_port.h"
#include "statemachine.h"




void QF_onStartup(void) { /* entered with interrupts locked */

}

void QF_onIdle(void) { /* entered with interrupts locked */
    QF_INT_UNLOCK(); /* unlock interrupts */

    QActive_postISR((QActive *) &nucleoPong, IDLE); // is used to post an event to your FSM

}

void Q_onAssert(char const Q_ROM *const Q_ROM_VAR file, int line) {
    (void) file; /* avoid compiler warning */
    (void) line; /* avoid compiler warning */
    QF_INT_LOCK();
    for (;;) {
    }
}




// GPIO button handler
void EXTI15_10_IRQHandler(void) {
    SysTick_Delay(20); // debouncing
    // make sure line 13 was triggered
    if (EXTI->PR1 & EXTI_PR1_PIF13) {

        // dispatch signal to state machine after exit from ISR
        QActive_postISR((QActive *) &nucleoPong, BTN_CLICK);
        // Clear interrupt pending bit
        EXTI->PR1 |= EXTI_PR1_PIF13;

//        for (volatile int i = 0; i < 0x10000; i++) ;
    } else {
        // otherwise, just clear & ignore the interrupt
        EXTI->PR1 |= (EXTI_PR1_PIF15 | EXTI_PR1_PIF14 | EXTI_PR1_PIF12 | EXTI_PR1_PIF11 | EXTI_PR1_PIF10);
    }
}



//void show_menu(struct config *current_cfg) { return; }
void menu_set_config(struct config *cfg) { return; }
//void hide_menu() { return; }

void show_cup_select_instructions() { return; }
void hide_cup_select_instructions() { return; }

void reset_cursor() { return; }
void track_cursor() { return; } // dispatches C_BTN signal once the button is pressed

void show_game_animation() { return; }
void hide_game_animation() { return; }
void show_finished_screen() { COLOR_WHITE; LCD_print("finished screen!", 20, 100); }
void hide_finished_screen() { LCD_clrScr(); }


#define TURN_PROMPT             "press <c> when it is your\nturn"
void prompt_turn() {
    LCD_setColorBg(0, 0, 0);
    COLOR_WHITE;
    LCD_printLong(TURN_PROMPT, 20, 20);
}
void hide_prompt_turn() {
    COLOR_BLACK;
    LCD_fillRect(20, 20, 20 + SmallFont[0] * sizeof TURN_PROMPT, 20 + 2*SmallFont[1]);
}

//void PongBot_throwBall() { return; }

//void behave_as_mouse() { return; }