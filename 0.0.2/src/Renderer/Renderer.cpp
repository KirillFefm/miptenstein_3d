#include "Renderer.h"
#include "../Utils/MathUtils.h"
#include <algorithm>

Renderer::Renderer() { for (int i = 0; i < SCREEN_WIDTH; i++) depthBuffer[i] = DEPTH; }

void Renderer::generateTextures() {
    for (int t = 0; t < 4; t++) {
        wallTextures[t].create(TEXTURE_SIZE, TEXTURE_SIZE);
        sf::Color base = t==0? sf::Color(180,160,140) : t==1? sf::Color(200,180,160) : t==2? sf::Color(160,140,120) : sf::Color(140,120,100);
        for (int x = 0; x < TEXTURE_SIZE; x++)
            for (int y = 0; y < TEXTURE_SIZE; y++) {
                sf::Color c = base;
                if (t==1 && (x%16==0 || y%16==0 || x%16==15 || y%16==15)) c = sf::Color(100,80,70);
                int n = rand()%20-10;
                c.r = std::min(255, std::max(0, c.r+n));
                c.g = std::min(255, std::max(0, c.g+n));
                c.b = std::min(255, std::max(0, c.b+n));
                wallTextures[t].setPixel(x, y, c);
            }
    }
    enemyTexture.create(128, 128, sf::Color::Transparent);
    for (int y = 0; y < 128; y++)
        for (int x = 0; x < 128; x++) {
            float cx = x - 64, cy = y - 64, dist = sqrtf(cx*cx + cy*cy);
            if (dist < 40 && y > 40) { float s = 1 - dist/40*0.5f; enemyTexture.setPixel(x, y, sf::Color(180*s, 20*s, 20*s)); }
            if (dist < 25 && y < 60) { float s = 1 - dist/25*0.3f; enemyTexture.setPixel(x, y, sf::Color(255*s, 200*s, 150*s)); }
            if (y > 30 && y < 45 && ((cx>10 && cx<20)||(cx<-10 && cx>-20)) && cy>5 && cy<15) enemyTexture.setPixel(x, y, sf::Color::Red);
            if (y > 90) { if (cx>-20 && cx<0) enemyTexture.setPixel(x, y, sf::Color(100,50,0)); if (cx>0 && cx<20) enemyTexture.setPixel(x, y, sf::Color(100,50,0)); }
        }
}

void Renderer::init() { generateTextures(); font.loadFromFile("arial.ttf"); }

void Renderer::drawWalls(sf::RenderWindow& w, const Player& p, float px, float py, float ra, float fov) {
    sf::VertexArray walls(sf::Quads);
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float rayAngle = ra - fov/2 + (float)x / SCREEN_WIDTH * fov;
        float rdx = cosf(rayAngle), rdy = sinf(rayAngle);
        float dist = 0; bool hit = false; int texX = 0, mapX = 0, mapY = 0;
        while (!hit && dist < DEPTH) {
            dist += 0.05f;
            float rx = px + rdx * dist, ry = py + rdy * dist;
            mapX = (int)rx; mapY = (int)ry;
            if (mapX >= 0 && mapX < MAP_WIDTH && mapY >= 0 && mapY < MAP_HEIGHT && Map::worldMap[mapX][mapY] == 1) {
                hit = true;
                float wx = rx - mapX, wy = ry - mapY;
                texX = (fabs(wx-0)<0.05f || fabs(wx-1)<0.05f) ? (int)(wy * TEXTURE_SIZE) : (int)(wx * TEXTURE_SIZE);
                texX = std::min(TEXTURE_SIZE-1, std::max(0, texX));
            }
        }
        if (hit) {
            float corr = dist * cosf(ra - rayAngle); depthBuffer[x] = corr;
            int h = (int)(SCREEN_HEIGHT / corr), top = (SCREEN_HEIGHT - h)/2, bot = top + h;
            int texIdx = (mapX + mapY) % 4;
            for (int y = top; y < bot; y++) {
                int texY = (int)((y - top) / (float)h * TEXTURE_SIZE);
                texY = std::min(TEXTURE_SIZE-1, std::max(0, texY));
                sf::Color c = wallTextures[texIdx].getPixel(texX, texY);
                float shade = 1 - (corr / DEPTH) * 0.5f;
                c.r *= shade; c.g *= shade; c.b *= shade;
                walls.append(sf::Vertex(sf::Vector2f((float)x, (float)y), c));
                walls.append(sf::Vertex(sf::Vector2f((float)x+1, (float)y), c));
                walls.append(sf::Vertex(sf::Vector2f((float)x+1, (float)y+1), c));
                walls.append(sf::Vertex(sf::Vector2f((float)x, (float)y+1), c));
            }
        }
    }
    w.draw(walls);
}

void Renderer::drawEnemy(sf::RenderWindow& w, const Enemy& e, float projX, float size, float gt) {
    sf::Sprite spr; sf::Texture tex; tex.loadFromImage(enemyTexture); spr.setTexture(tex);
    float voff = 0, rot = 0, alpha = 1;
    switch (e.state) {
        case EnemyState::ALIVE: voff = sinf(gt*10 + e.x)*3; break;
        case EnemyState::DYING: rot = e.fallRotation; voff = e.fallOffset * size; alpha = e.deathTimer / ENEMY_DEATH_ANIM_TIME; break;
        case EnemyState::CORPSE: rot = 90; voff = size*0.3f; alpha = std::min(1.0f, e.corpseTimer/2.0f); break;
        case EnemyState::DEAD: return;
    }
    spr.setPosition(projX - size/2, SCREEN_HEIGHT/2 - size/2 + voff);
    spr.setScale(size/128, size/128); spr.setRotation(rot);
    spr.setColor(sf::Color(255,255,255, (sf::Uint8)(255*alpha)));
    w.draw(spr);
    if (e.state == EnemyState::ALIVE && e.health < ENEMY_HEALTH) {
        sf::RectangleShape bar(sf::Vector2f(size * (e.health/(float)ENEMY_HEALTH), 5));
        bar.setPosition(projX - size/2, SCREEN_HEIGHT/2 - size/2 - 10 + voff);
        bar.setFillColor(sf::Color::Red);
        w.draw(bar);
    }
}

void Renderer::drawSprites(sf::RenderWindow& w, const Player& p, float px, float py, float ra, float fov, const EnemyManager& em, const PickupManager& pm, float gt) {
    struct SD { float dist, x, y; int type; const Enemy* e; const Pickup* pu; float size; };
    std::vector<SD> sprites;
    for (auto& e : em.getEnemies()) {
        if (e.state == EnemyState::DEAD) continue;
        float dx = e.x - px, dy = e.y - py, dist = sqrtf(dx*dx+dy*dy);
        if (dist < DEPTH && isVisible(px, py, e.x, e.y, Map::worldMap)) sprites.push_back({dist, e.x, e.y, 0, &e, nullptr, 0.9f});
    }
    for (auto& pu : pm.getPickups()) {
        if (!pu.active) continue;
        float dx = pu.x - px, dy = pu.y - py, dist = sqrtf(dx*dx+dy*dy);
        if (dist < DEPTH && isVisible(px, py, pu.x, pu.y, Map::worldMap)) sprites.push_back({dist, pu.x, pu.y, 1+pu.type, nullptr, &pu, 0.25f});
    }
    std::sort(sprites.begin(), sprites.end(), [](const SD& a, const SD& b) { return a.dist > b.dist; });
    for (auto& s : sprites) {
        float dx = s.x - px, dy = s.y - py, ang = atan2f(dy, dx), diff = ang - ra;
        while (diff < -M_PI) diff += 2*M_PI; while (diff > M_PI) diff -= 2*M_PI;
        if (fabs(diff) < fov/1.5f) {
            float projX = (diff / fov) * SCREEN_WIDTH + SCREEN_WIDTH/2;
            float size = SCREEN_HEIGHT / s.dist * s.size;
            bool occ = false;
            for (int x = std::max(0, (int)(projX - size/2)); x <= std::min(SCREEN_WIDTH-1, (int)(projX + size/2)) && !occ; x++)
                if (depthBuffer[x] < s.dist) occ = true;
            if (!occ) {
                if (s.type == 0) drawEnemy(w, *s.e, projX, size, gt);
                // === ИСПРАВЛЕНО: ОТРИСОВКА ЛУТА ===
                else if (s.type == 1) { // Аптечка
                    float bob = sinf(s.pu->bobTimer * 3.0f) * 5.0f;
                    sf::RectangleShape medkit(sf::Vector2f(size, size * 0.6f));
                    medkit.setPosition(projX - size/2, SCREEN_HEIGHT/2 + bob);
                    medkit.setFillColor(sf::Color::White);
                    medkit.setOutlineColor(sf::Color::Red);
                    medkit.setOutlineThickness(2);
                    w.draw(medkit);
                    sf::RectangleShape crossH(sf::Vector2f(size * 0.6f, size * 0.1f));
                    crossH.setPosition(projX - size * 0.3f, SCREEN_HEIGHT/2 + size * 0.25f + bob);
                    crossH.setFillColor(sf::Color::Red);
                    w.draw(crossH);
                    sf::RectangleShape crossV(sf::Vector2f(size * 0.1f, size * 0.6f));
                    crossV.setPosition(projX - size * 0.05f, SCREEN_HEIGHT/2 + bob);
                    crossV.setFillColor(sf::Color::Red);
                    w.draw(crossV);
                }
                else if (s.type == 2) { // Патроны
                    float bob = sinf(s.pu->bobTimer * 3.0f) * 5.0f;
                    sf::RectangleShape ammoBox(sf::Vector2f(size * 1.2f, size * 0.5f));
                    ammoBox.setPosition(projX - size * 0.6f, SCREEN_HEIGHT/2 + bob);
                    ammoBox.setFillColor(sf::Color(139, 90, 43));
                    ammoBox.setOutlineColor(sf::Color(100, 60, 20));
                    ammoBox.setOutlineThickness(1);
                    w.draw(ammoBox);
                    for (int i = 0; i < 3; i++) {
                        sf::RectangleShape bullet(sf::Vector2f(size * 0.2f, size * 0.06f));
                        bullet.setPosition(projX - size * 0.3f + i * size * 0.25f, SCREEN_HEIGHT/2 + size * 0.2f + bob);
                        bullet.setFillColor(sf::Color(255, 215, 0));
                        w.draw(bullet);
                    }
                }
                else if (s.type == 3) { // Броня
                    float bob = sinf(s.pu->bobTimer * 3.0f) * 5.0f;
                    sf::ConvexShape armor;
                    armor.setPointCount(3);
                    armor.setPoint(0, sf::Vector2f(projX, SCREEN_HEIGHT/2 - size * 0.3f + bob));
                    armor.setPoint(1, sf::Vector2f(projX + size * 0.5f, SCREEN_HEIGHT/2 + size * 0.3f + bob));
                    armor.setPoint(2, sf::Vector2f(projX - size * 0.5f, SCREEN_HEIGHT/2 + size * 0.3f + bob));
                    armor.setFillColor(sf::Color(50, 50, 150, 200));
                    armor.setOutlineColor(sf::Color(100, 100, 255));
                    armor.setOutlineThickness(2);
                    w.draw(armor);
                }
            }
        }
    }
}

void Renderer::drawTracers(sf::RenderWindow& w, const Weapon& wp, float px, float py, float ra, float fov) {
    for (auto& t : wp.getTracers()) {
        if (!t.active) continue;
        float dx1 = t.startX - px, dy1 = t.startY - py, dx2 = t.endX - px, dy2 = t.endY - py;
        float a1 = atan2f(dy1, dx1) - ra, a2 = atan2f(dy2, dx2) - ra;
        float d1 = sqrtf(dx1*dx1+dy1*dy1), d2 = sqrtf(dx2*dx2+dy2*dy2);
        if (d1 < DEPTH && d2 < DEPTH) {
            float x1 = a1/fov*SCREEN_WIDTH + SCREEN_WIDTH/2, x2 = a2/fov*SCREEN_WIDTH + SCREEN_WIDTH/2;
            sf::Vertex line[] = { sf::Vertex(sf::Vector2f(x1, SCREEN_HEIGHT/2), sf::Color(255,255,100,(sf::Uint8)(200*t.timer/TRACER_FADE_TIME))),
                                  sf::Vertex(sf::Vector2f(x2, SCREEN_HEIGHT/2), sf::Color(255,200,50,(sf::Uint8)(150*t.timer/TRACER_FADE_TIME))) };
            w.draw(line, 2, sf::Lines);
        }
    }
}

void Renderer::drawHUD(sf::RenderWindow& w, const Player& p) {
    sf::RectangleShape bg(sf::Vector2f(SCREEN_WIDTH, 100)); bg.setFillColor(sf::Color(0,0,0,180)); w.draw(bg);
    int hearts = p.health / 20;
    for (int i = 0; i < 5; i++) {
        if (i < hearts) {
            sf::CircleShape h1(10), h2(10); h1.setPosition(20+i*40, 20); h2.setPosition(32+i*40, 20);
            sf::ConvexShape hb; hb.setPointCount(3); hb.setPoint(0, sf::Vector2f(18+i*40, 36)); hb.setPoint(1, sf::Vector2f(42+i*40, 36)); hb.setPoint(2, sf::Vector2f(30+i*40, 43));
            h1.setFillColor(sf::Color::Red); h2.setFillColor(sf::Color::Red); hb.setFillColor(sf::Color::Red);
            w.draw(h1); w.draw(h2); w.draw(hb);
        }
    }
    if (p.armor > 0) { sf::RectangleShape bar(sf::Vector2f(200 * p.armor/100.0f, 15)); bar.setPosition(20, 65); bar.setFillColor(sf::Color(0,100,200)); w.draw(bar); }
    sf::Text ammo(std::to_string(p.ammo)+" / "+std::to_string(p.maxAmmo), font, 24); ammo.setPosition(250, 30); ammo.setFillColor(sf::Color::Yellow); w.draw(ammo);
    sf::Text score("Score: " + std::to_string(p.score), font, 32); score.setPosition(SCREEN_WIDTH-200, 20); score.setFillColor(sf::Color::White); w.draw(score);
}

void Renderer::drawCrosshair(sf::RenderWindow& w, const Player& p) {
    if (!p.isAiming) {
        sf::CircleShape o(8); o.setFillColor(sf::Color::Transparent); o.setOutlineColor(sf::Color::White); o.setOutlineThickness(1); o.setPosition(SCREEN_WIDTH/2-8, SCREEN_HEIGHT/2-8); w.draw(o);
        sf::CircleShape i(2); i.setFillColor(sf::Color::Red); i.setPosition(SCREEN_WIDTH/2-2, SCREEN_HEIGHT/2-2); w.draw(i);
    } else {
        sf::RectangleShape hl(sf::Vector2f(50, 1)), vl(sf::Vector2f(1, 50));
        hl.setPosition(SCREEN_WIDTH/2-25, SCREEN_HEIGHT/2); vl.setPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT/2-25);
        hl.setFillColor(sf::Color::Red); vl.setFillColor(sf::Color::Red);
        w.draw(hl); w.draw(vl);
        sf::CircleShape dot(2); dot.setFillColor(sf::Color::Red); dot.setPosition(SCREEN_WIDTH/2-2, SCREEN_HEIGHT/2-2); w.draw(dot);
    }
}

void Renderer::drawMinimap(sf::RenderWindow& w, const Player& p) {
    float scale = 6, ox = SCREEN_WIDTH - MAP_WIDTH*scale - 20, oy = SCREEN_HEIGHT - MAP_HEIGHT*scale - 20;
    sf::RectangleShape bg(sf::Vector2f(MAP_WIDTH*scale, MAP_HEIGHT*scale)); bg.setPosition(ox, oy); bg.setFillColor(sf::Color(0,0,0,150)); w.draw(bg);
    for (int i = 0; i < MAP_WIDTH; i++)
        for (int j = 0; j < MAP_HEIGHT; j++)
            if (Map::worldMap[i][j] == 1) {
                sf::RectangleShape c(sf::Vector2f(scale, scale)); c.setPosition(ox+i*scale, oy+j*scale); c.setFillColor(sf::Color(100,100,100)); w.draw(c);
            }
    sf::CircleShape dot(3); dot.setPosition(ox + p.x*scale - 3, oy + p.y*scale - 3); dot.setFillColor(sf::Color::Green); w.draw(dot);
}

void Renderer::render(sf::RenderWindow& w, const Player& p, const EnemyManager& em, const PickupManager& pm, Weapon& wp, float gt) {
    w.clear(sf::Color(30,30,40));
    sf::RectangleShape floor(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT/2)); floor.setPosition(0, SCREEN_HEIGHT/2); floor.setFillColor(sf::Color(60,60,70)); w.draw(floor);
    sf::RectangleShape ceil(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT/2)); ceil.setPosition(0, 0); ceil.setFillColor(sf::Color(30,30,40)); w.draw(ceil);
    float fov = p.isAiming ? p.aimFOV : FOV;
    float ra = p.angle + p.leanAngle;
    float px = p.x + cosf(p.angle + M_PI/2) * p.leanOffset;
    float py = p.y + sinf(p.angle + M_PI/2) * p.leanOffset;
    drawWalls(w, p, px, py, ra, fov);
    drawTracers(w, wp, px, py, ra, fov);
    drawSprites(w, p, px, py, ra, fov, em, pm, gt);
    wp.draw(w, p, gt);
    drawHUD(w, p);
    drawCrosshair(w, p);
    drawMinimap(w, p);
    if (p.invulnerabilityTimer > 0) { sf::RectangleShape dmg(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT)); dmg.setFillColor(sf::Color(255,0,0, (sf::Uint8)(50*p.invulnerabilityTimer))); w.draw(dmg); }
}