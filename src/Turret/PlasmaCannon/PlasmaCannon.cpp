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
    // Handle any plasma cannon specific updates here
}

void PlasmaCannon::fire() {
    if (getAmmo() > 0) {
        std::cout << "PLASMA CANNON FIRED!" << std::endl;
        
        // Get all spawn points in world space (already rotated)
        auto worldSpawnPoints = getWorldSpawnPoints();
        
        // Create bullets at each spawn point
        for (const auto& spawnPoint : worldSpawnPoints) {
            GameManager::Instance->createBullet(
                spawnPoint,      // position
                10.0f,          // damage
                300.0f,         // speed
                getGlobalAngle(), // rotation in degrees
                nullptr
            );
        }
        
        Turret::fire();
    }
}