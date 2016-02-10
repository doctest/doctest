cmake_minimum_required(VERSION 2.8)

project(all)

option(CI_COVERAGE "coverage" OFF)

enable_testing()

include(scripts/ucm/cmake/ucm.cmake)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    #ucm_add_flags(-Werror)
    ucm_add_flags(--save-temps)
    ucm_add_flags(-std=c++98)
    ucm_add_flags(-pedantic)
    ucm_add_flags(-m64)
    ucm_add_flags(-O2)
    ucm_add_flags(-fvisibility=hidden)
    ucm_add_flags(-fstrict-aliasing)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    ucm_add_flags(-ansi)
    ucm_add_flags(-Wall)
    ucm_add_flags(-Wextra)
    ucm_add_flags(-Wconversion)
    ucm_add_flags(-Wno-missing-field-initializers)
    ucm_add_flags(-Wold-style-cast)
    ucm_add_flags(-Wfloat-equal)
    ucm_add_flags(-Wlogical-op)
    ucm_add_flags(-Wundef)
    ucm_add_flags(-Wredundant-decls)
    ucm_add_flags(-Wshadow)
    ucm_add_flags(-Wstrict-overflow=5)
    ucm_add_flags(-Wwrite-strings)
    ucm_add_flags(-Wpointer-arith)
    ucm_add_flags(-Wcast-qual)
    ucm_add_flags(-Wformat=2)
    ucm_add_flags(-Wswitch-default)
    ucm_add_flags(-Wmissing-include-dirs)
    ucm_add_flags(-Wcast-align)
    ucm_add_flags(-Wformat-nonliteral)
    ucm_add_flags(-Wparentheses)
    ucm_add_flags(-Winit-self)
    ucm_add_flags(-Wuninitialized)
    ucm_add_flags(-Wswitch-enum)
    ucm_add_flags(-Wno-endif-labels)
    ucm_add_flags(-Wunused-function)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    ucm_add_flags(-Weverything)
    ucm_add_flags(-Wno-global-constructors)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    ucm_add_flags(/W4)
endif()

if(CI_COVERAGE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
    
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/scripts/coveralls-cmake/cmake)
    include(Coveralls)
    coveralls_turn_on_coverage()
    
    coveralls_setup("${CMAKE_SOURCE_DIR}/doctest/doctest.h" ON "${CMAKE_SOURCE_DIR}/scripts/coveralls-cmake/cmake")
endif()

ucm_print_flags()

include_directories("doctest/") # this is needed here so the coverage tools work - because the "../../doctest" relative path will confuse it

add_subdirectory(examples/dev_testing)
add_subdirectory(examples/disabled)
add_subdirectory(examples/hello_world)
add_subdirectory(examples/multi_dll)
add_subdirectory(examples/multiprocess)
add_subdirectory(examples/stress_test)
