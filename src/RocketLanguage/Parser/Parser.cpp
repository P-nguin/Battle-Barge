#include "Parser.h"

namespace Rocket {

std::string NumberExpr::toString() const {
    return std::to_string(value);
}

std::string CommandExpr::toString() const {
    std::string result = "(" + name;
    for (const auto& arg : arguments) {
        result += " " + arg->toString();
    }
    result += ")";
    return result;
}

std::string BindingExpr::toString() const {
    return "$" + name;
}

Token Parser::peek() const {
    if (isAtEnd()) return tokens.back();
    return tokens[current];
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return tokens[current - 1];
}

bool Parser::isAtEnd() const {
    return current >= tokens.size() || tokens[current].type == TokenType::END;
}

ExprPtr Parser::parseAtom() {
    Token token = advance();
    switch (token.type) {
        case TokenType::NUMBER:
            return std::make_shared<NumberExpr>(std::stod(token.value));
        case TokenType::SYMBOL:
            return std::make_shared<CommandExpr>(token.value, std::vector<ExprPtr>());
        case TokenType::BINDING:
            return std::make_shared<BindingExpr>(token.value);
        default:
            throw ParseError("Unexpected token", token);
    }
}

ExprPtr Parser::parseList() {
    // Consume left paren
    advance();
    
    if (peek().type != TokenType::SYMBOL) {
        throw ParseError("Expected command name", peek());
    }

    Token commandToken = advance();
    std::vector<ExprPtr> arguments;
    
    // Parse arguments until right paren
    while (peek().type != TokenType::RPAREN) {
        if (isAtEnd()) {
            throw ParseError("Unterminated list", tokens.back());
        }
        arguments.push_back(parse());
    }
    
    // Consume right paren
    advance();
    
    return std::make_shared<CommandExpr>(commandToken.value, arguments);
}

ExprPtr Parser::parse() {
    if (isAtEnd()) {
        throw ParseError("Unexpected end of input", tokens.back());
    }

    if (peek().type == TokenType::LPAREN) {
        return parseList();
    }

    return parseAtom();
}

std::vector<ExprPtr> Parser::parseAll() {
    std::vector<ExprPtr> expressions;
    
    while (!isAtEnd()) {
        expressions.push_back(parse());
    }
    
    return expressions;
}

}