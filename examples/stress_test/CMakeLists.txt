cmake_minimum_required(VERSION 2.8)

project(stress_test)

include_directories("../../doctest/")

if(MSVC)
	add_definitions(/bigobj)
endif()

add_executable(${PROJECT_NAME} main.cpp test.cpp)

add_test(${PROJECT_NAME} ${PROJECT_NAME})
