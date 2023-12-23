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

    std::string errorMessage;

    errorMessage.append("'");
    errorMessage.append(key);
    errorMessage.append("'");
    errorMessage.append(" Not Exist");

    return new JSONError(errorMessage);
}

void JSONArray::append(JSONValue* value) { m_values.push_back(value); }

JSONErrorOr<JSONValue*> JSONArray::get(uint64_t index) {
    if (index < m_values.size()) {
        return m_values[index];
    }

    std::string errorMessage;

    errorMessage.append("Index ");
    errorMessage.append(std::to_string(index));
    errorMessage.append(" out of bounds for array size ");
    errorMessage.append(std::to_string(m_values.size()));

    return new JSONError(errorMessage);
}

uint64_t JSONArray::size() { return m_values.size(); }

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

    return new JSONError("Not a String");
}

JSONErrorOr<JSONNumber*> JSONValue::getNumber() {
    if (isNumber()) {
        return m_value.m_number;
    }

    return new JSONError("Not a Number");
}

JSONErrorOr<JSONObject*> JSONValue::getObject() {
    if (isObject()) {
        return m_value.m_object;
    }

    return new JSONError("Not a Object");
}

JSONErrorOr<JSONArray*> JSONValue::getArray() {
    if (isArray()) {
        return m_value.m_array;
    }

    return new JSONError("Not a Array");
}

JSONErrorOr<JSONBoolean*> JSONValue::getBoolean() {
    if (isBoolean()) {
        return m_value.m_boolean;
    }

    return new JSONError("Not a Boolean");
}

JSONErrorOr<JSONNull*> JSONValue::getNull() {
    if (isNull()) {
        return m_value.m_null;
    }

    return new JSONError("Not a Null");
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
    if (value.isError()) {
        return value.error();
    }

    if (current()->type() != JSONTokenType::_EOF) {
        std::string errorMessage;

        errorMessage.append("Unexpected token '");
        errorMessage.append(current()->value());
        errorMessage.append("' at position ");

        return new JSONError(errorMessage, current()->pos());
    }

    m_value = value.value();

    return value;
}

JSONErrorOr<JSONObject*> JSONParser::parseObject() {
    // NOTE: parseObject is only being called when there is LEFT_CURLY_BRACE
    // TOKEN '{'

    next();
    JSONObject* object = new JSONObject();

    while (current()->type() != JSONTokenType::RIGHT_CURLY_BRACE) {
        std::string key;
        if (current()->type() != JSONTokenType::STRING) {
            return new JSONError(
                "Expect double-quoted property name in JSON at position ",
                current()->pos());
        }
        key = current()->value();
        next();
        if (current()->type() != JSONTokenType::COLON) {
            return new JSONError("Expect ':' at position ", current()->pos());
        }
        next();
        JSONErrorOr<JSONValue*> value = parseValue();
        if (value.isError()) {
            return value.error();
        }
        object->set(key, value.value());
        if (current()->type() == JSONTokenType::COMMA) {
            next();
            if (current()->type() != JSONTokenType::STRING) {
                return new JSONError(
                    "Expect double-quoted property name in JSON at position ",
                    current()->pos());
            }
        } else {
            break;
        }
    }

    if (current()->type() != JSONTokenType::RIGHT_CURLY_BRACE) {
        return new JSONError("Expect '}' at position ", current()->pos());
    }

    next();

    return object;
}

JSONErrorOr<JSONArray*> JSONParser::parseArray() {
    if (current()->type() != JSONTokenType::LEFT_SQUARE_BRACE) {
        return new JSONError("Expect '[' at position ", current()->pos());
    }
    next();
    JSONArray* array = new JSONArray();

    while (current()->type() != JSONTokenType::RIGHT_SQUARE_BRACE) {
        JSONErrorOr<JSONValue*> value = parseValue();
        if (value.isError()) {
            if (array->size() == 0) {
                return new JSONError("Expect ']' at position ",
                                     current()->pos());
            }
            return value.error();
        }
        array->append(value.value());
        if (current()->type() == JSONTokenType::COMMA) {
            next();
        } else {
            break;
        }
    }

    if (current()->type() != JSONTokenType::RIGHT_SQUARE_BRACE) {
        return new JSONError("Expect ']' at position ", current()->pos());
    }

    next();

    return array;
}

JSONErrorOr<JSONValue*> JSONParser::parseValue() {
    if (current()->type() == JSONTokenType::LEFT_CURLY_BRACE) {
        auto object = parseObject();
        if (!object.isError()) {
            return new JSONValue(object.value());
        } else {
            return object.error();
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

    std::string errorMessage;
    errorMessage.append("Unexpected Token '");
    errorMessage.append(current()->value());
    errorMessage.append("' at position ");
    return new JSONError(errorMessage, current()->pos());
}

JSONErrorOr<JSONString*> JSONParser::parseString() {
    if (current()->type() != JSONTokenType::STRING) {
        return new JSONError("Expect '\"' at position ", current()->pos());
    }

    JSONString* string = new JSONString(current()->value());

    next();

    return string;
}

JSONErrorOr<JSONNumber*> JSONParser::parseNumber() {
    // NOTE: parseNumber is only being called when there is NUMBER Token.

    std::string::size_type sz;
    double value = std::stod(current()->value(), &sz);
    JSONNumber* number = new JSONNumber(value);

    next();

    return number;
}

JSONErrorOr<JSONBoolean*> JSONParser::parseBoolean() {
    // NOTE: parseBoolean is only being called when there is BOOLEAN Token
    // 'true' or 'false'.

    JSONBoolean* boolean =
        new JSONBoolean(current()->value() == "true" ? true : false);

    next();

    return boolean;
}

JSONErrorOr<JSONNull*> JSONParser::parseNull() {
    // NOTE: parseBoolean is only being called when there is NULL_ Token 'null'.
    next();

    return new JSONNull();
}