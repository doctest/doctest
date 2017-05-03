cmake_minimum_required(VERSION 3.0)

project(playground)

include(../cmake/common.cmake)

# not using doctest_add_executable because the "doctest" target might not be visible
# since it's defined in the root "CMakeLists.txt" file and not im "common.cmake"
include_directories("../../doctest/")
add_executable(${PROJECT_NAME} main.cpp test.cpp)
add_dependencies(${PROJECT_NAME} assemble_single_header)

doctest_add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}> -nv)
