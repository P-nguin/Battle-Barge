#ifndef GRUNT_H
#define GRUNT_H

#include "../Robot/Robot.h"

class Grunt : public Robot {
public:
    // Constants for Grunt stats
    static constexpr float DEFAULT_HEALTH = 50.0f;
    static constexpr float DEFAULT_ARMOR = 1.0f;
    static constexpr float DEFAULT_SPEED = 5.0f;
    static constexpr float DEFAULT_ROTATION_SPEED = 30.0f;
    static constexpr float DEFAULT_ENERGY = 75.0f;
    static constexpr float DEFAULT_SIZE = 16.0f;

    Grunt(Vector2 position, float rotation = 0.0f);
    
    void update(float deltaTime) override;
    void render() override;

protected:
    void onCodeStart() override;
    void onCodeStop() override;
};

#endif