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
    USB_Init();
}

static void device_init() {
    Init_nunchuk(NUNCHUK_I2C);
}

int main(void) {
    volatile RCC_TypeDef *rcc = RCC;
    volatile USB_OTG_GlobalTypeDef *usb = USB_OTG_FS;
    peripheral_init();
    printf("\r\nInitialized peripherals\r\n");
    // wait for button press before proceeding
    while (GPIO_ReadPin(BTN_GPIO, BTN_GPIO_PIN)) ;
    device_init();
    printf("Initialize devices\r\n");
    GPIO_TogglePin(LED_GPIO, LED_GPIO_PIN);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1) {
        Nunchuk_updateValues();
        fflush(NULL);
        printf("joystick: <%d, %d>\r\n", Nunchuk_readJoystickX(), Nunchuk_readJoystickY());
    }
#pragma clang diagnostic pop

    return 0;
}