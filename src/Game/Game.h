#pragma once
#include <SFML/Graphics.hpp>
#include "../Player/Player.h"
#include "../Enemy/EnemyManager.h"
#include "../Pickup/Pickup.h"
#include "../Weapon/Weapon.h"
#include "../Renderer/Renderer.h"

class Game {
private:
    sf::RenderWindow m_window;
    Player m_player;
    EnemyManager m_enemyManager;
    PickupManager m_pickupManager;
    Weapon m_weapon;
    Renderer m_renderer;
    float m_gameTime;
    
    void handleInput(float dt);
    void update(float dt);
    void render();
    
public:
    Game();
    void run();
};