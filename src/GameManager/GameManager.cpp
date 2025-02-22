#include "GameManager.h"

GameManager* GameManager::Instance = nullptr;

GameManager::GameManager(int mapWidth, int mapHeight) 
    : robotManager(RobotManager::getInstance())
{
    if ((Instance != nullptr)) {
        std::cout << "GameManager instance already exists!" << std::endl;
        return;
    }   
    Instance = this;
    
    tilemap = std::make_unique<TileMap>(mapWidth, mapHeight);
    currentMode = GameMode::PLAY;

    // Initalize Player
    float playerWidth = 32;
    float playerHeight = 32;
    std::vector<Vector2> playerVertices = {
        {0, 0},
        {playerWidth, 0},
        {playerWidth, playerHeight},
        {0, playerHeight}
    };
    Vector2 playerPosition = {400, 300};
    initializePlayer(playerVertices, playerPosition, 0.f, 10.f, 0.f, 64.0f, nullptr);
}

void GameManager::initializePlayer(const std::vector<Vector2>& vertices, Vector2 position, float rotation, float health, float armour, float speed, Texture2D* texture) {
    player = std::make_unique<Player>(vertices, position, rotation, health, armour, speed, texture);
    camera.setTrackedEntity(player.get());
}

void GameManager::handleCameraControls() {
    // Check for robot clicks
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = camera.getScreenToWorld(GetMousePosition());
        
        // First check robots
        for (const auto& robot : robotManager.getRobots()) {
            if (robot->getHitBox().checkCollision(HitBox({mousePos}, mousePos, 0))) {
                attachCameraToEntity(robot.get());
                return;
            }
        }
        
        // If no robot was clicked, check if player was clicked
        if (player && player->getHitBox().checkCollision(HitBox({mousePos}, mousePos, 0))) {
            attachCameraToEntity(player.get());
            return;
        }
    }
}
void GameManager::attachCameraToEntity(Entity* entity) {
    camera.setTrackedEntity(entity);
}

void GameManager::handlePlayModeInput() {
    handleCameraControls();
}

void GameManager::createBullet(Vector2 position, float damage, float speed, float rotation, Texture2D* texture) {
    // Create bullet hitbox
    float bulletSize = 4.0f;  // Small bullet size
    std::vector<Vector2> bulletVertices = {
        {-bulletSize/2, -bulletSize/2},
        {bulletSize/2, -bulletSize/2},
        {bulletSize/2, bulletSize/2},
        {-bulletSize/2, bulletSize/2}
    };

    auto bullet = std::make_unique<Bullet>(
        bulletVertices,
        position,
        rotation,
        damage,
        speed,
        texture
    );
    
    bullets.push_back(std::move(bullet));
}

void GameManager::updateBullets(float deltaTime) {
    // Update bullet positions
    for (auto& bullet : bullets) {
        bullet->update(deltaTime);
    }

    // Clean up bullets that are out of bounds or have hit something
    cleanupBullets();
    checkBulletCollisions();
}

void GameManager::cleanupBullets() {
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const auto& bullet) {
                Vector2 pos = bullet->getPosition();
                return pos.x < 0 || pos.x > GetScreenWidth() ||
                       pos.y < 0 || pos.y > GetScreenHeight();
            }
        ),
        bullets.end()
    );
}

void GameManager::checkBulletCollisions() {
    // Iterate through bullets and check collisions with enemies
    for (auto& bullet : bullets) {
        for (auto& enemy : enemies) {
            if (bullet->getHitBox().checkCollision(enemy->getHitBox())) {
                if (enemy->takeDamage(bullet.get())) {
                    removeEnemy(enemy.get());
                }
            }
        }
    }
}

void GameManager::addTurret(std::unique_ptr<Turret> turret) {
    turrets.push_back(std::move(turret));
}

void GameManager::removeTurret(Turret* turret) {
    turrets.erase(
        std::remove_if(turrets.begin(), turrets.end(),
            [turret](const auto& t) { return t.get() == turret; }
        ),
        turrets.end()
    );
}

void GameManager::updateTurrets(float deltaTime) {
    for (auto& turret : turrets) {
        turret->update(deltaTime);
    }
}

void GameManager::addEnemy(std::unique_ptr<Enemy> enemy) {
    enemies.push_back(std::move(enemy));
}

void GameManager::removeEnemy(Enemy* enemy) {
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [enemy](const auto& e) { return e.get() == enemy; }
        ),
        enemies.end()
    );
}

void GameManager::updateEnemies(float deltaTime) {
    for (auto& enemy : enemies) {
        enemy->update(deltaTime);
    }
}

void GameManager::handleBuildModeInput() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        Tile* clickedTile = tilemap->getTileAtScreenPosition(mousePos);
        if (clickedTile) {
            // For now, just toggle between VOID and STUFF
            if (clickedTile->type == TileType::VOID) {
                int tileX = static_cast<int>(mousePos.x) / Tile::TILE_SIZE;
                int tileY = static_cast<int>(mousePos.y) / Tile::TILE_SIZE;
                tilemap->setTile(tileX, tileY, new StuffTile(tileX, tileY));
            } else {
                int tileX = static_cast<int>(mousePos.x) / Tile::TILE_SIZE;
                int tileY = static_cast<int>(mousePos.y) / Tile::TILE_SIZE;
                tilemap->setTile(tileX, tileY, new VoidTile(tileX, tileY));
            }
        }
    }
}

void GameManager::update(float deltaTime) {
    camera.update(deltaTime);

    if (IsKeyPressed(KEY_B)) {
        std::cout << "Switching GameMode to ";
        if(currentMode == GameMode::PLAY) {
            currentMode = GameMode::BUILD;
            std::cout << "BUILD" << std::endl;
        }
        else if(currentMode == GameMode::BUILD) {
            currentMode = GameMode::PLAY;
            std::cout << "PLAY" << std::endl;
        }
    }
    else if (currentMode == GameMode::PLAY) {
        handlePlayModeInput();
        player->update(deltaTime);
        updateTurrets(deltaTime);
        updateEnemies(deltaTime);
        robotManager.updateRobots(deltaTime);
        for (Entity& entity : entities) {
            entity.update(deltaTime);
        }

        updateBullets(deltaTime);
    } else if (currentMode == GameMode::BUILD) {
        handleBuildModeInput();
    }
}

void GameManager::render() {
    BeginMode2D(camera.getCamera());

    tilemap->render();
    if(currentMode == GameMode::BUILD) {
        tilemap->renderGrid();
    }
    
    for (const auto& turret : turrets) {
        turret->render();
    }
    
    for (Entity& entity : entities) {
        entity.render();
    }
    
    for (const auto& bullet : bullets) {
        bullet->render();
    }

    for (const auto& enemy : enemies) {
        enemy->render();
    }

    player->render();

    robotManager.renderRobots();
}