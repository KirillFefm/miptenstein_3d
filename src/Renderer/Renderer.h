#pragma once
#include <SFML/Graphics.hpp>
#include "../Player/Player.h"
#include "../Enemy/EnemyManager.h"
#include "../Pickup/Pickup.h"
#include "../Weapon/Weapon.h"
#include "../Map/Map.h"
#include <vector>

class Renderer {
private:
    sf::Image m_wallTextures[4], m_enemyTexture;
    sf::Font m_font;
    float m_depthBuffer[Config::SCREEN_WIDTH];
    
    void generateTextures();
    void drawWalls(sf::RenderWindow& w, const Player& p, float px, float py, float ra, float fov);
    void drawEnemy(sf::RenderWindow& w, const Enemy& e, float projX, float size, float gt);
    void drawSprites(sf::RenderWindow& w, const Player& p, float px, float py, float ra, float fov, const EnemyManager& em, const PickupManager& pm, float gt);
    void drawTracers(sf::RenderWindow& w, const Weapon& wp, float px, float py, float ra, float fov);
    void drawHUD(sf::RenderWindow& w, const Player& p);
    void drawCrosshair(sf::RenderWindow& w, const Player& p);
    void drawMinimap(sf::RenderWindow& w, const Player& p);
    
public:
    Renderer();
    void init();
    void render(sf::RenderWindow& w, const Player& p, const EnemyManager& em, const PickupManager& pm, Weapon& wp, float gt);
};