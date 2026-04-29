#pragma once
#include "../Entity/Entity.h"
#include "../Core/Config.h"
#include <algorithm>
#include <cmath>

class Player : public Entity {
private:
    int m_ammo;
    int m_maxAmmo;
    int m_armor;
    int m_maxArmor;
    int m_score;
    float m_invulnerabilityTimer;
    float m_weaponBob;
    float m_weaponRecoil;
    float m_leanAngle;
    float m_leanOffset;
    bool m_isAiming;

public:
    Player();
    
    void update(float dt) override;
    void takeDamage(int damage) override;
    void heal(int amount) override;
    
    void addAmmo(int amount);
    void addArmor(int amount);
    void addScore(int points);
    
    int getAmmo() const { return m_ammo; }
    int getMaxAmmo() const { return m_maxAmmo; }
    int getArmor() const { return m_armor; }
    int getMaxArmor() const { return m_maxArmor; }
    int getScore() const { return m_score; }
    float getSpeed() const { return Config::PLAYER_SPEED; }
    float getWeaponBob() const { return m_weaponBob; }
    float getWeaponRecoil() const { return m_weaponRecoil; }
    float getLeanAngle() const { return m_leanAngle; }
    float getLeanOffset() const { return m_leanOffset; }
    bool isAiming() const { return m_isAiming; }
    float getAimFOV() const { return Config::FOV * 0.6f; }
    float getInvulnerabilityTimer() const { return m_invulnerabilityTimer; }
    
    void setAmmo(int ammo) { m_ammo = std::min(m_maxAmmo, ammo); }
    void setArmor(int armor) { m_armor = std::min(m_maxArmor, armor); }
    void setAiming(bool aiming) { m_isAiming = aiming; }
    void setLean(float angle, float offset) { m_leanAngle = angle; m_leanOffset = offset; }
    void setWeaponBob(float bob) { m_weaponBob = bob; }
    void setWeaponRecoil(float recoil) { m_weaponRecoil = recoil; }
};