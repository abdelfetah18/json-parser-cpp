#include "JSONLexer.h"

void JSONLexer::advance(uint64_t num) { m_pos += num; }

uint64_t JSONLexer::pos() { return m_pos; }

char JSONLexer::current() { return m_input[m_pos]; };

bool JSONLexer::match(char c) { return c == current(); }
bool JSONLexer::isEOF() { return m_pos >= m_input.length(); }

bool JSONLexer::isWhiteSpace(char c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

JSONToken *JSONLexer::getTokenAndAdvance(std::string value, uint64_t pos,
                                         JSONTokenType type, uint64_t num) {
    JSONToken *token = new JSONToken(value, pos, type);
    advance(num);
    return token;
}

JSONToken *JSONLexer::getTokenAndAdvance(std::string value, uint64_t pos,
                                         JSONTokenType type) {
    return getTokenAndAdvance(value, pos, type, 1);
}

bool JSONLexer::isStringLiteral(std::string str) {
    uint64_t pos = 0;
    while (!isEOF() && pos < str.length()) {
        if (m_input[m_pos + pos] != str[pos]) {
            return false;
        }
        pos += 1;
    }
    return true;
}

bool JSONLexer::isDigit() { return current() >= '0' && current() <= '9'; }
bool JSONLexer::isControlChar() {
    // FIXME: CHeck agains control chars.
    return false;
}

void JSONLexer::skipWhiteSpace() {
    while (!isEOF() && isWhiteSpace(current())) {
        advance(1);
    }
}

JSONToken *JSONLexer::next() {
    skipWhiteSpace();
    if (pos() >= m_input.length()) {
        return new JSONToken("EOF", pos(), JSONTokenType::_EOF);
    }

    if (match('{')) {
        return getTokenAndAdvance("{", pos(), JSONTokenType::LEFT_CURLY_BRACE);
    }

    if (match('}')) {
        return getTokenAndAdvance("}", pos(), JSONTokenType::RIGHT_CURLY_BRACE);
    }

    if (match(',')) {
        return getTokenAndAdvance(",", pos(), JSONTokenType::COMMA);
    }

    if (match(':')) {
        return getTokenAndAdvance(":", pos(), JSONTokenType::COLON);
    }

    if (match('[')) {
        return getTokenAndAdvance("[", pos(), JSONTokenType::LEFT_SQUARE_BRACE);
    }

    if (match(']')) {
        return getTokenAndAdvance("]", pos(),
                                  JSONTokenType::RIGHT_SQUARE_BRACE);
    }

    if (isWhiteSpace(current())) {
        std::string c;
        c.push_back(current());
        return getTokenAndAdvance(c, pos(), JSONTokenType::WHITE_SPACE);
    }

    if (isStringLiteral("true")) {
        return getTokenAndAdvance("true", pos(), JSONTokenType::BOOLEAN, 4);
    }

    if (isStringLiteral("false")) {
        return getTokenAndAdvance("false", pos(), JSONTokenType::BOOLEAN, 5);
    }

    if (isStringLiteral("null")) {
        return getTokenAndAdvance("null", pos(), JSONTokenType::NULL_, 4);
    }

    if (match('"')) {
        advance(1);
        std::string str;
        while (!isEOF() && !isControlChar() && !match('"')) {
            if (match('\\')) {
                advance(1);
                str.push_back(current());
            }else{
                str.push_back(current());
            }
            advance(1);
        }

        if (match('"')) {
            advance(1);
            return new JSONToken(str, pos() - str.length(),
                                 JSONTokenType::STRING);
        }
        // FIXME: Report Error.
        return new JSONToken(str, pos(), JSONTokenType::_INVALID);
    }

    if (isDigit() || match('-')) {
        std::string number;
        number.push_back(current());
        advance(1);
        while (isDigit()) {
            number.push_back(current());
            advance(1);
        }
        if (match('.')) {
            number.push_back(current());
            advance(1);
            while (isDigit()) {
                number.push_back(current());
                advance(1);
            }
        }

        if (match('e') || match('E')) {
            number.push_back(current());
            advance(1);
            if (match('-') || match('+')) {
                number.push_back(current());
                advance(1);
            }
            while (isDigit()) {
                number.push_back(current());
                advance(1);
            }
        }

        return new JSONToken(number, pos() - number.length(),
                             JSONTokenType::NUMBER);
    }

    std::string invalid;
    invalid.push_back(current());
    return new JSONToken(invalid, pos(), JSONTokenType::_INVALID);
}