#include <cstdlib>
#include <iostream>
#include "include/World.h"
#include "include/Executing.h"

void World::fillVectices() {
    int counter = 0;
        for (int y = 0; y < Settings::height; ++y)
            for (int x = 0; x < Settings::width; ++x)
            vertices[counter++] = {((float)1.8*x/Settings::width) - 0.9f,-(float)1.8*y/Settings::height + 0.9f};
}

void World::fillColors() {
    int counter = 0;
    for (int x = 0; x < Settings::width; ++x)
        for (int y = 0; y < Settings::height; ++y)
            colors[counter++] = {0.0f,0.0f,0.0f};
}

void World::fillExecutables() {
        for (int y = 0; y < Settings::height; ++y)
            for (int x = 0; x < Settings::width; ++x)
                executables[x][y] = nullptr;
}

void World::fillObjects() {
    for (int y = 0; y < Settings::height; ++y)
        for (int x = 0; x < Settings::width; ++x)
            objects[x][y] = nullptr;
}

World::World() {
    fillColors();
    fillVectices();
    fillExecutables();
    fillObjects();
}

void World::setColor(int num, fColor color) {
    colors[num] = color;
}

void World::setColor(int x, int y, fColor color) {
    int num = y * Settings::width + x;
    colors[num] = color;
}

void World::appendExecutable(int num) {
    iPoint p = numToPoint(num);
    appendExecutable(p, emptyAbilities);
}

void World::appendExecutable(iPoint & position) {
    appendExecutable(position, emptyAbilities);
}

void World::appendExecutable(int num, Abilities_t abilities) {
    iPoint p = numToPoint(num);
    appendExecutable(p, abilities);
}

template<typename T>
void World::appendWorldObject(iPoint & position) {

}

template<>
void World::appendWorldObject<EnergySource>(iPoint & position) {
    worldObjectsCount++;
    EnergySource * energySource = energyPool->get();
    energySource->position = position;
    objects[position.x][position.y] = (BasicObject*)energySource;
    colors[pointToNum(position)] = energySource->color;
}

void World::appendExecutable(iPoint &position, Abilities_t abilities) {
    executableCount++;
    Executable * executable = ePoolPtr->getMutated();
    executable->position = position;
    executable->color = fColor(abilities);
    executables[position.x][position.y] = executable;
    colors[pointToNum(position)] = executables[position.x][position.y]->color;
}

void World::appendExecutableRandomPosition(Abilities_t abilities, size_t count) {
    for (size_t i = 0; i < count ; i++)
        appendExecutableRandomPosition(abilities);
}

void World::appendExecutableRandomPosition(Abilities_t abilities) {
    iPoint position = getRandomFreeCell();
    appendExecutable(position, abilities);
}

size_t World::freeCells() {
    return Settings::height * Settings::width - (getExecutablesCount() + getWorldObjectCount());
}

bool World::isPointFree(const iPoint & point) {
    BasicObject * object = objects[point.x][point.y];
    bool isObstructionObject = (object == nullptr) ? true : !object->isObstruction;
    return ((executables[point.x][point.y] == nullptr) && isObstructionObject);
}

bool World::hasExecutable(const iPoint & point) {
    return executables[point.x][point.y] != nullptr;
}

void World::removeExecutable(iPoint point) {
    if (hasExecutable(point)) {
        executables[point.x][point.y] = nullptr;
    }
    colors[pointToNum(point)] = {0.0,0.0,0.0};
    executableCount--;
}

void World::makeMove(Executable *subject, iPoint offset)
{
    if (offset.x == 0 && offset.y == 0) return;
    if (subject == nullptr) return;

    iPoint oldPosition = subject->position;
    iPoint newPosition = oldPosition + offset;

    if(newPosition.x >= (int)Settings::width) newPosition.x = 0;
    else if(newPosition.x < 0) newPosition.x = Settings::width - 1;

    if (newPosition.y >= (int)Settings::height) newPosition.y = 0;
    else if(newPosition.y < 0) newPosition.y = Settings::height - 1;

    if (!isPointFree(newPosition)) return;

    subject->position = newPosition;
    colors[pointToNum(newPosition)] = subject->color;

    if(objects[oldPosition.x][oldPosition.y] == nullptr)
        colors[pointToNum(oldPosition)] = {0.0,0.0,0.0};
    else
        colors[pointToNum(oldPosition)] = objects[oldPosition.x][oldPosition.y]->color;

    executables[oldPosition.x][oldPosition.y] = nullptr;
    executables[newPosition.x][newPosition.y] = subject;
}

inline iPoint World::getRandomFreeCell() noexcept {
    if (freeCells() == 0) return {0,0};
    iPoint result = randomPosition();
    while (!isPointFree(result))
        result = randomPosition();
    return result;
}

inline size_t World::getExecutablesCount() const noexcept {
    return executableCount;
}

inline size_t World::getWorldObjectCount() const noexcept {
    return worldObjectsCount;
}

