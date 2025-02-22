#ifndef TURRET_H
#define TURRET_H

#include <iostream>
#include "../Entity/Entity.h"

enum class AmmoType {
    HE
};

enum class TurretType {
    CANNON
};

enum class TurretCommands {
    FIRE,
    TURNLEFT,
    TURNRIGHT,
    RELOAD,
};

class Turret : public InteractableEntity {
protected:
    float forwardAngle;
    float localAngle;
    float globalAngle;
    float MAXIMUN_ANGLE;
    float turnRate;
    float range;
    float fireRate;
    float accuracy;
    float MAXIMUN_ACCURACY;
    float MINIMUN_ACCURACY;

    int MAXIMUN_AMMO;
    int ammo;
    float reloadTime;
    AmmoType ammoType;
    TurretType turretType;

    std::vector<Vector2> bulletSpawnPoints; // Local coordinates for bullet spawns

public:
    Turret(const std::vector<Vector2> &vertices, Vector2 position,
           float forwardAngle, float health, float armour,
           float turnRate, float range, float fireRate,
           int ammo, float reloadTime, Texture2D *texture,
           AmmoType ammoType, TurretType turretType);
    virtual ~Turret() = default;

    virtual void fire();
    virtual void reload();
    void rotate(float angle, float deltaTime); // Updated to include deltaTime

    float getGlobalAngle() const { return globalAngle; }
    float getLocalAngle() const { return localAngle; }
    int getAmmo() const { return ammo; }

    void render();
    void interact(TurretCommands cmd);

    void addBulletSpawnPoint(Vector2 localPoint);
    std::vector<Vector2> getWorldSpawnPoints() const;
};

#endif