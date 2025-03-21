#ifndef TILE_H
#define TILE_H

#include "raylib.h"
#include <string>

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
    virtual void renderPreview() const = 0;
    virtual std::string getInfo() const;
};

class VoidTile : public Tile {
public:
    VoidTile(int x, int y);
    void render() const override;
    void renderPreview() const override;
};

class StuffTile : public Tile {
public:
    StuffTile(int x, int y);
    void render() const override;
    void renderPreview() const override;
};

#endif
