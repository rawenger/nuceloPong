/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "systick.h"

static volatile uint32_t usTick;

void SysTick_Init() {
    NVIC_SetPriorityGrouping(0x00000003U); // 4 bits for pre-emption priority, 0 bits for subpriority

    uint32_t ticks = SYSCLOCK_FREQ / ((uint32_t) TICK_FREQ * 1000);

    // set reload register
    SysTick->LOAD = (uint32_t)(ticks - 1UL);

    // set Priority for Systick Interrupt
    NVIC_SetPriority(SysTick_IRQn, 1);//(1UL << __NVIC_PRIO_BITS) - 1UL);

    // Load the SysTick Counter Value
    SysTick->VAL = 0UL;

    usTick = 0U;

    // Enable SysTick IRQ and SysTick Timer using system clock source
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
    NVIC_EnableIRQ(SysTick_IRQn);
}

// overflows after ~17 minutes — should be fine
void SysTick_Handler() {
    ++usTick;
}

void HAL_Delay(uint32_t Delay) {
    uint32_t initial = usTick;

    // handle case where clock is about to change
    if (Delay < 0xFFFFFFFFU)
        Delay += 1;

    while ((usTick - initial) < 1000U * Delay) ;
}

void SysTick_Delay(uint32_t msDelay) {
    uint32_t initial = usTick;

    if (msDelay < 0xFFFFFFFFU / 1000U)
        msDelay += 1;

    while ((usTick - initial) < 1000U * msDelay) ;
}

void SysTick_uDelay(uint32_t usDelay) {
    uint32_t initial = usTick;

    if (usDelay < 0xFFFFFFFFU)
        usDelay += 1;

    while ((usTick - initial) < usDelay) ;
}


uint32_t HAL_GetTick() {
    return usTick / 1000U;
}

uint32_t SysTick_GetClk() {
    return usTick / 1000U;
}