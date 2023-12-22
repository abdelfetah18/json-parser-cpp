#include "JSONToken.h"

std::string JSONToken::value() { return m_value; }
uint64_t JSONToken::pos() { return m_pos; }
JSONTokenType JSONToken::type(){ return m_type; }
