#include "Weapon.h"
#include "../Enemy/EnemyManager.h"
#include "../Map/Map.h"
#include "../Core/MathUtils.h"

void Weapon::createMuzzleFlash() {
    m_muzzleFlashTexture.create(64, 64, sf::Color::Transparent);
    for (int y = 0; y < 64; y++)
        for (int x = 0; x < 64; x++) {
            float cx = x - 32, cy = y - 32, dist = sqrtf(cx*cx + cy*cy);
            if (dist < 30) {
                float alpha = 1.0f - dist / 30.0f;
                m_muzzleFlashTexture.setPixel(x, y, sf::Color(255, 255, 200, (sf::Uint8)(255 * alpha)));
            }
        }
}

void Weapon::init() {
    createMuzzleFlash();
    m_weaponTexture.create(500, 400, sf::Color::Transparent);
    for (int y = 150; y < 250; y++)
        for (int x = 100; x < 400; x++) {
            float d = fabs(y - 200) / 50.0f;
            sf::Uint8 s = (sf::Uint8)(70 * (1 - d * 0.3f) + 30 * (x - 100) / 300.0f);
            m_weaponTexture.setPixel(x, y, sf::Color(s + 20, s, s - 20));
        }
    for (int y = 175; y < 185; y++)
        for (int x = 80; x < 95; x++)
            m_weaponTexture.setPixel(x, y, sf::Color(255, 50, 50));
}

void Weapon::shoot(Player& player) {
    if (player.getAmmo() <= 0) return;
    player.setAmmo(player.getAmmo() - 1);
    player.setWeaponRecoil(1.0f);
    float spread = player.isAiming() ? Config::AIM_SPREAD : Config::WEAPON_SPREAD;
    float bulletAngle = player.getAngle() + player.getLeanAngle() + (rand() % 100 - 50) / 500.0f * spread;
    float startX = player.getX() + cosf(player.getAngle() + 3.14159f/2) * player.getLeanOffset();
    float startY = player.getY() + sinf(player.getAngle() + 3.14159f/2) * player.getLeanOffset();
    m_bullets.push_back({startX, startY, cosf(bulletAngle), sinf(bulletAngle), 0.0f, startX, startY, true});
}

void Weapon::update(float dt, Player& player, EnemyManager& enemyManager) {
    for (auto& bullet : m_bullets) {
        if (!bullet.active) continue;
        float oldX = bullet.x, oldY = bullet.y;
        bullet.x += bullet.dirX * Config::BULLET_SPEED * dt;
        bullet.y += bullet.dirY * Config::BULLET_SPEED * dt;
        bullet.distance += Config::BULLET_SPEED * dt;
        m_tracers.push_back({oldX, oldY, bullet.x, bullet.y, Config::TRACER_FADE_TIME, true});
        int mapX = (int)bullet.x, mapY = (int)bullet.y;
        if (mapX >= 0 && mapX < Config::MAP_WIDTH && mapY >= 0 && mapY < Config::MAP_HEIGHT)
            if (Map::getWorldMap()[mapX][mapY] == 1 || bullet.distance > Config::BULLET_RANGE) { bullet.active = false; continue; }
        for (auto& enemy : enemyManager.getEnemies()) {
            if (enemy.getState() != Config::EnemyState::ALIVE) continue;
            if (MathUtils::distance(bullet.x, bullet.y, enemy.getX(), enemy.getY()) < enemy.getRadius()) {
                enemy.takeDamage(Config::WEAPON_DAMAGE);
                if (!enemy.isAlive()) player.addScore(Config::ENEMY_KILL_SCORE);
                bullet.active = false;
                break;
            }
        }
    }
    m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(), [](const Bullet& b) { return !b.active; }), m_bullets.end());
    for (auto& t : m_tracers) t.timer -= dt;
    m_tracers.erase(std::remove_if(m_tracers.begin(), m_tracers.end(), [](const Tracer& t) { return t.timer <= 0; }), m_tracers.end());
}

void Weapon::draw(sf::RenderWindow& window, const Player& player, float gameTime) {
    float bob = sinf(gameTime * 15) * 5 * player.getWeaponBob();
    float recoil = player.getWeaponRecoil() * 30;
    sf::Sprite spr; sf::Texture tex; tex.loadFromImage(m_weaponTexture); spr.setTexture(tex);
    if (player.isAiming()) {
        spr.setPosition(Config::SCREEN_WIDTH/2 - 200 + bob, Config::SCREEN_HEIGHT/2 - 100 + recoil);
        spr.setScale(1.4f, 1.4f);
    } else {
        spr.setPosition(Config::SCREEN_WIDTH - 500 + bob, Config::SCREEN_HEIGHT - 300 + recoil);
        spr.setScale(1.0f, 1.0f);
    }
    window.draw(spr);
    if (player.getWeaponRecoil() > 0.7f) {
        sf::Sprite flash; sf::Texture ftex; ftex.loadFromImage(m_muzzleFlashTexture); flash.setTexture(ftex);
        flash.setPosition(Config::SCREEN_WIDTH - 420 + bob, Config::SCREEN_HEIGHT - 220 + recoil);
        flash.setScale(1.5f, 1.0f);
        window.draw(flash);
    }
}