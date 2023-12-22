#pragma once

#include <cstdint>
#include <iostream>

#include "../json-error/JSONErrorOr.h"
#include "../json-lexer/JSONLexer.h"
#include "../json-types/JSONArray.h"
#include "../json-types/JSONBoolean.h"
#include "../json-types/JSONNull.h"
#include "../json-types/JSONNumber.h"
#include "../json-types/JSONObject.h"
#include "../json-types/JSONString.h"
#include "../json-types/JSONValue.h"

class JSONParser {
   private:
    JSONLexer* m_lexer = nullptr;
    JSONValue* m_value = nullptr;
    JSONToken* m_current;

    JSONToken* current();
    JSONToken* next();

   public:
    JSONParser(std::string input);
    JSONErrorOr<JSONValue*> parse();
    JSONErrorOr<JSONObject*> parseObject();
    JSONErrorOr<JSONArray*> parseArray();
    JSONErrorOr<JSONValue*> parseValue();
    JSONErrorOr<JSONString*> parseString();
    JSONErrorOr<JSONNumber*> parseNumber();
    JSONErrorOr<JSONBoolean*> parseBoolean();
    JSONErrorOr<JSONNull*> parseNull();
};
