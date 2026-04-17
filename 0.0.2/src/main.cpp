#include <SFML/Graphics.hpp>
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Pickup/Pickup.h"
#include "Weapon/Weapon.h"
#include "Renderer/Renderer.h"
#include "Map/Map.h"
#include "Utils/MathUtils.h"

bool canMoveWithLean(float nx, float ny, float r, float off, float ang) {
    float lx = nx + cosf(ang + M_PI/2) * off, ly = ny + sinf(ang + M_PI/2) * off;
    return Map::isWalkable(lx, ly, r);
}

int main() {
    sf::RenderWindow w(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Wolfenstein 3D", sf::Style::Close);
    w.setFramerateLimit(60); w.setMouseCursorVisible(false);
    Map::initMap();
    Player p; EnemyManager em; PickupManager pm; Weapon wp; Renderer r;
    em.init(); pm.init(); wp.init(); r.init();
    sf::Clock c; float gt = 0;
    while (w.isOpen() && p.isAlive()) {
        float dt = c.restart().asSeconds(); gt += dt;
        sf::Event e; while (w.pollEvent(e)) { if (e.type == sf::Event::Closed) w.close(); if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) wp.shoot(p); if (e.type == sf::Event::KeyPressed) { if (e.key.code == sf::Keyboard::Q) { p.leanAngle = -0.3f; p.leanOffset = -0.5f; } if (e.key.code == sf::Keyboard::E) { p.leanAngle = 0.3f; p.leanOffset = 0.5f; } } }
        p.isAiming = sf::Mouse::isButtonPressed(sf::Mouse::Right);
        sf::Vector2i mp = sf::Mouse::getPosition(w);
        p.angle += (mp.x - SCREEN_WIDTH/2) * (p.isAiming ? 0.001f : 0.002f);
        sf::Mouse::setPosition(sf::Vector2i(SCREEN_WIDTH/2, SCREEN_HEIGHT/2), w);
        float spd = p.speed * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) spd *= 1.8f;
        if (p.isAiming) spd *= 0.5f;
        float nx = p.x, ny = p.y, mf = 0, ms = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) mf += 1; if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) mf -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) ms -= 1; if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) ms += 1;
        bool mv = (mf != 0 || ms != 0);
        if (mv) {
            p.weaponBob = std::min(1.0f, p.weaponBob + dt*3);
            float len = sqrtf(mf*mf + ms*ms); mf /= len; ms /= len;
            nx += (cosf(p.angle)*mf + cosf(p.angle + M_PI/2)*ms) * spd;
            ny += (sinf(p.angle)*mf + sinf(p.angle + M_PI/2)*ms) * spd;
        } else p.weaponBob = std::max(0.0f, p.weaponBob - dt*3);
        if (canMoveWithLean(nx, p.y, PLAYER_RADIUS, p.leanOffset, p.angle)) p.x = nx;
        if (canMoveWithLean(p.x, ny, PLAYER_RADIUS, p.leanOffset, p.angle)) p.y = ny;
        p.update(dt); wp.update(dt, p, em); em.update(dt, p.x, p.y, p); pm.update(dt, p.x, p.y, p);
        r.render(w, p, em, pm, wp, gt);
        w.display();
        if (em.areAllDead()) { sf::sleep(sf::seconds(3)); break; }
    }
    return 0;
}