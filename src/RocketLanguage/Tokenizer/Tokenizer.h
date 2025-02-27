#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

namespace Rocket {

enum class TokenType {
    NUMBER,
    STRING,
    SYMBOL,     // forward, +, *, /
    LPAREN,     // (
    RPAREN,     // )
    BINDING,    // $pos_x, $pos_y, $direction, $id
    END         // End of input marker
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;

    Token(TokenType t, std::string v, int l, int c);
    std::string toString() const;
};

class Tokenizer {
private:
    std::string input;
    size_t position;
    int line;
    int column;

    char peek() const;
    char advance();
    void skipWhitespace();
    
    Token readNumber();
    Token readString();
    Token readSymbol();
    Token readBinding();
    bool isAtEnd() const;

public:
    explicit Tokenizer(std::string source);
    Token nextToken();
    std::vector<Token> tokenize();
};

}

#endif