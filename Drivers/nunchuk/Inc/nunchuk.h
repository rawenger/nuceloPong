/* Wrapper library for C++ nunchuk_controller class to be called safely from C
 *
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#ifndef NUCLEOPONG_NUNCHUK_H
#define NUCLEOPONG_NUNCHUK_H

#include "i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

void Init_nunchuk(I2C_TypeDef *i2c_channel);
void Nunchuk_updateValues();
int Nunchuk_readJoystickX();
int Nunchuk_readJoystickY();
int Nunchuk_readCButton();
int Nunchuk_readZButton();

#ifdef __cplusplus
}
#endif

#endif //NUCLEOPONG_NUNCHUK_H
