cmake_minimum_required(VERSION 2.8)

project(disabled)

include_directories("../../doctest/")

add_executable(${PROJECT_NAME} main.cpp)

add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
