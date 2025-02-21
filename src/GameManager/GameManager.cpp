#include "GameManager.h"

GameManager* GameManager::Instance = nullptr;

GameManager::GameManager(int mapWidth, int mapHeight) {
    if ((Instance != nullptr)) {
        std::cout << "GameManager instance already exists!" << std::endl;
        return;
    }   
    Instance = this;
    
    tilemap = std::make_unique<TileMap>(mapWidth, mapHeight);
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
}

void GameManager::checkBulletCollisions() {
    for (auto& bullet : bullets) {
        for (auto& enemy : enemies) {
            if (bullet->getHitBox().checkCollision(enemy->getHitBox())) {
                std::cout << "Taking damage" << std::endl;
                enemy->takeDamage(*bullet);
            }
        }
    }
    std::cout << std::endl;
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


void GameManager::update(float deltaTime) {
    handleTurretInput();
    updateTurrets(deltaTime);
    updateBullets(deltaTime);
    updateEnemies(deltaTime);

    for (Entity& entity : entities) {
        entity.update(deltaTime);
    }

    if (enemies.size() > 0 && bullets.size() > 0){
        checkBulletCollisions();
    }

}

void GameManager::render() {
    tilemap->render();
    
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