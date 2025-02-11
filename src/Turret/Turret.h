#ifndef TURRET_H
#define TURRET_H

#include <iostream>
#include "Entity/Entity.h"

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

class Turret : public Entity {
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

public:
    Turret(float x, float y, float forwardsAngle, float width, float height, float health, float armour, float turnRate, float range, float fireRate, int ammo, float reloadTime, Texture2D* texture, AmmoType ammoType, TurretType turretType);
    
    virtual void fire();
    virtual void reload();

    void rotate(float angle);

    void update(float deltaTime) override = 0;
    virtual void render() override;
    virtual void interact(TurretCommands cmd);
};

#endif