/* main.h */

#ifndef __MAIN_H
#define __MAIN_H

#include "stm32l4xx_hal.h"

#define SYSCLOCK_FREQ                   80000000U // 80 MHz

#ifdef __cplusplus
extern "C" {
#endif

_Noreturn void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */