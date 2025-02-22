#include "Robot.h"

Robot::Robot(const std::vector<Vector2>& vertices,
            Vector2 position,
            float rotation,
            float health,
            float armour,
            float speed,
            float maxEnergy)
    : Entity(vertices, position, rotation, health, armour, speed, nullptr),
      rotationSpeed(180.0f),
      minEnergyThreshold(10.0f),
      currentState(RobotState::POWERED_OFF),
      instructionPointer(0),
      isCodeRunning(false),
      isMoving(false),
      moveTarget{0, 0},
      remainingDistance(0),
      isRotating(false),
      targetRotation(0),
      remainingRotation(0)
{}

Robot::~Robot() {}

bool Robot::powerOn() {
    if (currentState == RobotState::POWERED_OFF) {
        currentState = RobotState::IDLE;
        onCodeStart();
        if (script) {
            isCodeRunning = true;
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
        onCodeStop();
        return true;
    }
    return false;
}

void Robot::setScript(std::shared_ptr<Script> newScript) {
    script = newScript;
    instructionPointer = 0;
    if (isPowered() && !isCodeRunning && script) {
        isCodeRunning = true;
        executeNextInstruction();
    }
}

void Robot::executeNextInstruction() {
    if (!script || !isCodeRunning || !isReadyForNextCommand()) {
        return;
    }

    try {
        script->ensureParsed();
        const auto& expressions = script->getParsedExpressions();
        
        if (expressions.empty()) {
            return;
        }

        // Reset instruction pointer if we've reached the end
        if (instructionPointer >= expressions.size()) {
            instructionPointer = 0;
        }

        // Create evaluator and execute current instruction
        Rocket::Evaluator evaluator(this);
        evaluator.evaluate(expressions[instructionPointer]);
        
        // Move to next instruction
        instructionPointer++;

    } catch (const std::exception& e) {
        std::cerr << "Error executing instruction: " << e.what() << std::endl;
        isCodeRunning = false;
    }
}

void Robot::queueCommand(std::unique_ptr<RobotCommand> command) {
    commandQueue.push(std::move(command));
    
    // If this is the only command and we're ready, execute it
    if (commandQueue.size() == 1 && isReadyForNextCommand()) {
        executeNextCommand();
    }
}

void Robot::executeNextCommand() {
    if (!commandQueue.empty() && isReadyForNextCommand()) {
        currentCommand = std::move(commandQueue.front());
        commandQueue.pop();
        currentCommand->execute(this);
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

void Robot::onCommandComplete() {
    currentCommand.reset();
    executeNextCommand();  // Try to execute next command
    
    // If no more commands and running script, try next instruction
    if (commandQueue.empty() && isCodeRunning) {
        executeNextInstruction();
    }
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
            onCommandComplete();
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
            onCommandComplete();
        }
    } else {
        // Continue rotation
        float rotateDir = (remainingRotation > 0) ? 1.0f : -1.0f;
        Entity::rotate(rotateAmount * rotateDir);
        remainingRotation -= rotateAmount * rotateDir;
    }
}