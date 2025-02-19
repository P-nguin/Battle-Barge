#include "Tile.h"

Tile::Tile(int x, int y, TileType type): x{x*TILE_SIZE}, y{y*TILE_SIZE}, type{type} {}

VoidTile::VoidTile(int x, int y): Tile(x, y, TileType::VOID) {}

void VoidTile::render() const {
    // for now, this class doesn't actually need a render
    DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, BLUE);
}

StuffTile::StuffTile(int x, int y): Tile(x, y, TileType::STUFF) {}

void StuffTile::render() const {
    DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, GRAY);
}