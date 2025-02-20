#include "Tokenizer.h"
#include <sstream>
#include <stdexcept>

namespace Rocket {

Token::Token(TokenType t, std::string v, int l, int c)
    : type(t), value(std::move(v)), line(l), column(c) {}

std::string Token::toString() const {
    std::stringstream ss;
    ss << "Token{type=";
    switch (type) {
        case TokenType::NUMBER: ss << "NUMBER"; break;
        case TokenType::SYMBOL: ss << "SYMBOL"; break;
        case TokenType::LPAREN: ss << "LPAREN"; break;
        case TokenType::RPAREN: ss << "RPAREN"; break;
        case TokenType::BINDING: ss << "BINDING"; break;
        case TokenType::END: ss << "END"; break;
    }
    ss << ", value='" << value << "'}";
    return ss.str();
}

Tokenizer::Tokenizer(std::string source)
    : input(std::move(source)), position(0), line(1), column(1) {}

char Tokenizer::peek() const {
    if (isAtEnd()) return '\0';
    return input[position];
}

char Tokenizer::advance() {
    char current = peek();
    position++;
    column++;
    if (current == '\n') {
        line++;
        column = 1;
    }
    return current;
}

void Tokenizer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            advance();
        } else {
            break;
        }
    }
}

Token Tokenizer::readNumber() {
    int startColumn = column;
    std::string number;
    
    if (peek() == '-') {
        number += advance();
    }
    
    while (!isAtEnd() && isdigit(peek())) {
        number += advance();
    }
    
    if (peek() == '.') {
        number += advance();
        
        if (!isdigit(peek())) {
            throw std::runtime_error("Expected digit after decimal point");
        }
        
        while (!isAtEnd() && isdigit(peek())) {
            number += advance();
        }
    }
    
    return Token(TokenType::NUMBER, number, line, startColumn);
}

Token Tokenizer::readSymbol() {
    int startColumn = column;
    std::string symbol;
    
    while (!isAtEnd()) {
        char c = peek();
        if (isalnum(c) || c == '-' || c == '+' || c == '*' || c == '/' || c == '_') {
            symbol += advance();
        } else {
            break;
        }
    }
    
    return Token(TokenType::SYMBOL, symbol, line, startColumn);
}

Token Tokenizer::readBinding() {
    int startColumn = column;
    advance(); // Skip the $
    
    std::string name;
    while (!isAtEnd()) {
        char c = peek();
        if (isalnum(c) || c == '_') {
            name += advance();
        } else {
            break;
        }
    }
    
    if (name.empty()) {
        throw std::runtime_error("Expected binding name after $");
    }
    
    return Token(TokenType::BINDING, name, line, startColumn);
}

bool Tokenizer::isAtEnd() const {
    return position >= input.length();
}

Token Tokenizer::nextToken() {
    skipWhitespace();
    
    if (isAtEnd()) {
        return Token(TokenType::END, "", line, column);
    }
    
    char c = peek();
    
    if (c == '(') {
        advance();
        return Token(TokenType::LPAREN, "(", line, column - 1);
    }
    
    if (c == ')') {
        advance();
        return Token(TokenType::RPAREN, ")", line, column - 1);
    }
    
    if (c == '$') {
        return readBinding();
    }
    
    if (isdigit(c) || (c == '-' && position + 1 < input.length() && isdigit(input[position + 1]))) {
        return readNumber();
    }
    
    return readSymbol();
}

std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> tokens;
    while (true) {
        Token token = nextToken();
        tokens.push_back(token);
        if (token.type == TokenType::END) break;
    }
    return tokens;
}

}