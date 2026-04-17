#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"

struct Bullet {
    float x, y;
    float dirX, dirY;
    float distance = 0.0f;
    bool active = false;
};

class Weapon {
private:
    std::vector<Bullet> bullets;
    sf::Image weaponTexture;
    
public:
    void init();
    void shoot(Player& player);
    void update(float dt, Player& player, EnemyManager& enemyManager);
    void draw(sf::RenderWindow& window, const Player& player, float gameTime);
    const std::vector<Bullet>& getBullets() const { return bullets; }
};