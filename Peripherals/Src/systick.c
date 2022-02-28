/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "systick.h"

static volatile uint32_t msTick;

void SysTick_Init() {
    NVIC_SetPriorityGrouping(0x00000003U); // 4 bits for pre-emption priority, 0 bits for subpriority

   uint32_t ticks = SYSCLOCK_FREQ / (1000 / (uint32_t)TICK_FREQ);

    // set reload register
    SysTick->LOAD = (uint32_t)(ticks - 1UL);

    // set Priority for Systick Interrupt
    NVIC_SetPriority(SysTick_IRQn, 1);//(1UL << __NVIC_PRIO_BITS) - 1UL);

    // Load the SysTick Counter Value
    SysTick->VAL = 0UL;

    msTick = 0U;

    // Enable SysTick IRQ and SysTick Timer using system clock source
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
    NVIC_EnableIRQ(SysTick_IRQn);
}

void SysTick_Handler() {
    ++msTick;
}

void HAL_Delay(uint32_t Delay) {
    uint32_t initial = msTick;

    // handle case where clock is about to change
    if (Delay < 0xFFFFFFFFU)
        Delay += TICK_FREQ;

    while ((msTick - initial) < Delay) ;
}

void SysTick_Delay(uint32_t msDelay) {
    uint32_t initial = msTick;

    if (msDelay < 0xFFFFFFFFU)
        msDelay += TICK_FREQ;

    while ((msTick - initial) < msDelay) ;
}

uint32_t HAL_GetTick() {
    return msTick;
}

uint32_t SysTick_GetClk() {
    return msTick;
}