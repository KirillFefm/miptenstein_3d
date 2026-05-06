#pragma once
#include <cmath>
#include <random>
#include "Config.h"
#include "../Map/Map.h"

namespace MathUtils {
    inline float distance(float x1, float y1, float x2, float y2) {
        return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    }

    inline bool isVisible(float x1, float y1, float x2, float y2, const int worldMap[Config::MAP_WIDTH][Config::MAP_HEIGHT]) {
        float dx = x2 - x1, dy = y2 - y1, dist = distance(x1, y1, x2, y2);
        if (dist < 0.1f) return true;
        float stepX = dx / dist * 0.1f, stepY = dy / dist * 0.1f;
        float checkX = x1, checkY = y1;
        for (float d = 0; d < dist; d += 0.1f) {
            checkX += stepX; checkY += stepY;
            int mapX = static_cast<int>(checkX), mapY = static_cast<int>(checkY);
            if (mapX >= 0 && mapX < Config::MAP_WIDTH && mapY >= 0 && mapY < Config::MAP_HEIGHT) {
                if (worldMap[mapX][mapY] == 1) return false;
                // Проверка разрушаемых объектов (непрозрачные)
                for (auto& obj : Map::getDestructibles()) {
                    if (!obj.active) continue;
                    float r = obj.type == Config::MapCell::BARREL ? Config::BARREL_RADIUS : Config::CRATE_RADIUS;
                    if (fabs(checkX - obj.x) < r && fabs(checkY - obj.y) < r) return false;
                }
            }
        }
        return true;
    }

    inline bool canMoveWithLean(float nx, float ny, float radius, float leanOffset, float angle,
                                const int worldMap[Config::MAP_WIDTH][Config::MAP_HEIGHT]) {
        float lx = nx + std::cos(angle + 3.14159f/2) * leanOffset;
        float ly = ny + std::sin(angle + 3.14159f/2) * leanOffset;
        for (float dx = -radius; dx <= radius; dx += radius * 0.5f)
            for (float dy = -radius; dy <= radius; dy += radius * 0.5f) {
                int mapX = static_cast<int>(lx + dx), mapY = static_cast<int>(ly + dy);
                if (mapX < 0 || mapX >= Config::MAP_WIDTH || mapY < 0 || mapY >= Config::MAP_HEIGHT) return false;
                if (worldMap[mapX][mapY] == 1) return false;
                // Проверка коллизии с разрушаемыми объектами
                
            }
        return true;
    }
}