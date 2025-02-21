#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <iostream>
#include <vector>
#include <memory>

#include "Map/TileMap.h"
#include "Entity/Entity.h"
#include "Bullet/Bullet.h"
#include "Turret/Turret.h"

class GameManager {
public:
    static GameManager* Instance;

private:
    std::unique_ptr<TileMap> tilemap;
    std::vector<Entity> entities;
    std::vector<std::unique_ptr<Bullet>> bullets;
    std::vector<std::unique_ptr<Turret>> turrets;
    
public:
    GameManager(int mapWidth, int mapHeight);
    ~GameManager() = default;

    TileMap* getTileMap() { return tilemap.get(); }
    
    void createBullet(Vector2 position, float damage, float speed, float rotation, Texture2D* texture);
    void updateBullets(float deltaTime);
    void cleanupBullets();

    void addTurret(std::unique_ptr<Turret> turret);
    void removeTurret(Turret* turret);
    void handleTurretInput();
    void updateTurrets(float deltaTime);
    
    void update(float deltaTime);
    void render();
};

#endif