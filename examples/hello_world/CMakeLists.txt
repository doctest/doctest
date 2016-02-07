cmake_minimum_required(VERSION 2.8)

project(hello_world)

include_directories("../../doctest/")

add_executable(${PROJECT_NAME} main.cpp)

add_test(${PROJECT_NAME} ${PROJECT_NAME})
