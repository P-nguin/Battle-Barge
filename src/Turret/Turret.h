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
    float forwardAngle;      // Base angle of the turret
    float localAngle;        // Local rotation relative to forward angle
    float globalAngle;       // Total rotation angle
    float MAXIMUN_ANGLE;     // Maximum rotation from forward angle
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
    Turret(const std::vector<Vector2>& vertices, Vector2 position, 
           float forwardAngle, float health, float armour,
           float turnRate, float range, float fireRate,
           int ammo, float reloadTime, Texture2D* texture,
           AmmoType ammoType, TurretType turretType);
    
    virtual void fire();
    virtual void reload();
    void rotate(float angle);

    float getGlobalAngle() const { return globalAngle; }
    float getLocalAngle() const { return localAngle; }
    int getAmmo() const { return ammo; }

    void update(float deltaTime) override = 0;
    void render() override;
    virtual void interact(TurretCommands cmd);
};

#endif