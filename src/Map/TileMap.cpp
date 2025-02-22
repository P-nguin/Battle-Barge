#include "TileMap.h"

TileMap::TileMap(int width, int height): width(width), height(height) {
    tiles.resize(height, std::vector<Tile*>(width, nullptr));
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            tiles[y][x] = new VoidTile(x, y);
        }
    }
}

TileMap::~TileMap() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            delete tiles[y][x];
        }
    }
}

void TileMap::setTile(int x, int y, Tile* tile) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        if (tiles[y][x] != nullptr) {
            delete tiles[y][x];
        }
        tiles[y][x] = tile;
    }
}

void TileMap::render() const {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (tiles[y][x] != nullptr) {
                tiles[y][x]->render();
            }
        }
    }
}

void TileMap::renderGrid() const {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int gridX = x * Tile::TILE_SIZE;
            int gridY = y * Tile::TILE_SIZE;
            DrawRectangleLines(gridX, gridY, Tile::TILE_SIZE, Tile::TILE_SIZE, LIGHTGRAY);
        }
    }
}

Tile* TileMap::getTileAtScreenPosition(Vector2 screenPos) {
    int tileX = static_cast<int>(screenPos.x / Tile::TILE_SIZE);
    int tileY = static_cast<int>(screenPos.y / Tile::TILE_SIZE);
    
    if (tileX >= 0 && tileX < width && tileY >= 0 && tileY < height) {
        return tiles[tileY][tileX];
    }
    return nullptr;
}

bool TileMap::isTileOccupied(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return tiles[y][x]->type != TileType::VOID;
    }
    return true;
}

void TileMap::removeTile(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        delete tiles[y][x];
        tiles[y][x] = new VoidTile(x, y);
    }
}