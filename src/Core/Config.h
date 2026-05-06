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
    
    
    static constexpr int MAX_ENEMIES = 12;
    static constexpr float ENEMY_RADIUS = 0.5f;
    static constexpr float ENEMY_SPEED = 2.0f;
    static constexpr int ENEMY_HEALTH = 2;
    static constexpr int ENEMY_DAMAGE = 15;
    static constexpr int ENEMY_BULLET_DAMAGE = 4;
    static constexpr float ENEMY_ATTACK_COOLDOWN = 1.5f;
    static constexpr float ENEMY_ATTACK_RANGE = 1.5f;
    static constexpr float ENEMY_VISIBILITY_RANGE = 10.0f;
    static constexpr float ENEMY_DEATH_ANIM_TIME = 1.0f;
    static constexpr float ENEMY_CORPSE_FADE_TIME = 5.0f;
    static constexpr float ENEMY_SHOOT_RANGE = 5.0f;
    static constexpr float ENEMY_SHOOT_COOLDOWN = 1.5f;
    static constexpr int MAX_ENEMY_BULLETS = 30;
    static constexpr float ENEMY_BULLET_SPEED = 15.0f;
    
    static constexpr int MAX_BARRELS = 10;
    static constexpr int MAX_CRATES = 10;
    static constexpr int BARREL_HEALTH = 4;
    static constexpr int CRATE_HEALTH = 4;
    static constexpr float BARREL_RADIUS = 0.6f;
    static constexpr float CRATE_RADIUS = 0.6f;
    
    enum class WeaponType : uint8_t { PISTOL = 0, RIFLE = 1, SHOTGUN = 2 };
    static constexpr int WEAPON_COUNT = 3;
    static constexpr int WEAPON_DAMAGE[3] = {1, 3, 2};
    static constexpr float BULLET_SPEED[3] = {30.0f, 40.0f, 20.0f};
    static constexpr float BULLET_RANGE[3] = {20.0f, 25.0f, 10.0f};
    static constexpr float WEAPON_SPREAD[3] = {0.05f, 0.03f, 0.15f};
    static constexpr float AIM_SPREAD[3] = {0.01f, 0.005f, 0.08f};
    static constexpr int WEAPON_MAX_AMMO[3] = {12, 30, 8};
    static constexpr float WEAPON_FIRE_RATE[3] = {0.3f, 0.1f, 0.6f};
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
    
    enum MapCell : int { EMPTY = 0, WALL = 1, BARREL = 2, CRATE = 3 };
};