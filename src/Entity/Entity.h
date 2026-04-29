#pragma once

class Entity {
protected:
    float m_x, m_y, m_angle, m_radius;
    int m_health, m_maxHealth;
    bool m_alive;
public:
    Entity(float x, float y, float radius, int health);
    virtual ~Entity() = default;
    virtual void update(float dt) = 0;
    virtual void takeDamage(int damage);
    virtual void heal(int amount);
    
    float getX() const { return m_x; }
    float getY() const { return m_y; }
    float getAngle() const { return m_angle; }
    float getRadius() const { return m_radius; }
    int getHealth() const { return m_health; }
    int getMaxHealth() const { return m_maxHealth; }
    bool isAlive() const { return m_alive; }
    
    void setPosition(float x, float y) { m_x = x; m_y = y; }
    void setAngle(float angle) { m_angle = angle; }
};