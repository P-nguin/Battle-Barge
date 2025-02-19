#include "TestRobot.h"

TestRobot::TestRobot(const std::vector<Vector2>& vertices,
                          Vector2 position,
                          float rotation,
                          float health,
                          float armour,
                          float speed,
                          float maxEnergy)
    : Robot(vertices, position, rotation, health, armour, speed, maxEnergy)
{
}

void TestRobot::update(float deltaTime) {
    //updateEnergy(deltaTime);
}

void TestRobot::render() {
    getHitBox().renderDebug(RED, BLUE);
}