#ifndef SCRIPT_H
#define SCRIPT_H

#include <string>
#include <vector>
#include <memory>
#include "RocketLanguage/Parser/Parser.h"
#include "RocketLanguage/Tokenizer/Tokenizer.h"

class Script {
private:
    std::string sourceCode;
    std::vector<Rocket::ExprPtr> parsedExpressions;
    bool isParsed;

    void parseSourceCode();

public:
    // Constructor with immediate parsing
    explicit Script(const std::string& source);
    
    // Constructor with delayed parsing
    static std::shared_ptr<Script> create(const std::string& source);
    
    // Getters
    const std::string& getSourceCode() const { return sourceCode; }
    const std::vector<Rocket::ExprPtr>& getParsedExpressions() const;
    bool getIsParsed() const { return isParsed; }
    
    // Parse the source code if not already parsed
    void ensureParsed();
    
    static std::shared_ptr<Script> loadFromFile(const std::string& filename);
    
    bool saveToFile(const std::string& filename) const;
};

#endif