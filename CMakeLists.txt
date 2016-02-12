cmake_minimum_required(VERSION 2.8)

project(all)

enable_testing()

include(CMakeParseArguments)

option(CI_COVERAGE "coverage" OFF)
option(TESTS_VALGRIND "valgrind" OFF)
option(TESTS_COLLECT_OUTPUT "collect test output" OFF)

if(TESTS_VALGRIND AND WIN32)
    set(TESTS_VALGRIND OFF CACHE BOOL "NO VALGRIND UNDER WINDOWS" FORCE)
    message(AUTHOR_WARNING "TEST_VALGRIND not supported on Windows!")
endif()

if(TESTS_VALGRIND AND TESTS_COLLECT_OUTPUT)
    message(FATAL_ERROR "TEST_VALGRIND and TESTS_COLLECT_OUTPUT options cannot both be set to ON!")
endif()

# add a customized overloaded version of add_test() to suite my needs
function(add_test)
    cmake_parse_arguments(ARG "NO_VALGRIND;NO_OUTPUT" "NAME" "COMMAND" ${ARGN})
    if(NOT "${ARG_UNPARSED_ARGUMENTS}" STREQUAL "" OR "${ARG_NAME}" STREQUAL "" OR "${ARG_COMMAND}" STREQUAL "")
        message(FATAL_ERROR "add_test() called with wrong options!")
    endif()
    
    # construct the command that will be called by the exec_test.cmake script
    set(the_command "")
    if(TESTS_VALGRIND AND NOT ARG_NO_VALGRIND)
        set(the_command "valgrind --leak-check=full --track-origins=yes --error-exitcode=1")
    endif()
    foreach(cur ${ARG_COMMAND})
        set(the_command "${the_command} ${cur}")
    endforeach()
    
    set(ADDITIONAL_FLAGS "")
    if(TESTS_COLLECT_OUTPUT AND NOT ARG_NO_OUTPUT)
        set(ADDITIONAL_FLAGS "-DTEST_OUTPUT_FILE=${CMAKE_CURRENT_SOURCE_DIR}/${ARG_NAME}.txt")
    endif()
    
    _add_test(NAME ${ARG_NAME} COMMAND ${CMAKE_COMMAND} -DCOMMAND=${the_command} ${ADDITIONAL_FLAGS} -P ${CMAKE_SOURCE_DIR}/scripts/exec_test.cmake)
endfunction()

# helper macro for adding flags
macro(add_flags)
    foreach(flag ${ARGV})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
    endforeach()
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

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/test.cpp)

add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)

#add_subdirectory(examples/dev_testing)
#add_subdirectory(examples/disabled)
#add_subdirectory(examples/hello_world)
#add_subdirectory(examples/multi_dll)
#add_subdirectory(examples/multiprocess)
if(NOT APPLE)
#    add_subdirectory(examples/stress_test) # compilers on osx just hang on this one...
endif()
