#pragma once
#include "Enemy.h"
#include "../Player/Player.h"
#include <vector>
#include <random>

struct EnemyBullet {
    float x, y, dirX, dirY, distance;
    bool active;
};

class EnemyManager {
private:
    std::vector<Enemy> m_enemies;
    std::mt19937 m_gen;
    std::vector<EnemyBullet> m_bullets;
    
public:
    EnemyManager();
    void init();
    void update(float dt, float playerX, float playerY, Player& player);
    std::vector<Enemy>& getEnemies() { return m_enemies; }
    const std::vector<Enemy>& getEnemies() const { return m_enemies; }
    std::vector<EnemyBullet>& getBullets() { return m_bullets; }
    bool areAllDead() const;
};