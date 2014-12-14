cmake_minimum_required(VERSION 2.8)

project(dev_testing)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
	set(CMAKE_C_FLAGS "-Wold-style-definition -Wnested-externs -Wbad-function-cast -Wdeclaration-after-statement -Wmissing-prototypes -Wstrict-prototypes ${CMAKE_C_FLAGS}")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	set(CMAKE_CXX_FLAGS "--save-temps -std=c++98 -pedantic -m64 -O2 -ansi -fvisibility=hidden -fstrict-aliasing -Wall -Wextra -Wconversion -Wno-missing-field-initializers -Wold-style-cast -Wfloat-equal -Wundef -Wredundant-decls -Wlogical-op -Wshadow -Wstrict-overflow=4 -Wpointer-arith -Wcast-qual -Wformat=2 -Wswitch-default -Wmissing-include-dirs -Wcast-align ${CMAKE_CXX_FLAGS}")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "-Weverything ${CMAKE_CXX_FLAGS}")
endif()

add_definitions(-DDOCTEST_SHORT_MACRO_NAMES)

include_directories("../../")

add_executable(exe main.cpp test.cpp)