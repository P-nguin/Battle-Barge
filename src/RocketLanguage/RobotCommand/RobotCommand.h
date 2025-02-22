#ifndef ROBOT_COMMAND_H
#define ROBOT_COMMAND_H

class Robot;

// Base command class
class RobotCommand {
public:
    virtual ~RobotCommand() = default;
    virtual void execute(Robot* robot) = 0;
    virtual bool isComplete(Robot* robot) const = 0;
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
};

#endif