#pragma once
#include <cstdint>

struct Config {
    static constexpr int SCREEN_WIDTH = 1280;
    static constexpr int SCREEN_HEIGHT = 720;
    static constexpr int MAP_WIDTH = 32;
    static constexpr int MAP_HEIGHT = 32;
    static constexpr int TEXTURE_SIZE = 64;
    static constexpr float FOV = 3.14159f / 3.0f;
    static constexpr float DEPTH = 20.0f;
    
    static constexpr float PLAYER_SPEED = 4.0f;
    static constexpr float PLAYER_RADIUS = 0.3f;
    static constexpr int PLAYER_MAX_HEALTH = 100;
    static constexpr int PLAYER_MAX_AMMO = 200;
    static constexpr int PLAYER_MAX_ARMOR = 100;
    
    static constexpr int MAX_ENEMIES = 15;
    static constexpr float ENEMY_RADIUS = 0.5f;
    static constexpr float ENEMY_SPEED = 2.0f;
    static constexpr int ENEMY_HEALTH = 2;
    static constexpr int ENEMY_DAMAGE = 15;
    static constexpr float ENEMY_ATTACK_COOLDOWN = 1.5f;
    static constexpr float ENEMY_ATTACK_RANGE = 1.5f;
    static constexpr float ENEMY_VISIBILITY_RANGE = 10.0f;
    static constexpr float ENEMY_DEATH_ANIM_TIME = 1.0f;
    static constexpr float ENEMY_CORPSE_FADE_TIME = 5.0f;
    
    static constexpr int WEAPON_DAMAGE = 1;
    static constexpr float BULLET_SPEED = 30.0f;
    static constexpr float BULLET_RANGE = 20.0f;
    static constexpr float WEAPON_SPREAD = 0.05f;
    static constexpr float AIM_SPREAD = 0.01f;
    static constexpr float TRACER_FADE_TIME = 0.15f;
    
    static constexpr int MAX_PICKUPS = 25;
    static constexpr float PICKUP_RADIUS = 0.2f;
    static constexpr float PICKUP_COLLECT_DIST = 1.2f;
    static constexpr int HEALTH_PICKUP_VALUE = 30;
    static constexpr int AMMO_PICKUP_VALUE = 25;
    static constexpr int ARMOR_PICKUP_VALUE = 25;
    static constexpr int PICKUP_SCORE = 50;
    static constexpr int ENEMY_KILL_SCORE = 100;
    
    enum PickupType : uint8_t { HEALTH = 0, AMMO = 1, ARMOR = 2 };
    enum class EnemyState : uint8_t { ALIVE = 0, DYING = 1, CORPSE = 2, DEAD = 3 };
};