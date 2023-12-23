#include "JSONError.h"

#include <iostream>

std::string JSONError::message() {
    if (hasPosition) {
        std::string str = m_message;
        str.append(std::to_string(m_pos));
        return str;
    }

    return m_message;
}