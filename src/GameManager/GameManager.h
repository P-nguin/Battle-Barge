#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <unordered_map>

#include "Map/TileMap.h"
#include "Entity/Entity.h"
#include "Bullet/Bullet.h"
#include "Turret/Turret.h"
#include "Entity/Enemy/Enemy.h"
#include "Robots/RobotManager/RobotManager.h"
#include "Robots/Robot/Robot.h"
#include "Player/Player.h"
#include "CameraController/CameraController.h"

class Enemy;
class Player;

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
    std::vector<std::unique_ptr<Enemy>> enemies;
    RobotManager& robotManager;
    std::unique_ptr<Player> player;
    CameraController camera;
    GameMode currentMode;

    std::unordered_map<size_t, Entity*> entityMap;
    
public:
    const float HIT_COOLDOWN = 1.0f;

    GameManager(int mapWidth, int mapHeight);
    ~GameManager() = default;

    TileMap* getTileMap() { return tilemap.get(); }
    RobotManager& getRobotManager() { return robotManager; }
    Player* getPlayer() { return player.get(); }
    const CameraController& getCamera() const { return camera; }

    void initializePlayer(const std::vector<Vector2>& vertices, Vector2 position, 
                         float rotation, float health, float armour, 
                         float speed, Texture2D* texture);

    void update(float deltaTime);
    void render();
    
    // Bullet management
    void createBullet(Vector2 position, float damage, float speed, float rotation, Texture2D* texture);
    void updateBullets(float deltaTime);
    void cleanupBullets();
    void checkBulletCollisions();

    // Turret management
    std::vector<std::unique_ptr<Turret>>& getTurrets() { return turrets; }
    void addTurret(std::unique_ptr<Turret> turret);
    void removeTurret(Turret* turret);
    void updateTurrets(float deltaTime);

    // Enemy management
    void addEnemy(std::unique_ptr<Enemy> enemy);
    void removeEnemy(Enemy* enemy);
    void updateEnemies(float deltaTime);

    // Camera management
    void handleCameraControls();
    void attachCameraToEntity(Entity* entity);

    CameraController& getCameraController() { return camera; }

    // Input handling
    void handleBuildModeInput();
    void handlePlayModeInput();

    // Entity management
    // We have way too many vectors of everything, we need to reorganize and optimize GameManager
    Entity* getEntityById(size_t id);
    void registerEntity(Entity* entity);
    void unregisterEntity(Entity* entity);
};

#endif