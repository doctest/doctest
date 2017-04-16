cmake_minimum_required(VERSION 2.8)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

include(../../scripts/cmake/common.cmake)

include_directories("../../doctest/")

add_executable(${PROJECT_NAME} main.cpp)

# temporarily disabled because sometimes it hangs... probably a multiprocess sync bug in the run.py script
#doctest_add_test(NO_VALGRIND NO_OUTPUT NAME ${PROJECT_NAME} COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/run.py $<TARGET_FILE:${PROJECT_NAME}>)
