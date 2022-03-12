#include <sys/cdefs.h>
/* nucleoPong -- A USB-based bot for the iOS GamePigeon Cup Pong game
 */

/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "main.h"
#include "bsp.h"

#include "qpn_port.h"
#include "statemachine.h"

static QEvent l_nucleoPongQueue[30];

QActiveCB const Q_ROM Q_ROM_VAR QF_active[] = {
        { (QActive *)0,            (QEvent *)0,          0                    },
        { (QActive *)&nucleoPong, l_nucleoPongQueue, Q_DIM(l_nucleoPongQueue)  }
};

Q_ASSERT_COMPILE(QF_MAX_ACTIVE == Q_DIM(QF_active) - 1);


extern USBD_HandleTypeDef hUsbDeviceFS;

static void peripheral_init() {
    SystemClock_Init();
    // some delay to get the system up & running before SysTick gets initialized
    for (volatile int a = 0; a < 0x5000; a++) ;
    SysTick_Init(); // start SysTick clock & interrupts

    GPIO_Init();
    USART_Init();
    I2C_Init();
    MX_USB_DEVICE_Init();
    SPI_Init();
}

static void device_init() {
    Init_nunchuk(NUNCHUK_I2C);

    PongBot_Init();

    // initialize display
    Display_Init();
}



extern void gotoball();
int main(void) {
    PongBot_ctor();
    peripheral_init();
    LOG("\r\nInitialized peripherals\r\n");
    // wait for button press before proceeding
//    while (GPIO_ReadPin(BTN_GPIO, BTN_GPIO_PIN)) ;
    LOG("Initialize devices\r\n");
    device_init();
    GPIO_SetPin(LED_GPIO, LED_GPIO_PIN);

    QF_INT_UNLOCK();
    QF_run();

    return 1;
}

_Noreturn void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

