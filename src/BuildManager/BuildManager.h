#ifndef BUILD_MANAGER_H
#define BUILD_MANAGER_H

#include "Map/TileMap.h"
#include "raylib.h"
#include "CameraController/CameraController.h"
#include <memory>

class BuildManager {
private:
    TileMap* tilemap;
    CameraController* camera;
    
    // Build mode state
    bool isActive;
    
    // Camera control for reference only
    Vector2 previousMousePosition;
    bool isDragging;
    
    // Current tile to place
    TileType currentTileType;
    Vector2 hoveredTilePos;
    
    // Preview rendering
    Color previewColor;
    
public:
    BuildManager(TileMap* tilemap, CameraController* camera);
    ~BuildManager() = default;
    
    // Mode control
    void enter();
    void exit();
    bool isInBuildMode() const { return isActive; }
    
    // Update and input handling
    void update(float deltaTime);
    void handleInput();
    void handleCameraMovement(float deltaTime);
    void handleMouseDrag();
    void handleBuildActions();
    
    // Tile placement
    void setTileType(TileType type);
    void placeTile(int x, int y);
    void updateHoveredTile();
    
    // Rendering
    void render();
    void renderTilePreview();
    void renderUI();
};

#endif