#include "Enemy.h"
#include "../Map/Map.h"
#include "../Utils/MathUtils.h"

EnemyManager::EnemyManager() : gen(rd()) {}

void EnemyManager::init() {
    std::uniform_real_distribution<float> posDist(2.0f, MAP_WIDTH - 2.0f);
    
    for (int i = 0; i < MAX_ENEMIES; i++) {
        float ex, ey;
        bool validPos = false;
        int attempts = 0;
        
        while (!validPos && attempts < 100) {
            ex = posDist(gen);
            ey = posDist(gen);
            
            if (Map::isEnemyWalkable(ex, ey, ENEMY_RADIUS) && distance(ex, ey, 2.5f, 2.5f) > 8.0f) {
                bool tooClose = false;
                for (const auto& other : enemies) {
                    if (distance(ex, ey, other.x, other.y) < 2.0f) {
                        tooClose = true;
                        break;
                    }
                }
                if (!tooClose) validPos = true;
            }
            attempts++;
        }
        if (validPos) {
            enemies.push_back({ex, ey});
        }
    }
}

void EnemyManager::update(float dt, float playerX, float playerY, Player& player) {
    for (auto& enemy : enemies) {
        if (!enemy.alive) continue;
        
        enemy.attackCooldown -= dt;
        enemy.moveTimer -= dt;
        enemy.animFrame += dt * 5.0f;
        
        float distToPlayer = distance(enemy.x, enemy.y, playerX, playerY);
        
        // Исправлено: передаем Map::worldMap как 5-й аргумент
        if (isVisible(enemy.x, enemy.y, playerX, playerY, Map::worldMap)) {
            if (distToPlayer < ENEMY_VISIBILITY_RANGE && distToPlayer > ENEMY_ATTACK_RANGE) {
                float angleToPlayer = atan2f(playerY - enemy.y, playerX - enemy.x);
                enemy.angle = angleToPlayer;
                
                float moveX = enemy.x + cosf(angleToPlayer) * ENEMY_SPEED * dt;
                float moveY = enemy.y + sinf(angleToPlayer) * ENEMY_SPEED * dt;
                
                if (Map::isEnemyWalkable(moveX, enemy.y, ENEMY_RADIUS)) enemy.x = moveX;
                if (Map::isEnemyWalkable(enemy.x, moveY, ENEMY_RADIUS)) enemy.y = moveY;
            } else if (distToPlayer <= ENEMY_ATTACK_RANGE && enemy.attackCooldown <= 0.0f) {
                player.takeDamage(ENEMY_DAMAGE);
                enemy.attackCooldown = ENEMY_ATTACK_COOLDOWN;
            }
        }
        
        // Исправлено: передаем Map::worldMap как 5-й аргумент
        if (distToPlayer >= ENEMY_VISIBILITY_RANGE || !isVisible(enemy.x, enemy.y, playerX, playerY, Map::worldMap)) {
            if (enemy.moveTimer <= 0.0f) {
                std::uniform_real_distribution<float> dirDist(0.0f, 2 * 3.14159f);
                enemy.moveDirection = dirDist(gen);
                enemy.moveTimer = 2.0f;
            }
            
            float moveX = enemy.x + cosf(enemy.moveDirection) * ENEMY_SPEED * 0.75f * dt;
            float moveY = enemy.y + sinf(enemy.moveDirection) * ENEMY_SPEED * 0.75f * dt;
            
            if (Map::isEnemyWalkable(moveX, enemy.y, ENEMY_RADIUS)) enemy.x = moveX;
            if (Map::isEnemyWalkable(enemy.x, moveY, ENEMY_RADIUS)) enemy.y = moveY;
        }
    }
}

bool EnemyManager::areAllDead() const {
    for (const auto& enemy : enemies) {
        if (enemy.alive) return false;
    }
    return true;
}