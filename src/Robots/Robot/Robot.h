#ifndef ROBOT_H
#define ROBOT_H

#include <memory>
#include <cmath>
#include "Entity/Entity.h"
#include "RocketLanguage/Evaluator/Evaluator.h"
#include "RocketLanguage/RobotCommand/RobotCommand.h"
#include "RocketLanguage/Script/Script.h"
#include "RocketLanguage/CommandObserver/CommandObserver.h"

enum class RobotState {
    POWERED_OFF,
    IDLE,
    RUNNING,
    ERROR
};

class Robot : public Entity, public CommandObserver {
public:
    Robot(const std::vector<Vector2>& vertices,
          Vector2 position,
          float rotation,
          float health,
          float armour,
          float speed,
          float maxEnergy = 100.0f);
          
    virtual ~Robot() = default;

    // Pure virtual methods to be implemented by derived classes
    virtual void update(float deltaTime) override = 0;
    virtual void render() override = 0;

    // Command Observer methods (from CommandObserver interface)
    void onCommandCompleted(Robot* robot, RobotCommand* completedCommand) override;
    void onCommandFailed(Robot* robot, RobotCommand* failedCommand, const std::string& errorMessage) override;

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
    void setCurrentCommand(std::unique_ptr<RobotCommand> command);
    RobotCommand* getCurrentCommand() { return currentCommand.get(); }
    
    // Movement control
    void startMove(float distance);
    void startRotate(float angle);
    float getRemainingMovement() const { return remainingDistance; }
    float getRemainingRotation() const { return remainingRotation; }
    bool isReadyForNextCommand() const { 
        return !isMoving && !isRotating && !currentCommand; 
    }

protected:
    // Lifecycle hooks for derived classes
    virtual void onCodeStart() {}
    virtual void onCodeStop() {}

    // Movement update helpers
    void updateMovement(float deltaTime);
    void updateRotation(float deltaTime);

    // Robot parameters and state
    float rotationSpeed = 180.0f;
    float minEnergyThreshold = 10.0f;
    RobotState currentState = RobotState::POWERED_OFF;

private:
    // Script management
    std::shared_ptr<Script> script;
    size_t instructionPointer = 0;
    bool isCodeRunning = false;

    // Current command
    std::unique_ptr<RobotCommand> currentCommand;

    // Movement state
    bool isMoving = false;
    Vector2 moveTarget = {0, 0};
    float remainingDistance = 0.0f;

    // Rotation state
    bool isRotating = false;
    float targetRotation = 0.0f;
    float remainingRotation = 0.0f;
};

#endif