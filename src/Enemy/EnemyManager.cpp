#include "EnemyManager.h"
#include "../Map/Map.h"
#include "../Core/MathUtils.h"

static std::random_device rd;

EnemyManager::EnemyManager() : m_gen(rd()) {}

void EnemyManager::init() {
    std::uniform_real_distribution<float> posDist(2.0f, Config::MAP_WIDTH - 2.0f);
    
    for (int i = 0; i < Config::MAX_ENEMIES; i++) {
        float ex, ey; bool validPos = false; int attempts = 0;
        while (!validPos && attempts < 100) {
            ex = posDist(m_gen); ey = posDist(m_gen);
            if (Map::isEnemyWalkable(ex, ey, Config::ENEMY_RADIUS) && 
                MathUtils::distance(ex, ey, 2.5f, 2.5f) > 8.0f) {
                bool tooClose = false;
                for (const auto& other : m_enemies)
                    if (MathUtils::distance(ex, ey, other.getX(), other.getY()) < 2.0f) { tooClose = true; break; }
                if (!tooClose) validPos = true;
            }
            attempts++;
        }
        if (validPos) m_enemies.emplace_back(ex, ey);
    }
}

void EnemyManager::update(float dt, float playerX, float playerY, Player& player) {
    for (auto& enemy : m_enemies) {
        enemy.addAnimFrame(dt * 5.0f);
        
        if (enemy.getState() == Config::EnemyState::ALIVE) {
            enemy.setAttackCooldown(enemy.getAttackCooldown() - dt);
            enemy.setMoveTimer(enemy.getMoveTimer() - dt);
            
            float distToPlayer = MathUtils::distance(enemy.getX(), enemy.getY(), playerX, playerY);
            
            if (MathUtils::isVisible(enemy.getX(), enemy.getY(), playerX, playerY, Map::getWorldMap())) {
                if (distToPlayer < Config::ENEMY_VISIBILITY_RANGE && distToPlayer > Config::ENEMY_ATTACK_RANGE) {
                    float angleToPlayer = std::atan2(playerY - enemy.getY(), playerX - enemy.getX());
                    enemy.setAngle(angleToPlayer);
                    
                    float moveX = enemy.getX() + std::cos(angleToPlayer) * Config::ENEMY_SPEED * dt;
                    float moveY = enemy.getY() + std::sin(angleToPlayer) * Config::ENEMY_SPEED * dt;
                    
                    if (Map::isEnemyWalkable(moveX, enemy.getY(), Config::ENEMY_RADIUS)) 
                        enemy.setPosition(moveX, enemy.getY());
                    if (Map::isEnemyWalkable(enemy.getX(), moveY, Config::ENEMY_RADIUS)) 
                        enemy.setPosition(enemy.getX(), moveY);
                } else if (distToPlayer <= Config::ENEMY_ATTACK_RANGE && enemy.getAttackCooldown() <= 0.0f) {
                    player.takeDamage(Config::ENEMY_DAMAGE);
                    enemy.setAttackCooldown(Config::ENEMY_ATTACK_COOLDOWN);
                }
            }
            
            if (distToPlayer >= Config::ENEMY_VISIBILITY_RANGE || 
                !MathUtils::isVisible(enemy.getX(), enemy.getY(), playerX, playerY, Map::getWorldMap())) {
                if (enemy.getMoveTimer() <= 0.0f) {
                    std::uniform_real_distribution<float> dirDist(0.0f, 2 * 3.14159f);
                    enemy.setMoveDirection(dirDist(m_gen));
                    enemy.setMoveTimer(2.0f);
                }
                
                float moveX = enemy.getX() + std::cos(enemy.getMoveDirection()) * Config::ENEMY_SPEED * 0.75f * dt;
                float moveY = enemy.getY() + std::sin(enemy.getMoveDirection()) * Config::ENEMY_SPEED * 0.75f * dt;
                
                if (Map::isEnemyWalkable(moveX, enemy.getY(), Config::ENEMY_RADIUS)) 
                    enemy.setPosition(moveX, enemy.getY());
                if (Map::isEnemyWalkable(enemy.getX(), moveY, Config::ENEMY_RADIUS)) 
                    enemy.setPosition(enemy.getX(), moveY);
            }
            
            if (enemy.getHealth() <= 0) {
                enemy.setState(Config::EnemyState::DYING);
                enemy.setDeathTimer(Config::ENEMY_DEATH_ANIM_TIME);
            }
        } else {
            enemy.update(dt);
        }
    }
}

bool EnemyManager::areAllDead() const {
    for (const auto& enemy : m_enemies)
        if (enemy.getState() == Config::EnemyState::ALIVE || enemy.getState() == Config::EnemyState::DYING) 
            return false;
    return true;
}