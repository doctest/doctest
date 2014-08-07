cmake_minimum_required(VERSION 2.6)

project(hello_world)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	set(CMAKE_CXX_FLAGS "-m64 -Wall -Wextra -pedantic ${CMAKE_CXX_FLAGS}")
endif()

include_directories("../../")

add_executable(hello_world main.cpp)