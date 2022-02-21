/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 *
 */

#ifndef NUCLEOPONG_USB_H
#define NUCLEOPONG_USB_H

#include "stm32l476xx.h"
#include "usb_descriptors.h"

#ifdef __cplusplus
extern "C" {
#endif

void USB_Init(void);

#ifdef __cplusplus
}
#endif
#endif //NUCLEOPONG_USB_H
