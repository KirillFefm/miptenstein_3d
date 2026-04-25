#include "Pickup.h"
#include "../Map/Map.h"
#include "../Utils/MathUtils.h"

PickupManager::PickupManager() : gen(rd()) {}

void PickupManager::init() {
    std::uniform_real_distribution<float> posDist(2.0f, MAP_WIDTH - 2.0f);
    for (int i = 0; i < MAX_PICKUPS; i++) {
        float px, py; bool validPos = false; int attempts = 0;
        while (!validPos && attempts < 100) {
            px = posDist(gen); py = posDist(gen);
            if (Map::isWalkable(px, py, PICKUP_RADIUS)) validPos = true;
            attempts++;
        }
        if (validPos) pickups.push_back({px, py, i % 3, true, 0.0f, 0.0f});
    }
}

void PickupManager::update(float dt, float playerX, float playerY, Player& player) {
    for (auto& pickup : pickups) {
        if (!pickup.active) continue;
        pickup.bobTimer += dt;
        if (distance(playerX, playerY, pickup.x, pickup.y) < PICKUP_COLLECT_DIST) {
            switch (pickup.type) {
                case 0: player.heal(HEALTH_PICKUP_VALUE); break;
                case 1: player.addAmmo(AMMO_PICKUP_VALUE); break;
                case 2: player.addArmor(ARMOR_PICKUP_VALUE); break;
            }
            pickup.active = false;
            player.score += PICKUP_SCORE;
        }
    }
}