/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */


#ifndef NUCLEOPONG_PONG_BOT_H
#define NUCLEOPONG_PONG_BOT_H

//#include <utility> // std::pair
#include <array>
#include <cstdint>

#include "systick.h"
#include "usbd_hid.h"

//using std::pair;
using std::array;

/* CUP NUMBERING:
 * STAGE 1:
 *     X X X X
 *      X 3 X
 *       1 2
 *        0
 *
 * STAGE 2:
 *    X X X
 *     5 6
 *      4
 *
 * STAGE 3:
 *    8 9
 *     7
 */
#define CUP0_X                (0)
#define CUP0_Y                (-175)
#define CUP1_X                (-12)
#define CUP1_Y                (-190)
#define CUP2_X                (-1 * CUP1_X)
#define CUP2_Y                (CUP1_Y)
#define CUP3_X                (0)
#define CUP3_Y                (-215)
#define CUP4_X                (0)
#define CUP4_Y                (-180)      // approximate
#define CUP5_X                (-15)    // approximate
#define CUP5_Y                (-210)     // approximate
#define CUP6_X                (-1 * CUP5_X)
#define CUP6_Y                (CUP5_Y)
#define CUP7_X                (0)
#define CUP7_Y                (-190)      // approximate
#define CUP8_X                (-10)     // approximate
#define CUP8_Y                (-205)     // approximate
#define CUP9_X                (-1 * CUP8_X)
#define CUP9_Y                (CUP8_Y)


extern USBD_HandleTypeDef hUsbDeviceFS;

static constexpr uint32_t POLLING_RATE = 16; // minimum delay between sequential HID reports

static inline void report_delay() { SysTick_Delay(POLLING_RATE); }

class mouse {
    friend class pong_bot;

public:
    constexpr mouse() = default;

private:
    static constexpr uint32_t report_size = 4;
    typedef array<int8_t, report_size> report_type;
    static report_type report;
    static constexpr auto &buttons = report[0];
    static constexpr auto &x = report[1];
    static constexpr auto &y = report[2];
    static constexpr auto &wheel = report[3];
//    static constexpr auto get_report = report.data();

    static int mouse_x;
    static int mouse_y;

    static void reset();
    static void find_ball();

    static inline void send_report() {
        USBD_HID_SendReport(&hUsbDeviceFS, reinterpret_cast<uint8_t *>(report.data()), report_size);
        report_delay();
    }

    static inline void release() {
        report.fill(0);
        send_report();
    }

    static inline void grab_ball() {
        find_ball();
        /*move_mouse(1, 0, 0);*/
    }

    static inline void follow_through() {
        buttons = 0;
        send_report();
        release();
    }

public:
//    static constexpr inline void set_buttons(int8_t new_buttons) { buttons = new_buttons; }
//    static constexpr inline void set_x(int8_t new_x) { x = new_x; }
//    static constexpr inline void set_y(int8_t new_y) { y = new_y; }
//    static constexpr inline void set_wheel(int8_t new_wheel) { wheel = new_wheel; }
    static inline void get_tracked(int &tracked_x, int &tracked_y) {tracked_x = mouse_x; tracked_y = mouse_y; }

    static void move(int8_t click, int8_t dx, int8_t dy);
    static void behave_as_mouse();
    static bool track_cursor(); // returns true if C button is pressed
    static void calibrate_sensitivity();
};


class pong_bot {
private:
    /*
     * Internal configuration parameters
     */
    static constexpr uint32_t FULL_THROWS = 4; // number of HID reports to send - 1



    struct coord_type {
        int x;
        int y;
    };

    bool random_mode;
    int cup;
    bool throw_1{true};
    bool missed{false};
    static constexpr array<coord_type , 10> cups{
            coord_type {CUP0_X, CUP0_Y},
            coord_type {CUP1_X, CUP1_Y},
            coord_type {CUP2_X, CUP2_Y},
            coord_type {CUP3_X, CUP3_Y},
            coord_type {CUP4_X, CUP4_Y},
            coord_type {CUP5_X, CUP5_Y},
            coord_type {CUP6_X, CUP6_Y},
            coord_type {CUP7_X, CUP7_Y},
            coord_type {CUP8_X, CUP8_Y},
            coord_type {CUP9_X, CUP9_Y}
    };
public:
    explicit pong_bot(bool rand=false);

    /**
     * @brief Throw ball at the next target cup
     * @return `true` if it's still the bot's turn, otherwise `false`
     */
    bool throw_ball();

    /**
     * @brief Called when the bot misses a shot to tell it to retry the same cup
     */
    void did_miss();

    /**
     * @brief Throws ball with given x & y power
     * @param x_pwr
     * @param y_pwr
     */
    void throw_xy(int x_pwr, int y_pwr);

    /**
     * @return Next cup to target (0-9)
     */
    [[nodiscard]] inline int get_cup() const { return cup; }
};

extern pong_bot *pb;


#endif //NUCLEOPONG_PONG_BOT_H

// unused but potentially useful macros
#if false
#define DY                      (-30) // spacing between cups 1 row apart
#define DX                      (50) // spacing between adjacent cups

/* STAGE 1:
 *      6 7 8 9
 *       3 4 5
 *        1 2
 *         0
 */

#define CUP0_X_1                (0)
#define CUP0_Y_1                (-441)
#define CUP0_1                  {CUP0_X_1, CUP0_Y_1} // 0, -175
#define CUP1_X_1                (CUP0_X_1 - (DX >> 1U))
#define CUP1_Y_1                (CUP0_Y_1 + DY)
#define CUP1_1                  {CUP1_X_1, CUP1_Y_1} // -12, -(175 to 180) (or -190)
#define CUP2_X_1                (CUP1_X_1 + DX)
#define CUP2_Y_1                (CUP0_Y_1 + DY)
#define CUP2_1                  {CUP2_X_1, CUP2_Y_1} // 12, -193
#define CUP4_X_1                (CUP0_X_1)
#define CUP4_Y_1                (CUP1_Y_1 + DY)
#define CUP4_1                  {CUP4_X_1, CUP4_Y_1} // 0, -215
#define CUP3_X_1                (CUP4_X_1 - DX)
#define CUP3_Y_1                (CUP1_Y_1 + DY)
#define CUP3_1                  {CUP3_X_1, CUP3_Y_1}
#define CUP5_X_1                (CUP4_X_1 + DX)
#define CUP5_Y_1                (CUP1_Y_1 + DY)
#define CUP5_1                  {CUP5_X_1, CUP5_Y_1}
#define CUP7_X                  (CUP1_X_1)
#define CUP7_Y                  (CUP3_Y_1 + DY)
#define CUP7                    {CUP7_X, CUP7_Y}
#define CUP8_X                  (CUP2_X_1)
#define CUP8_Y                  (CUP3_Y_1 + DY)
#define CUP8                    {CUP8_X, CUP8_Y}
#define CUP6_X                  (CUP7_X - DX)
#define CUP6_Y                  (CUP3_Y_1 + DY)
#define CUP6                    {CUP6_X, CUP6_Y}
#define CUP9_X                  (CUP8_X + DX)
#define CUP9_Y                  (CUP3_Y_1 + DY)
#define CUP9                    {CUP9_X, CUP9_Y}

/* STAGE 2:
 *   3 4 5
 *    1 2
 *     0
 */
#define CUP0_X_2                (0)
#define CUP0_Y_2                (-455)
#define CUP0_2                  {CUP0_X_2, CUP0_Y_2} // 0, -275
#define CUP1_X_2                (CUP0_X_2 - (DX >> 1U))
#define CUP1_Y_2                (CUP0_Y_2 + DY)
#define CUP1_2                  {CUP1_X_2, CUP1_Y_2} // -15, -245
#define CUP2_X_2                (CUP1_X_2 + DX)
#define CUP2_Y_2                (CUP0_Y_2 + DY)
#define CUP2_2                  {CUP2_X_2, CUP2_Y_2} // -27, -320
#define CUP4_X_2                (CUP0_X_2)
#define CUP4_Y_2                (CUP1_Y_2 + DY)
#define CUP4_2                  {CUP4_X_2, CUP4_Y_2}
#define CUP3_X_2                (CUP4_X_2 - DX)
#define CUP3_Y_2                (CUP1_Y_2 + DY)
#define CUP3_2                  {CUP3_X_2, CUP3_Y_2}
#define CUP5_X_2                (CUP4_X_2 + DX)
#define CUP5_Y_2                (CUP1_Y_2 + DY)
#define CUP5_2                  {CUP5_X_2, CUP5_Y_2}

/* STAGE 3:
 *   1 2
 *    0
 */
#define CUP0_X_3                (0)
#define CUP0_Y_3                (CUP1_Y_1) // -471
#define CUP0_3                  {CUP0_X_3, CUP0_Y_3} // 0, -265
#define CUP1_X_3                (CUP0_X_3 - (DX >> 1U))
#define CUP1_Y_3                (CUP0_Y_3 + DY)
#define CUP1_3                  {CUP1_X_3, CUP1_Y_3} // -25, -327
#define CUP2_X_3                (CUP1_X_3 + DX)
#define CUP2_Y_3                (CUP0_Y_3 + DY)
#define CUP2_3                  {CUP2_X_3, CUP2_Y_3} // -25, -327
#endif