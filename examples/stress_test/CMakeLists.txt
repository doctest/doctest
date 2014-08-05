cmake_minimum_required(VERSION 2.8)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Clang dies on linking this example - shame... - hope it gets a lot faster in the future
    # GCC just takes a lot of time - want faster travis builds
    return()
endif()

project(stress_test)

include(../../scripts/flags.cmake)

include_directories("../../doctest/")

if(MSVC)
	add_definitions(/bigobj)
endif()

add_executable(${PROJECT_NAME} main.cpp test.cpp)

add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
