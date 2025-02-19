#ifndef ROBOT_H
#define ROBOT_H

#include <fstream>
#include <string>
#include "../Entity/Entity.h"

enum class RobotState {
    POWERED_OFF,
    IDLE,
    RUNNING,
    ERROR
};

class Robot : public Entity {
public:
    Robot(const std::vector<Vector2>& vertices,
          Vector2 position,
          float rotation,
          float health,
          float armour,
          float speed,
          float maxEnergy = 100.0f);
          
    virtual ~Robot();

    virtual void update(float deltaTime) override = 0;
    virtual void render() override = 0;

    bool powerOn();
    bool powerOff();
    bool recharge(float amount);

    RobotState getState() const { return currentState; }
    float getEnergyLevel() const { return energyLevel; }
    float getMaxEnergy() const { return maxEnergy; }
    bool isPowered() const { return currentState != RobotState::POWERED_OFF; }
    bool isRunningCode() const { return isCodeRunning; }
    
    void setMaxEnergy(float newMax);
    void setEnergyLevel(float newLevel);
    void updateEnergy(float deltaTime);

private:
    const float MAX_ROTATION_SPEED = 180.0f;
    const float MIN_ENERGY_THRESHOLD = 10.0f;

    float energyLevel;
    float maxEnergy;
    bool isCodeRunning;
    RobotState currentState;
};

#endif