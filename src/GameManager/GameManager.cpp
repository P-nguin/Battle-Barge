#include "GameManager.h"

GameManager* GameManager::Instance = nullptr;

GameManager::GameManager(int mapWidth, int mapHeight) {
    if ((Instance != nullptr)) {
        std::cout << "GameManager instance already exists!" << std::endl;
        return;
    }   
    Instance = this;
    
    tilemap = std::make_unique<TileMap>(mapWidth, mapHeight);
    currentMode = GameMode::PLAY;
}

void GameManager::createBullet(Vector2 position, float damage, float speed, float rotation, Texture2D* texture) {
    std::vector<Vector2> bulletVertices = {
        {-2, -2},
        {2, -2},
        {2, 2},
        {-2, 2}
    };

    bullets.push_back(std::make_unique<Bullet>(
        bulletVertices,
        position,
        rotation,
        damage,
        speed,
        texture
    ));
}

void GameManager::updateBullets(float deltaTime) {
    for (auto& bullet : bullets) {
        bullet->update(deltaTime);
    }
    cleanupBullets();
    checkBulletCollisions();
}

void GameManager::checkBulletCollisions() {
    if (bullets.empty() || enemies.empty()) {
        return;
    }
    std::vector<Enemy*> enemiesToRemove;
    for (auto& bullet : bullets) {
        for (auto& enemy : enemies) {
            if (bullet->getHitBox().checkCollision(enemy->getHitBox())) {
                // std::cout << "interscting" << std::endl;
                if (enemy->takeDamage(bullet.get())){
                    enemiesToRemove.push_back(enemy.get());
                }
            }
        }
    }

    for (auto& enemy : enemiesToRemove) {
        removeEnemy(enemy);
    }
}

void GameManager::cleanupBullets() {
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const std::unique_ptr<Bullet>& bullet) {
                Vector2 pos = bullet->getPosition();
                return pos.x < 0 || pos.x > GetScreenWidth() || 
                       pos.y < 0 || pos.y > GetScreenHeight();
            }
        ),
        bullets.end()
    );
}

void GameManager::addTurret(std::unique_ptr<Turret> turret) {
    turrets.push_back(std::move(turret));
}

void GameManager::removeTurret(Turret* turret) {
    turrets.erase(
        std::remove_if(turrets.begin(), turrets.end(),
            [turret](const std::unique_ptr<Turret>& t) {
                return t.get() == turret;
            }
        ),
        turrets.end()
    );
}

void GameManager::updateTurrets(float deltaTime) {
    for (auto& turret : turrets) {
        turret->update(deltaTime);
    }
}


void GameManager::addEnemy(std::unique_ptr<Enemy> enemy){
    enemies.push_back(std::move(enemy));
}

void GameManager::removeEnemy(Enemy* enemy){
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [enemy](const std::unique_ptr<Enemy>& e){
                return e.get() == enemy;
            }
        ),
        enemies.end()
    );
}

void GameManager::updateEnemies(float deltaTime){
    for (auto& enemy : enemies){
        enemy->update(deltaTime);
    }
}

void GameManager::handleTurretInput() {
    for (auto& turret : turrets) {
        if (IsKeyPressed(KEY_SPACE)) {
            turret->interact(TurretCommands::FIRE);
        }
        if (IsKeyDown(KEY_LEFT)) {
            turret->interact(TurretCommands::TURNLEFT);
        }
        if (IsKeyDown(KEY_RIGHT)) {
            turret->interact(TurretCommands::TURNRIGHT);
        }
        if (IsKeyPressed(KEY_R)) {
            turret->interact(TurretCommands::RELOAD);
        }
    }
}

void GameManager::handleBuildModeInput() {
    // Right-click to set tile to void
    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        int tileX = static_cast<int>(mousePos.x / Tile::TILE_SIZE);
        int tileY = static_cast<int>(mousePos.y / Tile::TILE_SIZE);
        
        Tile* clickedTile = tilemap->getTileAtScreenPosition(mousePos);
        
        if (clickedTile && clickedTile->type != TileType::VOID) {
            tilemap->removeTile(tileX, tileY);
        }
    }
    
    // Left-click to place stuff tile or show info
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        int tileX = static_cast<int>(mousePos.x / Tile::TILE_SIZE);
        int tileY = static_cast<int>(mousePos.y / Tile::TILE_SIZE);
        
        Tile* clickedTile = tilemap->getTileAtScreenPosition(mousePos);
        
        if (clickedTile) {
            if (clickedTile->type == TileType::VOID) {
                tilemap->setTile(tileX, tileY, new StuffTile(tileX, tileY));
            } else {
                std::cout << clickedTile->getInfo() << std::endl;
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
        else if(currentMode == GameMode:: BUILD) {
            currentMode = GameMode::PLAY;
            std::cout << "PLAY" << std::endl;
        }
    }
    else if (currentMode == GameMode::PLAY) {
    

    handleTurretInput();
    updateTurrets(deltaTime);
    updateBullets(deltaTime);
    updateEnemies(deltaTime);

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

    for (const auto& enemy : enemies){
        enemy->render();
    }
}