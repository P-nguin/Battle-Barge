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

void PlasmaCannon::interact() {
    if (IsKeyPressed(KEY_R)) {
        // std::cout << "Reload" << std::endl;
        Turret::turretInteract(TurretCommands::RELOAD);
    }
    if (IsKeyPressed(KEY_E)) {
        // std::cout << "Fire" << std::endl;
        Turret::turretInteract(TurretCommands::FIRE);
    }
    if (IsKeyDown(KEY_A)) {
        // std::cout << "Turn left" << std::endl;
        Turret::turretInteract(TurretCommands::TURNLEFT);
    }
    if (IsKeyDown(KEY_D)) {
        // std::cout << "Turn right" << std::endl;
        Turret::turretInteract(TurretCommands::TURNRIGHT);
    }
}
