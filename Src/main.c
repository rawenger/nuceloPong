/* nucleoPong -- A USB-based bot for the iOS GamePigeon Cup Pong game
 */

/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "main.h"
#include <stdio.h>

static void peripheral_init() {
    System_Clock_Init();
    GPIO_Init();
    USART_Init();
    I2C_Init();
}


int main(void) {
    peripheral_init();
    printf("Initialized\r\n");
    GPIO_TogglePin(LED_GPIO, LED_GPIO_PIN);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1) {
        if (!GPIO_ReadPin(BTN_GPIO, BTN_GPIO_PIN)) {
            GPIO_TogglePin(LED_GPIO, LED_GPIO_PIN);
            printf("Toggled LED %s\r\n", GPIO_ReadPin(LED_GPIO, LED_GPIO_PIN) ? "on" : "off");
            for (volatile int a = 0; a < 0x100000; a++);
        }
    }
#pragma clang diagnostic pop

    return 0;
}