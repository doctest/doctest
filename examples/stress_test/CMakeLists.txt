cmake_minimum_required(VERSION 2.8)

if(APPLE)
    return() # compilers on osx just hang on this one...
endif()

project(stress_test)

include_directories("../../doctest/")

if(MSVC)
	add_definitions(/bigobj)
endif()

add_executable(${PROJECT_NAME} main.cpp test.cpp)

add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
