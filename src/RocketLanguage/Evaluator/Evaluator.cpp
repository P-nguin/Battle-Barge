#include "Evaluator.h"
#include "Robots/Robot/Robot.h"

namespace Rocket {

double Value::asNumber() const {
    if (!isNumber()) throw std::runtime_error("Value is not a number");
    return number;
}

void Evaluator::setupCommands() {
    // Movement commands
    commands["forward"] = [this](const std::vector<Value>& args) {
        if (args.size() != 1 || !args[0].isNumber()) {
            throw std::runtime_error("forward expects one number argument");
        }
        float distance = args[0].asNumber();
        robot->queueCommand(std::make_unique<MoveForwardCommand>(distance));
        return Value();
    };

    commands["cw"] = [this](const std::vector<Value>& args) {
        if (args.size() != 1 || !args[0].isNumber()) {
            throw std::runtime_error("cw expects one number argument");
        }
        robot->queueCommand(std::make_unique<RotateCommand>(args[0].asNumber()));
        return Value();
    };

    commands["ccw"] = [this](const std::vector<Value>& args) {
        if (args.size() != 1 || !args[0].isNumber()) {
            throw std::runtime_error("ccw expects one number argument");
        }
        robot->queueCommand(std::make_unique<RotateCommand>(-args[0].asNumber()));
        return Value();
    };

    // Math operations remain unchanged
    commands["+"] = [](const std::vector<Value>& args) {
        if (args.empty()) return Value(0.0);
        double result = args[0].asNumber();
        for (size_t i = 1; i < args.size(); i++) {
            result += args[i].asNumber();
        }
        return Value(result);
    };

    commands["-"] = [](const std::vector<Value>& args) {
        if (args.empty()) throw std::runtime_error("- requires at least one argument");
        if (args.size() == 1) return Value(-args[0].asNumber());
        
        double result = args[0].asNumber();
        for (size_t i = 1; i < args.size(); i++) {
            result -= args[i].asNumber();
        }
        return Value(result);
    };

    commands["*"] = [](const std::vector<Value>& args) {
        if (args.empty()) return Value(1.0);
        double result = args[0].asNumber();
        for (size_t i = 1; i < args.size(); i++) {
            result *= args[i].asNumber();
        }
        return Value(result);
    };

    commands["/"] = [](const std::vector<Value>& args) {
        if (args.size() != 2) throw std::runtime_error("/ requires exactly two arguments");
        double divisor = args[1].asNumber();
        if (divisor == 0) throw std::runtime_error("Division by zero");
        return Value(args[0].asNumber() / divisor);
    };
}

Value Evaluator::evaluateBinding(const std::string& name) {
    if (name == "pos_x") {
        return Value(robot->getPosition().x);
    }
    if (name == "pos_y") {
        return Value(robot->getPosition().y);
    }
    if (name == "direction") {
        return Value(robot->getRotation());
    }
    throw std::runtime_error("Unknown binding: $" + name);
}

Value Evaluator::evaluate(const ExprPtr& expr) {
    if (auto num = std::dynamic_pointer_cast<NumberExpr>(expr)) {
        return Value(num->getValue());
    }
    
    if (auto binding = std::dynamic_pointer_cast<BindingExpr>(expr)) {
        return evaluateBinding(binding->getName());
    }
    
    if (auto cmd = std::dynamic_pointer_cast<CommandExpr>(expr)) {
        auto it = commands.find(cmd->getName());
        if (it == commands.end()) {
            throw std::runtime_error("Unknown command: " + cmd->getName());
        }
        
        std::vector<Value> args;
        for (const auto& arg : cmd->getArguments()) {
            args.push_back(evaluate(arg));
        }
        
        return it->second(args);
    }
    
    throw std::runtime_error("Invalid expression type");
}

std::vector<Value> Evaluator::evaluateAll(const std::vector<ExprPtr>& expressions) {
    std::vector<Value> results;
    for (const auto& expr : expressions) {
        results.push_back(evaluate(expr));
    }
    return results;
}

}