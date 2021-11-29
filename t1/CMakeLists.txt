cmake_minimum_required(VERSION 3.10)

project(hello_cmake)

SET(SRC_LIST ../main.cpp)

message(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})
message(STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${SRC_LIST})

