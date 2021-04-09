//
// Created by ukio on 23.07.2020.
//

#ifndef LIVE_IMPLEMENTATIONEXECUTABLEFUNCTIONS_H
#define LIVE_IMPLEMENTATIONEXECUTABLEFUNCTIONS_H

#include "World.h"
#include "Executing.h"

/*
 *      128 64 ...... 2 1
 * BITS : 1 1 1 1 0 0 0 0 is 255 - 1 - 2 - 4 - 8
 *        | | | | | | | |
 *     y_offset | | | | |
 *           x_offset | |
 *               sign_y |
 *                 sign_x
 */



union PositionOffset{
    uint8_t data;
    struct {
        bool sign_x: 1;
        bool sign_y: 1;
        uint8_t x_offset: 3;
        uint8_t y_offset: 3;
    };
};

struct Environment {
    World * world;
    Executor * executor;
    Executable * object;
};
inline void makeMove(Environment env, Command_u & command) noexcept;
inline void eatTree(Environment env, Command_u & command) noexcept;
inline void chargeFromEnergy(Environment env,  Command_u & command) noexcept;

static void (*commandsImplementations[Settings::maximumCommandValue])(Environment env, Command_u & command) = {
        makeMove, eatTree, chargeFromEnergy
};

inline void makeMove(Environment env, Command_u & command) noexcept {
    PositionOffset p_offset = {command.argument[0]};
    int x_offset = (p_offset.sign_x) ? (p_offset.x_offset > 0) : -(p_offset.x_offset > 0);
    int y_offset = (p_offset.sign_y) ? (p_offset.y_offset > 0) : -(p_offset.y_offset > 0);
    iPoint offset = {x_offset,y_offset};
    env.world->makeMove(env.object, offset);
}

inline void eatTree(Environment env, Command_u & command) noexcept {

}

inline void chargeFromEnergy(Environment env,  Command_u & command) noexcept {
    int x_start = std::max(0, env.object->position.x - 1);
    int y_start = std::max(0, env.object->position.y - 1);

    int x_end = std::min((int)Settings::width-1, env.object->position.x + 1);
    int y_end = std::max((int)Settings::height-1, env.object->position.y + 1);

    for (int x = x_start; x <= x_end; x++)
        for (int y = y_start; y <= y_end; y++) {
            if (env.world->objects[x][y] != nullptr){
                if(env.world->objects[x][y]->type == BasicObject::EnergySource)
                    env.object->energy += ((EnergySource*)env.world->objects[x][y])->energy;
            }
        }
}


#endif //LIVE_IMPLEMENTATIONEXECUTABLEFUNCTIONS_H
