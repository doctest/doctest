cmake_minimum_required(VERSION 2.8)

project(ci)

enable_testing()

option(COVERALLS "Generate coverage data" OFF)

if(COVERALLS)
    include(Coveralls)
    coveralls_turn_on_coverage()
    coveralls_setup("doctest/doctest.h" ON "${PROJECT_SOURCE_DIR}/ci/coveralls-cmake/cmake")
endif()

add_subdirectory(examples/dev_testing)
