/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

/*****************************************************************************
* STM32L476RG Qnano port qpn_port.h
*****************************************************************************/
#ifndef qpn_port_h
#define qpn_port_h

#ifdef __cplusplus
extern "C" {
#endif

#define Q_PARAM_SIZE            0
#define QF_TIMEEVT_CTR_SIZE     1

/* maximum # active objects--must match EXACTLY the QF_active[] definition  */
#define QF_MAX_ACTIVE           1

/* interrupt locking policy for XPS compiler. we only care about the GPIO button interrupts here */
#define QF_INT_LOCK()           __disable_irq();// NVIC_DisableIRQ(EXTI15_10_IRQn);
#define QF_INT_UNLOCK()         __enable_irq();// NVIC_EnableIRQ(EXTI15_10_IRQn);
#define Q_ROM_PTR(rom_var_)     rom_var_
#define Q_ROM_BYTE(rom_var_)    rom_var_

#define QF_ISR_NEST
#define Q_ROM
#define Q_ROM_VAR
#define Q_REENTRANT
#define Q_ISR_NEST

#ifdef __cplusplus
}
#endif

#include <stdint.h>       /* IAR provides C99-standard exact-width integers */
#include "Src/qepn.h"         /* QEP-nano platform-independent public interface */
#include "Src/qfn.h"           /* QF-nano platform-independent public interface */



#include "bsp.h"


#endif                                                        /* qpn_port_h */