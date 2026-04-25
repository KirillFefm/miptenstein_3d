#include "Weapon.h"
#include "../Map/Map.h"
#include "../Utils/MathUtils.h"

void Weapon::createMuzzleFlash() {
    muzzleFlashTexture.create(64, 64, sf::Color::Transparent);
    for (int y = 0; y < 64; y++)
        for (int x = 0; x < 64; x++) {
            float cx = x - 32, cy = y - 32, dist = sqrtf(cx*cx + cy*cy);
            if (dist < 30) {
                float alpha = 1.0f - dist / 30.0f;
                muzzleFlashTexture.setPixel(x, y, sf::Color(255, 255, 200, (sf::Uint8)(255 * alpha)));
            }
        }
}

void Weapon::init() {
    createMuzzleFlash();
    weaponTexture.create(500, 400, sf::Color::Transparent);
    for (int y = 150; y < 250; y++)
        for (int x = 100; x < 400; x++) {
            float d = fabs(y - 200) / 50.0f;
            sf::Uint8 s = (sf::Uint8)(70 * (1 - d * 0.3f) + 30 * (x - 100) / 300.0f);
            weaponTexture.setPixel(x, y, sf::Color(s + 20, s, s - 20));
        }
    for (int y = 175; y < 185; y++)
        for (int x = 80; x < 95; x++)
            weaponTexture.setPixel(x, y, sf::Color(255, 50, 50));
}

void Weapon::shoot(Player& player) {
    if (player.ammo <= 0) return;
    player.ammo--;
    player.weaponRecoil = 1.0f;
    float spread = player.isAiming ? AIM_SPREAD : WEAPON_SPREAD;
    float bulletAngle = player.angle + player.leanAngle + (rand() % 100 - 50) / 500.0f * spread;
    float startX = player.x + cosf(player.angle + 3.14159f/2) * player.leanOffset;
    float startY = player.y + sinf(player.angle + 3.14159f/2) * player.leanOffset;
    bullets.push_back({startX, startY, cosf(bulletAngle), sinf(bulletAngle), 0.0f, startX, startY, true});
}

void Weapon::update(float dt, Player& player, EnemyManager& enemyManager) {
    for (auto& bullet : bullets) {
        if (!bullet.active) continue;
        float oldX = bullet.x, oldY = bullet.y;
        bullet.x += bullet.dirX * BULLET_SPEED * dt;
        bullet.y += bullet.dirY * BULLET_SPEED * dt;
        bullet.distance += BULLET_SPEED * dt;
        tracers.push_back({oldX, oldY, bullet.x, bullet.y, TRACER_FADE_TIME, true});
        int mapX = (int)bullet.x, mapY = (int)bullet.y;
        if (mapX >= 0 && mapX < MAP_WIDTH && mapY >= 0 && mapY < MAP_HEIGHT)
            if (Map::worldMap[mapX][mapY] == 1 || bullet.distance > BULLET_RANGE) { bullet.active = false; continue; }
        for (auto& enemy : enemyManager.getEnemies()) {
            if (enemy.state != EnemyState::ALIVE) continue;
            if (distance(bullet.x, bullet.y, enemy.x, enemy.y) < enemy.radius) {
                enemy.health -= WEAPON_DAMAGE;
                if (enemy.health <= 0) player.score += ENEMY_KILL_SCORE;
                bullet.active = false;
                break;
            }
        }
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) { return !b.active; }), bullets.end());
    for (auto& t : tracers) t.timer -= dt;
    tracers.erase(std::remove_if(tracers.begin(), tracers.end(), [](const Tracer& t) { return t.timer <= 0; }), tracers.end());
}

void Weapon::draw(sf::RenderWindow& window, const Player& player, float gameTime) {
    float bob = sinf(gameTime * 15) * 5 * player.weaponBob;
    float recoil = player.weaponRecoil * 30;
    sf::Sprite spr; sf::Texture tex; tex.loadFromImage(weaponTexture); spr.setTexture(tex);
    if (player.isAiming) {
        spr.setPosition(SCREEN_WIDTH/2 - 200 + bob, SCREEN_HEIGHT/2 - 100 + recoil);
        spr.setScale(1.4f, 1.4f);
    } else {
        spr.setPosition(SCREEN_WIDTH - 500 + bob, SCREEN_HEIGHT - 300 + recoil);
        spr.setScale(1.0f, 1.0f);
    }
    window.draw(spr);
    if (player.weaponRecoil > 0.7f) {
        sf::Sprite flash; sf::Texture ftex; ftex.loadFromImage(muzzleFlashTexture); flash.setTexture(ftex);
        flash.setPosition(SCREEN_WIDTH - 420 + bob, SCREEN_HEIGHT - 220 + recoil);
        flash.setScale(1.5f, 1.0f);
        window.draw(flash);
    }
}