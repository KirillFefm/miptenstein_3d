#include "Player.h"

void Player::update(float dt) {
    invulnerabilityTimer -= dt;
    if (invulnerabilityTimer < 0) invulnerabilityTimer = 0;
    weaponRecoil *= 0.8f;
    if (weaponRecoil < 0.01f) weaponRecoil = 0.0f;
    leanAngle *= 0.9f;
    leanOffset *= 0.9f;
    if (std::fabs(leanAngle) < 0.01f) leanAngle = 0.0f;
    if (std::fabs(leanOffset) < 0.01f) leanOffset = 0.0f;
}

void Player::takeDamage(int damage) {
    if (invulnerabilityTimer > 0) return;
    if (armor > 0) {
        int armorAbsorb = std::min(damage / 2, armor);
        armor -= armorAbsorb;
        damage -= armorAbsorb;
    }
    health -= damage;
    if (health < 0) health = 0;
    invulnerabilityTimer = 1.0f;
}

void Player::heal(int amount) { health = std::min(maxHealth, health + amount); }
void Player::addAmmo(int amount) { ammo = std::min(maxAmmo, ammo + amount); }
void Player::addArmor(int amount) { armor = std::min(maxArmor, armor + amount); }