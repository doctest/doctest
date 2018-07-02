cmake_minimum_required(VERSION 3.0)

project(playground)

include(../cmake/common.cmake)

include_directories(../../doctest/)

doctest_add_executable(${PROJECT_NAME} main.cpp test.cpp)

doctest_add_test(NAME ${PROJECT_NAME} NO_OUTPUT COMMAND $<TARGET_FILE:${PROJECT_NAME}> -nv)

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ctest -C $<CONFIGURATION> --output-on-failure)
