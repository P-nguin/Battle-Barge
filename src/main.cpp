/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/
#include "raylib.h"

#include "Player/Player.h"
#include "Turret/PlasmaCannon/PlasmaCannon.h"
#include "Turret/Turret.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include "Map/TileMap.h"

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

    TileMap tileMap(10, 10);
	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");

    float playerWidth = 32;
    float playerHeight = 32;
    std::vector<Vector2> playerVertices = {
        {0, 0},
        {playerWidth, 0},
        {playerWidth, playerHeight},
        {0, playerHeight}
    };
    Vector2 playerPosition = {400, 300};  // Start player in middle of screen
    
    Player player(
        playerVertices,
        playerPosition,
        0.0f,           // initial rotation
        10.0f,          // health
        0.0f,           // armour
        64.0f,          // speed
        &wabbit         // texture (using nullptr for now to see hitbox visualization)
    );

    float turretWidth = 40;
    float turretHeight = 40;
    std::vector<Vector2> turretVertices = {
        {-turretWidth/2, -turretHeight/2},
        {turretWidth/2, -turretHeight/2},
        {turretWidth/2, turretHeight/2},
        {-turretWidth/2, turretHeight/2}
    };
    Vector2 turretPosition = {600, 300};  // Position it to the right of the player

    PlasmaCannon plasmaCannon(
        turretVertices,
        turretPosition,
        0.0f,           // forward angle
        20.0f,          // health
        5.0f,           // armour
        5.0f,           // turn rate (degrees per second)
        200.0f,         // range
        1.0f,           // fire rate
        10,             // ammo
        2.0f,           // reload time
        nullptr         // texture
    );
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
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

		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

        tileMap.render();

		// draw some text using the default font
		DrawText("Hello Raylib", 200,200,20,WHITE);

        player.render();

        plasmaCannon.render();

		// draw our texture to the screen
		DrawTexture(wabbit, 400, 200, WHITE);
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
