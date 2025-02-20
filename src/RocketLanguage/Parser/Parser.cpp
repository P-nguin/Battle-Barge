#include "Parser.h"

namespace Rocket {

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

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw ParseError(message, peek());
}

ExprPtr Parser::parseAtom() {
    Token token = advance();
    switch (token.type) {
        case TokenType::NUMBER:
            return std::make_shared<NumberExpr>(std::stod(token.value));
        case TokenType::SYMBOL:
            return std::make_shared<CommandExpr>(token.value, std::vector<ExprPtr>());
        case TokenType::REGISTER:
            return std::make_shared<RegisterExpr>(token.value);
        default:
            throw ParseError("Unexpected token", token);
    }
}

ExprPtr Parser::parseList() {
    // Consume the left parenthesis
    consume(TokenType::LPAREN, "Expected '('");
    
    // Parse the command/operator name
    if (!check(TokenType::SYMBOL)) {
        throw ParseError("Expected command or operator", peek());
    }
    Token commandToken = advance();
    std::vector<ExprPtr> arguments;
    
    // Parse arguments until we hit the closing parenthesis
    while (!check(TokenType::RPAREN)) {
        if (isAtEnd()) {
            throw ParseError("Unterminated list, expected ')'", tokens.back());
        }
        arguments.push_back(parse());
    }
    
    // Consume the closing parenthesis
    consume(TokenType::RPAREN, "Expected ')'");
    
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

}