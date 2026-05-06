#pragma once
#include "../Core/Config.h"
#include <vector>

struct Destructible {
    float x, y;
    int health;
    int maxHealth;
    Config::MapCell type;
    bool active;
};

class Map {
private:
    static int s_worldMap[Config::MAP_WIDTH][Config::MAP_HEIGHT];
    static std::vector<Destructible> s_destructibles;

public:
    static void initMap();
    static const int (&getWorldMap())[Config::MAP_WIDTH][Config::MAP_HEIGHT] { return s_worldMap; }
    static std::vector<Destructible>& getDestructibles() { return s_destructibles; }
    static bool isWalkable(float x, float y, float radius, const std::vector<Destructible>& destructibles);
    static void damageDestructible(int index, int damage);
};