cmake_minimum_required(VERSION 3.0)

project(playground)

include(../cmake/common.cmake)

include_directories(../../doctest/)

doctest_add_executable(${PROJECT_NAME} main.cpp test.cpp)

doctest_add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}> -nv)
