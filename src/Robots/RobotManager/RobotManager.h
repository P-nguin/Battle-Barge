#ifndef ROBOT_MANAGER_H
#define ROBOT_MANAGER_H

#include <vector>
#include <memory>

class Robot;

class RobotManager {
private:
    std::vector<std::unique_ptr<Robot>> robots;

public:
    void addRobot(std::unique_ptr<Robot> robot);
    void removeRobot(Robot* robot);
    void updateRobots(float deltaTime);
    void renderRobots() const;
    
    // Singleton pattern
    static RobotManager& getInstance();
    
private:
    RobotManager() = default;
    RobotManager(const RobotManager&) = delete;
    RobotManager& operator=(const RobotManager&) = delete;
};

#endif