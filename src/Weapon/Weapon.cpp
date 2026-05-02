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

void Weapon::generateWeaponTexture(Config::WeaponType type) {
    int idx = (int)type;
    m_weaponTextures[idx].create(500, 400, sf::Color::Transparent);
    
    if (type == Config::WeaponType::PISTOL) {
        // Затвор
        for (int y = 140; y < 155; y++)
            for (int x = 120; x < 280; x++)
                m_weaponTextures[idx].setPixel(x, y, sf::Color(50, 50, 50));
        // Ствол
        for (int y = 145; y < 150; y++)
            for (int x = 80; x < 125; x++)
                m_weaponTextures[idx].setPixel(x, y, sf::Color(40, 40, 40));
        // Рукоятка
        for (int y = 150; y < 220; y++)
            for (int x = 180; x < 210; x++)
                m_weaponTextures[idx].setPixel(x, y, sf::Color(120, 80, 50));
        // Спусковая скоба
        for (int y = 155; y < 165; y++)
            for (int x = 150; x < 185; x++)
                if (y == 155 || y == 164 || x == 150 || x == 184)
                    m_weaponTextures[idx].setPixel(x, y, sf::Color(30, 30, 30));
        // Мушка
        for (int y = 141; y < 144; y++)
            for (int x = 78; x < 82; x++)
                m_weaponTextures[idx].setPixel(x, y, sf::Color(255, 60, 60));
        // Целик
        for (int y = 138; y < 142; y++)
            for (int x = 270; x < 285; x++)
                m_weaponTextures[idx].setPixel(x, y, sf::Color(40, 40, 40));
    }
    else if (type == Config::WeaponType::RIFLE) {
        // Приклад
        for (int y = 120; y < 180; y++)
            for (int x = 320; x < 380; x++) {
                int shade = 100 + (x - 320);
                m_weaponTextures[idx].setPixel(x, y, sf::Color(shade, 70, 40));
            }
        // Ствольная коробка
        for (int y = 135; y < 165; y++)
            for (int x = 150; x < 330; x++)
                m_weaponTextures[idx].setPixel(x, y, sf::Color(30, 30, 30));
        // Ствол
        for (int y = 145; y < 155; y++)
            for (int x = 60; x < 155; x++)
                m_weaponTextures[idx].setPixel(x, y, sf::Color(40, 40, 40));
        // Цевьё
        for (int y = 150; y < 170; y++)
            for (int x = 200; x < 320; x++)
                m_weaponTextures[idx].setPixel(x, y, sf::Color(100, 80, 50));
        // Рукоятка
        for (int y = 160; y < 230; y++)
            for (int x = 250; x < 280; x++)
                m_weaponTextures[idx].setPixel(x, y, sf::Color(110, 70, 40));
        // Магазин
        for (int y = 170; y < 220; y++)
            for (int x = 180; x < 210; x++)
                m_weaponTextures[idx].setPixel(x, y, sf::Color(25, 25, 25));
        // Мушка
        for (int y = 140; y < 144; y++)
            for (int x = 55; x < 62; x++)
                m_weaponTextures[idx].setPixel(x, y, sf::Color(255, 60, 60));
        // Прицел
        for (int y = 130; y < 140; y++)
            for (int x = 280; x < 300; x++)
                m_weaponTextures[idx].setPixel(x, y, sf::Color(50, 50, 50));
    }
    else if (type == Config::WeaponType::SHOTGUN) {
        // Приклад
        for (int y = 110; y < 160; y++)
            for (int x = 340; x < 400; x++) {
                int shade = 120 + (x - 340) / 2;
                m_weaponTextures[idx].setPixel(x, y, sf::Color(shade, 60, 30));
            }
        // Ствольная коробка
        for (int y = 130; y < 160; y++)
            for (int x = 200; x < 350; x++)
                m_weaponTextures[idx].setPixel(x, y, sf::Color(50, 40, 40));
        // Два ствола
        for (int barrel = 0; barrel < 2; barrel++) {
            int by = 138 + barrel * 8;
            for (int y = by; y < by + 4; y++)
                for (int x = 80; x < 210; x++)
                    m_weaponTextures[idx].setPixel(x, y, sf::Color(60, 50, 50));
        }
        // Рукоятка
        for (int y = 155; y < 240; y++)
            for (int x = 260; x < 300; x++)
                m_weaponTextures[idx].setPixel(x, y, sf::Color(130, 90, 50));
        // Мушка
        for (int y = 134; y < 138; y++)
            for (int x = 75; x < 82; x++)
                m_weaponTextures[idx].setPixel(x, y, sf::Color(255, 60, 60));
    }
}

void Weapon::init() {
    createMuzzleFlash();
    for (int i = 0; i < Config::WEAPON_COUNT; i++)
        generateWeaponTexture((Config::WeaponType)i);
}

void Weapon::shoot(Player& player) {
    if (!player.canShoot()) return;
    player.setAmmo(player.getAmmo() - 1);
    player.setWeaponRecoil(1.0f);
    player.setFireTimer(player.getFireRate());
    float spread = player.getBulletSpread();
    float bulletAngle = player.getAngle() + player.getLeanAngle() + (rand() % 100 - 50) / 500.0f * spread;
    float startX = player.getX() + cosf(player.getAngle() + 3.14159f/2) * player.getLeanOffset();
    float startY = player.getY() + sinf(player.getAngle() + 3.14159f/2) * player.getLeanOffset();
    m_bullets.push_back({startX, startY, cosf(bulletAngle), sinf(bulletAngle), 0.0f, startX, startY, player.getWeaponDamage(), true});
}

void Weapon::update(float dt, Player& player, EnemyManager& enemyManager) {
    for (auto& bullet : m_bullets) {
        if (!bullet.active) continue;
        float oldX = bullet.x, oldY = bullet.y;
        float speed = player.getBulletSpeed();
        bullet.x += bullet.dirX * speed * dt;
        bullet.y += bullet.dirY * speed * dt;
        bullet.distance += speed * dt;
        m_tracers.push_back({oldX, oldY, bullet.x, bullet.y, Config::TRACER_FADE_TIME, true});
        int mapX = (int)bullet.x, mapY = (int)bullet.y;
        if (mapX >= 0 && mapX < Config::MAP_WIDTH && mapY >= 0 && mapY < Config::MAP_HEIGHT)
            if (Map::getWorldMap()[mapX][mapY] == 1 || bullet.distance > player.getBulletRange()) { bullet.active = false; continue; }
        for (auto& enemy : enemyManager.getEnemies()) {
            if (enemy.getState() != Config::EnemyState::ALIVE) continue;
            if (MathUtils::distance(bullet.x, bullet.y, enemy.getX(), enemy.getY()) < enemy.getRadius()) {
                enemy.setHealth(enemy.getHealth() - bullet.damage);
                if (enemy.getHealth() <= 0) { enemy.setState(Config::EnemyState::DYING); enemy.setDeathTimer(Config::ENEMY_DEATH_ANIM_TIME); player.addScore(Config::ENEMY_KILL_SCORE); }
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
    sf::Sprite spr; sf::Texture tex; tex.loadFromImage(m_weaponTextures[(int)player.getCurrentWeapon()]); spr.setTexture(tex);
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