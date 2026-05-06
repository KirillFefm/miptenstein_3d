#include "Map.h"
#include <cmath>

int Map::s_worldMap[Config::MAP_WIDTH][Config::MAP_HEIGHT];
std::vector<Destructible> Map::s_destructibles;

void Map::initMap() {
    int tempMap[Config::MAP_WIDTH][Config::MAP_HEIGHT] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1},
        {1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1},
        {1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1},
        {1,0,0,1,0,0,1,1,1,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1,0,0,1,0,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,0,0,1,0,0,1,1,1,1,1,0,0,1,1,1,1,1,0,1,0,0,1,0,0,0,0,0,1},
        {1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1},
        {1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1},
        {1,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,1,1,0,0,0,0,1,0,0,1,1,1,1,0,0,1,0,0,0,0,1,1,1,0,0,0,0,1},
        {1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
        {1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
        {1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1},
        {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
        {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
    for (int i = 0; i < Config::MAP_WIDTH; i++)
        for (int j = 0; j < Config::MAP_HEIGHT; j++)
            s_worldMap[i][j] = tempMap[i][j];
    
    // Добавляем бочки и ящики
    s_destructibles.push_back({5.5f, 8.5f, Config::BARREL_HEALTH, Config::BARREL_HEALTH, Config::MapCell::BARREL, true});
    s_destructibles.push_back({10.5f, 8.5f, Config::CRATE_HEALTH, Config::CRATE_HEALTH, Config::MapCell::CRATE, true});
    s_destructibles.push_back({5.5f, 14.5f, Config::BARREL_HEALTH, Config::BARREL_HEALTH, Config::MapCell::BARREL, true});
    s_destructibles.push_back({14.5f, 14.5f, Config::CRATE_HEALTH, Config::CRATE_HEALTH, Config::MapCell::CRATE, true});
    s_destructibles.push_back({20.5f, 10.5f, Config::BARREL_HEALTH, Config::BARREL_HEALTH, Config::MapCell::BARREL, true});
    s_destructibles.push_back({8.5f, 20.5f, Config::CRATE_HEALTH, Config::CRATE_HEALTH, Config::MapCell::CRATE, true});
}

bool Map::isWalkable(float x, float y, float radius, const std::vector<Destructible>& destructibles) {
    for (float dx = -radius; dx <= radius; dx += radius * 0.5f) {
        for (float dy = -radius; dy <= radius; dy += radius * 0.5f) {
            int mapX = static_cast<int>(x + dx);
            int mapY = static_cast<int>(y + dy);
            if (mapX < 0 || mapX >= Config::MAP_WIDTH || mapY < 0 || mapY >= Config::MAP_HEIGHT) return false;
            if (s_worldMap[mapX][mapY] == 1) return false;
            for (auto& d : destructibles) {
                if (!d.active) continue;
                float r = (d.type == Config::MapCell::BARREL) ? Config::BARREL_RADIUS : Config::CRATE_RADIUS;
                if (fabs(d.x - (x + dx)) < r && fabs(d.y - (y + dy)) < r) return false;
            }
        }
    }
    return true;
}

void Map::damageDestructible(int index, int damage) {
    if (index >= 0 && index < (int)s_destructibles.size() && s_destructibles[index].active) {
        s_destructibles[index].health -= damage;
        if (s_destructibles[index].health <= 0) s_destructibles[index].active = false;
    }
}