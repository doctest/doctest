cmake_minimum_required(VERSION 2.8)

project(dev_testing)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --save-temps -std=c++98 -pedantic -m64 -O2 -fvisibility=hidden -fstrict-aliasing")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -Wall -Wextra -Wconversion -Wno-missing-field-initializers -Wold-style-cast -Wfloat-equal -Wlogical-op -Wundef -Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wwrite-strings -Wpointer-arith -Wcast-qual -Wformat=2 -Wswitch-default -Wmissing-include-dirs -Wcast-align -Wformat-nonliteral -Wparentheses -Winit-self -Wuninitialized -Wswitch-enum -Wno-endif-labels -Wunused-function")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-global-constructors")
endif()

if(MSVC)
    add_definitions(/W4)
endif()

add_definitions(-DDOCTEST_CONFIG_SHORT_MACRO_NAMES)
add_definitions(-DDOCTEST_CONFIG_IMPLEMENT)

include_directories("../../doctest/")

add_executable(exe main.cpp
    #test.cpp
)

add_test(dev_testing ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/exe)