#pragma once
#include <SFML/Graphics.hpp>
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "../Pickup/Pickup.h"
#include "../Weapon/Weapon.h"
#include "../Map/Map.h"

class Renderer {
private:
    sf::Image wallTextures[4];
    sf::Image enemyTexture;
    sf::Font font;
    float depthBuffer[SCREEN_WIDTH];
    
    void generateTextures();
    void drawWalls(sf::RenderWindow& window, const Player& player, float playerRenderX, float playerRenderY, float renderAngle, float currentFOV);
    void drawSprites(sf::RenderWindow& window, const Player& player, float playerRenderX, float playerRenderY, float renderAngle, float currentFOV,
                     const EnemyManager& enemyManager, const PickupManager& pickupManager, float gameTime);
    void drawHUD(sf::RenderWindow& window, const Player& player);
    void drawCrosshair(sf::RenderWindow& window, const Player& player);
    void drawMinimap(sf::RenderWindow& window, const Player& player);
    
public:
    Renderer();
    void init();
    void render(sf::RenderWindow& window, const Player& player, const EnemyManager& enemyManager,
                const PickupManager& pickupManager, Weapon& weapon, float gameTime);
};