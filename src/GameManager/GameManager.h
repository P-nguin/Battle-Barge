#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

#include "Map/TileMap.h"
#include "Entity/Entity.h"
#include "Bullet/Bullet.h"
#include "Turret/Turret.h"
#include "Entity/Enemy/Enemy.h"
#include "Robots/RobotManager/RobotManager.h"
#include "Robots/Robot/Robot.h"
#include "Player/Player.h"
#include "CameraController/CameraController.h"
#include "BuildManager/BuildManager.h"

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
    std::unique_ptr<BuildManager> buildManager;
    GameMode currentMode;
    
public:
    const float HIT_COOLDOWN = 1.0f;

    GameManager(int mapWidth, int mapHeight);
    ~GameManager() = default;

    TileMap* getTileMap() { return tilemap.get(); }
    RobotManager& getRobotManager() { return robotManager; }
    Player* getPlayer() { return player.get(); }
    BuildManager* getBuildManager() { return buildManager.get(); }
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

    void handlePlayModeInput();
};

#endif