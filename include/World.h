//
// Created by ukio on 30.06.2020.
//

#ifndef LIVE_WORLD_H
#define LIVE_WORLD_H

#include <vector>
#include "Basics.h"
#include "Settings.h"
#include "WorldObjects.h"
#include <unordered_set>

class Executable;
class ExecutingPool;
class World {
public:
    World();

    fPoint vertices[Settings::width * Settings::height];
    fColor colors[Settings::width * Settings::height];
    Executable* executables[Settings::width][Settings::height];
    BasicObject* objects[Settings::width][Settings::height];

    ExecutingPool *ePoolPtr;
    WorldObjectsPool<EnergySource> *energyPool;
    size_t executableCount = 0;
    size_t worldObjectsCount = 0;
    void fillVectices();
    void fillColors();

    void setColor(int num, fColor color);
    void setColor(int x, int y, fColor color);

    void appendExecutable(int num);
    void appendExecutable(iPoint &position);
    void appendExecutable(int num, Abilities_t abilities);
    void appendExecutable(iPoint &position, Abilities_t abilities);

    void fillExecutables();
    void appendExecutableRandomPosition(Abilities_t abilities);
    bool isPointFree(const iPoint &other);
    size_t freeCells();
    inline size_t getExecutablesCount() const noexcept;

    void removeExecutable(iPoint point);

    void makeMove(Executable *subject, iPoint offset);

    void appendExecutableRandomPosition(Abilities_t abilities, size_t  count);

    bool hasExecutable(const iPoint &point);

    template<class T>
    void appendWorldObject(iPoint &position);

    inline size_t getWorldObjectCount() const noexcept;

    inline iPoint getRandomFreeCell() noexcept;

    void fillObjects();
};


#endif //LIVE_WORLD_H
