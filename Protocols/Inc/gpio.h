/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#ifndef NUCLEOPONG_GPIO_H
#define NUCLEOPONG_GPIO_H

#include "stm32l476xx.h"
#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif


void GPIO_Init();

uint8_t GPIO_ReadPin(GPIO_TypeDef *gpio_port, uint8_t pin_no);
void GPIO_SetPin(GPIO_TypeDef *gpio_port, uint8_t pin_no);
void GPIO_ClearPin(GPIO_TypeDef *gpio_port, uint8_t pin_no);
void GPIO_TogglePin(GPIO_TypeDef *gpio_port, uint8_t pin_no);

#ifdef __cplusplus
}
#endif
#endif //NUCLEOPONG_GPIO_H
