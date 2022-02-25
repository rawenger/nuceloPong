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

    QActive_postISR((QActive *) &nucleoPong, NO_INT); // is used to post an event to your FSM

}

void Q_onAssert(char const Q_ROM *const Q_ROM_VAR file, int line) {
    (void) file; /* avoid compiler warning */
    (void) line; /* avoid compiler warning */
    QF_INT_LOCK();
    for (;;) {
    }
}