#ifndef COMMAND_OBSERVER_H
#define COMMAND_OBSERVER_H

#include <string>

class Robot;
class RobotCommand;

class CommandObserver {
public:
    virtual ~CommandObserver() = default;
    
    // Called when a command is completed
    virtual void onCommandCompleted(Robot* robot, RobotCommand* completedCommand) = 0;
    
    // Called when a command fails
    virtual void onCommandFailed(Robot* robot, RobotCommand* failedCommand, const std::string& errorMessage) = 0;
};

#endif