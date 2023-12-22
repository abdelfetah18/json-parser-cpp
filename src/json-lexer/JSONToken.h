#pragma once

#include <cinttypes>
#include <string>

enum JSONTokenType {
    LEFT_CURLY_BRACE,
    RIGHT_CURLY_BRACE,
    COMMA,
    COLON,
    LEFT_SQUARE_BRACE,
    RIGHT_SQUARE_BRACE,
    WHITE_SPACE,
    BOOLEAN,
    NULL_,
    DOUBLE_QUOTES,
    BACK_SLASH,
    DOUBLE_BACK_SLASH,
    BACKSPACE,
    FORMFEED,
    LINEFEED,
    CARRIAGE_RETURN,
    HORIZONTAL_TAB,
    UNICODE_CHAR,
    STRING,
    MINUS,
    PLUS,
    NUMBER,
    POINT,
    _EOF,
    _INVALID
};

class JSONToken {
   private:
    std::string m_value;
    uint64_t m_pos;
    JSONTokenType m_type;

   public:
    JSONToken(std::string value, uint64_t pos, JSONTokenType type)
        : m_value(value), m_pos(pos), m_type(type) {}
    std::string value();
    uint64_t pos();
    JSONTokenType type();
};
