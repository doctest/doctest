cmake_minimum_required(VERSION 2.8)

project(ci)

enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/ci/coveralls-cmake/cmake)
option(COVERALLS "Generate coveralls data" OFF)

if(COVERALLS)
    include(Coveralls)
    coveralls_turn_on_coverage()
    coveralls_setup("doctest/doctest.h" ON "${PROJECT_SOURCE_DIR}/ci/coveralls-cmake/cmake")
endif()

add_subdirectory(examples/dev_testing)

add_test(coverage exe)
