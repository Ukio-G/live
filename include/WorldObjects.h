//
// Created by ukio on 30.06.2020.
//

#ifndef LIVE_WORLDOBJECTS_H
#define LIVE_WORLDOBJECTS_H

#include "Basics.h"
#include "Settings.h"
#include <vector>
#include <set>
#include <unordered_set>

class BasicObject
{
public:
    enum Type : short {
        NoType, EnergySource
    };
    BasicObject()= default;
    BasicObject(iPoint point, bool obstruction, fColor color, bool destroyable, bool killable) :
    position(point), isObstruction(obstruction), color(color), isDestroyable(destroyable), isKillable(killable) {};
    iPoint position;
    bool isObstruction = false;
    bool isDestroyable = false;
    bool isKillable = false;
    Type type = NoType;
    fColor color;

};

class EnergySource : public BasicObject
{
public:
    EnergySource() = default;
    EnergySource(size_t energy) :
    BasicObject({0,0}, false, {0.1,0.5,0.7}, true, false),
    energy(energy) {
        type = BasicObject::EnergySource;
    };
    size_t energy;
};


template<class T>
class WorldObjectsPool {
public:
    size_t count = MaximumObject<T>::value;
    std::vector<T*> available;
    std::unordered_set<BasicObject*> used;

    void init();
    void deleteAvailable();
    void deleteUsed();
    T* get();
    void release(T*);
};


template<class T>
void WorldObjectsPool<T>::init() {
    available.reserve(count);
    for (int i = 0; i < count; i++) {
        T* object = new T;
        available.push_back(object);
    }
}



template<class T>
void WorldObjectsPool<T>::deleteAvailable() {

}

template<class T>
void WorldObjectsPool<T>::deleteUsed() {

}

template<class T>
T *WorldObjectsPool<T>::get() {
    if (available.empty())
        return nullptr;
    T* result = available.back();
    available.pop_back();
    used.insert(result);
    return result;
}

template<class T>
void WorldObjectsPool<T>::release(T * object) {
    used.erase(object);
    available.push_back(object);
}

#endif //LIVE_WORLDOBJECTS_H
