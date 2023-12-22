#pragma once

#include <cstdint>
#include <iostream>
#include <string>

#include "JSONToken.h"

class JSONLexer {
   private:
    std::string m_input;
    uint64_t m_pos = 0;

   public:
    JSONLexer(std::string input) : m_input(input) {}
    void advance(uint64_t num);
    uint64_t pos();
    char current();
    bool isEOF();
    bool match(char c);
    bool isWhiteSpace(char c);
    bool isStringLiteral(std::string str);
    bool isDigit();
    bool isControlChar();
    void skipWhiteSpace();

    JSONToken* getTokenAndAdvance(std::string value, uint64_t pos,
                                  JSONTokenType type, uint64_t num);
    JSONToken* getTokenAndAdvance(std::string value, uint64_t pos,
                                  JSONTokenType type);
    JSONToken* next();
};
