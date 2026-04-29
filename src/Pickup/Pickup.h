#pragma once
#include "../Core/Config.h"
#include "../Player/Player.h"
#include <vector>
#include <random>

struct Pickup {
    float x, y;
    int type;
    bool active = true;
    float bobTimer = 0.0f;
    float rotationAngle = 0.0f;
};

class PickupManager {
private:
    std::vector<Pickup> m_pickups;
    static std::random_device s_rd;
    static std::mt19937 s_gen;
public:
    PickupManager();
    void init();
    void update(float dt, float playerX, float playerY, Player& player);
    const std::vector<Pickup>& getPickups() const { return m_pickups; }
};