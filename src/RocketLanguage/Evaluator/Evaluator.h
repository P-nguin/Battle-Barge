#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <unordered_map>
#include <functional>
#include "raylib.h"
#include "raymath.h"
#include "../Parser/Parser.h"
#include "Robots/RobotManager/RobotManager.h"

class Robot;

namespace Rocket {

class Value {
private:
    enum class Type {
        NUMBER,
        VOID
    };

    Type type;
    double number;

public:
    Value() : type(Type::VOID), number(0) {}
    Value(double n) : type(Type::NUMBER), number(n) {}

    bool isNumber() const { return type == Type::NUMBER; }
    double asNumber() const;
};

class Evaluator {
private:
    Robot* robot;
    std::unordered_map<std::string, std::function<Value(const std::vector<Value>&)>> commands;

    void setupCommands();
    Value evaluateBinding(const std::string& name);

public:
    explicit Evaluator(Robot* robot) : robot(robot) {
        setupCommands();
    }

    // Evaluate a single expression
    Value evaluate(const ExprPtr& expr);
    
    // Evaluate multiple expressions
    std::vector<Value> evaluateAll(const std::vector<ExprPtr>& expressions);
};

}

#endif