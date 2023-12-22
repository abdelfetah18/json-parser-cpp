#ifndef JSON_VALUE
#define JSON_VALUE

#include "../json-error/JSONErrorOr.h"
#include "JSONArray.h"
#include "JSONBoolean.h"
#include "JSONNull.h"
#include "JSONNumber.h"
#include "JSONObject.h"
#include "JSONString.h"

// Forward Declaration
class JSONArray;

enum JSONValueType { STRING_T, NUMBER_T, OBJECT_T, ARRAY_T, BOOLEAN_T, NULL_T };
class JSONValue {
   private:
    JSONValueType m_type;
    union Value {
        JSONString* m_string;
        JSONNumber* m_number;
        JSONObject* m_object;
        JSONArray* m_array;
        JSONBoolean* m_boolean;
        JSONNull* m_null;
    } m_value;

   public:
    JSONValue(JSONString* value);
    JSONValue(JSONNumber* value);
    JSONValue(JSONObject* value);
    JSONValue(JSONArray* value);
    JSONValue(JSONBoolean* value);
    JSONValue(JSONNull* value);

    JSONErrorOr<JSONString*> getString();
    JSONErrorOr<JSONNumber*> getNumber();
    JSONErrorOr<JSONObject*> getObject();
    JSONErrorOr<JSONArray*> getArray();
    JSONErrorOr<JSONBoolean*> getBoolean();
    JSONErrorOr<JSONNull*> getNull();

    bool isString();
    bool isNumber();
    bool isObject();
    bool isArray();
    bool isBoolean();
    bool isNull();

    void print(uint64_t indent);
};

#endif