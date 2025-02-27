#include "Grunt.h"
#include <iostream>

Grunt::Grunt(Vector2 position, float rotation)
    : Robot(
        {
            {-DEFAULT_SIZE/2, -DEFAULT_SIZE/2},
            {DEFAULT_SIZE/2, -DEFAULT_SIZE/2},
            {DEFAULT_SIZE/2, DEFAULT_SIZE/2},
            {-DEFAULT_SIZE/2, DEFAULT_SIZE/2}
        },
        position,
        rotation,
        DEFAULT_HEALTH,
        DEFAULT_ARMOR,
        DEFAULT_SPEED,
        DEFAULT_ENERGY
    )
{
    rotationSpeed = DEFAULT_ROTATION_SPEED;
}

void Grunt::update(float deltaTime) {
    updateMovement(deltaTime);
    updateRotation(deltaTime);

    auto* currentCommand = getCurrentCommand();
    if(currentCommand) {
        currentCommand->update(this, deltaTime);
    }
}

void Grunt::render() {
    getHitBox().renderDebug(RED, BLUE);
}

void Grunt::onCodeStart() {
    std::cout << "Grunt powered on" << std::endl;
}

void Grunt::onCodeStop() {
    std::cout << "Grunt powered off" << std::endl;
}