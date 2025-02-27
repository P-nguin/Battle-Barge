#include "Evaluator.h"
#include "Robots/Robot/Robot.h"
#include "RocketLanguage/RobotCommand/RobotCommand.h"

namespace Rocket {

double Value::asNumber() const {
    if (!isNumber()) throw std::runtime_error("Value is not a number");
    return number;
}

std::string Value::asString() const {
    if (!isString()) throw std::runtime_error("Value is not a string");
    return str;
}

void Evaluator::setupCommands() {
    // Movement commands
    commands["forward"] = [this](const std::vector<Value>& args) {
        if (args.size() != 1 || !args[0].isNumber()) {
            throw std::runtime_error("forward expects one number argument");
        }
        float distance = args[0].asNumber();
        robot->setCurrentCommand(std::make_unique<MoveForwardCommand>(distance));
        return Value();
    };

    commands["cw"] = [this](const std::vector<Value>& args) {
        if (args.size() != 1 || !args[0].isNumber()) {
            throw std::runtime_error("cw expects one number argument");
        }
        robot->setCurrentCommand(std::make_unique<RotateCommand>(args[0].asNumber()));
        return Value();
    };

    commands["ccw"] = [this](const std::vector<Value>& args) {
        if (args.size() != 1 || !args[0].isNumber()) {
            throw std::runtime_error("ccw expects one number argument");
        }
        robot->setCurrentCommand(std::make_unique<RotateCommand>(-args[0].asNumber()));
        return Value();
    };
    
    // Interact commands
    commands["interact-with"] = [this](const std::vector<Value>& args) {
        for (size_t i = 0; i < args.size(); i++) {
            const auto& arg = args[i];
            std::cout << "Arg " << i << " type: " 
                    << (arg.isNumber() ? "NUMBER" : (arg.isString() ? "STRING" : "VOID"));
            
            if (arg.isNumber()) {
                std::cout << ", value: " << arg.asNumber();
            } else if (arg.isString()) {
                std::cout << ", value: \"" << arg.asString() << "\"";
            }
            std::cout << std::endl;
        }

        if (args.size() != 2 || !args[0].isNumber() || !args[1].isString()) {
            throw std::runtime_error("interact-with expects an entity ID (number) and a command string");
        }
        
        size_t entityId = static_cast<size_t>(args[0].asNumber());
        std::string commandStr = args[1].asString();
        
        // Check if robot already has a command - if so, don't create a new one
        if (robot->getCurrentCommand()) {
            std::cout << "Robot already has an active command - waiting for it to complete" << std::endl;
            return Value();
        }
        
        // Create and set the command
        try {
            robot->setCurrentCommand(std::make_unique<InteractWithCommand>(entityId, commandStr));
        } catch (const std::exception& e) {
            std::cerr << "Error creating interaction command: " << e.what() << std::endl;
            // Re-throw to notify the caller
            throw;
        }
        return Value();
};

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
    if (name == "id") {
        return Value(static_cast<double>(robot->getId()));
    }
    throw std::runtime_error("Unknown binding: $" + name);
}

Value Evaluator::evaluate(const ExprPtr& expr) {
    if (auto num = std::dynamic_pointer_cast<NumberExpr>(expr)) {
        return Value(num->getValue());
    }

    if (auto str = std::dynamic_pointer_cast<StringExpr>(expr)) {
        return Value(str->getValue());
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

std::string Value::toString() const {
    if (isNumber()) {
        return std::to_string(number);
    }
    return "void";
}

}