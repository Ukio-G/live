//
// Created by ukio on 30.06.2020.
//

#ifndef LIVE_EXECUTING_H
#define LIVE_EXECUTING_H

#include <cstdint>
#include "Basics.h"
#include "Settings.h"
#include "World.h"
#include <bitset>
#include <unordered_set>

class World;
enum Command
{
    GO, TURN
};

// configuration bits:
// absorbing energy block
// absorbing other

class Executable {
public:
    Executable(){};
    Executable(iPoint position, fColor color);
    Executable(iPoint position, fColor color, Abilities_t abilities);

    void generateRandomProgram();
    void mutateSelf();
    void crossoverSelfFromOther(Program_t source);

    iPoint position;
    Abilities_t abilities;
    uint8_t instructionPointer = 0;
    uint16_t energy = Settings::baseEnergy;
    Program_t program;
    fColor color;

    inline void generateRandomCommand(char *first_command_byte) noexcept;

    inline void generateRandomArguments(char *first_command_byte) noexcept;

    inline void copyProgram(Command_u *source) noexcept;
};

class Executor
{
public:
    World * world;
    void executeWorld();
    void execute(Executable *subject);
};

class ExecutingPool {
public:
    size_t count = Settings::width * Settings::height;
    std::vector<Executable*> available;
    std::unordered_set<Executable*> used;

    void init();
    void deleteAvailable();
    void deleteUsed();
    void mutateAvailable();
    void updateFirmware(Program_t program);
    Executable* get();
    Executable* getMutated();
    void release(Executable*);
};

#endif //LIVE_EXECUTING_H
