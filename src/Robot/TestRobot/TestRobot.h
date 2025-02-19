#ifndef ROBOT_TESTING_H
#define ROBOT_TESTING_H

#include "../Robot.h"

class TestRobot : public Robot {
public:
    TestRobot(const std::vector<Vector2>& vertices,
                 Vector2 position,
                 float rotation,
                 float health,
                 float armour,
                 float speed,
                 float maxEnergy = 100.0f);

    void update(float deltaTime) override;
    void render() override;
};

#endif