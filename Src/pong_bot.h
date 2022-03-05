/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

//
// Created by Ryan Wenger on 2/27/22.
//

#ifndef NUCLEOPONG_PONG_BOT_H
#define NUCLEOPONG_PONG_BOT_H

class pong_bot {
private:
    bool random_mode;
    int cup;
    int stage;
public:
    explicit pong_bot(bool rand=false);
    bool throw_ball();
};


#endif //NUCLEOPONG_PONG_BOT_H
