#include "Enemy.h"

Enemy::Enemy(float x, float y)
    : Entity(x, y, Config::ENEMY_RADIUS, Config::ENEMY_HEALTH)
    , m_state(Config::EnemyState::ALIVE)
    , m_attackCooldown(0.0f), m_moveTimer(0.0f), m_moveDirection(0.0f)
    , m_animFrame(0.0f), m_deathTimer(0.0f), m_corpseTimer(0.0f)
    , m_fallRotation(0.0f), m_fallOffset(0.0f) {
}

void Enemy::update(float dt) {
    switch (m_state) {
        case Config::EnemyState::DYING:
            m_deathTimer -= dt;
            m_fallRotation = (1.0f - m_deathTimer / Config::ENEMY_DEATH_ANIM_TIME) * 90.0f;
            m_fallOffset = (1.0f - m_deathTimer / Config::ENEMY_DEATH_ANIM_TIME) * 0.5f;
            if (m_deathTimer <= 0.0f) {
                m_state = Config::EnemyState::CORPSE;
                m_corpseTimer = Config::ENEMY_CORPSE_FADE_TIME;
            }
            break;
        case Config::EnemyState::CORPSE:
            m_corpseTimer -= dt;
            if (m_corpseTimer <= 0.0f) {
                m_state = Config::EnemyState::DEAD;
                m_alive = false;
            }
            break;
        default: break;
    }
}

void Enemy::takeDamage(int damage) {
    if (m_state != Config::EnemyState::ALIVE) return;
    Entity::takeDamage(damage);
    if (!m_alive) {
        m_state = Config::EnemyState::DYING;
        m_deathTimer = Config::ENEMY_DEATH_ANIM_TIME;
    }
}