#ifndef ROBOT_COMMAND_H
#define ROBOT_COMMAND_H

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

class Robot;
class CommandObserver;
class GameManager;
class Entity;
class InteractableEntity;

// Base command class
class RobotCommand {
public:
    enum class CommandState {
        NOT_STARTED,
        IN_PROGRESS,
        COMPLETED,
        FAILED
    };

    virtual ~RobotCommand() = default;
    virtual void execute(Robot* robot) = 0;
    virtual bool isComplete(Robot* robot) const = 0;
    
    virtual void update(Robot* robot, float deltaTime) {}

    CommandState getState() const { return currentState; }
    void setState(CommandState newState) { currentState = newState; }

    void setErrorMessage(const std::string& message) { errorMessage = message; }
    std::string getErrorMessage() const { return errorMessage; }

    void addObserver(CommandObserver* observer) { 
        if (observer && !hasObserver(observer)) {
            observers.push_back(observer); 
        }
    }

    void removeObserver(CommandObserver* observer) {
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer),
            observers.end()
        );
    }

    bool hasObserver(CommandObserver* observer) const {
        return std::find(observers.begin(), observers.end(), observer) != observers.end();
    }

    const std::vector<CommandObserver*>& getObservers() const { return observers; }

protected:
    CommandState currentState = CommandState::NOT_STARTED;
    std::string errorMessage;
    std::vector<CommandObserver*> observers;
};

// Move forward command
class MoveForwardCommand : public RobotCommand {
private:
    float distance;
    float remainingDistance;
    bool started;

public:
    explicit MoveForwardCommand(float dist) 
        : distance(dist), remainingDistance(dist), started(false) {}

    void execute(Robot* robot) override;
    bool isComplete(Robot* robot) const override;
    void update(Robot* robot, float deltaTime) override;
};

// Rotate command
class RotateCommand : public RobotCommand {
private:
    float angle;
    float remainingAngle;
    bool started;

public:
    explicit RotateCommand(float ang) 
        : angle(ang), remainingAngle(ang), started(false) {}

    void execute(Robot* robot) override;
    bool isComplete(Robot* robot) const override;
    void update(Robot* robot, float deltaTime) override;
};

// Interact command
class InteractWithCommand : public RobotCommand {
private:
    size_t targetEntityId;
    std::string command;
    bool started;
    bool completed;
    bool interactionStarted;

public:
    InteractWithCommand(size_t entityId, const std::string& cmd)
        : targetEntityId(entityId), command(cmd), started(false), 
            completed(false), interactionStarted(false) {}

    void execute(Robot* robot) override;
    bool isComplete(Robot* robot) const override;
    void update(Robot* robot, float deltaTime) override;
    
private:
    Entity* findEntityById(size_t id);
    bool tryInteract(Robot* robot, InteractableEntity* target);
};

#endif