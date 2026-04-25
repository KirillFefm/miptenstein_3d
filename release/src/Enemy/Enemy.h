#pragma once
#include "../Utils/Constants.h"
#include "../Player/Player.h"
#include <vector>
#include <random>

enum class EnemyState { ALIVE, DYING, CORPSE, DEAD };

struct Enemy {
    float x, y;
    float angle = 0.0f;
    EnemyState state = EnemyState::ALIVE;
    float attackCooldown = 0.0f;
    float moveTimer = 0.0f;
    float moveDirection = 0.0f;
    int health = ENEMY_HEALTH;
    float radius = ENEMY_RADIUS;
    float animFrame = 0.0f;
    float deathTimer = 0.0f;
    float corpseTimer = 0.0f;
    float fallRotation = 0.0f;
    float fallOffset = 0.0f;
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