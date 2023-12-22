#include "JSONParser.h"

bool JSONObject::isEmpty() { return m_pairs.empty(); }

void JSONObject::set(std::string key, JSONValue* value) {
    m_pairs[key] = value;
}

void JSONObject::print(uint64_t indent) {
    std::cout << '{' << std::endl;

    uint64_t index = 0;
    for (auto pair : m_pairs) {
        for (uint64_t i = 0; i < indent + 4; i++) {
            std::cout << ' ';
        }
        std::cout << pair.first << ": ";
        pair.second->print(indent + 4);

        index++;
        if (index < m_pairs.size()) {
            std::cout << ',' << std::endl;
        }
    }

    std::cout << std::endl;
    for (uint64_t i = 0; i < indent; i++) {
        std::cout << ' ';
    }
    std::cout << '}';
}

JSONErrorOr<JSONValue*> JSONObject::get(std::string key) {
    if (m_pairs.contains(key)) {
        return m_pairs[key];
    }
    // TODO: Return Error NotFound.
}

void JSONArray::append(JSONValue* value) { m_values.push_back(value); }

JSONErrorOr<JSONValue*> JSONArray::get(uint64_t index) {
    if (index < m_values.size()) {
        return m_values[index];
    }
    // TODO: Return Error OutOfBounds.
}

void JSONArray::print(uint64_t indent) {
    std::cout << "[ ";

    uint64_t index = 0;
    for (auto value : m_values) {
        value->print(indent);
        index++;
        if (index < m_values.size()) {
            std::cout << ", ";
        }
    }

    std::cout << " ]";
}

JSONValue::JSONValue(JSONString* value) : m_type(JSONValueType::STRING_T) {
    m_value.m_string = value;
}

JSONValue::JSONValue(JSONNumber* value) : m_type(JSONValueType::NUMBER_T) {
    m_value.m_number = value;
}

JSONValue::JSONValue(JSONObject* value) : m_type(JSONValueType::OBJECT_T) {
    m_value.m_object = value;
}

JSONValue::JSONValue(JSONArray* value) : m_type(JSONValueType::ARRAY_T) {
    m_value.m_array = value;
}

JSONValue::JSONValue(JSONBoolean* value) : m_type(JSONValueType::BOOLEAN_T) {
    m_value.m_boolean = value;
}

JSONValue::JSONValue(JSONNull* value) : m_type(JSONValueType::NULL_T) {
    m_value.m_null = value;
}

JSONErrorOr<JSONString*> JSONValue::getString() {
    if (isString()) {
        return m_value.m_string;
    }

    // TODO: Return Error Its is not a String.
}

JSONErrorOr<JSONNumber*> JSONValue::getNumber() {
    if (isNumber()) {
        return m_value.m_number;
    }

    // TODO: Return Error Its is not a Number.
}

JSONErrorOr<JSONObject*> JSONValue::getObject() {
    if (isObject()) {
        return m_value.m_object;
    }

    // TODO: Return Error Its is not a Object.
}

JSONErrorOr<JSONArray*> JSONValue::getArray() {
    if (isArray()) {
        return m_value.m_array;
    }

    // TODO: Return Error Its is not a Array.
}

JSONErrorOr<JSONBoolean*> JSONValue::getBoolean() {
    if (isBoolean()) {
        return m_value.m_boolean;
    }

    // TODO: Return Error Its is not a Boolean.
}

JSONErrorOr<JSONNull*> JSONValue::getNull() {
    if (isNull()) {
        return m_value.m_null;
    }

    // TODO: Return Error Its is not a Null.
}

bool JSONValue::isString() { return m_type == JSONValueType::STRING_T; }

bool JSONValue::isNumber() { return m_type == JSONValueType::NUMBER_T; }

bool JSONValue::isObject() { return m_type == JSONValueType::OBJECT_T; }

bool JSONValue::isArray() { return m_type == JSONValueType::ARRAY_T; }

bool JSONValue::isBoolean() { return m_type == JSONValueType::BOOLEAN_T; }

bool JSONValue::isNull() { return m_type == JSONValueType::NULL_T; }

void JSONValue::print(uint64_t indent) {
    if (isNumber()) {
        m_value.m_number->print(indent);
    }

    if (isString()) {
        m_value.m_string->print(indent);
    }

    if (isObject()) {
        m_value.m_object->print(indent);
    }

    if (isArray()) {
        m_value.m_array->print(indent);
    }

    if (isBoolean()) {
        m_value.m_boolean->print(indent);
    }

    if (isNull()) {
        m_value.m_null->print(indent);
    }
}

std::string JSONString::value() { return m_value; }

void JSONString::print(uint64_t indent) { std::cout << '"' << value() << '"'; }

double JSONNumber::value() { return m_value; }

void JSONNumber::print(uint64_t indent) { std::cout << value(); }

bool JSONBoolean::value() { return m_value; }

void JSONBoolean::print(uint64_t indent) {
    std::cout << (value() ? "true" : "false");
}

std::string JSONNull::value() { return "null"; }

void JSONNull::print(uint64_t indent) { std::cout << value(); }

JSONParser::JSONParser(std::string input) {
    m_lexer = new JSONLexer(input);
    m_current = m_lexer->next();
}

JSONToken* JSONParser::current() { return m_current; }

JSONToken* JSONParser::next() {
    m_current = m_lexer->next();
    return m_current;
}

JSONErrorOr<JSONValue*> JSONParser::parse() {
    auto value = parseValue();
    if (!value.isError()) {
        m_value = value.value();
    } else {
        // TODO: Report Error
    }
    return value;
}

JSONErrorOr<JSONObject*> JSONParser::parseObject() {
    if (current()->type() != JSONTokenType::LEFT_CURLY_BRACE) {
        // TODO: Report Error Expected '{';
    }
    next();
    JSONObject* object = new JSONObject();

    while (current()->type() != JSONTokenType::RIGHT_CURLY_BRACE) {
        std::string key;
        if (current()->type() != JSONTokenType::STRING) {
            // TODO: Report Error Expected '"';
        }
        key = current()->value();
        next();
        if (current()->type() != JSONTokenType::COLON) {
            // TODO: Report Error Expected ':';
        }
        next();
        JSONErrorOr<JSONValue*> value = parseValue();
        if (value.isError()) {
            // TODO: Report Error.
        }
        object->set(key, value.value());
        if (current()->type() == JSONTokenType::COMMA) {
            next();
            if (current()->type() != JSONTokenType::STRING) {
                // TODO: Report Error Expected '"';
            }
        }
    }

    if (current()->type() != JSONTokenType::RIGHT_CURLY_BRACE) {
        // TODO: Report Error Expected '}';
    }

    next();

    return object;
}

JSONErrorOr<JSONArray*> JSONParser::parseArray() {
    if (current()->type() != JSONTokenType::LEFT_SQUARE_BRACE) {
        // TODO: Report Error Expected '[';
    }
    next();
    JSONArray* array = new JSONArray();

    while (current()->type() != JSONTokenType::RIGHT_SQUARE_BRACE) {
        JSONErrorOr<JSONValue*> value = parseValue();
        if (value.isError()) {
            // TODO: Report Error.
        }
        array->append(value.value());
        if (current()->type() == JSONTokenType::COMMA) {
            next();
            // TODO: Expect A JSONValue
        }
    }

    if (current()->type() != JSONTokenType::RIGHT_SQUARE_BRACE) {
        // TODO: Report Error Expected ']';
    }

    next();

    return array;
}

JSONErrorOr<JSONValue*> JSONParser::parseValue() {
    if (current()->type() == JSONTokenType::LEFT_CURLY_BRACE) {
        auto object = parseObject();
        if (!object.isError()) {
            return new JSONValue(object.value());
        }
    }

    if (current()->type() == JSONTokenType::LEFT_SQUARE_BRACE) {
        auto array = parseArray();
        if (!array.isError()) {
            return new JSONValue(array.value());
        }
    }

    if (current()->type() == JSONTokenType::STRING) {
        auto str = parseString();
        if (!str.isError()) {
            return new JSONValue(str.value());
        }
    }

    if (current()->type() == JSONTokenType::NUMBER) {
        auto number = parseNumber();
        if (!number.isError()) {
            return new JSONValue(number.value());
        }
    }

    if (current()->type() == JSONTokenType::BOOLEAN) {
        auto boolean = parseBoolean();
        if (!boolean.isError()) {
            return new JSONValue(boolean.value());
        }
    }

    if (current()->type() == JSONTokenType::NULL_) {
        auto null_ = parseNull();
        if (!null_.isError()) {
            return new JSONValue(null_.value());
        }
    }
    // TODO: Report Error Invalid Token.
}

JSONErrorOr<JSONString*> JSONParser::parseString() {
    if (current()->type() != JSONTokenType::STRING) {
        // TODO: Report Error Expected '"'.
    }

    JSONString* string = new JSONString(current()->value());

    next();

    return string;
}

JSONErrorOr<JSONNumber*> JSONParser::parseNumber() {
    if (current()->type() != JSONTokenType::NUMBER) {
        // TODO: Report Error Expected Number.
    }

    // TODO: Convert it to double
    std::string::size_type sz;
    double value = std::stod(current()->value(), &sz);
    JSONNumber* number = new JSONNumber(value);

    next();

    return number;
}

JSONErrorOr<JSONBoolean*> JSONParser::parseBoolean() {
    if (current()->type() != JSONTokenType::BOOLEAN) {
        // TODO: Report Error Expected Boolean.
    }

    JSONBoolean* boolean =
        new JSONBoolean(current()->value() == "true" ? true : false);

    next();

    return boolean;
}

JSONErrorOr<JSONNull*> JSONParser::parseNull() {
    if (current()->type() != JSONTokenType::NULL_) {
        // TODO: Report Error Expected Boolean.
    }
    next();

    return new JSONNull();
}