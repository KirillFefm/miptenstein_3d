#pragma once
#include "../Utils/Constants.h"

class Map {
public:
    static int worldMap[MAP_WIDTH][MAP_HEIGHT];
    static void initMap();
    static bool isWalkable(float x, float y, float radius = 0.3f);
    static bool isEnemyWalkable(float x, float y, float radius);
};