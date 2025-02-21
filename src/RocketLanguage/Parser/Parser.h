#ifndef PARSER_H
#define PARSER_H

#include "../Tokenizer/Tokenizer.h"
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>

namespace Rocket {

// Forward declarations
class Expression;
using ExprPtr = std::shared_ptr<Expression>;

// Base Expression class
class Expression {
public:
    virtual ~Expression() = default;
    virtual std::string toString() const = 0;
};

// Number literal (42, 3.14)
class NumberExpr : public Expression {
    double value;
public:
    explicit NumberExpr(double v) : value(v) {}
    double getValue() const { return value; }
    std::string toString() const override;
};

// Command expression ((forward 10), (+ 1 2))
class CommandExpr : public Expression {
    std::string name;
    std::vector<ExprPtr> arguments;
public:
    CommandExpr(std::string n, std::vector<ExprPtr> args) 
        : name(std::move(n)), arguments(std::move(args)) {}
    
    const std::string& getName() const { return name; }
    const std::vector<ExprPtr>& getArguments() const { return arguments; }
    std::string toString() const override;
};

// Built-in binding ($pos_x, $pos_y, $direction)
class BindingExpr : public Expression {
    std::string name;
public:
    explicit BindingExpr(std::string n) : name(std::move(n)) {}
    const std::string& getName() const { return name; }
    std::string toString() const override;
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

public:
    explicit Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}
    
    // Parse a single expression
    ExprPtr parse();
    
    // Parse multiple expressions
    std::vector<ExprPtr> parseAll();
    
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