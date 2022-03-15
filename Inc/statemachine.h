/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#ifndef NUCLEOPONG_STATEMACHINE_H
#define NUCLEOPONG_STATEMACHINE_H

#ifdef __cplusplus
extern "C" {
#endif

#define __GENERATE_ENUM_1(ENUMITEM)                         ENUMITEM,
#define __GENERATE_ENUM_2(ENUMITEM, VALUE)                  ENUMITEM = (VALUE),
#define __GENERATE_ENUM_X(x, ENUMITEM, VALUE, FUNC, ...)    FUNC


#define FOREACH_SIG(SIG) \
        SIG(BTN_CLICK, Q_USER_SIG)   \
        SIG(C_BTN)        \
        SIG(FINISHED)     \
        SIG(RESUME)       \
        SIG(OPTIONS)      \
        SIG(RESTART)      \
        SIG(MOUSE)        \
        SIG(IDLE)

#define GENERATE_ENUM(...)              __GENERATE_ENUM_X(,##__VA_ARGS__, \
                                                            __GENERATE_ENUM_2(__VA_ARGS__), \
                                                            __GENERATE_ENUM_1(__VA_ARGS__))
#define GENERATE_STRING(STRING, ...) #STRING,

#ifdef DEBUG_LOGGING
#define LOG_SIGNAL()                     { if (Q_SIG(me) < IDLE && Q_SIG(me) != 0) \
                                                LOG("%s: %s\r\n", \
                                                    __func__,      \
                                                    SIG_NAMES[ Q_SIG(me) - Q_ENTRY_SIG]); }
#else
#define LOG_SIGNAL()
#endif

enum PongBotSignals {
    FOREACH_SIG(GENERATE_ENUM)
};

static const char *SIG_NAMES[] = {
        "Q_ENTRY_SIG",
        "Q_EXIT_SIG",
        "Q_INIT_SIG",
        "Q_TIMEOUT_SIG",
        FOREACH_SIG(GENERATE_STRING)
};

extern struct PongBotTag nucleoPong;

void PongBot_ctor(void);


#ifdef __cplusplus
}
#endif


#endif //NUCLEOPONG_STATEMACHINE_H
