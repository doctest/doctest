cmake_minimum_required(VERSION 2.8)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

include(../../scripts/cmake/common.cmake)

include_directories("../../doctest/")

doctest_add_library(lib_1 STATIC lib_1_src1.cpp lib_1_src2.cpp)
doctest_add_library(lib_2 STATIC lib_2_src.cpp)

doctest_add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} lib_1)
target_link_libraries(${PROJECT_NAME} lib_2)

doctest_add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)

# pulls the force linking functionality
include(doctest_force_link_static_lib_in_target.cmake)

doctest_force_link_static_lib_in_target(${PROJECT_NAME} lib_1)
doctest_force_link_static_lib_in_target(${PROJECT_NAME} lib_2)
