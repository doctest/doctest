cmake_minimum_required(VERSION 2.8)

project(all)

enable_testing()

option(CI_COVERAGE "coverage" OFF)
option(VALGRIND "valgrind" OFF)

if(VALGRIND)
    function(add_test ARGV)
        # adding the test twice - once for normal execution and once from valgrind with error code from valgrind errors
        _add_test(${ARGV})
        _add_test(valgrind --leak-check=full --track-origins=yes --error-exitcode=1 ${ARGV})
    endfunction()
endif()

macro(add_flags)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}
endmacro()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    #add_flags(-Werror)
    add_flags(--save-temps)
    add_flags(-std=c++98)
    add_flags(-pedantic)
    add_flags(-m64)
    add_flags(-O2)
    add_flags(-fvisibility=hidden)
    add_flags(-fstrict-aliasing)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_flags(-ansi)
    add_flags(-Wall)
    add_flags(-Wextra)
    add_flags(-Wconversion)
    add_flags(-Wno-missing-field-initializers)
    add_flags(-Wold-style-cast)
    add_flags(-Wfloat-equal)
    add_flags(-Wlogical-op)
    add_flags(-Wundef)
    add_flags(-Wredundant-decls)
    add_flags(-Wshadow)
    add_flags(-Wstrict-overflow=5)
    add_flags(-Wwrite-strings)
    add_flags(-Wpointer-arith)
    add_flags(-Wcast-qual)
    add_flags(-Wformat=2)
    add_flags(-Wswitch-default)
    add_flags(-Wmissing-include-dirs)
    add_flags(-Wcast-align)
    add_flags(-Wformat-nonliteral)
    add_flags(-Wparentheses)
    add_flags(-Winit-self)
    add_flags(-Wuninitialized)
    add_flags(-Wswitch-enum)
    add_flags(-Wno-endif-labels)
    add_flags(-Wunused-function)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_flags(-Weverything)
    add_flags(-Wno-global-constructors)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_flags(/W4)
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
if(NOT APPLE)
    add_subdirectory(examples/stress_test) # compilers on that platform just hang on this one...
endif()
