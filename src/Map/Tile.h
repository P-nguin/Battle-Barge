#ifndef TILE_H
#define TILE_H

#include "raylib.h"

enum class TileType {
    VOID,
    STUFF
};

class Tile {
public:
    int x, y;
    TileType type;

    static const int TILE_SIZE = 32;

    Tile(int x, int y, TileType type);
    virtual void render() const = 0;
};

class VoidTile : public Tile {
public:
    VoidTile(int x, int y);
    void render() const override;
};

class StuffTile : public Tile {
public:
    StuffTile(int x, int y);
    void render() const override;
};

#endif