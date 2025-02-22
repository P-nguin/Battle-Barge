#include "RobotCommand.h"
#include "Robots/Robot/Robot.h"

void MoveForwardCommand::execute(Robot* robot) {
    if (!started) {
        robot->startMove(distance);
        started = true;
    }
    remainingDistance = robot->getRemainingMovement();
}

bool MoveForwardCommand::isComplete(Robot* robot) const {
    return started && robot->getRemainingMovement() <= 0;
}

void RotateCommand::execute(Robot* robot) {
    if (!started) {
        robot->startRotate(angle);
        started = true;
    }
    remainingAngle = robot->getRemainingRotation();
}

bool RotateCommand::isComplete(Robot* robot) const {
    return started && robot->getRemainingRotation() == 0;
}