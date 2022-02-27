/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#ifndef NUCLEOPONG_CUP_PONG_BOT_H
#define NUCLEOPONG_CUP_PONG_BOT_H

#define REPORT_DELAY            15 // delay in ms between successive mouse reports

//void get_buttons(mouse *m);

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
#define CUP0_1                  {CUP0_X_1, CUP0_Y_1}
#define CUP1_X_1                (CUP0_X_1 - DX / 2)
#define CUP1_Y_1                (CUP0_Y_1 + DY)
#define CUP1_1                  {CUP1_X_1, CUP1_Y_1}
#define CUP2_X_1                (CUP1_X_1 + DX)
#define CUP2_Y_1                (CUP0_Y_1 + DY)
#define CUP2_1                  {CUP2_X_1, CUP2_Y_1}
#define CUP4_X_1                (CUP0_X_1)
#define CUP4_Y_1                (CUP1_Y_1 + DY)
#define CUP4_1                  {CUP4_X_1, CUP4_Y_1}
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
#define CUP0_2                  {CUP0_X_2, CUP0_Y_2}
#define CUP1_X_2                (CUP0_X_2 - DX / 2)
#define CUP1_Y_2                (CUP0_Y_2 + DY)
#define CUP1_2                  {CUP1_X_2, CUP1_Y_2}
#define CUP2_X_2                (CUP1_X_2 + DX)
#define CUP2_Y_2                (CUP0_Y_2 + DY)
#define CUP2_2                  {CUP2_X_2, CUP2_Y_2}
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
#define CUP0_3                  {CUP0_X_3, CUP0_Y_3}
#define CUP1_X_3                (CUP0_X_3 - DX / 2)
#define CUP1_Y_3                (CUP0_Y_3 + DY)
#define CUP1_3                  {CUP1_X_3, CUP1_Y_3}
#define CUP2_X_3                (CUP1_X_3 + DX)
#define CUP2_Y_3                (CUP0_Y_3 + DY)
#define CUP2_3                  {CUP2_X_3, CUP2_Y_3}

#ifdef __cplusplus
extern "C" {
#endif

// adjusts the cursor sensitivity in iPad settings to a consistent value
void Mouse_calibrateSensitivity();

// move the mouse cursor and track its position
void Mouse_trackCursor();

void Mouse_behaveAsMouse();

#ifdef __cplusplus
}
#endif

#endif //NUCLEOPONG_CUP_PONG_BOT_H
