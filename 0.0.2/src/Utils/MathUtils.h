#pragma once
#include <cmath>
#include <random>
#include "Constants.h"

extern std::random_device rd;
extern std::mt19937 gen;

inline float distance(float x1, float y1, float x2, float y2) {
    return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

bool isVisible(float x1, float y1, float x2, float y2, const int worldMap[MAP_WIDTH][MAP_HEIGHT]);