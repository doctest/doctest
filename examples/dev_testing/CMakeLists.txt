cmake_minimum_required(VERSION 2.8)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

include(../../scripts/flags.cmake)

add_definitions(-DDOCTEST_CONFIG_SHORT_MACRO_NAMES)

include_directories("../../doctest/")

add_executable(${PROJECT_NAME} main.cpp)

add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}> -dt-count=1)
