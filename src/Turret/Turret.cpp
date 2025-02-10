#include "Turret.h"

Turret::Turret(float x, float y, float forwardAngle, float width, float height, float health, float armour, float turnRate, float range, float fireRate, int ammo, float reloadTime, Texture2D* texture, AmmoType ammoType, TurretType turretType)
    : Entity(x, y, width, height, health, armour, 0, true, texture),
      forwardAngle(forwardAngle),
      localAngle(0.0f),
      globalAngle(0.0f),
      turnRate(turnRate),
      range(range),
      fireRate(fireRate),
      accuracy(0.0f),
      MAXIMUN_ACCURACY(0.0f),
      MINIMUN_ACCURACY(0.0f),
      ammo(ammo),
      reloadTime(reloadTime),
      ammoType(ammoType),
      turretType(turretType) 
{}

void Turret::fire() {
    if (ammo > 0) {
        // Play firing sound effect
        // Spawn projectile or bullet
        std::cout << "FIRE" << std::endl;
        ammo--;
    } else {
        // Play out of ammo sound effect
        std:: cout << "NO AMMO" << std::endl;
    }
}

void Turret::rotate(float angle) {
    localAngle += angle;
    localAngle = fmod(localAngle, 360.0f); // Wrap localAngle to range [0, 360)

    localAngle = std::max(localAngle, -MAXIMUN_ANGLE);
    localAngle = std::min(localAngle, MAXIMUN_ANGLE);

    globalAngle = forwardAngle + localAngle;
    globalAngle = fmod(globalAngle, 360.0f); // Wrap globalAngle to range [0, 360)
}

