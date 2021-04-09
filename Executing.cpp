//
// Created by ukio on 30.06.2020.
//

#include <cstring>
#include <iostream>
#include "include/Executing.h"
#include <algorithm>
#include <set>
#include "include/ImplementationExecutableFunctions.h"

void Executor::execute(Executable *subject) {
    if (subject->instructionPointer > Settings::programSize - 1)
        subject->instructionPointer = 0;


    Command_u current_instruction = subject->program[subject->instructionPointer];
    uint8_t command = *((uint8_t*)&current_instruction);

    Environment env = {world, this, subject};
    commandsImplementations[command](env, current_instruction);

    subject->energy--;
    subject->instructionPointer++;

    if (subject->energy <= 0) {
        world->removeExecutable(subject->position);
        world->ePoolPtr->release(subject);
    }

    if (subject->instructionPointer > Settings::programSize - 1)
        subject->instructionPointer = 0;
}

void Executor::executeWorld() {
    auto exec_objects = world->ePoolPtr->used;
    for (auto object: exec_objects)
        execute(object);
    if (world->getExecutablesCount() == 0)
        world->appendExecutableRandomPosition(emptyAbilities, Settings::baseExecutableCount);
}

Executable::Executable(iPoint position, fColor color) : position(position), color(color) {
    abilities.reset();
    generateRandomProgram();
}

Executable::Executable(iPoint position, fColor color, Abilities_t abilities) : position(position), color(color), abilities(abilities) {
    generateRandomProgram();
}

inline void Executable::generateRandomCommand(char * first_command_byte) noexcept{
    *first_command_byte = std::rand() % Settings::maximumCommandValue;
}

inline void Executable::generateRandomArguments(char * first_command_byte) noexcept{
    *(first_command_byte+1) = std::rand() % Settings::maximumArgumentValue;
    *(first_command_byte+2) = std::rand() % Settings::maximumArgumentValue;
    *(first_command_byte+3) = std::rand() % Settings::maximumArgumentValue;
}

void Executable::generateRandomProgram() {
    /* Generate commands */
    for (int i = 0; i < Settings::programSize; ++i) {
        char * first_byte = (char *) &program[i];
        generateRandomCommand(first_byte);
        generateRandomArguments(first_byte);
    }
}

void Executable::mutateSelf() {
    std::set<size_t> indexes;
    while(indexes.size() < Settings::mutateBytesCount)
        indexes.insert(std::rand() % Settings::programSize);

    for(auto i : indexes){
        char * first_byte = (char *) &program[i];
        generateRandomCommand(first_byte);
        generateRandomArguments(first_byte);
    }
}

void Executable::crossoverSelfFromOther(Program_t source) {
    copyProgram(source);
    mutateSelf();
}

void Executable::copyProgram(Command_u *source) noexcept {
    if(source != program)
        memcpy(program, source, Settings::programSize);
}

void ExecutingPool::init() {
    available.reserve(count);
    for (int i = 0; i < count; i++) {
        Executable* executable = new Executable;
        executable->generateRandomProgram();
        executable->abilities.reset();
        available.push_back(executable);
    }
}

void ExecutingPool::deleteAvailable() {
    for(auto item : available)
        delete item;
}

void ExecutingPool::deleteUsed() {
    for(auto item : used)
        delete item;
}

void ExecutingPool::mutateAvailable() {
    for(auto item : available)
        if(std::rand() % Settings::mutateProbability == 0)
            item->mutateSelf();
}

Executable *ExecutingPool::get() {
    if (available.empty())
        return nullptr;
    Executable* result = available.back();
    available.pop_back();
    used.insert(result);
    result->energy = Settings::baseEnergy;
    result->instructionPointer = 0;
    return result;
}

void ExecutingPool::release(Executable *executable) {
        used.erase(executable);
        available.push_back(executable);
}

Executable *ExecutingPool::getMutated() {
    if (available.empty())
        return nullptr;
    Executable* result = available.back();
    available.pop_back();
    used.insert(result);
    result->mutateSelf();
    result->energy = Settings::baseEnergy;
    result->instructionPointer = 0;
    return result;
}

void ExecutingPool::updateFirmware(Program_t program) {
    Program_t buffer;
    memcpy(buffer, program, Settings::programSize);
    for(auto item : available)
        item->copyProgram(buffer);
}
