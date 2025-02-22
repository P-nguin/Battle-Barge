#ifndef ROBOT_H
#define ROBOT_H

#include <memory>
#include <queue>
#include <cmath>
#include "Entity/Entity.h"
#include "RocketLanguage/Evaluator/Evaluator.h"
#include "RocketLanguage/RobotCommand/RobotCommand.h"
#include "RocketLanguage/Script/Script.h"

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

    // Power management
    bool powerOn();
    bool powerOff();
    bool isPowered() const { return currentState != RobotState::POWERED_OFF; }
    RobotState getState() const { return currentState; }

    // Script management
    void setScript(std::shared_ptr<Script> newScript);
    std::shared_ptr<Script> getScript() const { return script; }
    void executeNextInstruction();
    bool isRunningCode() const { return isCodeRunning; }

    // Command handling
    void queueCommand(std::unique_ptr<RobotCommand> command);
    
    // Movement control
    void startMove(float distance);
    void startRotate(float angle);
    float getRemainingMovement() const { return remainingDistance; }
    float getRemainingRotation() const { return remainingRotation; }
    bool isReadyForNextCommand() const { return !isMoving && !isRotating; }

protected:
    virtual void onCodeStart() {}
    virtual void onCodeStop() {}
    virtual void onCommandComplete();

    void updateMovement(float deltaTime);
    void updateRotation(float deltaTime);

    // Parameters
    float rotationSpeed;
    float minEnergyThreshold;
    RobotState currentState;

private:
    void executeNextCommand();

    // Script
    std::shared_ptr<Script> script;
    size_t instructionPointer;
    bool isCodeRunning;

    // Command queue
    std::queue<std::unique_ptr<RobotCommand>> commandQueue;
    std::unique_ptr<RobotCommand> currentCommand;

    // Movement state
    bool isMoving;
    Vector2 moveTarget;
    float remainingDistance;

    // Rotation state
    bool isRotating;
    float targetRotation;
    float remainingRotation;
};

#endif