#pragma once

#include <string>

class JSONError {
   private:
    std::string m_message;

   public:
    std::string message();
};