#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <iostream>
#include <vector>

#include "Map/TileMap.h"

class GameManager {

    public:
        GameManager *Instance = nullptr;

    private:
        TileMap tilemap;
        
        

    public:
        GameManager(TileMap tilemap);

        TileMap getTileMap() { return tilemap; }

        
        void update();
        void render();
};

#endif