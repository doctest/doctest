cmake_minimum_required(VERSION 2.8)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

include(../../scripts/common.cmake)

# suppress all warnings because this is not a real example of using the library
if(NOT MSVC)
    add_compiler_flags(-w)
else()
    add_compiler_flags(/w)
endif()

include_directories("../../doctest/")

add_executable(${PROJECT_NAME} main.cpp test.cpp)

add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}> -dt-count=1)
