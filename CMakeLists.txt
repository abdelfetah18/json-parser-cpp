cmake_minimum_required(VERSION 3.5)

project(JSONParser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp)

set(HEADERS )

add_executable(JSONParser ${SOURCES} ${HEADERS})