#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"

struct Tracer {
    float startX, startY, endX, endY;
    float timer = 0.0f;
    bool active = false;
};

struct Bullet {
    float x, y, dirX, dirY;
    float distance = 0.0f;
    float startX, startY;
    bool active = false;
};

class Weapon {
private:
    std::vector<Bullet> bullets;
    std::vector<Tracer> tracers;
    sf::Image weaponTexture;
    sf::Image muzzleFlashTexture;
    void createMuzzleFlash();
public:
    void init();
    void shoot(Player& player);
    void update(float dt, Player& player, EnemyManager& enemyManager);
    void draw(sf::RenderWindow& window, const Player& player, float gameTime);
    const std::vector<Tracer>& getTracers() const { return tracers; }
};