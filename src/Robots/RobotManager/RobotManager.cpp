#include "RobotManager.h"
#include "Robots/Robot/Robot.h"

RobotManager& RobotManager::getInstance() {
    static RobotManager instance;
    return instance;
}

void RobotManager::addRobot(std::unique_ptr<Robot> robot) {
    robots.push_back(std::move(robot));
}

void RobotManager::removeRobot(Robot* robot) {
    robots.erase(
        std::remove_if(robots.begin(), robots.end(),
            [robot](const std::unique_ptr<Robot>& r) { return r.get() == robot; }
        ),
        robots.end()
    );
}

void RobotManager::updateRobots(float deltaTime) {
    for (auto& robot : robots) {
        robot->update(deltaTime);
    }
}

void RobotManager::renderRobots() const {
    for (const auto& robot : robots) {
        robot->render();
    }
}