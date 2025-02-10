#ifndef TILEMAP_H
#define TILEMAP_H

#include "Tile.h"
#include <vector>

class TileMap {
public:
    int width, height;
    std::vector<std::vector<Tile*>> tiles;

    TileMap(int width, int height);
    ~TileMap();
    void setTile(int x, int y, Tile* tile);
    void render() const;
};

#endif