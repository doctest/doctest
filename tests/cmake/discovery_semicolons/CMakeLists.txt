cmake_minimum_required(VERSION 3.14)
project(discovery_semicolons LANGUAGES CXX)
enable_testing()
add_executable(discovery_semicolons main.cpp)
target_include_directories(discovery_semicolons PRIVATE "${DOCTEST_SOURCE_DIR}")
include("${DOCTEST_SOURCE_DIR}/scripts/cmake/doctest.cmake")
doctest_discover_tests(discovery_semicolons
    TEST_LIST discovered_tests
    PROPERTIES PASS_REGULAR_EXPRESSION "assertions: 1"
)
set_property(DIRECTORY APPEND PROPERTY TEST_INCLUDE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/verify.cmake")
