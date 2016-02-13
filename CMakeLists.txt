cmake_minimum_required(VERSION 2.8)

project(all)

enable_testing()

option(CI_COVERAGE "coverage" OFF)

macro(add_flags)
    foreach(flag ${ARGV})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
    endforeach()
endmacro()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_flags(--save-temps)
endif()

if(CI_COVERAGE)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/scripts/coveralls-cmake/cmake)
    include(Coveralls)
    coveralls_turn_on_coverage()
    
    coveralls_setup("${CMAKE_SOURCE_DIR}/doctest/doctest.h" ON "${PROJECT_SOURCE_DIR}/scripts/coveralls-cmake/cmake")
endif()

include_directories("doctest/")

add_executable(${PROJECT_NAME} main.cpp)

add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
