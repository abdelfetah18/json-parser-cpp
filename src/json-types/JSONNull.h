#ifndef JSON_NULL
#define JSON_NULL

#include <string>
#include <cinttypes>

class JSONNull {
   public:
    std::string value();
    void print(uint64_t indent);
};

#endif