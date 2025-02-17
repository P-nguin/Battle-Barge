#include "PlasmaCannon.h"

PlasmaCannon::PlasmaCannon(const std::vector<Vector2>& vertices, Vector2 position,
                          float forwardAngle, float health, float armour,
                          float turnRate, float range, float fireRate,
                          int ammo, float reloadTime, Texture2D* texture)
    : Turret(vertices, position, forwardAngle, health, armour,
             turnRate, range, fireRate, ammo, reloadTime, texture,
             AmmoType::HE, TurretType::CANNON)
{ }

void PlasmaCannon::update(float deltaTime) {
    
}

void PlasmaCannon::fire() {
    if (getAmmo() > 0) {
        std::cout << "PLASMA CANNON FIRED!" << std::endl;
        Turret::fire();
    }
}