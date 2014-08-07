cmake_minimum_required(VERSION 2.8)

project(disabled)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	set(CMAKE_CXX_FLAGS "-m64 -Wall -Wextra --save-temps -std=c++98 -pedantic ${CMAKE_CXX_FLAGS}")
endif()

add_definitions(-DDOCTEST_GLOBAL_DISABLE)

include_directories("../../")

add_executable(exe main.cpp)