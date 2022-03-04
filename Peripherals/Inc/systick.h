/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */


#ifndef NUCLEOPONG_SYSTICK_H
#define NUCLEOPONG_SYSTICK_H

#include "stm32l4xx.h"
// We need to override default HAL_GetTick and HAL_Delay implementations,
// since the USB module heavily depends on them. Luckily they are weakly linked, so this is
// not a problem.
#include "stm32l4xx_hal.h"



#ifdef __cplusplus
extern "C" {
#endif

#define TICK_FREQ                       1000U // 1 MHz
#define SYSCLOCK_FREQ                   80000000U // 80 MHz


/// non-API implementation of HAL_Init(); see @ref stm32l4xx_hal.c:152
void SysTick_Init();

/**
 * @brief A blocking delay which lasts for the specified time
 * @param msDelay time to delay in ms
 */
void SysTick_Delay(uint32_t msDelay);

/**
 * @brief A blocking delay which lasts for the specified time
 * @param usDelay time to delay in μs
 */
void SysTick_uDelay(uint32_t usDelay);

void SysTick_Handler() __attribute__ ((interrupt ("IRQ")));

uint32_t SysTick_GetClk();

#ifdef __cplusplus
}
#endif

#endif //NUCLEOPONG_SYSTICK_H
