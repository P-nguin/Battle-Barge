#include "raylib.h"
#include "Map/TileMap.h"
#include "Player/Player.h"
#include "Turret/PlasmaCannon/PlasmaCannon.h"
#include "Turret/Turret.h"
#include "RocketLanguage/Tokenizer/Tokenizer.h"
#include "RocketLanguage/Parser/Parser.h"
#include "RocketLanguage/Evaluator/Evaluator.h"
#include "Robot/TestRobot/TestRobot.h"
#include "resource_dir.h"
#include <iostream>

void testExpr(Rocket::Evaluator& evaluator, const std::string& input) {
    std::cout << "\nExecuting: " << input << "\n";
    
    Rocket::Tokenizer tokenizer(input);
    auto tokens = tokenizer.tokenize();
    
    Rocket::Parser parser(tokens);
    auto expressions = parser.parseAll();
    
    auto results = evaluator.evaluateAll(expressions);
    std::cout << "Results:\n";
    for (const auto& result : results) {
        if (result.isNumber()) {
            std::cout << result.asNumber() << "\n";
        } else {
            std::cout << "void\n";
        }
    }
}


void testBindings() {
    std::cout << "\n=== Testing Robot Bindings ===\n";
    
    TestRobot* testRobot = new TestRobot(
        {{-20, -20}, {20, -20}, {20, 20}, {-20, 20}},
        Vector2{0, 0},  // Start at origin
        0.0f,          // Initial rotation
        100.0f,        // health
        0.0f,          // armour
        5.0f,          // speed
        100.0f         // maxEnergy
    );
    
    Rocket::Evaluator evaluator(testRobot);

    // Test initial position and direction
    std::cout << "\nInitial state:";
    testExpr(evaluator, "$pos_x");      // Should be 0
    testExpr(evaluator, "$pos_y");      // Should be 0
    testExpr(evaluator, "$direction");   // Should be 0

    // Move forward and check position
    testExpr(evaluator, "(forward 10)");
    testExpr(evaluator, "$pos_x");      // Should be 10 (moved along x-axis)
    testExpr(evaluator, "$pos_y");      // Should be 0

    // Turn 90 degrees clockwise and move
    testExpr(evaluator, "(cw 90)");
    testExpr(evaluator, "$direction");   // Should be 90
    testExpr(evaluator, "(forward 10)");
    testExpr(evaluator, "$pos_x");      // Should still be 10
    testExpr(evaluator, "$pos_y");      // Should be 10

    // Test multiple commands and binding checks
    testExpr(evaluator, "(ccw 45) (forward 10) $direction $pos_x $pos_y");

    delete testRobot;
}

void testMath() {
    std::cout << "\n=== Testing Math Operations ===\n";
    
    TestRobot* testRobot = new TestRobot(
        {{-20, -20}, {20, -20}, {20, 20}, {-20, 20}},
        Vector2{0, 0},
        0.0f, 100.0f, 0.0f, 5.0f, 100.0f
    );
    
    Rocket::Evaluator evaluator(testRobot);

    // Test single expressions
    testExpr(evaluator, "(+ 1 2)");             // Should be 3
    testExpr(evaluator, "(- 5 3)");             // Should be 2
    
    // Test multiple expressions
    testExpr(evaluator, "(+ 3 4) (+ 3 4)");     // Should be 7, 7
    testExpr(evaluator, "(* 2 3) (+ 1 2) (- 5 3)"); // Should be 6, 3, 2

    delete testRobot;
}

int main() {
    std::cout << "Running Rocket Language tests...\n";
    testMath();
    testBindings();
    std::cout << "\nTests completed.\n";

    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(1280, 800, "Hello Raylib");
    SearchAndSetResourceDir("resources");
  
    GameManager gameManager(30, 30);
    
    Texture2D* wabbitTexture = new Texture2D(LoadTexture("wabbit_alpha.png"));
    Texture2D* turretTexture = new Texture2D(LoadTexture("Turrets/PlasmaTurret/SingleTurretLoaded.png"));
    Texture2D* bulletTexture = new Texture2D(LoadTexture("wabbit_alpha.png")); // For bullets

    float playerWidth = 32;
    float playerHeight = 32;
    std::vector<Vector2> playerVertices = {
        {0, 0},
        {playerWidth, 0},
        {playerWidth, playerHeight},
        {0, playerHeight}
    };
    Vector2 playerPosition = {400, 300};
    Player player(playerVertices, playerPosition, 0.0f, 10.0f, 0.0f, 64.0f, wabbitTexture);

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
        5.0f,                    // turn rate
        200.0f,                  // range
        1.0f,                    // fire rate
        10,                      // ammo
        2.0f,                    // reload time
        turretTexture            // texture
    );

    PlasmaCannon* turretPtr = plasmaTurret.get();
    gameManager.addTurret(std::move(plasmaTurret));
    
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        
        // Update player
        player.update(deltaTime);

        // Update game manager (handles turret and bullet updates)
        gameManager.update(deltaTime);

        // Direct turret testing through pointer
        if (IsKeyPressed(KEY_SPACE)) {
            turretPtr->interact(TurretCommands::FIRE);
        }
        if (IsKeyDown(KEY_LEFT)) {
            turretPtr->interact(TurretCommands::TURNLEFT);
        }
        if (IsKeyDown(KEY_RIGHT)) {
            turretPtr->interact(TurretCommands::TURNRIGHT);
        }
        if (IsKeyPressed(KEY_R)) {
            turretPtr->interact(TurretCommands::RELOAD);
        }

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
        
        player.render();
        DrawText(debugInfo, 10, 10, 20, WHITE);
        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 100, 20, WHITE);
        
        EndDrawing();
    }

    // Cleanup
    // UnloadTexture(*wabbitTexture); Player does this in the dtor
    UnloadTexture(*bulletTexture);
    // delete wabbitTexture; Player does this in the dtor
    delete bulletTexture;

    CloseWindow();
    return 0;
}