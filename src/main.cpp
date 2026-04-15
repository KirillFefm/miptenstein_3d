#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Pickup/Pickup.h"
#include "Weapon/Weapon.h"
#include "Renderer/Renderer.h"
#include "Map/Map.h"
#include "Utils/MathUtils.h"

bool canMoveWithLean(float newX, float newY, float radius, float leanOffset, float angle) {
    float leanX = newX + cosf(angle + 3.14159f/2) * leanOffset;
    float leanY = newY + sinf(angle + 3.14159f/2) * leanOffset;
    return Map::isWalkable(leanX, leanY, radius);
}

int main() {
    std::cout << "Starting Wolfenstein 3D..." << std::endl;
    
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Wolfenstein 3D - Tactical Edition", sf::Style::Close);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);
    
    Map::initMap();
    
    Player player;
    EnemyManager enemyManager;
    PickupManager pickupManager;
    Weapon weapon;
    Renderer renderer;
    
    enemyManager.init();
    pickupManager.init();
    weapon.init();
    renderer.init();
    
    sf::Clock clock;
    float gameTime = 0.0f;
    
    while (window.isOpen() && player.isAlive()) {
        float dt = clock.restart().asSeconds();
        gameTime += dt;
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    weapon.shoot(player);
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Q) {
                    player.leanAngle = -0.3f;
                    player.leanOffset = -0.5f;
                }
                if (event.key.code == sf::Keyboard::E) {
                    player.leanAngle = 0.3f;
                    player.leanOffset = 0.5f;
                }
            }
        }
        
        player.isAiming = sf::Mouse::isButtonPressed(sf::Mouse::Right);
        
        // Управление мышью
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        float mouseSensitivity = player.isAiming ? 0.001f : 0.002f;
        float mouseDelta = (mousePos.x - SCREEN_WIDTH/2) * mouseSensitivity;
        player.angle += mouseDelta;
        sf::Mouse::setPosition(sf::Vector2i(SCREEN_WIDTH/2, SCREEN_HEIGHT/2), window);
        
        // Движение
        float moveSpeed = player.speed * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) moveSpeed *= 1.8f;
        if (player.isAiming) moveSpeed *= 0.5f;
        
        float newX = player.x, newY = player.y;
        float moveForward = 0.0f, moveStrafe = 0.0f;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) moveForward += 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) moveForward -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) moveStrafe -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) moveStrafe += 1.0f;
        
        bool isMoving = (moveForward != 0.0f || moveStrafe != 0.0f);
        
        if (isMoving) {
            player.weaponBob = std::min(1.0f, player.weaponBob + dt * 3.0f);
            
            float len = sqrtf(moveForward * moveForward + moveStrafe * moveStrafe);
            moveForward /= len;
            moveStrafe /= len;
            
            float moveAngle = player.angle;
            newX += (cosf(moveAngle) * moveForward + cosf(moveAngle + 3.14159f/2) * moveStrafe) * moveSpeed;
            newY += (sinf(moveAngle) * moveForward + sinf(moveAngle + 3.14159f/2) * moveStrafe) * moveSpeed;
        } else {
            player.weaponBob = std::max(0.0f, player.weaponBob - dt * 3.0f);
        }
        
        if (canMoveWithLean(newX, player.y, PLAYER_RADIUS, player.leanOffset, player.angle)) player.x = newX;
        if (canMoveWithLean(player.x, newY, PLAYER_RADIUS, player.leanOffset, player.angle)) player.y = newY;
        
        player.update(dt);
        weapon.update(dt, player, enemyManager);
        enemyManager.update(dt, player.x, player.y, player);
        pickupManager.update(dt, player.x, player.y, player);
        
        renderer.render(window, player, enemyManager, pickupManager, weapon, gameTime);
        
        window.display();
        
        if (enemyManager.areAllDead()) {
            std::cout << "Victory! Score: " << player.score << std::endl;
            sf::sleep(sf::seconds(3));
            break;
        }
    }
    
    if (!player.isAlive()) {
        std::cout << "Game Over! Score: " << player.score << std::endl;
        sf::sleep(sf::seconds(2));
    }
    
    return 0;
}