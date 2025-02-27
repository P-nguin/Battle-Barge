#include "RobotCommand.h"
#include "Entity/Entity.h"
#include "GameManager/GameManager.h"
#include "Robots/Robot/Robot.h"
#include "RocketLanguage/CommandObserver/CommandObserver.h"
#include "Turret/Turret.h"

void MoveForwardCommand::execute(Robot* robot) {
    if (!started) {
        robot->startMove(distance);
        started = true;
        currentState = CommandState::IN_PROGRESS;
    }
    remainingDistance = robot->getRemainingMovement();
}

bool MoveForwardCommand::isComplete(Robot* robot) const {
    return started && robot->getRemainingMovement() <= 0;
}

void MoveForwardCommand::update(Robot* robot, float deltaTime) {
    if (isComplete(robot)) {
        if (currentState != CommandState::COMPLETED) {
            currentState = CommandState::COMPLETED;
            
            auto observersCopy = observers;
            for (auto observer : observersCopy) {
                observer->onCommandCompleted(robot, this);
            }
        }
    }
}

void RotateCommand::execute(Robot* robot) {
    if (!started) {
        robot->startRotate(angle);
        started = true;
        currentState = CommandState::IN_PROGRESS;
    }
    remainingAngle = robot->getRemainingRotation();
}

bool RotateCommand::isComplete(Robot* robot) const {
    // std::cout << "IsComplete Check:" << std::endl;
    // std::cout << "Started: " << (started ? "Yes" : "No") << std::endl;
    // std::cout << "Remaining Movement: " << robot->getRemainingMovement() << std::endl;
    // std::cout << "Command Complete: " << (started && robot->getRemainingRotation() ? "Yes" : "No") << std::endl;
    return started && robot->getRemainingRotation() == 0;
}

void RotateCommand::update(Robot* robot, float deltaTime) {
    if (isComplete(robot)) {
        if (currentState != CommandState::COMPLETED) {
            currentState = CommandState::COMPLETED;
            
            auto observersCopy = observers;
            for (auto observer : observersCopy) {
                observer->onCommandCompleted(robot, this);
            }
        }
    }
}

// In RocketLanguage/RobotCommand/RobotCommand.cpp, replace the entire execute method:

void InteractWithCommand::execute(Robot* robot) {
    if (started) return;
    
    std::cout << "InteractWithCommand: Starting interaction with entity ID " << targetEntityId << std::endl;
    
    started = true;
    currentState = CommandState::IN_PROGRESS;
}

bool InteractWithCommand::isComplete(Robot* robot) const {
    return completed;
}

void InteractWithCommand::update(Robot* robot, float deltaTime) {
    if (!started || completed) return;
    
    if (!interactionStarted) {
        interactionStarted = true;
        
        Entity* targetEntity = findEntityById(targetEntityId);
        if (!targetEntity) {
            for (auto observer : observers) {
                observer->onCommandFailed(robot, this, errorMessage);
            }
            return;
        }
        
        InteractableEntity* interactableTarget = dynamic_cast<InteractableEntity*>(targetEntity);
        if (!interactableTarget) {
            setErrorMessage("Target entity is not interactable");
            currentState = CommandState::FAILED;

            for (auto observer : observers) {
                observer->onCommandFailed(robot, this, errorMessage);
            }
            return;
        }
        
        bool success = tryInteract(robot, interactableTarget);
        if (!success) {
            // For fire command, we might want to retry if it's just a cooldown issue
            // Check if it's a fire command and handle accordingly
            if (command == "fire") {
                Turret* turret = dynamic_cast<Turret*>(interactableTarget);
                if (turret && turret->getFireCooldown() > 0) {
                    // This is a cooldown issue - we'll retry in next update
                    std::cout << "Turret on cooldown, will retry..." << std::endl;
                    interactionStarted = false;  // Reset so we try again
                    return;
                }
            }
            
            // Permanent failure
            currentState = CommandState::FAILED;
            
            auto observersCopy = observers;
            for (auto observer : observersCopy) {
                observer->onCommandFailed(robot, this, errorMessage);
            }
            return;
        }
        
        // Mark as completed
        completed = true;
        currentState = CommandState::COMPLETED;
        
        // Notify observers of command completion - but only once
        auto observersCopy = observers;
        for (auto observer : observersCopy) {
            observer->onCommandCompleted(robot, this);
        }
    }
}

Entity* InteractWithCommand::findEntityById(size_t id) {
    Entity* entity = GameManager::Instance->getEntityById(id);
    if (!entity) {
        setErrorMessage("Target entity not found with ID: " + std::to_string(id));
    }
    return entity;
}

bool InteractWithCommand::tryInteract(Robot* robot, InteractableEntity* target) {
    if (!target->canBeInteractedWith(robot)) {
        setErrorMessage("Robot is not close enough to interact with the target");
        return false;
    }
    
    // Check if the target supports the requested command
    if (!target->canInteract(command)) {
        setErrorMessage("Target does not support the command: " + command);
        return false;
    }
    
    if (!target->interact(command)) {
        setErrorMessage("Interaction failed with command: " + command);
        return false;
    }
    
    return true;
}