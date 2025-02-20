#ifndef PARSER_H
#define PARSER_H

#include "Tokenizer/Tokenizer.h"
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <variant>

namespace Rocket {

class Expression;
using ExprPtr = std::shared_ptr<Expression>;

class Expression {
public:
    virtual ~Expression() = default;
    virtual std::string toString() const = 0;
};

// Number literal
class NumberExpr : public Expression {
    double value;
public:
    explicit NumberExpr(double v) : value(v) {}
    double getValue() const { return value; }
    std::string toString() const override {
        return std::to_string(value);
    }
};

// Command or operator
class CommandExpr : public Expression {
    std::string name;
    std::vector<ExprPtr> arguments;
public:
    CommandExpr(std::string n, std::vector<ExprPtr> args) 
        : name(std::move(n)), arguments(std::move(args)) {}
    
    const std::string& getName() const { return name; }
    const std::vector<ExprPtr>& getArguments() const { return arguments; }
    
    std::string toString() const override {
        std::string result = "(" + name;
        for (const auto& arg : arguments) {
            result += " " + arg->toString();
        }
        result += ")";
        return result;
    }
};

// Register reference
class RegisterExpr : public Expression {
    std::string name;
public:
    explicit RegisterExpr(std::string n) : name(std::move(n)) {}
    const std::string& getName() const { return name; }
    std::string toString() const override {
        return "$" + name;
    }
};

class Parser {
private:
    std::vector<Token> tokens;
    size_t current = 0;

    Token peek() const;
    Token advance();
    bool isAtEnd() const;
    
    ExprPtr parseAtom();
    ExprPtr parseList();
    
    bool check(TokenType type) const;
    bool match(TokenType type);
    Token consume(TokenType type, const std::string& message);

public:
    explicit Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}
    
    ExprPtr parse();
    
    class ParseError : public std::runtime_error {
        Token token;
    public:
        ParseError(const std::string& message, const Token& token)
            : std::runtime_error(message), token(token) {}
        const Token& getToken() const { return token; }
    };
};

}
#endif