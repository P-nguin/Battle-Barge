#include "raylib.h"
#include "Map/TileMap.h"
#include "Player/Player.h"
#include "Turret/PlasmaCannon/PlasmaCannon.h"
#include "Turret/Turret.h"
#include "Robots/Grunt/Grunt.h"
#include "Robots/RobotManager/RobotManager.h"
#include "GameManager/GameManager.h"
#include "BuildManager/BuildManager.h"
#include "Spaceship/Spaceship.h"
#include "Spaceship/SpaceshipTile.h"
#include "resource_dir.h"
#include <iostream>

int main() {

    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(1280, 800, "Hello Raylib");
    SearchAndSetResourceDir("resources");
  
    GameManager gameManager(30, 30);
    
    Texture2D* wabbitTexture = new Texture2D(LoadTexture("wabbit_alpha.png"));
    Texture2D* turretTexture = new Texture2D(LoadTexture("Turrets/PlasmaTurret/SingleTurretLoaded.png"));
    Texture2D* bulletTexture = new Texture2D(LoadTexture("wabbit_alpha.png")); // For bullets

    // Setup plasma turret
    float turretWidth = 40;
    float turretHeight = 40;
    std::vector<Vector2> turretVertices = {
        {-turretWidth/2, -turretHeight/2},
        {turretWidth/2, -turretHeight/2},
        {turretWidth/2, turretHeight/2},
        {-turretWidth/2, turretHeight/2}
    };

    auto plasmaTurret = std::make_unique<PlasmaCannon>(
        turretVertices,          // vertices
        Vector2{600, 300},       // position
        0.0f,                    // forward angle
        100.0f,                  // health
        5.0f,                    // armor
        15.0f,                    // turn rate
        200.0f,                  // range
        1.0f,                    // fire rate
        10,                      // ammo
        2.0f,                    // reload time
        turretTexture            // texture
    );

    PlasmaCannon* turretPtr = plasmaTurret.get();
    gameManager.addTurret(std::move(plasmaTurret));

    // Create a test enemy
    float enemyWidth = 32;
    float enemyHeight = 32;
    std::vector<Vector2> enemyVertices = {
        {-enemyWidth/2, -enemyHeight/2},
        {enemyWidth/2, -enemyHeight/2},
        {enemyWidth/2, enemyHeight/2},
        {-enemyWidth/2, enemyHeight/2}
    };

    Vector2 enemyPosition = {700, 300};
    std::unique_ptr<Enemy> enemyTest = std::make_unique<Enemy>(
        enemyVertices, enemyPosition, 0, 30.0f, 0.0f, 64.0f, nullptr, 
        1.0f, 1.0f, Vector2{700, 300}
    );

    gameManager.addEnemy(std::move(enemyTest));

    // Create and setup Grunt robot with the test script
    auto grunt = std::make_unique<Grunt>(Vector2{200, 200});
    grunt->powerOn();
    grunt->setScript(Script::loadFromFile("Scripts/testScript.txt"));
    gameManager.getRobotManager().addRobot(std::move(grunt));




    // Create spaceship
    std::vector<std::vector<SpaceshipTile*>> spaceshipTiles = {
        { new WallTile(), new WallTile(), new WallTile(), new WallTile() },
        { new WallTile(), new FloorTile(), new FloorTile(), new WallTile() },
        { new WallTile(), new FloorTile(), new FloorTile(), new WallTile() },
        { new WallTile(), new WallTile(), new WallTile(), new WallTile() }
    };
    
    Spaceship spaceship = Spaceship(spaceshipTiles);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // Update game manager (handles turret and bullet updates)
        gameManager.update(deltaTime);

        // Debug info
        const char* debugInfo = TextFormat(
            "Turret Info:\n"
            "Position: (%.1f, %.1f)\n"
            "Rotation: %.1f degrees\n"
            "Ammo: %d",
            turretPtr->getPosition().x,
            turretPtr->getPosition().y,
            turretPtr->getGlobalAngle(),
            turretPtr->getAmmo()
        );

        BeginDrawing();
        ClearBackground(BLACK);
        
        gameManager.render();
        
        // Draw debug info after rendering the game
        DrawText(debugInfo, 10, 10, 20, WHITE);
        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 100, 20, WHITE);

        spaceship.render();
        spaceship.update();
        
        EndDrawing();
    }

    // Cleanup
    UnloadTexture(*bulletTexture);
    delete bulletTexture;

    CloseWindow();
    return 0;
}