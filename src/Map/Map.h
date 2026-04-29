#pragma once
#include "../Core/Config.h"

class Map {
private:
    static int s_worldMap[Config::MAP_WIDTH][Config::MAP_HEIGHT];

public:
    static void initMap();
    static const int (&getWorldMap())[Config::MAP_WIDTH][Config::MAP_HEIGHT] { return s_worldMap; }
    static bool isWalkable(float x, float y, float radius);
    static bool isEnemyWalkable(float x, float y, float radius);
};