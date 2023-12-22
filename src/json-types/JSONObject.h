#ifndef JSON_OBJECT
#define JSON_OBJECT

#include <string>
#include <unordered_map>

#include "JSONValue.h"

// Forward Declaration
class JSONValue;

class JSONObject {
   private:
    std::unordered_map<std::string, JSONValue*> m_pairs;

   public:
    bool isEmpty();
    void set(std::string key, JSONValue* value);
    JSONErrorOr<JSONValue*> get(std::string key);
    // TODO: Maybe add method for each type.

    void print(uint64_t indent);
};

#endif