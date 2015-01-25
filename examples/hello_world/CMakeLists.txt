cmake_minimum_required(VERSION 2.8)

project(hello_world)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "-m64 -Wall -Wextra --save-temps -std=c++98 -pedantic -Wconversion -Werror -fvisibility=hidden ${CMAKE_CXX_FLAGS}")
endif()

include_directories("../../doctest/")

add_executable(exe main.cpp)