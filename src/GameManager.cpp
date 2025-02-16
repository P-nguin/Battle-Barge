#include "GameManager.h"

GameManager::GameManager(TileMap tilemap): tilemap(tilemap) {
    if ((Instance != nullptr)) {
        std::cout << "GameManager instance already exists!" << std::endl;
        return;
    }   
    Instance = this;
}

void GameManager::update() {
    
}
