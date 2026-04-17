#include "Renderer.h"
#include "../Utils/MathUtils.h"
#include <sstream>
#include <algorithm>

Renderer::Renderer() {
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        depthBuffer[i] = DEPTH;
    }
}

void Renderer::generateTextures() {
    // Генерация текстур стен
    for (int t = 0; t < 4; t++) {
        wallTextures[t].create(TEXTURE_SIZE, TEXTURE_SIZE);
        for (int x = 0; x < TEXTURE_SIZE; x++) {
            for (int y = 0; y < TEXTURE_SIZE; y++) {
                int shade = 100 + t * 40;
                if ((x/8 + y/8) % 2 == 0) {
                    wallTextures[t].setPixel(x, y, sf::Color(shade, shade-20, shade-40));
                } else {
                    wallTextures[t].setPixel(x, y, sf::Color(shade-30, shade-50, shade-70));
                }
            }
        }
    }
    
    // Генерация 3D текстуры врага
    enemyTexture.create(128, 128, sf::Color::Transparent);
    for (int y = 0; y < 128; y++) {
        for (int x = 0; x < 128; x++) {
            float cx = x - 64;
            float cy = y - 64;
            float dist = sqrtf(cx*cx + cy*cy);
            
            // Тело
            if (dist < 40 && y > 40) {
                float shade = 1.0f - (dist / 40) * 0.5f;
                sf::Uint8 r = static_cast<sf::Uint8>(180 * shade);
                sf::Uint8 g = static_cast<sf::Uint8>(20 * shade);
                sf::Uint8 b = static_cast<sf::Uint8>(20 * shade);
                enemyTexture.setPixel(x, y, sf::Color(r, g, b));
            }
            // Голова
            if (dist < 25 && y < 60) {
                float shade = 1.0f - (dist / 25) * 0.3f;
                sf::Uint8 r = static_cast<sf::Uint8>(255 * shade);
                sf::Uint8 g = static_cast<sf::Uint8>(200 * shade);
                sf::Uint8 b = static_cast<sf::Uint8>(150 * shade);
                enemyTexture.setPixel(x, y, sf::Color(r, g, b));
            }
            // Глаза
            if (y > 30 && y < 45) {
                if ((cx > 10 && cx < 20) || (cx < -10 && cx > -20)) {
                    if (cy > 5 && cy < 15) {
                        enemyTexture.setPixel(x, y, sf::Color::Red);
                    }
                }
            }
            // Ноги
            if (y > 90) {
                if (cx > -20 && cx < 0) {
                    enemyTexture.setPixel(x, y, sf::Color(100, 50, 0));
                }
                if (cx > 0 && cx < 20) {
                    enemyTexture.setPixel(x, y, sf::Color(100, 50, 0));
                }
            }
        }
    }
}

void Renderer::init() {
    generateTextures();
    
    if (!font.loadFromFile("arial.ttf")) {
        #ifdef _WIN32
            font.loadFromFile("C:/Windows/Fonts/arial.ttf");
        #endif
    }
}

void Renderer::drawWalls(sf::RenderWindow& window, const Player& player, float playerRenderX, float playerRenderY, float renderAngle, float currentFOV) {
    sf::VertexArray walls(sf::Quads);
    
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float rayAngle = (renderAngle - currentFOV/2.0f) + (static_cast<float>(x) / SCREEN_WIDTH) * currentFOV;
        float rayDirX = cosf(rayAngle);
        float rayDirY = sinf(rayAngle);
        
        float distToWall = 0.0f;
        bool hit = false;
        int textureX = 0;
        int mapX = 0, mapY = 0;
        
        while (!hit && distToWall < DEPTH) {
            distToWall += 0.05f;
            float rayX = playerRenderX + rayDirX * distToWall;
            float rayY = playerRenderY + rayDirY * distToWall;
            
            mapX = static_cast<int>(rayX);
            mapY = static_cast<int>(rayY);
            if (mapX >= 0 && mapX < MAP_WIDTH && mapY >= 0 && mapY < MAP_HEIGHT) {
                if (Map::worldMap[mapX][mapY] == 1) {
                    hit = true;
                    
                    // Определение текстурной координаты
                    float wallX = rayX - mapX;
                    float wallY = rayY - mapY;
                    if (fabs(wallX - 0.0f) < 0.05f || fabs(wallX - 1.0f) < 0.05f) {
                        textureX = static_cast<int>(wallY * TEXTURE_SIZE);
                    } else {
                        textureX = static_cast<int>(wallX * TEXTURE_SIZE);
                    }
                    textureX = std::min(TEXTURE_SIZE - 1, std::max(0, textureX));
                }
            }
        }
        
        if (hit) {
            float correctedDist = distToWall * cosf(renderAngle - rayAngle);
            depthBuffer[x] = correctedDist;
            
            int wallHeight = static_cast<int>(SCREEN_HEIGHT / correctedDist);
            int wallTop = (SCREEN_HEIGHT - wallHeight) / 2;
            int wallBottom = wallTop + wallHeight;
            
            int texIndex = (mapX + mapY) % 4;
            
            // Отрисовка текстурированной стены
            for (int y = wallTop; y < wallBottom; y++) {
                int textureY = static_cast<int>((y - wallTop) / static_cast<float>(wallHeight) * TEXTURE_SIZE);
                textureY = std::min(TEXTURE_SIZE - 1, std::max(0, textureY));
                
                sf::Color texColor = wallTextures[texIndex].getPixel(textureX, textureY);
                
                float shade = 1.0f - (correctedDist / DEPTH) * 0.5f;
                texColor.r = static_cast<sf::Uint8>(texColor.r * shade);
                texColor.g = static_cast<sf::Uint8>(texColor.g * shade);
                texColor.b = static_cast<sf::Uint8>(texColor.b * shade);
                
                walls.append(sf::Vertex(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)), texColor));
                walls.append(sf::Vertex(sf::Vector2f(static_cast<float>(x+1), static_cast<float>(y)), texColor));
                walls.append(sf::Vertex(sf::Vector2f(static_cast<float>(x+1), static_cast<float>(y+1)), texColor));
                walls.append(sf::Vertex(sf::Vector2f(static_cast<float>(x), static_cast<float>(y+1)), texColor));
            }
        }
    }
    
    window.draw(walls);
}

void Renderer::drawSprites(sf::RenderWindow& window, const Player& player, float playerRenderX, float playerRenderY, float renderAngle, float currentFOV,
                           const EnemyManager& enemyManager, const PickupManager& pickupManager, float gameTime) {
    struct SpriteData {
        float distance;
        float x, y;
        int type; // 0 - враг, 1 - аптечка, 2 - патроны, 3 - броня
        const Enemy* enemy;
        const Pickup* pickup;
        float size;
    };
    
    std::vector<SpriteData> visibleSprites;
    
    // Сбор врагов
    for (const auto& enemy : enemyManager.getEnemies()) {
        if (!enemy.alive) continue;
        
        float dx = enemy.x - playerRenderX;
        float dy = enemy.y - playerRenderY;
        float dist = sqrtf(dx*dx + dy*dy);
        
        if (dist < DEPTH && isVisible(playerRenderX, playerRenderY, enemy.x, enemy.y, Map::worldMap)) {
            visibleSprites.push_back({dist, enemy.x, enemy.y, 0, &enemy, nullptr, 0.9f});
        }
    }
    
    // Сбор предметов
    for (const auto& pickup : pickupManager.getPickups()) {
        if (!pickup.active) continue;
        
        float dx = pickup.x - playerRenderX;
        float dy = pickup.y - playerRenderY;
        float dist = sqrtf(dx*dx + dy*dy);
        
        if (dist < DEPTH && isVisible(playerRenderX, playerRenderY, pickup.x, pickup.y, Map::worldMap)) {
            int type = 1 + pickup.type;
            visibleSprites.push_back({dist, pickup.x, pickup.y, type, nullptr, &pickup, 0.25f});
        }
    }
    
    // Сортировка по расстоянию (дальние рисуются первыми)
    std::sort(visibleSprites.begin(), visibleSprites.end(),
        [](const SpriteData& a, const SpriteData& b) { return a.distance > b.distance; });
    
    // Отрисовка спрайтов
    for (const auto& sprite : visibleSprites) {
        float dx = sprite.x - playerRenderX;
        float dy = sprite.y - playerRenderY;
        float angleToSprite = atan2f(dy, dx);
        float angleDiff = angleToSprite - renderAngle;
        
        while (angleDiff < -3.14159f) angleDiff += 2*3.14159f;
        while (angleDiff > 3.14159f) angleDiff -= 2*3.14159f;
        
        if (fabs(angleDiff) < currentFOV/1.5f) {
            float projX = (angleDiff / currentFOV) * SCREEN_WIDTH + SCREEN_WIDTH/2.0f;
            float size = SCREEN_HEIGHT / sprite.distance * sprite.size;
            
            int startX = std::max(0, static_cast<int>(projX - size/2));
            int endX = std::min(SCREEN_WIDTH - 1, static_cast<int>(projX + size/2));
            
            bool occluded = false;
            for (int x = startX; x <= endX && !occluded; x++) {
                if (depthBuffer[x] < sprite.distance) {
                    occluded = true;
                }
            }
            
            if (!occluded) {
                if (sprite.type == 0) { // Враг
                    sf::Sprite enemySprite;
                    sf::Texture tex;
                    tex.loadFromImage(enemyTexture);
                    enemySprite.setTexture(tex);
                    
                    enemySprite.setPosition(projX - size/2, SCREEN_HEIGHT/2 - size/2);
                    enemySprite.setScale(size / 128.0f, size / 128.0f);
                    
                    window.draw(enemySprite);
                    
                    // Индикатор здоровья врага
                    if (sprite.enemy->health < ENEMY_HEALTH) {
                        sf::RectangleShape healthBar(sf::Vector2f(size, 5));
                        healthBar.setPosition(projX - size/2, SCREEN_HEIGHT/2 - size/2 - 10);
                        healthBar.setFillColor(sf::Color::Red);
                        window.draw(healthBar);
                    }
                } else if (sprite.type == 1) { // Аптечка
                    float bobOffset = sinf(sprite.pickup->bobTimer * 3.0f) * 5.0f;
                    
                    // Коробка аптечки
                    sf::RectangleShape medkit(sf::Vector2f(size, size * 0.6f));
                    medkit.setPosition(projX - size/2, SCREEN_HEIGHT/2 + bobOffset);
                    medkit.setFillColor(sf::Color::White);
                    medkit.setOutlineColor(sf::Color::Red);
                    medkit.setOutlineThickness(2);
                    window.draw(medkit);
                    
                    // Красный крест
                    sf::RectangleShape crossH(sf::Vector2f(size * 0.6f, size * 0.1f));
                    crossH.setPosition(projX - size * 0.3f, SCREEN_HEIGHT/2 + size * 0.25f + bobOffset);
                    crossH.setFillColor(sf::Color::Red);
                    window.draw(crossH);
                    
                    sf::RectangleShape crossV(sf::Vector2f(size * 0.1f, size * 0.6f));
                    crossV.setPosition(projX - size * 0.05f, SCREEN_HEIGHT/2 + bobOffset);
                    crossV.setFillColor(sf::Color::Red);
                    window.draw(crossV);
                    
                } else if (sprite.type == 2) { // Патроны
                    float bobOffset = sinf(sprite.pickup->bobTimer * 3.0f) * 5.0f;
                    
                    // Коробка патронов
                    sf::RectangleShape ammoBox(sf::Vector2f(size * 1.2f, size * 0.5f));
                    ammoBox.setPosition(projX - size * 0.6f, SCREEN_HEIGHT/2 + bobOffset);
                    ammoBox.setFillColor(sf::Color(139, 90, 43));
                    ammoBox.setOutlineColor(sf::Color(100, 60, 20));
                    ammoBox.setOutlineThickness(1);
                    window.draw(ammoBox);
                    
                    // Патроны внутри
                    for (int i = 0; i < 3; i++) {
                        sf::RectangleShape bullet(sf::Vector2f(size * 0.2f, size * 0.06f));
                        bullet.setPosition(projX - size * 0.3f + i * size * 0.25f, SCREEN_HEIGHT/2 + size * 0.2f + bobOffset);
                        bullet.setFillColor(sf::Color(255, 215, 0));
                        window.draw(bullet);
                    }
                    
                } else if (sprite.type == 3) { // Броня
                    float bobOffset = sinf(sprite.pickup->bobTimer * 3.0f) * 5.0f;
                    
                    // Бронепластина
                    sf::ConvexShape armor;
                    armor.setPointCount(3);
                    armor.setPoint(0, sf::Vector2f(projX, SCREEN_HEIGHT/2 - size * 0.3f + bobOffset));
                    armor.setPoint(1, sf::Vector2f(projX + size * 0.5f, SCREEN_HEIGHT/2 + size * 0.3f + bobOffset));
                    armor.setPoint(2, sf::Vector2f(projX - size * 0.5f, SCREEN_HEIGHT/2 + size * 0.3f + bobOffset));
                    armor.setFillColor(sf::Color(50, 50, 150, 200));
                    armor.setOutlineColor(sf::Color(100, 100, 255));
                    armor.setOutlineThickness(2);
                    window.draw(armor);
                }
            }
        }
    }
}

void Renderer::drawHUD(sf::RenderWindow& window, const Player& player) {
    // Фон HUD
    sf::RectangleShape hudBg(sf::Vector2f(SCREEN_WIDTH, 100));
    hudBg.setPosition(0, 0);
    hudBg.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(hudBg);
    
    // Здоровье (сердечки)
    int heartsCount = player.health / 20;
    for (int i = 0; i < 5; i++) {
        float heartX = 20 + i * 40;
        float heartY = 20;
        
        if (i < heartsCount) {
            sf::CircleShape heart1(10);
            heart1.setPosition(heartX, heartY);
            heart1.setFillColor(sf::Color::Red);
            
            sf::CircleShape heart2(10);
            heart2.setPosition(heartX + 12, heartY);
            heart2.setFillColor(sf::Color::Red);
            
            sf::ConvexShape heartBottom;
            heartBottom.setPointCount(3);
            heartBottom.setPoint(0, sf::Vector2f(heartX - 2, heartY + 16));
            heartBottom.setPoint(1, sf::Vector2f(heartX + 24, heartY + 16));
            heartBottom.setPoint(2, sf::Vector2f(heartX + 11, heartY + 23));
            heartBottom.setFillColor(sf::Color::Red);
            
            window.draw(heart1);
            window.draw(heart2);
            window.draw(heartBottom);
        }
    }
    
    // Броня
    if (player.armor > 0) {
        sf::RectangleShape armorBar(sf::Vector2f(200 * (player.armor / 100.0f), 15));
        armorBar.setPosition(20, 65);
        armorBar.setFillColor(sf::Color(0, 100, 200));
        window.draw(armorBar);
    }
    
    // Патроны
    sf::Text ammoText;
    ammoText.setFont(font);
    ammoText.setCharacterSize(24);
    ammoText.setFillColor(sf::Color::Yellow);
    ammoText.setOutlineColor(sf::Color::Black);
    ammoText.setOutlineThickness(2);
    ammoText.setPosition(250, 30);
    ammoText.setString(std::to_string(player.ammo) + " / " + std::to_string(player.maxAmmo));
    window.draw(ammoText);
    
    // Счет
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(32);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(2);
    scoreText.setPosition(SCREEN_WIDTH - 200, 20);
    scoreText.setString("Score: " + std::to_string(player.score));
    window.draw(scoreText);
}

void Renderer::drawCrosshair(sf::RenderWindow& window, const Player& player) {
    if (!player.isAiming) {
        sf::CircleShape outer(8);
        outer.setFillColor(sf::Color::Transparent);
        outer.setOutlineColor(sf::Color::White);
        outer.setOutlineThickness(1);
        outer.setPosition(SCREEN_WIDTH/2 - 8, SCREEN_HEIGHT/2 - 8);
        window.draw(outer);
        
        sf::CircleShape inner(2);
        inner.setFillColor(sf::Color::Red);
        inner.setPosition(SCREEN_WIDTH/2 - 2, SCREEN_HEIGHT/2 - 2);
        window.draw(inner);
    } else {
        // Прицельная сетка для точной стрельбы
        sf::RectangleShape hLine(sf::Vector2f(50, 1));
        hLine.setPosition(SCREEN_WIDTH/2 - 25, SCREEN_HEIGHT/2);
        hLine.setFillColor(sf::Color::Red);
        window.draw(hLine);
        
        sf::RectangleShape vLine(sf::Vector2f(1, 50));
        vLine.setPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 25);
        vLine.setFillColor(sf::Color::Red);
        window.draw(vLine);
        
        sf::CircleShape dot(2);
        dot.setFillColor(sf::Color::Red);
        dot.setPosition(SCREEN_WIDTH/2 - 2, SCREEN_HEIGHT/2 - 2);
        window.draw(dot);
    }
}

void Renderer::drawMinimap(sf::RenderWindow& window, const Player& player) {
    float mapScale = 6.0f;
    float offsetX = SCREEN_WIDTH - MAP_WIDTH * mapScale - 20;
    float offsetY = SCREEN_HEIGHT - MAP_HEIGHT * mapScale - 20;
    
    // Фон миникарты
    sf::RectangleShape mapBg(sf::Vector2f(MAP_WIDTH * mapScale, MAP_HEIGHT * mapScale));
    mapBg.setPosition(offsetX, offsetY);
    mapBg.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(mapBg);
    
    // Стены
    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            if (Map::worldMap[i][j] == 1) {
                sf::RectangleShape cell(sf::Vector2f(mapScale, mapScale));
                cell.setPosition(offsetX + i * mapScale, offsetY + j * mapScale);
                cell.setFillColor(sf::Color(100, 100, 100));
                window.draw(cell);
            }
        }
    }
    
    // Игрок
    sf::CircleShape playerDot(3);
    playerDot.setPosition(offsetX + player.x * mapScale - 3, offsetY + player.y * mapScale - 3);
    playerDot.setFillColor(sf::Color::Green);
    window.draw(playerDot);
    
    // Направление взгляда
    sf::RectangleShape dirLine(sf::Vector2f(20, 1));
    dirLine.setPosition(offsetX + player.x * mapScale, offsetY + player.y * mapScale);
    dirLine.setRotation(player.angle * 180 / 3.14159f);
    dirLine.setFillColor(sf::Color::Yellow);
    window.draw(dirLine);
}

void Renderer::render(sf::RenderWindow& window, const Player& player, const EnemyManager& enemyManager,
                      const PickupManager& pickupManager, Weapon& weapon, float gameTime) {
    window.clear(sf::Color(30, 30, 40));
    
    // Пол и потолок
    sf::RectangleShape floor(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT/2));
    floor.setPosition(0, SCREEN_HEIGHT/2);
    floor.setFillColor(sf::Color(60, 60, 70));
    window.draw(floor);
    
    sf::RectangleShape ceiling(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT/2));
    ceiling.setPosition(0, 0);
    ceiling.setFillColor(sf::Color(30, 30, 40));
    window.draw(ceiling);
    
    float currentFOV = player.isAiming ? player.aimFOV : FOV;
    float renderAngle = player.angle + player.leanAngle;
    float playerRenderX = player.x + cosf(player.angle + 3.14159f/2) * player.leanOffset;
    float playerRenderY = player.y + sinf(player.angle + 3.14159f/2) * player.leanOffset;
    
    // Отрисовка стен
    drawWalls(window, player, playerRenderX, playerRenderY, renderAngle, currentFOV);
    
    // Отрисовка спрайтов
    drawSprites(window, player, playerRenderX, playerRenderY, renderAngle, currentFOV, enemyManager, pickupManager, gameTime);
    
    // Отрисовка оружия
    weapon.draw(window, player, gameTime);
    
    // HUD
    drawHUD(window, player);
    drawCrosshair(window, player);
    drawMinimap(window, player);
    
    // Подсказки управления
    sf::Text controls;
    controls.setFont(font);
    controls.setCharacterSize(14);
    controls.setFillColor(sf::Color::White);
    controls.setOutlineColor(sf::Color::Black);
    controls.setOutlineThickness(1);
    controls.setPosition(10, SCREEN_HEIGHT - 30);
    controls.setString("WASD - Move | Mouse - Aim | LMB - Shoot | RMB - Aim | Shift - Run | Q/E - Lean");
    window.draw(controls);
    
    // Эффект получения урона
    if (player.invulnerabilityTimer > 0.0f) {
        sf::RectangleShape damageOverlay(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        damageOverlay.setFillColor(sf::Color(255, 0, 0, static_cast<sf::Uint8>(50 * player.invulnerabilityTimer)));
        window.draw(damageOverlay);
    }
}