#pragma once
#include "../Entity/Entity.h"
#include "../Core/Config.h"

class Enemy : public Entity {
private:
    Config::EnemyState m_state;
    float m_attackCooldown;
    float m_moveTimer;
    float m_moveDirection;
    float m_animFrame;
    float m_deathTimer;
    float m_corpseTimer;
    float m_fallRotation;
    float m_fallOffset;

public:
    Enemy(float x, float y);
    
    void update(float dt) override;
    void takeDamage(int damage) override;
    
    Config::EnemyState getState() const { return m_state; }
    float getAttackCooldown() const { return m_attackCooldown; }
    float getMoveTimer() const { return m_moveTimer; }
    float getMoveDirection() const { return m_moveDirection; }
    float getAnimFrame() const { return m_animFrame; }
    float getDeathTimer() const { return m_deathTimer; }
    float getCorpseTimer() const { return m_corpseTimer; }
    float getFallRotation() const { return m_fallRotation; }
    float getFallOffset() const { return m_fallOffset; }
    
    void setState(Config::EnemyState state) { m_state = state; }
    void setAttackCooldown(float cd) { m_attackCooldown = cd; }
    void setMoveTimer(float mt) { m_moveTimer = mt; }
    void setMoveDirection(float dir) { m_moveDirection = dir; }
    void setDeathTimer(float dt) { m_deathTimer = dt; }
    void setCorpseTimer(float ct) { m_corpseTimer = ct; }
    void setFallRotation(float rot) { m_fallRotation = rot; }
    void setFallOffset(float off) { m_fallOffset = off; }
    void addAnimFrame(float dt) { m_animFrame += dt; }
};