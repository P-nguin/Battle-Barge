#include "GameManager.h"

GameManager* GameManager::Instance = nullptr;

GameManager::GameManager(int mapWidth, int mapHeight) 
    : robotManager(RobotManager::getInstance())  // Get reference to RobotManager singleton
{
    if ((Instance != nullptr)) {
        std::cout << "GameManager instance already exists!" << std::endl;
        return;
    }   
    Instance = this;
    
    tilemap = std::make_unique<TileMap>(mapWidth, mapHeight);
    currentMode = GameMode::PLAY;
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
        updateTurrets(deltaTime);
        updateBullets(deltaTime);
        updateEnemies(deltaTime);
        robotManager.updateRobots(deltaTime);

        for (Entity& entity : entities) {
            entity.update(deltaTime);
        }
    } else if (currentMode == GameMode::BUILD) {
        handleBuildModeInput();
    }
}

void GameManager::render() {
    tilemap->render();
    if(currentMode == GameMode::BUILD) tilemap->renderGrid();
    
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

    robotManager.renderRobots();
}