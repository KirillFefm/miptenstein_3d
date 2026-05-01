#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../Player/Player.h"

class EnemyManager;

struct Tracer {
    float startX, startY, endX, endY;
    float timer = 0.0f;
    bool active = false;
};

struct Bullet {
    float x, y, dirX, dirY;
    float distance = 0.0f;
    float startX, startY;
    int damage;
    bool active = false;
};

class Weapon {
private:
    std::vector<Bullet> m_bullets;
    std::vector<Tracer> m_tracers;
    sf::Image m_weaponTextures[Config::WEAPON_COUNT];
    sf::Image m_muzzleFlashTexture;
    void createMuzzleFlash();
    void generateWeaponTexture(Config::WeaponType type);
public:
    void init();
    void shoot(Player& player);
    void update(float dt, Player& player, EnemyManager& enemyManager);
    void draw(sf::RenderWindow& window, const Player& player, float gameTime);
    const std::vector<Tracer>& getTracers() const { return m_tracers; }
};