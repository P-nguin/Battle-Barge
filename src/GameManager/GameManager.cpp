#include "GameManager.h"

GameManager* GameManager::Instance = nullptr;

GameManager::GameManager(TileMap tilemap): tilemap(tilemap) {
    if ((Instance != nullptr)) {
        std::cout << "GameManager instance already exists!" << std::endl;
        return;
    }   
    Instance = this;
}

void GameManager::update(float deltaTime) {
    for (Entity &entity : entities) {
        entity.update(deltaTime);
    }
}

void GameManager::render() {
    tilemap.render();
    for (Entity &entity : entities) {
        entity.render();
    }
}