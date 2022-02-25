/* main.h */

#ifndef __MAIN_H
#define __MAIN_H

#include "stm32l4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BTN_GPIO                        GPIOC
#define BTN_GPIO_PIN                    13
#define LED_GPIO                        GPIOA
#define LED_GPIO_PIN                    5

#define NUNCHUK_I2C I2C1

#define SYSCLOCK_FREQ                   80000000U // 80 MHz

void Error_Handler(void);

//void SystemClock_Config();

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */