/* main.h */

#ifndef __MAIN_H
#define __MAIN_H

#include "usart.h"
#include "gpio.h"
#include "i2c.h"
#include "sysclock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BTN_GPIO GPIOC
#define BTN_GPIO_PIN 13
#define LED_GPIO GPIOA
#define LED_GPIO_PIN 5

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
