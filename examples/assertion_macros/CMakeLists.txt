cmake_minimum_required(VERSION 2.8)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

include(../../scripts/common.cmake)

include_directories("../../doctest/")

add_executable(${PROJECT_NAME} main.cpp)

if(DEFINED ENV{TRAVIS} AND APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.5)
    # don't do tests for GCC 4.4 on OSX - the output will be different from the reference output
    # for some reason the summary after the tests run isn't printed... see this build - https://travis-ci.org/onqtam/doctest/jobs/135979391
    return()
endif()

add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
