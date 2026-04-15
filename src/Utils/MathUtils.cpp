#include "MathUtils.h"

std::random_device rd;
std::mt19937 gen(rd());

bool isVisible(float x1, float y1, float x2, float y2, const int worldMap[MAP_WIDTH][MAP_HEIGHT]) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dist = distance(x1, y1, x2, y2);
    
    if (dist < 0.1f) return true;
    
    float stepX = dx / dist * 0.1f;
    float stepY = dy / dist * 0.1f;
    
    float checkX = x1;
    float checkY = y1;
    
    for (float d = 0; d < dist; d += 0.1f) {
        checkX += stepX;
        checkY += stepY;
        
        int mapX = static_cast<int>(checkX);
        int mapY = static_cast<int>(checkY);
        
        if (mapX >= 0 && mapX < MAP_WIDTH && mapY >= 0 && mapY < MAP_HEIGHT) {
            if (worldMap[mapX][mapY] == 1) {
                return false;
            }
        }
    }
    return true;
}