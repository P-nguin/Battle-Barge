#include "Tile.h"

Tile::Tile(int x, int y, TileType type): x{x*TILE_SIZE}, y{y*TILE_SIZE}, type{type} {}

std::string Tile::getInfo() const {
    std::string typeStr;
    switch (type) {
        case TileType::VOID: typeStr = "Void"; break;
        case TileType::STUFF: typeStr = "Stuff"; break;
    }
    return "Tile Type: " + typeStr + 
           "\nPosition: (" + std::to_string(x/TILE_SIZE) + ", " + std::to_string(y/TILE_SIZE) + ")";
}

VoidTile::VoidTile(int x, int y): Tile(x, y, TileType::VOID) {}

void VoidTile::render() const {
    // for now, this class doesn't actually need a render
    DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, BLUE);
}

void VoidTile::renderPreview() const {
    Color previewColor = BLUE;
    previewColor.a = 128;
    DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, previewColor);
    DrawRectangleLines(x, y, TILE_SIZE, TILE_SIZE, WHITE);
}

StuffTile::StuffTile(int x, int y): Tile(x, y, TileType::STUFF) {}

void StuffTile::render() const {
    DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, GRAY);
}

void StuffTile::renderPreview() const {
    Color previewColor = GRAY;
    previewColor.a = 128;
    DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, previewColor);
    DrawRectangleLines(x, y, TILE_SIZE, TILE_SIZE, WHITE);
}