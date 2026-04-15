#pragma once
#include "../Utils/Constants.h"
#include "../Player/Player.h"
#include <vector>
#include <random>

struct Enemy {
    float x, y;
    float angle = 0.0f;
    bool alive = true;
    float attackCooldown = 0.0f;
    float moveTimer = 0.0f;
    float moveDirection = 0.0f;
    int health = ENEMY_HEALTH;
    float radius = ENEMY_RADIUS;
    float animFrame = 0.0f;
};

class EnemyManager {
private:
    std::vector<Enemy> enemies;
    std::mt19937 gen;
    
public:
    EnemyManager();
    void init();
    void update(float dt, float playerX, float playerY, Player& player);
    const std::vector<Enemy>& getEnemies() const { return enemies; }
    std::vector<Enemy>& getEnemies() { return enemies; }
    bool areAllDead() const;
};