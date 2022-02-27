/*
 *
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#ifndef NUCLEOPONG_NUNCHUK_CONTROLLER_H
#define NUCLEOPONG_NUNCHUK_CONTROLLER_H

#include "i2c.h"

class nunchuk {
private:
    static constexpr uint32_t I2C_timeout_delay = 1000U;
    static constexpr uint32_t nunchuk_write_addr = 0xA4U;
    static constexpr uint32_t nunchuk_read_addr = 0xA4U;
    static constexpr uint32_t sensitivy_threshold = 10U;
    static constexpr uint8_t init_commands[2] = {0xF0, 0x55};
    static constexpr uint8_t begin_commands[2] = {0xFB, 0x00};
    static constexpr uint8_t zero = 0x00;
    static constexpr uint32_t max_refresh_rate = 10U; // update every ≥ 10ms

    I2C_TypeDef *i2c_channel;

    class joystick {
    private:
        struct coords {
            int x;
            int y;
        };

        coords real_coords;

        static constexpr coords CENTER{129, 130};
//        static constexpr coords TOP_CENTER{123,225};
//        static constexpr coords TOP_RIGHT{194, 200};
//        static constexpr coords CENTER_RIGHT{218,132};
//        static constexpr coords BOTTOM_RIGHT{190,63};
//        static constexpr coords BOTTOM_CENTER{128,39};
//        static constexpr coords BOTTOM_LEFT{59,65};
//        static constexpr coords CENTER_LEFT{32,137};
//        static constexpr coords TOP_LEFT{58,200};

    public:
        inline void set_coords(int x, int y) { real_coords = {x, y}; }
        [[nodiscard]] inline int get_x() const { return real_coords.x / sensitivy_threshold - CENTER.x / sensitivy_threshold; }
        [[nodiscard]] inline int get_y() const { return real_coords.y / sensitivy_threshold - CENTER.y / sensitivy_threshold; }
    };

    uint8_t raw_data[6]{};
    bool c{}, z{};
    joystick stick{};

    // time since the nunchuk I2C was last read
    uint32_t last_update;

    void read_peripheral();
public:
    explicit nunchuk(I2C_TypeDef *i2c);

    // at some point I plan to implement some sort of non-blocking delay somewhere in the code.
    // Once that happens, I'll redesign this implementation to make it so that the
    // blocking (~100ms) delay at the end of read_peripheral is run in a non-blocking mode, and calls to the
    // getters below will then be able to call read_peripheral internally, without necessarily having to wait
    // for the read to happen--if the delay has not expired, then old values can simply be reused.

    [[nodiscard]] inline int get_stick_x() { read_peripheral(); return stick.get_x(); }
    [[nodiscard]] inline int get_stick_y() { read_peripheral(); return stick.get_y(); }
    [[nodiscard]] inline bool get_c() { read_peripheral(); return c; }
    [[nodiscard]] inline bool get_z() { read_peripheral(); return z; }

    // allows specifying a refresh rate so rapid subsequent calls to this function don't cause
    // unnecessary blocking while waiting on I2C data
    int get_stick_x(uint32_t refresh);
    int get_stick_y(uint32_t refresh);
    bool get_c(uint32_t refresh);
    bool get_z(uint32_t refresh);

};


#endif //NUCLEOPONG_NUNCHUK_CONTROLLER_H
