#include "PlasmaCannon.h"

PlasmaCannon::PlasmaCannon(const std::vector<Vector2>& vertices, Vector2 position,
                          float forwardAngle, float health, float armour,
                          float turnRate, float range, float fireRate,
                          int ammo, float reloadTime, Texture2D* texture)
    : Turret(vertices, position, forwardAngle, health, armour,
             turnRate, range, fireRate, ammo, reloadTime, texture,
             AmmoType::HE, TurretType::CANNON)
{
    initializeBulletSpawns();
}

void PlasmaCannon::initializeBulletSpawns() {
    addBulletSpawnPoint({0, 0});
}

void PlasmaCannon::update(float deltaTime) {
    Turret::update(deltaTime);
}

bool PlasmaCannon::fire() {
    if (getAmmo() > 0 && getFireCooldown() <= 0.0f) {
        std::cout << "PLASMA CANNON FIRED!" << std::endl;
        
        // Create bullets at all spawn points
        auto worldSpawnPoints = getWorldSpawnPoints();
        for (const auto& spawnPoint : worldSpawnPoints) {
            GameManager::Instance->createBullet(
                spawnPoint,      // position
                10.0f,          // damage
                300.0f,         // speed
                getGlobalAngle(), // rotation in degrees
                nullptr
            );
        }
        
        // Call the parent's fire() to handle ammo reduction and cooldown
        Turret::fire();
        return true;  // Always return true when we actually fire
    } else {
        // Call the parent's fire to get the messages, but return false
        Turret::fire();
        return false;  // Return false for cooldown or no ammo
    }
}

bool PlasmaCannon::interact(const std::string& commandStr) {
    return Turret::interact(commandStr);
}

bool PlasmaCannon::canInteract(const std::string& cmdStr) const {
    return Turret::canInteract(cmdStr);
}