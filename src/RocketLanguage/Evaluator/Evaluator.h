#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <unordered_map>
#include <functional>
#include <string>
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
        STRING,
        VOID
    };

    Type type;
    double number;
    std::string str;

public:
    Value() : type(Type::VOID), number(0) {}
    Value(double n) : type(Type::NUMBER), number(n) {}
    Value(const std::string& s) : type(Type::STRING), str(s) {}

    bool isNumber() const { return type == Type::NUMBER; }
    bool isString() const { return type == Type::STRING; }

    double asNumber() const;
    std::string asString() const;

    std::string toString() const;
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

    Value evaluate(const ExprPtr& expr);
    std::vector<Value> evaluateAll(const std::vector<ExprPtr>& expressions);
};

}

#endif