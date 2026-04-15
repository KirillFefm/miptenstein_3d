#include "Weapon.h"
#include "../Map/Map.h"
#include "../Utils/MathUtils.h"

void Weapon::init() {
    // Создаем 3D текстуру оружия с мушкой
    weaponTexture.create(500, 400, sf::Color::Transparent);
    
    // Ствол
    for (int y = 150; y < 250; y++) {
        for (int x = 100; x < 400; x++) {
            float distFromCenter = fabs(y - 200) / 50.0f;
            sf::Uint8 shade = static_cast<sf::Uint8>(80 * (1.0f - distFromCenter * 0.3f));
            weaponTexture.setPixel(x, y, sf::Color(shade, shade, shade));
        }
    }
    
    // Мушка (яркая, для прицеливания)
    for (int y = 175; y < 185; y++) {
        for (int x = 80; x < 95; x++) {
            weaponTexture.setPixel(x, y, sf::Color::Red);
        }
    }
    
    // Целик
    for (int y = 195; y < 205; y++) {
        for (int x = 350; x < 370; x++) {
            weaponTexture.setPixel(x, y, sf::Color(100, 100, 100));
        }
    }
    
    // Рукоятка
    for (int y = 230; y < 350; y++) {
        for (int x = 150; x < 200; x++) {
            weaponTexture.setPixel(x, y, sf::Color(100, 70, 40));
        }
    }
    
    // Магазин
    for (int y = 250; y < 320; y++) {
        for (int x = 200; x < 230; x++) {
            weaponTexture.setPixel(x, y, sf::Color(50, 50, 50));
        }
    }
}

void Weapon::shoot(Player& player) {
    if (player.ammo <= 0) return;
    
    player.ammo--;
    player.weaponRecoil = 1.0f;
    
    float spread = player.isAiming ? AIM_SPREAD : WEAPON_SPREAD;
    float bulletAngle = player.angle + player.leanAngle + (rand() % 100 - 50) / 500.0f * spread;
    
    bullets.push_back({
        player.x + cosf(player.angle + 3.14159f/2) * player.leanOffset,
        player.y + sinf(player.angle + 3.14159f/2) * player.leanOffset,
        cosf(bulletAngle), sinf(bulletAngle),
        0.0f, true
    });
}

void Weapon::update(float dt, Player& player, EnemyManager& enemyManager) {
    for (auto& bullet : bullets) {
        if (!bullet.active) continue;
        
        bullet.x += bullet.dirX * BULLET_SPEED * dt;
        bullet.y += bullet.dirY * BULLET_SPEED * dt;
        bullet.distance += BULLET_SPEED * dt;
        
        int mapX = static_cast<int>(bullet.x);
        int mapY = static_cast<int>(bullet.y);
        if (mapX >= 0 && mapX < MAP_WIDTH && mapY >= 0 && mapY < MAP_HEIGHT) {
            if (Map::worldMap[mapX][mapY] == 1 || bullet.distance > BULLET_RANGE) {
                bullet.active = false;
                continue;
            }
        }
        
        for (auto& enemy : enemyManager.getEnemies()) {
            if (!enemy.alive) continue;
            float dist = distance(bullet.x, bullet.y, enemy.x, enemy.y);
            if (dist < enemy.radius) {
                enemy.health -= WEAPON_DAMAGE;
                if (enemy.health <= 0) {
                    enemy.alive = false;
                    player.score += ENEMY_KILL_SCORE;
                }
                bullet.active = false;
                break;
            }
        }
    }
    
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), 
        [](const Bullet& b) { return !b.active; }), bullets.end());
}

void Weapon::draw(sf::RenderWindow& window, const Player& player, float gameTime) {
    sf::Sprite weaponSprite;
    sf::Texture tex;
    tex.loadFromImage(weaponTexture);
    weaponSprite.setTexture(tex);
    
    float bobOffset = sinf(gameTime * 15.0f) * 5.0f * player.weaponBob;
    float recoilOffset = player.weaponRecoil * 30.0f;
    
    // Позиция оружия зависит от режима прицеливания
    if (player.isAiming) {
        // При прицеливании оружие поднимается к центру экрана
        weaponSprite.setPosition(SCREEN_WIDTH/2 - 200 + bobOffset, SCREEN_HEIGHT/2 - 100 + recoilOffset);
        weaponSprite.setScale(1.3f, 1.3f);
    } else {
        weaponSprite.setPosition(SCREEN_WIDTH - 500 + bobOffset, SCREEN_HEIGHT - 300 + recoilOffset);
        weaponSprite.setScale(1.0f, 1.0f);
    }
    
    window.draw(weaponSprite);
}