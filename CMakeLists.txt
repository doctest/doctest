cmake_minimum_required(VERSION 2.8)

project(all)

option(CI_COVERAGE "coverage" OFF)

enable_testing()

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

if(CI_COVERAGE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
    
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/scripts/coveralls-cmake/cmake)
	include(Coveralls)
	coveralls_turn_on_coverage()
    
	coveralls_setup("${CMAKE_SOURCE_DIR}/doctest/doctest.h" ON "${CMAKE_SOURCE_DIR}/scripts/coveralls-cmake/cmake")
endif()

include_directories("doctest/") # this is needed here so the coverage tools work - because the "../../doctest" relative path will confuse it

add_subdirectory(examples/dev_testing)
add_subdirectory(examples/disabled)
add_subdirectory(examples/hello_world)
add_subdirectory(examples/multi_dll)
add_subdirectory(examples/multiprocess)
add_subdirectory(examples/stress_test)
