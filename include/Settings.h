//
// Created by ukio on 30.06.2020.
//

#ifndef LIVE_SETTINGS_H
#define LIVE_SETTINGS_H


#include <cstdint>
#include <cstddef>

class EnergySource;

template<class T>
class MaximumObject { };

template<>
class MaximumObject<EnergySource> {
public:
    const static size_t value = 2;
};


class Settings {
public:

    static const size_t width = 20;
    static const size_t height = 20;

    constexpr static const float pointSize = 30.5f;

    static const size_t maximumCommandValue = 3;    // Quantity of possible actions (move, eat, kill, etc...)
    static const size_t maximumArgumentValue = 255;   // Argument's value cannot be more, than this value

    static const size_t abilitiesCount = 3;            // ???
    static const size_t programSize = 20;              // Count of commands (one instruction + 3 arguments), written to each executable memory
    static const size_t mutateBytesCount = 3;          // Count of bytes should be changed in executable per one mutate
    static const size_t mutateProbability = 50;        // Probability of mutate (%)
    static const size_t baseEnergy = 100;

    static const size_t baseExecutableCount = 20;      // Count of executables will be created, then world reset
};


#endif //LIVE_SETTINGS_H
