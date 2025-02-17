#include "raylib.h"
#include "Player/Player.h"
#include "Turret/PlasmaCannon/PlasmaCannon.h"
#include "Turret/Turret.h"
#include "RocketLanguage/Tokenizer/Tokenizer.h"
#include "resource_dir.h"
#include <iostream>

// Test function declarations
void testTokenizer();
void printToken(const Rocket::Token& token);

int main() {
    // Run tokenizer tests first
    std::cout << "Running Tokenizer tests...\n";
    testTokenizer();
    std::cout << "Tokenizer tests completed.\n\n";

    // Regular game initialization
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(1280, 800, "Hello Raylib");
    SearchAndSetResourceDir("resources");
    
    Texture wabbit = LoadTexture("wabbit_alpha.png");

    float playerWidth = 32;
    float playerHeight = 32;
    std::vector<Vector2> playerVertices = {
        {0, 0},
        {playerWidth, 0},
        {playerWidth, playerHeight},
        {0, playerHeight}
    };
    Vector2 playerPosition = {400, 300};
    
    Player player(
        playerVertices,
        playerPosition,
        0.0f,           // initial rotation
        10.0f,          // health
        0.0f,           // armour
        64.0f,          // speed
        nullptr         // texture
    );

    float turretWidth = 40;
    float turretHeight = 40;
    std::vector<Vector2> turretVertices = {
        {-turretWidth/2, -turretHeight/2},
        {turretWidth/2, -turretHeight/2},
        {turretWidth/2, turretHeight/2},
        {-turretWidth/2, turretHeight/2}
    };
    Vector2 turretPosition = {600, 300};

    PlasmaCannon plasmaCannon(
        turretVertices,
        turretPosition,
        0.0f,           // forward angle
        20.0f,          // health
        5.0f,           // armour
        5.0f,           // turn rate
        200.0f,         // range
        1.0f,           // fire rate
        10,             // ammo
        2.0f,           // reload time
        nullptr         // texture
    );
    
    while (!WindowShouldClose()) {
        float getDeltaTime = GetFrameTime();
        player.update(getDeltaTime);

        if (IsKeyPressed(KEY_SPACE)) {
            plasmaCannon.interact(TurretCommands::FIRE);
        }
        if (IsKeyDown(KEY_LEFT)) {
            plasmaCannon.interact(TurretCommands::TURNLEFT);
        }
        if (IsKeyDown(KEY_RIGHT)) {
            plasmaCannon.interact(TurretCommands::TURNRIGHT);
        }
        if (IsKeyPressed(KEY_R)) {
            plasmaCannon.interact(TurretCommands::RELOAD);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Hello Raylib", 200,200,20,WHITE);
        player.render();
        plasmaCannon.render();
        DrawTexture(wabbit, 400, 200, WHITE);
        EndDrawing();
    }

    UnloadTexture(wabbit);
    CloseWindow();
    return 0;
}

// Helper function to print token information
void printToken(const Rocket::Token& token) {
    std::cout << "Token{type=";
    switch (token.type) {
        case Rocket::TokenType::NUMBER: std::cout << "NUMBER"; break;
        case Rocket::TokenType::SYMBOL: std::cout << "SYMBOL"; break;
        case Rocket::TokenType::LPAREN: std::cout << "LPAREN"; break;
        case Rocket::TokenType::RPAREN: std::cout << "RPAREN"; break;
        case Rocket::TokenType::REGISTER: std::cout << "REGISTER"; break;
        case Rocket::TokenType::END: std::cout << "END"; break;
    }
    std::cout << ", value='" << token.value 
              << "', line=" << token.line 
              << ", column=" << token.column << "}\n";
}

// Test function to verify tokenizer functionality
void testTokenizer() {
    // Test 1: Basic command
    std::cout << "\nTest 1: Basic command\n";
    {
        Rocket::Tokenizer tokenizer("(forward 10)");
        auto tokens = tokenizer.tokenize();
        for (const auto& token : tokens) {
            printToken(token);
        }
    }

    // Test 2: Multiple commands
    std::cout << "\nTest 2: Multiple commands\n";
    {
        Rocket::Tokenizer tokenizer("(forward 10)\n(turn 45)");
        auto tokens = tokenizer.tokenize();
        for (const auto& token : tokens) {
            printToken(token);
        }
    }

    // Test 3: Registers and arithmetic
    std::cout << "\nTest 3: Registers and arithmetic\n";
    {
        Rocket::Tokenizer tokenizer("(+ $pos (- 20 5))");
        auto tokens = tokenizer.tokenize();
        for (const auto& token : tokens) {
            printToken(token);
        }
    }

    // Test 4: Negative and decimal numbers
    std::cout << "\nTest 4: Negative and decimal numbers\n";
    {
        Rocket::Tokenizer tokenizer("-42 3.14 -0.5");
        auto tokens = tokenizer.tokenize();
        for (const auto& token : tokens) {
            printToken(token);
        }
    }

    // Test 5: Complex expression
    std::cout << "\nTest 5: Complex expression\n";
    {
        Rocket::Tokenizer tokenizer("(forward (+ $distance (* 2 $speed)))");
        auto tokens = tokenizer.tokenize();
        for (const auto& token : tokens) {
            printToken(token);
        }
    }

    // Test 6: Error cases
    std::cout << "\nTest 6: Error cases\n";
    try {
        Rocket::Tokenizer tokenizer("$");
        tokenizer.nextToken();
        std::cout << "Error: Should have thrown an exception for invalid register\n";
    } catch (const std::runtime_error& e) {
        std::cout << "Successfully caught error: " << e.what() << "\n";
    }

    try {
        Rocket::Tokenizer tokenizer("42.");
        tokenizer.nextToken();
        std::cout << "Error: Should have thrown an exception for invalid decimal\n";
    } catch (const std::runtime_error& e) {
        std::cout << "Successfully caught error: " << e.what() << "\n";
    }
}