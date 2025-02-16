#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <iostream>
#include <vector>

#include "Map/TileMap.h"
#include "Entity/Entity.h"

class GameManager {

    public:
        static GameManager *Instance;

    private:
        TileMap tilemap;
        std::vector<Entity> entities;
        // std::vector<Bullet> bullets;
        // std::vector<Turret> towers;
        
    public:
        GameManager(TileMap tilemap);

        TileMap getTileMap() { return tilemap; }

        void update(float deltaTime);
        void render();
};

#endif