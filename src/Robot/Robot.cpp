#include "Robot.h"
#include <cmath>

Robot::Robot(const std::vector<Vector2>& vertices,
            Vector2 position,
            float rotation,
            float health,
            float armour,
            float speed,
            float maxEnergy)
    : Entity(vertices, position, rotation, health, armour, speed, nullptr),
      energyLevel(maxEnergy),
      maxEnergy(maxEnergy),
      isCodeRunning(false),
      currentState(RobotState::POWERED_OFF)
{}

Robot::~Robot() {}

void Robot::updateEnergy(float deltaTime) {
    if (currentState == RobotState::POWERED_OFF) return;
    
    float consumption = deltaTime * 0.1f;
    energyLevel = std::max(0.0f, energyLevel - consumption);
    
    if (energyLevel <= 0) {
        powerOff();
    }
}

bool Robot::powerOn() {
    if (energyLevel > MIN_ENERGY_THRESHOLD) {
        currentState = RobotState::IDLE;
        return true;
    }
    currentState = RobotState::ERROR;
    return false;
}

bool Robot::powerOff() {
    isCodeRunning = false;
    currentState = RobotState::POWERED_OFF;
    return true;
}

bool Robot::recharge(float amount) {
    if (amount < 0) return false;
    energyLevel = std::min(maxEnergy, energyLevel + amount);
    return true;
}

void Robot::setMaxEnergy(float newMax) {
    if (newMax <= 0) return;
    maxEnergy = newMax;
    energyLevel = std::min(energyLevel, maxEnergy);
}

void Robot::setEnergyLevel(float newLevel) {
    if (newLevel < 0) return;
    energyLevel = std::min(newLevel, maxEnergy);
}