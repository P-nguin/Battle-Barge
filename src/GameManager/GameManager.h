#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <iostream>
#include <vector>
#include <memory>

#include "Map/TileMap.h"
#include "Entity/Entity.h"
#include "Bullet/Bullet.h"
#include "Turret/Turret.h"
#include "Entity/Enemy/Enemy.h"
#include <bits/stdc++.h>

class Enemy;

enum class GameMode {
    PLAY,
    BUILD
};

class GameManager {
public:
    static GameManager* Instance;

private:
    std::unique_ptr<TileMap> tilemap;
    std::vector<Entity> entities;
    std::vector<std::unique_ptr<Bullet>> bullets;
    std::vector<std::unique_ptr<Turret>> turrets;

    GameMode currentMode;
    std::vector<std::unique_ptr<Enemy>> enemies;
    
public:
    const float HIT_COOLDOWN = 1.0f;


    GameManager(int mapWidth, int mapHeight);
    ~GameManager() = default;

    TileMap* getTileMap() { return tilemap.get(); }

    void update(float deltaTime);
    void render();
    
    void createBullet(Vector2 position, float damage, float speed, float rotation, Texture2D* texture);
    void updateBullets(float deltaTime);
    void cleanupBullets();
    void checkBulletCollisions();

    std::vector<std::unique_ptr<Turret>>& getTurrets() { return turrets; }
    void addTurret(std::unique_ptr<Turret> turret);
    void removeTurret(Turret* turret);
    void updateTurrets(float deltaTime);

    void handleTurretInput();
    void handleBuildModeInput();
    void addEnemy(std::unique_ptr<Enemy> enemy);
    void removeEnemy(Enemy* enemy);
    void updateEnemies(float deltaTime);
};

#endif