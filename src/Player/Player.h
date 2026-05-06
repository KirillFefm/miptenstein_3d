#pragma once
#include "../Entity/Entity.h"
#include "../Core/Config.h"
#include <algorithm>
#include <cmath>

class Player : public Entity {
private:
    int m_ammo[Config::WEAPON_COUNT];
    int m_maxAmmo[Config::WEAPON_COUNT];
    int m_armor, m_maxArmor, m_score;
    float m_invulnerabilityTimer, m_weaponBob, m_weaponRecoil;
    float m_leanAngle, m_leanOffset;
    bool m_isAiming, m_isCrouching;
    Config::WeaponType m_currentWeapon;
    float m_fireTimer;
    

public:
    Player();
    void update(float dt) override;
    void takeDamage(int damage) override;
    void heal(int amount) override;
    void addAmmo(int amount);
    void addArmor(int amount);
    void addScore(int points);
    bool canShoot();
    
    int getAmmo() const { return m_ammo[(int)m_currentWeapon]; }
    int getMaxAmmo() const { return m_maxAmmo[(int)m_currentWeapon]; }
    int getArmor() const { return m_armor; }
    int getScore() const { return m_score; }
    float getSpeed() const { return Config::PLAYER_SPEED; }
    float getWeaponBob() const { return m_weaponBob; }
    float getWeaponRecoil() const { return m_weaponRecoil; }
    float getLeanAngle() const { return m_leanAngle; }
    float getLeanOffset() const { return m_leanOffset; }
    bool isAiming() const { return m_isAiming; }
    
    float getAimFOV() const { return Config::FOV * 0.6f; }
    float getInvulnerabilityTimer() const { return m_invulnerabilityTimer; }
    Config::WeaponType getCurrentWeapon() const { return m_currentWeapon; }
    float getFireRate() const { return Config::WEAPON_FIRE_RATE[(int)m_currentWeapon]; }
    float getBulletSpread() const { return m_isAiming ? Config::AIM_SPREAD[(int)m_currentWeapon] : Config::WEAPON_SPREAD[(int)m_currentWeapon]; }
    int getWeaponDamage() const { return Config::WEAPON_DAMAGE[(int)m_currentWeapon]; }
    float getBulletSpeed() const { return Config::BULLET_SPEED[(int)m_currentWeapon]; }
    float getBulletRange() const { return Config::BULLET_RANGE[(int)m_currentWeapon]; }
    
    
    
    void setAmmo(int ammo) { m_ammo[(int)m_currentWeapon] = std::min(m_maxAmmo[(int)m_currentWeapon], ammo); }
    void setArmor(int armor) { m_armor = std::min(m_maxArmor, armor); }
    void setAiming(bool aiming) { m_isAiming = aiming; }
    
    void setLean(float angle, float offset) { m_leanAngle = angle; m_leanOffset = offset; }
    void setWeaponBob(float bob) { m_weaponBob = bob; }
    void setWeaponRecoil(float recoil) { m_weaponRecoil = recoil; }
    void setFireTimer(float t) { m_fireTimer = t; }
    float getFireTimer() const { return m_fireTimer; }
    void switchWeapon(Config::WeaponType type) { m_currentWeapon = type; }
    
    
};