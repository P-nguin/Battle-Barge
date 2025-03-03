#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <vector>
#include "raylib.h"

enum SpaceShipTile {
    WALL,
    FLOOR,
    TURRET
};

class Spaceship {
private:
    
    
    std::vector<std::vector<SpaceShipTile>> tiles;
    Vector2 position;


public:
    Spaceship();

    std::vector<std::vector<SpaceShipTile>> getTiles() { return tiles; }

    
    void update();
    void render();



};




#endif