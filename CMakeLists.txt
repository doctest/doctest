cmake_minimum_required(VERSION 2.8)

project(ci)

option(CI_COVERAGE "coverage" OFF)

enable_testing()


if(CI_COVERAGE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
    
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/ci/coveralls-cmake/cmake)
	include(Coveralls)
	coveralls_turn_on_coverage()
    
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    #-g -O0 -fprofile-arcs -ftest-coverage
    #-coverage -O0
    
    # make all tests end up in the root of the repo
    #SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
    #SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
    #SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

add_subdirectory(examples/dev_testing)

if(CI_COVERAGE)
	coveralls_setup(
        "${PROJECT_SOURCE_DIR}/doctest/doctest.h"
        ON
        "${PROJECT_SOURCE_DIR}/ci/coveralls-cmake/cmake")
endif()
