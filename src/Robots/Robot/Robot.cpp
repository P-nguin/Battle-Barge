#include "Robot.h"
#include <iostream>

Robot::Robot(const std::vector<Vector2>& vertices,
             Vector2 position,
             float rotation,
             float health,
             float armour,
             float speed,
             float maxEnergy)
    : Entity(vertices, position, rotation, health, armour, speed, nullptr)
{}

bool Robot::powerOn() {
    if (currentState == RobotState::POWERED_OFF) {
        currentState = RobotState::IDLE;
        onCodeStart();
        
        // If script exists and is not already running, start it
        if (script && !isCodeRunning) {
            isCodeRunning = true;
            instructionPointer = 0;
            executeNextInstruction();
        }
        
        return true;
    }
    return false;
}

bool Robot::powerOff() {
    if (currentState != RobotState::POWERED_OFF) {
        currentState = RobotState::POWERED_OFF;
        isCodeRunning = false;
        
        // Clear any ongoing command
        if (currentCommand) {
            currentCommand.reset();
        }
        
        onCodeStop();
        return true;
    }
    return false;
}

void Robot::setScript(std::shared_ptr<Script> newScript) {
    script = newScript;
    instructionPointer = 0;
    
    // If powered on and not running a script, start it
    if (isPowered() && !isCodeRunning && script) {
        isCodeRunning = true;
        executeNextInstruction();
    }
}

void Robot::setCurrentCommand(std::unique_ptr<RobotCommand> command) {
    // Ensure we're not overwriting an existing command
    if (currentCommand) {
        throw std::runtime_error("Cannot set new command while another is in progress");
    }

    // Add this robot as an observer to the command
    command->addObserver(this);
    
    // Set the current command
    currentCommand = std::move(command);
    
    // Execute the command
    currentCommand->execute(this);
}

void Robot::onCommandCompleted(Robot* robot, RobotCommand* completedCommand) {
    std::cout << "Command Completed - Robot ID: " << getId() << std::endl;
    
    // Verify this completion is for our current command
    if (currentCommand.get() == completedCommand) {
        std::cout << "  Clearing current command and moving to next instruction" << std::endl;
        
        // Important: Reset the current command BEFORE trying to execute the next instruction
        currentCommand.reset();
        
        // Only move to the next instruction if this is the current robot
        if (robot == this && isCodeRunning) {
            std::cout << "  Moving to instruction #" << (instructionPointer + 1) << std::endl;
            
            // Move to the next instruction
            instructionPointer++;
            
            // Try to execute the next instruction
            executeNextInstruction();
        }
    } else {
        std::cout << "  WARNING: Completed command is not the current command!" << std::endl;
    }
}

void Robot::onCommandFailed(Robot* robot, RobotCommand* failedCommand, const std::string& errorMessage) {
    // Handle command failure
    std::cerr << "Command failed: " << errorMessage << std::endl;
    
    // Stop script execution or take appropriate error handling action
    if (currentCommand.get() == failedCommand) {
        currentCommand.reset();
        isCodeRunning = false;
        currentState = RobotState::ERROR;
    }
}

void Robot::executeNextInstruction() {
    std::cout << "Execute Next Instruction:" << std::endl;
    std::cout << "  Script exists: " << (script ? "Yes" : "No") << std::endl;
    std::cout << "  Is Code Running: " << isCodeRunning << std::endl;
    std::cout << "  Current Command: " << (currentCommand ? "Exists" : "NULL") << std::endl;
    std::cout << "  Instruction Pointer: " << instructionPointer << std::endl;

    // If there's no script, code isn't running, or there's already a command in progress,
    // don't try to execute the next instruction
    if (!script || !isCodeRunning) {
        std::cout << "  Execution blocked: " 
                 << (!script ? "No script" : "Code not running")
                 << std::endl;
        return;
    }

    // If there's a command in progress, just return and wait for it to complete
    if (currentCommand) {
        std::cout << "  Command in progress, waiting for completion" << std::endl;
        return;
    }

    try {
        script->ensureParsed();
        const auto& expressions = script->getParsedExpressions();

        std::cout << "  Total Expressions: " << expressions.size() << std::endl;
        
        if (expressions.empty()) {
            return;
        }

        // Reset instruction pointer if we've reached the end
        if (instructionPointer >= expressions.size()) {
            std::cout << "  Resetting instruction pointer" << std::endl;
            instructionPointer = 0;
        }

        // Print current instruction
        std::cout << "  Executing: " << expressions[instructionPointer]->toString() << std::endl;

        // Create evaluator and execute current instruction
        Rocket::Evaluator evaluator(this);
        
        // Try to evaluate the expression
        try {
            evaluator.evaluate(expressions[instructionPointer]);
            
            // Only increment instruction pointer if we don't have a pending command
            // If we have a pending command, we'll increment after it completes
            if (!currentCommand) {
                instructionPointer++;
                // If there's no command, immediately try to execute the next instruction
                executeNextInstruction();
            }
        } catch (const std::exception& e) {
            std::cerr << "Error evaluating expression: " << e.what() << std::endl;
            // Increment the instruction pointer anyway to avoid getting stuck
            instructionPointer++;
            // Try the next instruction
            executeNextInstruction();
        }

    } catch (const std::exception& e) {
        std::cerr << "Error executing instruction: " << e.what() << std::endl;
        isCodeRunning = false;
        currentState = RobotState::ERROR;
    }
}

void Robot::startMove(float distance) {
    if (isMoving) return;

    // Calculate target position
    float angle = getRotation() * DEG2RAD;
    Vector2 direction = {
        cosf(angle),
        sinf(angle)
    };
    
    isMoving = true;
    remainingDistance = distance;
    moveTarget = Vector2Add(getPosition(), Vector2Scale(direction, distance));
}

void Robot::startRotate(float angle) {
    if (isRotating) return;

    isRotating = true;
    
    // Calculate shortest rotation path
    remainingRotation = angle;
    while (remainingRotation > 180.0f) remainingRotation -= 360.0f;
    while (remainingRotation < -180.0f) remainingRotation += 360.0f;
    
    targetRotation = fmod(getRotation() + remainingRotation, 360.0f);
    if (targetRotation < 0) targetRotation += 360.0f;
}

void Robot::updateMovement(float deltaTime) {
    if (!isMoving) return;

    float moveAmount = speed * deltaTime;
    if (moveAmount >= remainingDistance) {
        // Final movement
        setPosition(moveTarget);
        isMoving = false;
        remainingDistance = 0;
        
        // Check if current command is complete
        if (currentCommand && currentCommand->isComplete(this)) {
            onCommandCompleted(this, currentCommand.get());
        }
    } else {
        // Continue movement
        float angle = getRotation() * DEG2RAD;
        Vector2 direction = {
            cosf(angle),
            sinf(angle)
        };
        Vector2 movement = Vector2Scale(direction, moveAmount);
        move(movement);
        remainingDistance -= moveAmount;
    }
}

void Robot::updateRotation(float deltaTime) {
    if (!isRotating) return;

    float rotateAmount = rotationSpeed * deltaTime;
    if (rotateAmount >= fabs(remainingRotation)) {
        // Final rotation
        setRotation(targetRotation);
        isRotating = false;
        remainingRotation = 0;
        
        // Check if current command is complete
        if (currentCommand && currentCommand->isComplete(this)) {
            onCommandCompleted(this, currentCommand.get());
        }
    } else {
        // Continue rotation
        float rotateDir = (remainingRotation > 0) ? 1.0f : -1.0f;
        rotate(rotateAmount * rotateDir);
        remainingRotation -= rotateAmount * rotateDir;
    }
}