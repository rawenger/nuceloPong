/* Wrapper library for C++ nunchuk_controller class to be called safely from C
 *
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "nunchuk.h"
#include "nunchuk_controller.h"

#ifdef __cplusplus
extern "C" {
#endif

static nunchuk *nc;

void Init_nunchuk(I2C_TypeDef *i2c_channel) {
    nc = new nunchuk(i2c_channel);
}

void Nunchuk_updateValues() {
    nc->read_peripheral();
}

int Nunchuk_readJoystickX() {
    return nc->get_stick_x();
}

int Nunchuk_readJoystickY() {
    return nc->get_stick_y();
}

int Nunchuk_readCButton() {
    return nc->get_c();
}

int Nunchuk_readZButton() {
    return nc->get_z();
}

#ifdef __cplusplus
}
#endif