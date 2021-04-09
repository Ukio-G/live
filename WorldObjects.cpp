//
// Created by ukio on 26.07.2020.
//
#include "include/WorldObjects.h"

template<>
void WorldObjectsPool<EnergySource>::init() {
    available.reserve(count);
    for (int i = 0; i < count; i++) {
        EnergySource* object = new EnergySource(4);
        available.push_back(object);
    }
}
