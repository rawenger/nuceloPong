/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "nunchuk_controller.h"
#include "systick.h"

nunchuk::nunchuk(I2C_TypeDef *i2c) : i2c_channel(i2c) {
    // send init commands to the nunchuk
    I2C_SendData(i2c_channel, nunchuk_write_addr, init_commands, 2);
    I2C_SendData(i2c_channel, nunchuk_write_addr, begin_commands, 2);

    // ensure the first call to read_peripheral always updates
    I2C_ReceiveData(i2c_channel, nunchuk_read_addr, raw_data, 6);
    I2C_SendData(i2c_channel, nunchuk_write_addr, &zero, 1);
    last_update = SysTick_GetClk();
}

void nunchuk::read_peripheral() {
    if (SysTick_GetClk() - last_update >= max_refresh_rate) {
        I2C_ReceiveData(i2c_channel, nunchuk_read_addr, raw_data, 6);
        I2C_SendData(i2c_channel, nunchuk_write_addr, &zero, 1);
        last_update = SysTick_GetClk();
    }

    stick.set_coords(raw_data[0], raw_data[1]);
//    accel_x_axis = (raw_data[2]) << 2;
//    accel_y_axis = (raw_data[3]) << 2;
//    accel_z_axis = (raw_data[4]) << 2;
    z = !(raw_data[5] & 0b1);
    c = !((raw_data[5] & 0b10) >> 1U);
//    accel_z_axis |= (raw_data[5] & 0xC0) >> 6U;
//    accel_y_axis |= (raw_data[5] & 0x30) >> 4U;
//    accel_y_axis |= (raw_data[5] & 0xC) >> 2U;
}

int nunchuk::get_stick_x(uint32_t refresh) {
    if (SysTick_GetClk() - last_update > refresh)
        read_peripheral();
    return stick.get_x();
}

int nunchuk::get_stick_y(uint32_t refresh) {
    if (SysTick_GetClk() - last_update > refresh)
        read_peripheral();
    return stick.get_y();
}

bool nunchuk::get_c(uint32_t refresh) {
    if (SysTick_GetClk() - last_update > refresh)
        read_peripheral();
    return c;
}

bool nunchuk::get_z(uint32_t refresh) {
    if (SysTick_GetClk() - last_update > refresh)
        read_peripheral();
    return z;
}
