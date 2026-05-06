#include "Game.h"
#include "../Map/Map.h"
#include "../Core/MathUtils.h"

Game::Game() 
    : m_window(sf::VideoMode(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT), "Miptenstein 3D", sf::Style::Close)
    , m_gameTime(0.0f) {
    m_window.setFramerateLimit(60);
    m_window.setMouseCursorVisible(false);
    Map::initMap();
    m_enemyManager.init();
    m_pickupManager.init();
    m_weapon.init();
    m_renderer.init();
}

void Game::handleInput(float dt) {
    sf::Event e;
    while (m_window.pollEvent(e)) {
        if (e.type == sf::Event::Closed) m_window.close();
        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
            m_weapon.shoot(m_player);
        if (e.type == sf::Event::KeyPressed) {
            if (e.key.code == sf::Keyboard::Q) m_player.setLean(-0.3f, -0.5f);
            if (e.key.code == sf::Keyboard::E) m_player.setLean(0.3f, 0.5f);
            if (e.key.code == sf::Keyboard::Num1) m_player.switchWeapon(Config::WeaponType::PISTOL);
            if (e.key.code == sf::Keyboard::Num2) m_player.switchWeapon(Config::WeaponType::RIFLE);
            if (e.key.code == sf::Keyboard::Num3) m_player.switchWeapon(Config::WeaponType::SHOTGUN);
            if (e.key.code == sf::Keyboard::Escape) m_window.close();
            
        }
    }
    
    m_player.setAiming(sf::Mouse::isButtonPressed(sf::Mouse::Right));
    
    
    sf::Vector2i mp = sf::Mouse::getPosition(m_window);
    float sens = m_player.isAiming() ? 0.001f : 0.002f;
    m_player.setAngle(m_player.getAngle() + (mp.x - Config::SCREEN_WIDTH/2) * sens);
    sf::Mouse::setPosition(sf::Vector2i(Config::SCREEN_WIDTH/2, Config::SCREEN_HEIGHT/2), m_window);
    
    float spd = Config::PLAYER_SPEED * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) spd *= 1.8f;
    if (m_player.isAiming()) spd *= 0.5f;
    
    
    float nx = m_player.getX(), ny = m_player.getY(), mf = 0, ms = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) mf += 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) mf -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) ms -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) ms += 1;
    
    bool mv = (mf != 0 || ms != 0);
    if (mv) {
        m_player.setWeaponBob(std::min(1.0f, m_player.getWeaponBob() + dt * 3));
        float len = sqrtf(mf*mf + ms*ms); mf /= len; ms /= len;
        float ang = m_player.getAngle();
        nx += (cosf(ang)*mf + cosf(ang + 3.14159f/2)*ms) * spd;
        ny += (sinf(ang)*mf + sinf(ang + 3.14159f/2)*ms) * spd;
    } else {
        m_player.setWeaponBob(std::max(0.0f, m_player.getWeaponBob() - dt * 3));
    }
    
    if (Map::isWalkable(nx, m_player.getY(), Config::PLAYER_RADIUS, Map::getDestructibles()))
        m_player.setPosition(nx, m_player.getY());
    if (Map::isWalkable(m_player.getX(), ny, Config::PLAYER_RADIUS, Map::getDestructibles()))
        m_player.setPosition(m_player.getX(), ny);
}

void Game::update(float dt) {
    m_player.update(dt);
    m_weapon.update(dt, m_player, m_enemyManager);
    m_enemyManager.update(dt, m_player.getX(), m_player.getY(), m_player);
    m_pickupManager.update(dt, m_player.getX(), m_player.getY(), m_player);
}

void Game::render() {
    m_renderer.render(m_window, m_player, m_enemyManager, m_pickupManager, m_weapon, m_gameTime);
    m_window.display();
}

void Game::run() {
    sf::Clock clock;
    while (m_window.isOpen() && m_player.isAlive()) {
        float dt = clock.restart().asSeconds();
        dt = std::min(dt, 0.05f);
        m_gameTime += dt;
        handleInput(dt);
        update(dt);
        render();
    }
}