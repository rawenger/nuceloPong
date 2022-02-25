#include <sys/cdefs.h>
/* nucleoPong -- A USB-based bot for the iOS GamePigeon Cup Pong game
 */

/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "main.h"
#include "bsp.h"

#include "nunchuk.h"

#include "qpn_port.h"
#include "statemachine.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

static QEvent l_ChromaticTunerQueue[30];

QActiveCB const Q_ROM Q_ROM_VAR QF_active[] = {
        { (QActive *)0,            (QEvent *)0,          0                    },
        { (QActive *)&nucleoPong, l_ChromaticTunerQueue, Q_DIM(l_ChromaticTunerQueue)  }
};

Q_ASSERT_COMPILE(QF_MAX_ACTIVE == Q_DIM(QF_active) - 1);


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
    SysTick_Init(); // basically just initializes SysTick clock

    GPIO_Init();
    USART_Init();
    I2C_Init();
    MX_USB_DEVICE_Init();
    SPI_Init();
}

static void device_init() {
    Init_nunchuk(NUNCHUK_I2C);
}


static mouse m = { .params = {.buttons = 0, .x = 0, .y = 0, .wheel = 0, .wakeup = 0 } };

void move_mouse() {
    Nunchuk_updateValues();
//        fflush(NULL);
    m.params.x = (int8_t) Nunchuk_readJoystickX();
    m.params.y = (int8_t) (-1 * Nunchuk_readJoystickY()); // mouse inverts y-axis
    m.params.buttons = (int8_t) (Nunchuk_readCButton() + (Nunchuk_readZButton() << 1));
//        printf("joystick: <%d, %d>\r\n", m.params.x, -1 * m.params.y);
    USBD_HID_SendReport(&hUsbDeviceFS, m.buf, 5);
}

int main(void) {
    PongBot_ctor();
    peripheral_init();
    printf("\r\nInitialized peripherals\r\n");
    // wait for button press before proceeding
    while (GPIO_ReadPin(BTN_GPIO, BTN_GPIO_PIN)) ;
    printf("Initialize devices\r\n");
    device_init();
    GPIO_TogglePin(LED_GPIO, LED_GPIO_PIN);

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

