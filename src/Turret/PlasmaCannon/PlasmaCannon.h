#ifndef PLASMA_CANNON_H
#define PLASMA_CANNON_H

#include "../Turret.h"

class PlasmaCannon : public Turret {
public:
    PlasmaCannon(const std::vector<Vector2>& vertices, Vector2 position, 
                 float forwardAngle, float health, float armour,
                 float turnRate, float range, float fireRate,
                 int ammo, float reloadTime, Texture2D* texture = nullptr);
    
    void update(float deltaTime) override;
    void fire() override;
};

#endif