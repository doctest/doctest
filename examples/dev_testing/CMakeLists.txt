cmake_minimum_required(VERSION 2.8)

project(dev_testing)

add_definitions(-DDOCTEST_CONFIG_SHORT_MACRO_NAMES)

include_directories("../../doctest/")

add_executable(${PROJECT_NAME} main.cpp)

add_test(${PROJECT_NAME} ${PROJECT_NAME})
