cmake_minimum_required(VERSION 2.8)

project(stress_test)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	set(CMAKE_CXX_FLAGS "-m64 -Wall -Wextra --save-temps -std=c++98 -pedantic -Wconversion -Werror -fvisibility=hidden ${CMAKE_CXX_FLAGS}")
endif()

include_directories("../../")

if(MSVC)
	add_definitions(/bigobj)
endif()

add_executable(exe main.cpp test.cpp)