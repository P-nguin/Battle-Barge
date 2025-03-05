#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <vector>
#include <memory>
#include "raylib.h"
#include "SpaceshipTile.h"



class Spaceship {
private:
    float health;
    std::vector<std::vector<SpaceshipTile*>> tiles;
    Vector2 position;


public:
    Spaceship(std::vector<std::vector<SpaceshipTile*>> tiles);

    Vector2 getPosition() { return position; }
    const std::vector<std::vector<SpaceshipTile*>> getTiles() const { return tiles; }

    void takeDamage(float amount){ health -= amount; }
    
    void update();
    void render();

};




#endif