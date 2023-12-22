#ifndef JSON_STRING
#define JSON_STRING

#include <string>

class JSONString {
   private:
    std::string m_value;

   public:
    JSONString(std::string value) : m_value(value) {}
    std::string value();

    void print(uint64_t indent);
};

#endif