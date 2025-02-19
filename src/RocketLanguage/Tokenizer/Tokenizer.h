#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

namespace Rocket {

// Token types that our language will recognize
enum class TokenType {
    NUMBER,     // Numeric literals (e.g., 42, -3.14)
    SYMBOL,     // Commands and operators (e.g., forward, +, *)
    LPAREN,     // Left parenthesis (
    RPAREN,     // Right parenthesis )
    REGISTER,   // Register references (e.g., $pos, $heading)
    END         // End of input marker
};

// Token structure to hold the token information
struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;

    Token(TokenType t, std::string v, int l, int c);
    
    // Helper method to convert token to string for debugging
    std::string toString() const;
};

class Tokenizer {
public:
    // Constructor takes the source code as input
    explicit Tokenizer(std::string source);
    
    // Get the next token from the input
    Token nextToken();
    
    // Helper methods to get all tokens at once
    std::vector<Token> tokenize();
    
    // Get current position information
    int getCurrentLine() const;
    int getCurrentColumn() const;

private:
    std::string input;        // Input source code
    size_t position;          // Current position in input
    int line;                 // Current line number
    int column;               // Current column number
    
    char peek() const;        // Look at current character without consuming
    char advance();           // Get current character and move to next
    void skipWhitespace();    // Skip over spaces, tabs, newlines
    
    Token readNumber();       // Read a numeric token
    Token readSymbol();       // Read a symbol token
    Token readRegister();     // Read a register token
    
    bool isAtEnd() const;
};

} // namespace RobotLang

#endif // TOKENIZER_H