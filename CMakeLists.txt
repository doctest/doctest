cmake_minimum_required(VERSION 2.8)

project(ci)

add_option(CI_COVERAGE "coverage" OFF)

enable_testing()

if(CI_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -coverage -O0")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -coverage -O0")
    #-g -O0 -fprofile-arcs -ftest-coverage
endif()

add_subdirectory(examples/dev_testing)
