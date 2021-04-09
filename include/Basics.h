//
// Created by ukio on 30.06.2020.
//

#ifndef LIVE_BASICS_H
#define LIVE_BASICS_H

#include <bitset>
#include <cmath>
#include "Settings.h"
#include <iostream>

#define PROGRAM_PRINT_FORMAT "%02X %02X %02X %02X"

struct fColor {
    float r;
    float g;
    float b;
    fColor() = default;
    fColor(float r, float g, float b) : r(r), g(g), b(b) {};
};

class Abilities_t : public std::bitset<Settings::abilitiesCount> {
public:
    Abilities_t() = default;
    Abilities_t(const std::bitset<Settings::abilitiesCount>& a) : std::bitset<Settings::abilitiesCount>(a) {};
    operator fColor() {
        return {0.5,0.5,0.5};
    }

    operator fColor() const {
        return {0.5,0.5,0.5};
    }
};

union Command_u{
    uint32_t fullInstruction;
    struct {
        uint8_t command;
        uint8_t argument[3];
    };
};

typedef Command_u Program_t[Settings::programSize];

static const Abilities_t emptyAbilities = Abilities_t().reset();

struct fPoint {
    float x;
    float y;
};

struct iPoint {
public:
    int x;
    int y;

    bool operator==(const iPoint & other) const {
        return (other.x == x && other.y == y);
    }

    bool operator!=(const iPoint & other) const {
        return (other.x != x || other.y != y);
    }

    bool operator<(const iPoint & other) const {
        int distance_this = distance({0,0});
        int distance_other = other.distance({0,0});
        return distance_this < distance_other;
    }

    int distance(const iPoint & other) const {
        int xx = (other.x - x) * (other.x - x);
        int yy = (other.y - y) * (other.y - y);
        int result = (int)std::sqrt(xx + yy);
        return  (result >= 0) ? result : -result;
    }

    iPoint operator+(const iPoint & other) const {
        return {x + other.x, y+other.y};
    }

    iPoint operator-(const iPoint & other) const {
        return {x - other.x, y-other.y};
    }
};

namespace std {
    template <> struct hash<iPoint>
    {
        size_t operator()(const iPoint & point) const
        {
            return hash<int>()(point.x) | hash<int>()(point.y);
        }
    };
}

inline iPoint numToPoint(int number) {
    return iPoint{static_cast<int>(number % Settings::width), static_cast<int>(number / Settings::width)};
}

inline int pointToNum(const iPoint & point) {
    return point.y * Settings::width + point.x;
}

inline iPoint randomPosition() {
    return {static_cast<int>(std::rand() % Settings::width), static_cast<int>(std::rand() % Settings::height)};
}

inline float fDistance(fPoint & fpoint1, fPoint & fpoint2) {
    float xx = (fpoint1.x - fpoint2.x) * (fpoint1.x - fpoint2.x);
    float yy = (fpoint1.y - fpoint2.y) * (fpoint1.y - fpoint2.y);
    float result = std::sqrt(xx + yy);
    return (result >= 0.0f) ? result : -result;
}

inline fPoint screenToNDC(fPoint click_point, iPoint screen_size) {
    float x_click =  (click_point.x / (float)screen_size.x);
    float y_click =  1 - (click_point.y / (float)screen_size.y);

    x_click = x_click * 2 - 1;
    y_click = y_click * 2 - 1;

    fPoint result = {x_click, y_click};
    return result;
}

#endif //LIVE_BASICS_H
