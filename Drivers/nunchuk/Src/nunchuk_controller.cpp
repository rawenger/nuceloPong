/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "nunchuk_controller.h"

nunchuk::nunchuk(I2C_TypeDef *i2c) : i2c_channel(i2c) {
    // initialize the nunchuk
    I2C_SendData(i2c_channel, nunchuk_write_addr, init_commands, 2);
    I2C_SendData(i2c_channel, nunchuk_write_addr, begin_commands, 2);
}

void nunchuk::read_peripheral() {
    I2C_ReceiveData(i2c_channel, nunchuk_read_addr, raw_data, 6);

    stick.set_coords(raw_data[0], raw_data[1]);
//    accel_x_axis = (raw_data[2]) << 2;
//    accel_y_axis = (raw_data[3]) << 2;
//    accel_z_axis = (raw_data[4]) << 2;
    z = !(raw_data[5] & 0b1);
    c = !((raw_data[5] & 0b10) >> 1U);
//    accel_z_axis |= (raw_data[5] & 0xC0) >> 6U;
//    accel_y_axis |= (raw_data[5] & 0x30) >> 4U;
//    accel_y_axis |= (raw_data[5] & 0xC) >> 2U;
    I2C_SendData(i2c_channel, nunchuk_write_addr, &zero, 1);
    for (volatile int a = 0; a < 0x10000; a++); // delay
}

