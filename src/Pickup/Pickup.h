#pragma once
#include "../Utils/Constants.h"
#include "../Player/Player.h"
#include <vector>
#include <random>

struct Pickup {
    float x, y;
    int type; // 0 - здоровье, 1 - патроны, 2 - броня
    bool active = true;
    float bobTimer = 0.0f;
    float rotationAngle = 0.0f;
};

class PickupManager {
private:
    std::vector<Pickup> pickups;
    std::mt19937 gen;
    
public:
    PickupManager();
    void init();
    void update(float dt, float playerX, float playerY, Player& player);
    const std::vector<Pickup>& getPickups() const { return pickups; }
};