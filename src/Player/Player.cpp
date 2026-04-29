#include "Player.h"

Player::Player()
    : Entity(2.5f, 2.5f, Config::PLAYER_RADIUS, Config::PLAYER_MAX_HEALTH)
    , m_ammo(30), m_maxAmmo(Config::PLAYER_MAX_AMMO)
    , m_armor(0), m_maxArmor(Config::PLAYER_MAX_ARMOR)
    , m_score(0)
    , m_invulnerabilityTimer(0.0f), m_weaponBob(0.0f), m_weaponRecoil(0.0f)
    , m_leanAngle(0.0f), m_leanOffset(0.0f), m_isAiming(false) {
}

void Player::update(float dt) {
    m_invulnerabilityTimer -= dt;
    if (m_invulnerabilityTimer < 0) m_invulnerabilityTimer = 0;
    m_weaponRecoil *= 0.8f;
    if (m_weaponRecoil < 0.01f) m_weaponRecoil = 0.0f;
    m_leanAngle *= 0.9f;
    m_leanOffset *= 0.9f;
    if (std::fabs(m_leanAngle) < 0.01f) m_leanAngle = 0.0f;
    if (std::fabs(m_leanOffset) < 0.01f) m_leanOffset = 0.0f;
}

void Player::takeDamage(int damage) {
    if (m_invulnerabilityTimer > 0) return;
    if (m_armor > 0) {
        int absorb = std::min(damage / 2, m_armor);
        m_armor -= absorb;
        damage -= absorb;
    }
    Entity::takeDamage(damage);
    m_invulnerabilityTimer = 1.0f;
}

void Player::heal(int amount) {
    Entity::heal(amount);
}

void Player::addAmmo(int amount) {
    m_ammo = std::min(m_maxAmmo, m_ammo + amount);
}

void Player::addArmor(int amount) {
    m_armor = std::min(m_maxArmor, m_armor + amount);
}

void Player::addScore(int points) {
    m_score += points;
}