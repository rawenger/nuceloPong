/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#ifndef NUCLEOPONG_BSP_H
#define NUCLEOPONG_BSP_H

#include "stm32l4xx.h"
#include <stdio.h>
#include "usart.h"
#include "gpio.h"
#include "i2c.h"
#include "sysclock.h"
#include "usbd_hid.h"
#include "usb_device.h"
#include "systick.h"
#include "spi.h"
#include "lcd_driver.h"
#include "nunchuk.h"

#ifdef __cplusplus
extern "C" {
#endif

void show_welcome_screen();

#ifdef __cplusplus
}
#endif

#endif //NUCLEOPONG_BSP_H
