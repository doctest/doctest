cmake_minimum_required(VERSION 2.8)

project(multi_dll)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	set(CMAKE_CXX_FLAGS "-m64 -Wall -Wextra --save-temps -std=c++98 -pedantic ${CMAKE_CXX_FLAGS}")
endif()

include_directories("../../")

add_library(dll_1 SHARED dll.cpp)

add_executable(multi_dll main.cpp)
target_link_libraries(multi_dll dll_1)