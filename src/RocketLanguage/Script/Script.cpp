#include "Script.h"
#include <fstream>
#include <stdexcept>

Script::Script(const std::string& source) 
    : sourceCode(source), isParsed(false) {
    parseSourceCode();
}

std::shared_ptr<Script> Script::create(const std::string& source) {
    return std::make_shared<Script>(source);
}

void Script::parseSourceCode() {
    if (isParsed) return;

    try {
        // Tokenize the source code
        Rocket::Tokenizer tokenizer(sourceCode);
        auto tokens = tokenizer.tokenize();

        // Parse the tokens into expressions
        Rocket::Parser parser(tokens);
        parsedExpressions = parser.parseAll();

        isParsed = true;
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to parse script: " + std::string(e.what()));
    }
}

const std::vector<Rocket::ExprPtr>& Script::getParsedExpressions() const {
    if (!isParsed) {
        throw std::runtime_error("Script has not been parsed yet");
    }
    return parsedExpressions;
}

void Script::ensureParsed() {
    if (!isParsed) {
        parseSourceCode();
    }
}

std::shared_ptr<Script> Script::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open script file: " + filename);
    }

    std::string source;
    std::string line;
    while (std::getline(file, line)) {
        source += line + "\n";
    }

    return create(source);
}

bool Script::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    file << sourceCode;
    return true;
}