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

if(CI_COVERAGE)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/scripts/coveralls-cmake/cmake)
    include(Coveralls)
    coveralls_turn_on_coverage()
    
    coveralls_setup("${CMAKE_SOURCE_DIR}/doctest/doctest.h" ON "${CMAKE_SOURCE_DIR}/scripts/coveralls-cmake/cmake")
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

macro(add_compiler_flags)
    foreach(flag ${ARGV})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
    endforeach()
endmacro()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    #add_compiler_flags(-Werror)
    #add_compiler_flags(--save-temps) # problematic with coveralls cmake stuff and clang - really weird...
    add_compiler_flags(-std=c++98)
    add_compiler_flags(-pedantic)
    add_compiler_flags(-m64)
    add_compiler_flags(-O2)
    add_compiler_flags(-fvisibility=hidden)
    add_compiler_flags(-fstrict-aliasing)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compiler_flags(-ansi)
    add_compiler_flags(-Wall)
    add_compiler_flags(-Wextra)
    add_compiler_flags(-Wconversion)
    add_compiler_flags(-Wno-missing-field-initializers)
    add_compiler_flags(-Wold-style-cast)
    add_compiler_flags(-Wfloat-equal)
    add_compiler_flags(-Wlogical-op)
    add_compiler_flags(-Wundef)
    add_compiler_flags(-Wredundant-decls)
    add_compiler_flags(-Wshadow)
    add_compiler_flags(-Wstrict-overflow=5)
    add_compiler_flags(-Wwrite-strings)
    add_compiler_flags(-Wpointer-arith)
    add_compiler_flags(-Wcast-qual)
    add_compiler_flags(-Wformat=2)
    add_compiler_flags(-Wswitch-default)
    add_compiler_flags(-Wmissing-include-dirs)
    add_compiler_flags(-Wcast-align)
    add_compiler_flags(-Wformat-nonliteral)
    add_compiler_flags(-Wparentheses)
    add_compiler_flags(-Winit-self)
    add_compiler_flags(-Wuninitialized)
    add_compiler_flags(-Wswitch-enum)
    add_compiler_flags(-Wno-endif-labels)
    add_compiler_flags(-Wunused-function)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compiler_flags(-Weverything)
    add_compiler_flags(-Wno-global-constructors)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compiler_flags(/W4)
endif()

include_directories("doctest/") # needed here so the coverage tools work - otherwise the "../../doctest" relative path fucks up

file(GLOB subdir_list "${CMAKE_SOURCE_DIR}/examples/*")
foreach(dir ${subdir_list})
    if(IS_DIRECTORY ${dir})
        get_filename_component(DIRNAME ${dir} NAME)
        add_subdirectory(examples/${DIRNAME})
    endif()
endforeach()
