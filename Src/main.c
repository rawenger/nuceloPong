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

static QEvent l_ChromaticTunerQueue[30];

QActiveCB const Q_ROM Q_ROM_VAR QF_active[] = {
        { (QActive *)0,            (QEvent *)0,          0                    },
        { (QActive *)&nucleoPong, l_ChromaticTunerQueue, Q_DIM(l_ChromaticTunerQueue)  }
};

Q_ASSERT_COMPILE(QF_MAX_ACTIVE == Q_DIM(QF_active) - 1);




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

    // initialize display
    // eventually put this in its own function lol
    LCD_Init();

    LCD_clrScr();
    LCD_setColor(255, 0, 255);
    LCD_fillRect(20, 20, 220, 300);

    LCD_setColorBg(0, 0, 0);
    LCD_setColor(0, 255, 0);
    LCD_setFont(BigFont);
    LCD_print("Starting...", 40, 6);
    LCD_setXY(20, 200, 40, 220);
    LCD_fastFill();

    SysTick_Delay(1000);

//    LCD_setColor(0, 0, 255);
//    LCD_fillRect(10, 10, 230, 310);
//
//    SysTick_Delay(1000);

    LCD_clrScr();
}

int main(void) {
    PongBot_ctor();
    peripheral_init();
    // disable btn interrupts before state machine is initialized
//    QF_INT_LOCK();
    LOG("\r\nInitialized peripherals\r\n");
    // wait for button press before proceeding
//    while (GPIO_ReadPin(BTN_GPIO, BTN_GPIO_PIN)) ;
    LOG("Initialize devices\r\n");
    device_init();
    GPIO_SetPin(LED_GPIO, LED_GPIO_PIN);

//    QF_INT_UNLOCK();
    QF_run();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1) {


//        for(volatile int a = 0; a < 0x1000; a++); // later add a delay function using SysTick;
    }
#pragma clang diagnostic pop

    return 0;
}

_Noreturn void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

