#include "Entity.h"
#include <algorithm>

Entity::Entity(float x, float y, float radius, int health)
    : m_x(x), m_y(y), m_angle(0), m_radius(radius)
    , m_health(health), m_maxHealth(health), m_alive(true) {}

void Entity::takeDamage(int damage) {
    if (!m_alive) return;
    m_health -= damage;
    if (m_health <= 0) { m_health = 0; m_alive = false; }
}

void Entity::heal(int amount) {
    if (!m_alive) return;
    m_health = std::min(m_maxHealth, m_health + amount);
}