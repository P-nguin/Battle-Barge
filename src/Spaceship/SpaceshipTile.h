#ifndef SPACESHIPTILE_H
#define SPACESHIPTILE_H

#include "raylib.h"
#include "resource_dir.h"
#include "Turret/PlasmaCannon/PlasmaCannon.h"

class SpaceshipTile {
public:
    static constexpr int TILE_SIZE = 32;

protected:
    float health;
    float maxHealth;
    float defense;
    float maxDefense;

public:
    SpaceshipTile(float health, float maxHealth, float defense, float maxDefense);
    virtual ~SpaceshipTile() = default;  // Virtual destructor for polymorphism

    virtual void render(Vector2 pos);
};

// Derived tile types
class WallTile : public SpaceshipTile {
public:
    WallTile();
    WallTile(float health, float maxHealth, float defense, float maxDefense);

    void render(Vector2 pos) override;
};

class FloorTile : public SpaceshipTile {
public:
    FloorTile();
    FloorTile(float health, float maxHealth, float defense, float maxDefense);
    void render(Vector2 pos) override;
};

class TurretTile : public SpaceshipTile {
private:
    Turret *turret;

public:
    TurretTile();
    TurretTile(Turret *turret);
    void render(Vector2 pos) override;
};

#endif
