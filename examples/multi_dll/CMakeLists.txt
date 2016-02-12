cmake_minimum_required(VERSION 2.8)

project(multi_dll)

include_directories("../../doctest/")

add_library(dll_1 SHARED dll.cpp)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} dll_1)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
