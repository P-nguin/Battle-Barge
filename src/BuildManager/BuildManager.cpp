#include "BuildManager.h"
#include <iostream>

BuildManager::BuildManager(TileMap* tilemap, CameraController* camera)
    : tilemap(tilemap)
    , camera(camera)
    , isActive(false)
    , isDragging(false)
    , currentTileType(TileType::STUFF)
    , previewColor(SKYBLUE)
{
    previewColor.a = 128; // 50% transparency for preview
}

void BuildManager::enter() {
    if (!isActive) {
        isActive = true;
        
        // Detach camera from player
        camera->setTrackedEntity(nullptr);
        
        std::cout << "Entered build mode" << std::endl;
    }
}

void BuildManager::exit() {
    if (isActive) {
        isActive = false;
        std::cout << "Exited build mode" << std::endl;
    }
}

void BuildManager::update(float deltaTime) {
    if (!isActive) return;
    
    handleInput();
    handleCameraMovement(deltaTime);
    handleMouseDrag();
    updateHoveredTile();
}

void BuildManager::handleInput() {
    // Exit build mode with ESC
    if (IsKeyPressed(KEY_ESCAPE)) {
        exit();
        return;
    }
    
    // Switch tile types
    if (IsKeyPressed(KEY_V)) {
        currentTileType = TileType::VOID;
        std::cout << "Selected VOID tile" << std::endl;
    } else if (IsKeyPressed(KEY_S)) {
        currentTileType = TileType::STUFF;
        std::cout << "Selected STUFF tile" << std::endl;
    }
    
    handleBuildActions();
}

void BuildManager::handleCameraMovement(float deltaTime) {
    Vector2 movement = {0, 0};
    float moveSpeed = 300.0f * deltaTime;
    
    // WASD camera movement
    if (IsKeyDown(KEY_W)) movement.y -= moveSpeed;
    if (IsKeyDown(KEY_S)) movement.y += moveSpeed;
    if (IsKeyDown(KEY_A)) movement.x -= moveSpeed;
    if (IsKeyDown(KEY_D)) movement.x += moveSpeed;
    
    // Apply movement if any
    if (movement.x != 0 || movement.y != 0) {
        Vector2 currentPos = camera->getCamera().target;
        camera->setTargetPosition({currentPos.x + movement.x, currentPos.y + movement.y});
    }
}

void BuildManager::handleMouseDrag() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        isDragging = true;
        previousMousePosition = GetMousePosition();
    }
    
    if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
        isDragging = false;
    }
    
    if (isDragging) {
        // Get the current mouse position
        Vector2 currentMousePosition = GetMousePosition();
        
        // Convert both previous and current positions to world space
        Vector2 prevWorldPos = camera->getScreenToWorld(previousMousePosition);
        Vector2 currentWorldPos = camera->getScreenToWorld(currentMousePosition);
        
        // Calculate displacement in world space
        Vector2 displacement = {
            prevWorldPos.x - currentWorldPos.x,
            prevWorldPos.y - currentWorldPos.y
        };
        
        // Move the camera by the displacement
        Vector2 currentPos = camera->getCamera().target;
        camera->setTargetPosition({currentPos.x + displacement.x, currentPos.y + displacement.y});
        
        // Update previous position for next frame
        previousMousePosition = currentMousePosition;
    }
}

void BuildManager::handleBuildActions() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mouseWorldPos = camera->getScreenToWorld(GetMousePosition());
        int tileX = static_cast<int>(mouseWorldPos.x) / Tile::TILE_SIZE;
        int tileY = static_cast<int>(mouseWorldPos.y) / Tile::TILE_SIZE;
        
        placeTile(tileX, tileY);
    }
}

void BuildManager::setTileType(TileType type) {
    currentTileType = type;
}

void BuildManager::placeTile(int x, int y) {
    if (x < 0 || x >= tilemap->width || y < 0 || y >= tilemap->height) {
        return;
    }
    
    switch (currentTileType) {
        case TileType::VOID:
            tilemap->setTile(x, y, new VoidTile(x, y));
            break;
        case TileType::STUFF:
            tilemap->setTile(x, y, new StuffTile(x, y));
            break;
    }
}

void BuildManager::updateHoveredTile() {
    Vector2 mouseWorldPos = camera->getScreenToWorld(GetMousePosition());
    int tileX = static_cast<int>(mouseWorldPos.x) / Tile::TILE_SIZE;
    int tileY = static_cast<int>(mouseWorldPos.y) / Tile::TILE_SIZE;
    
    hoveredTilePos = {static_cast<float>(tileX), static_cast<float>(tileY)};
}

void BuildManager::render() {
    if (!isActive) return;
    
    renderTilePreview();
    renderUI();
}

void BuildManager::renderTilePreview() {
    int tileX = static_cast<int>(hoveredTilePos.x);
    int tileY = static_cast<int>(hoveredTilePos.y);
    
    // Ensure the tile is within bounds
    if (tileX >= 0 && tileX < tilemap->width && tileY >= 0 && tileY < tilemap->height) {
        // Create a temporary tile of the current type
        Tile* previewTile = nullptr;
        
        switch (currentTileType) {
            case TileType::VOID:
                previewTile = new VoidTile(tileX, tileY);
                break;
            case TileType::STUFF:
                previewTile = new StuffTile(tileX, tileY);
                break;
        }
        
        if (previewTile) {
            // Render the preview and clean up
            previewTile->renderPreview();
            delete previewTile;
        }
    }
}

void BuildManager::renderUI() {
    // Draw UI elements when in build mode
    DrawText("BUILD MODE", 10, GetScreenHeight() - 30, 20, YELLOW);
    
    const char* tileText = "";
    switch (currentTileType) {
        case TileType::VOID: tileText = "TILE: VOID (V)"; break;
        case TileType::STUFF: tileText = "TILE: STUFF (S)"; break;
    }
    DrawText(tileText, 10, GetScreenHeight() - 60, 20, WHITE);
    
    DrawText("WASD: Move Camera | Right-Click: Drag | Left-Click: Place | ESC or B: Exit", 
            GetScreenWidth() / 2 - 300, GetScreenHeight() - 30, 16, WHITE);
}