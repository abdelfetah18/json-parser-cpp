#ifndef JSON_ARRAY
#define JSON_ARRAY

#include <vector>

#include "JSONValue.h"

class JSONArray {
   private:
    std::vector<JSONValue*> m_values;

   public:
    void append(JSONValue* value);
    JSONErrorOr<JSONValue*> get(uint64_t index);
    uint64_t size();

    void print(uint64_t indent);
};

#endif