cmake_minimum_required(VERSION 2.8)

project(disabled)

include_directories("../../doctest/")

add_definitions(-DDOCTEST_GLOBAL_DISABLE)

add_executable(${PROJECT_NAME} main.cpp)

add_test(${PROJECT_NAME} ${PROJECT_NAME})
