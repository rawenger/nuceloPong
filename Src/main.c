/* nucleoPong -- A USB-based bot for the iOS GamePigeon Cup Pong game
 */

/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "i2c.h"
#include "sysclock.h"
#include "usbd_hid.h"
#include "usb_device.h"

#include "nunchuk.h"
#include <stdio.h>

extern USBD_HandleTypeDef hUsbDeviceFS;

typedef union {
    struct {
        int8_t buttons;
        int8_t x;
        int8_t y;
        int8_t wheel;
        int8_t wakeup;
    } params;
    uint8_t buf[5];
} mouse;

static void peripheral_init() {
    SystemClock_Init();
    HAL_Init(); // basically just initializes SysTick clock

    GPIO_Init();
    USART_Init();
    I2C_Init();
    MX_USB_DEVICE_Init();
}

static void device_init() {
    Init_nunchuk(NUNCHUK_I2C);
}

int main(void) {
    peripheral_init();
    printf("\r\nInitialized peripherals\r\n");
    // wait for button press before proceeding
    while (GPIO_ReadPin(BTN_GPIO, BTN_GPIO_PIN)) ;
    printf("Initialize devices\r\n");
    device_init();
    GPIO_TogglePin(LED_GPIO, LED_GPIO_PIN);

    mouse m = { .params = {.buttons = 0, .x = 0, .y = 0, .wheel = 0, .wakeup = 0 } };

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1) {
        Nunchuk_updateValues();
//        fflush(NULL);
        m.params.x = (int8_t) Nunchuk_readJoystickX();
        m.params.y = (int8_t) (-1 * Nunchuk_readJoystickY()); // mouse inverts y-axis
        m.params.buttons = (int8_t) (Nunchuk_readCButton() + (Nunchuk_readZButton() << 1));
//        printf("joystick: <%d, %d>\r\n", m.params.x, -1 * m.params.y);
        USBD_HID_SendReport(&hUsbDeviceFS, m.buf, 5);

//        for(volatile int a = 0; a < 0x1000; a++); // later add a delay function using SysTick;
//        HAL_Delay(1000);
    }
#pragma clang diagnostic pop

    return 0;
}

void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

