#pragma once

// Настройки экрана
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int MAP_WIDTH = 32;
const int MAP_HEIGHT = 32;
const int TEXTURE_SIZE = 64;
const float FOV = 3.14159f / 3.0f;
const float DEPTH = 20.0f;
const int MAX_ENEMIES = 15;
const int MAX_PICKUPS = 25;

// Настройки игрока
const float PLAYER_SPEED = 4.0f;
const float PLAYER_RADIUS = 0.3f;
const int PLAYER_MAX_HEALTH = 100;
const int PLAYER_MAX_AMMO = 200;
const int PLAYER_MAX_ARMOR = 100;

// Настройки врагов
const float ENEMY_RADIUS = 0.5f;
const float ENEMY_SPEED = 2.0f;
const int ENEMY_HEALTH = 2;
const int ENEMY_DAMAGE = 15;
const float ENEMY_ATTACK_COOLDOWN = 1.5f;
const float ENEMY_ATTACK_RANGE = 1.5f;
const float ENEMY_VISIBILITY_RANGE = 10.0f;

// Настройки оружия
const int WEAPON_DAMAGE = 1;
const float BULLET_SPEED = 30.0f;
const float BULLET_RANGE = 20.0f;
const float WEAPON_SPREAD = 0.05f;
const float AIM_SPREAD = 0.01f;

// Настройки предметов
const float PICKUP_RADIUS = 0.2f;
const float PICKUP_COLLECT_DIST = 1.2f;
const int HEALTH_PICKUP_VALUE = 30;
const int AMMO_PICKUP_VALUE = 25;
const int ARMOR_PICKUP_VALUE = 25;
const int PICKUP_SCORE = 50;
const int ENEMY_KILL_SCORE = 100;