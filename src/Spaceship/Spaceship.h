#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <vector>
#include "raylib.h"
#include "SpaceshipTile.h"


class Spaceship {
private:
    float health;
    std::vector<std::vector<SpaceshipTile*>> tiles;
    Vector2 position;


public:
    Spaceship(std::vector<std::vector<SpaceshipTile*>> tiles);

    std::vector<std::vector<SpaceshipTile*>> getTiles() { return tiles; }

    
    void update();
    void render();


};




#endif