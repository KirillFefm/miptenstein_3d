#pragma once
#include "../Utils/Constants.h"
#include <algorithm>
#include <cmath>

class Player {
public:
    float x = 2.5f, y = 2.5f;
    float angle = 0.0f;
    int health = PLAYER_MAX_HEALTH;
    int maxHealth = PLAYER_MAX_HEALTH;
    int ammo = 30;
    int maxAmmo = PLAYER_MAX_AMMO;
    int armor = 0;
    int maxArmor = PLAYER_MAX_ARMOR;
    int score = 0;
    float speed = PLAYER_SPEED;
    float invulnerabilityTimer = 0.0f;
    float weaponBob = 0.0f;
    float weaponRecoil = 0.0f;
    float leanAngle = 0.0f;
    float leanOffset = 0.0f;
    bool isAiming = false;
    float aimFOV = FOV * 0.6f;
    
    void update(float dt);
    void takeDamage(int damage);
    void heal(int amount);
    void addAmmo(int amount);
    void addArmor(int amount);
    bool isAlive() const { return health > 0; }
};