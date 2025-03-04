#include "SpaceshipTile.h"

SpaceshipTile::SpaceshipTile(float health, float maxHealth, float defense, float maxDefense): 
health{health}, maxHealth{maxHealth}, 
defense{defense}, maxDefense{maxDefense} 
{}

WallTile::WallTile(): SpaceshipTile(100, 100, 0,0) {}
WallTile::WallTile(float health, float maxHealth, float defense, float maxDefense): SpaceshipTile(health,maxHealth,defense,maxDefense) {}

FloorTile::FloorTile(): SpaceshipTile(100, 100, 0,0) {}
FloorTile::FloorTile(float health, float maxHealth, float defense, float maxDefense): SpaceshipTile(health,maxHealth,defense,maxDefense) {}

TurretTile::TurretTile(): SpaceshipTile(100, 100, 0,0), 
turret(
    std::vector<Vector2>{
        {-20.0/2, -20.0/2},
        {20.0/2, -20.0/2},
        {20.0/2, 20.0/2},
        {-20.0/2, 20.0/2}
    },
    Vector2{600, 300},       // position
    0.0f,                    // forward angle
    100.0f,                  // health
    5.0f,                    // armor
    15.0f,                    // turn rate
    200.0f,                  // range
    1.0f,                    // fire rate
    10,                      // ammo
    2.0f,
    nullptr)
{}

TurretTile::TurretTile(PlasmaCannon turret): SpaceshipTile(turret.getHealth() ,turret.getHealth(),turret.getArmour(),turret.getArmour()), turret{turret} {}

ControlTile::ControlTile(): SpaceshipTile(100, 100, 0,0) {}
ControlTile::ControlTile(float health, float maxHealth, float defense, float maxDefense): SpaceshipTile(health,maxHealth,defense,maxDefense) {} 







