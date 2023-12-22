cmake_minimum_required(VERSION 3.5)

project(JSONParser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/json-lexer/JSONLexer.cpp
    src/json-parser/JSONParser.cpp
    src/json-error/JSONError.cpp
    src/json-lexer/JSONToken.cpp
    src/main.cpp)

set(HEADERS
    src/json-lexer/JSONLexer.h
    src/json-parser/JSONParser.h
    src/json-error/JSONError.h
    src/json-error/JSONErrorOr.h
    src/json-lexer/JSONToken.h)

add_executable(JSONParser ${SOURCES} ${HEADERS})