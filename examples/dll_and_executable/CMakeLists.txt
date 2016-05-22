cmake_minimum_required(VERSION 2.8)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

include(../../scripts/common.cmake)

if(DEFINED ENV{TRAVIS} AND APPLE)
    set(CMAKE_MACOSX_RPATH ON)
    # force 64 bit for OSX because -m32 fails >>> SOMETIMES <<< (x86_64 vs 386 - like the dll is not linked with -m32...)
    add_compiler_flags(-m64)
endif()

include_directories("../../doctest/")

add_library(dll SHARED dll.cpp)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} dll)

add_test(NO_OUTPUT NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
