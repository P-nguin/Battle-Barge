#ifndef SpaceshipTile_H
#define SpaceshipTile_H

#include "Turret/PlasmaCannon/PlasmaCannon.h"
#include "Spaceship.h"

class SpaceshipTile {
private:
    float health;
    float maxHealth;
    float defense;
    float maxDefense;

public:
    SpaceshipTile(float health, float maxHealth, float defense, float maxDefense);

    virtual void render(Spaceship *spaceship) = 0;
};


class WallTile : SpaceshipTile {
public:
    WallTile();
    WallTile(float health, float maxHealth, float defense, float maxDefense);

};

class FloorTile : SpaceshipTile {
public:
    FloorTile();
    FloorTile(float health, float maxHealth, float defense, float maxDefense);

};

class TurretTile: SpaceshipTile {
private:
    PlasmaCannon turret;
public:
    TurretTile();
    TurretTile(PlasmaCannon turret);

};

class ControlTile : SpaceshipTile, IInteractable {

public:
    ControlTile();
    ControlTile(float health, float maxHealth, float defense, float maxDefense);

    void interact() override;
};

#endif