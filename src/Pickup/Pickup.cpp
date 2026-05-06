#include "Pickup.h"
#include "../Map/Map.h"
#include "../Core/MathUtils.h"

std::random_device PickupManager::s_rd;
std::mt19937 PickupManager::s_gen(PickupManager::s_rd());

PickupManager::PickupManager() {}

void PickupManager::init() {
    std::uniform_real_distribution<float> posDist(2.0f, Config::MAP_WIDTH - 2.0f);
    for (int i = 0; i < Config::MAX_PICKUPS; i++) {
        float px, py; bool validPos = false; int attempts = 0;
        while (!validPos && attempts < 100) {
            px = posDist(s_gen); py = posDist(s_gen);
            if (Map::isWalkable(px, py, Config::PICKUP_RADIUS, Map::getDestructibles())) validPos = true;
            attempts++;
        }
        if (validPos) m_pickups.push_back({px, py, i % 3, true, 0.0f, 0.0f});
    }
}

void PickupManager::update(float dt, float playerX, float playerY, Player& player) {
    for (auto& pickup : m_pickups) {
        if (!pickup.active) continue;
        pickup.bobTimer += dt;
        if (MathUtils::distance(playerX, playerY, pickup.x, pickup.y) < Config::PICKUP_COLLECT_DIST) {
            switch (pickup.type) {
                case Config::HEALTH: player.heal(Config::HEALTH_PICKUP_VALUE); break;
                case Config::AMMO:   player.addAmmo(Config::AMMO_PICKUP_VALUE); break;
                case Config::ARMOR:  player.addArmor(Config::ARMOR_PICKUP_VALUE); break;
            }
            pickup.active = false;
            player.addScore(Config::PICKUP_SCORE);  // ← ИСПРАВЛЕНО
        }
    }
}