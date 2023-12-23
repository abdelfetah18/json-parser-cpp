#pragma once

#include <string>
#include <cinttypes>

class JSONError {
   private:
    std::string m_message;
    uint64_t m_pos = 0;
    bool hasPosition = false;

   public:
    JSONError(std::string error_message, uint64_t pos) : m_message(error_message), m_pos(pos), hasPosition(true) { };
    JSONError(std::string error_message) : m_message(error_message), hasPosition(false) { };
    std::string message();
};