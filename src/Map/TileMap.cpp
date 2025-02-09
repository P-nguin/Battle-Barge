#include "TileMap.h"

/*
tiles:
(0,0) (1,0) (2,0)
(0,1) (1,1) (2,1)
(0,2) (1,2) (2,2)
*/

TileMap::TileMap(int width, int height): width(width), height(height) {
    tiles.resize(height, std::vector<Tile*>(height, nullptr));
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            tiles[y][x] = new VoidTile(y, x);
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