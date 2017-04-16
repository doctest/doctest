if(common_included)
    return()
endif()
set(common_included true)

include(CMakeParseArguments)

# cache this for use inside of the function
set(CURRENT_LIST_DIR_CACHED ${CMAKE_CURRENT_LIST_DIR})

enable_testing()

set(TEST_MODE "COMPARE" CACHE STRING "Test mode - normal/run through valgrind/collect output/compare with output")
set_property(CACHE TEST_MODE PROPERTY STRINGS "NORMAL;VALGRIND;COLLECT;COMPARE")
option(WITH_CPP11 "With C++11 enabled" OFF)

# a custom version of add_test() to suite my needs
function(doctest_add_test)
    cmake_parse_arguments(ARG "NO_VALGRIND;NO_OUTPUT" "NAME" "COMMAND" ${ARGN})
    if(NOT "${ARG_UNPARSED_ARGUMENTS}" STREQUAL "" OR "${ARG_NAME}" STREQUAL "" OR "${ARG_COMMAND}" STREQUAL "")
        message(FATAL_ERROR "doctest_add_test() called with wrong options!")
    endif()
    
    set(the_test_mode NORMAL)
    
    # construct the command that will be called by the exec_test.cmake script
    set(the_command "")
    if(${TEST_MODE} STREQUAL "VALGRIND" AND NOT ARG_NO_VALGRIND)
        set(the_test_mode VALGRIND)
        set(the_command "valgrind -v --leak-check=full --track-origins=yes --error-exitcode=1")
    endif()
    foreach(cur ${ARG_COMMAND})
        set(the_command "${the_command} ${cur}")
    endforeach()
    # append the argument for removing paths from filenames in the output so tests give the same output everywhere
    set(the_command "${the_command} --dt-no-path-filenames=1")
    # append the argument for substituting source line numbers with 0 in the output so tests give the same output when lines change a bit
    set(the_command "${the_command} --dt-no-line-numbers=1")
    # append the argument for ignoring the exit code of the test programs because some are intended to have failing tests
    set(the_command "${the_command} --dt-no-exitcode=1")
    # append the argument for not printing the framework version so reference output doesn't have to be recommitted when the version is bumped
    set(the_command "${the_command} --dt-no-version=1")
    
    string(STRIP ${the_command} the_command)
    
    if(${TEST_MODE} STREQUAL "COLLECT" OR ${TEST_MODE} STREQUAL "COMPARE")
        if(NOT ARG_NO_OUTPUT)
            file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test_output/)
            set(the_test_mode ${TEST_MODE})
            list(APPEND ADDITIONAL_FLAGS -DTEST_OUTPUT_FILE=${CMAKE_CURRENT_SOURCE_DIR}/test_output/${ARG_NAME}.txt)
            list(APPEND ADDITIONAL_FLAGS -DTEST_TEMP_FILE=${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/temp_test_output.txt)
        endif()
    endif()
    
    list(APPEND ADDITIONAL_FLAGS -DTEST_MODE=${the_test_mode})
    
    add_test(NAME ${ARG_NAME} COMMAND ${CMAKE_COMMAND} -DCOMMAND=${the_command} ${ADDITIONAL_FLAGS} -P ${CURRENT_LIST_DIR_CACHED}/exec_test.cmake)
endfunction()

macro(add_compiler_flags)
    foreach(flag ${ARGV})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
    endforeach()
endmacro()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compiler_flags(-Werror)
    add_compiler_flags(-pedantic)
    add_compiler_flags(-pedantic-errors)
    add_compiler_flags(-fvisibility=hidden)
    add_compiler_flags(-fstrict-aliasing)
    
    if(WITH_CPP11)
        add_compiler_flags(-std=c++0x)
    else()
        add_compiler_flags(-std=c++98)
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compiler_flags(-Wall)
    add_compiler_flags(-Wextra)
    add_compiler_flags(-fdiagnostics-show-option)
    add_compiler_flags(-Wconversion)
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
    add_compiler_flags(-Wswitch-enum)
    add_compiler_flags(-Wnon-virtual-dtor)
    add_compiler_flags(-Wctor-dtor-privacy)
    add_compiler_flags(-Wsign-conversion)
    add_compiler_flags(-Wdisabled-optimization)
    add_compiler_flags(-Weffc++)
    add_compiler_flags(-Winline)
    add_compiler_flags(-Winvalid-pch)
    add_compiler_flags(-Wmissing-declarations)
    add_compiler_flags(-Woverloaded-virtual)
    if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.6)
        add_compiler_flags(-Wnoexcept)
    endif()
    #add_compiler_flags(-Waggregate-return) # GCC 4.8 does not silence this even with "#pragma GCC diagnostic ignored"
    
    if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
        add_compiler_flags(-Wdouble-promotion)
        add_compiler_flags(-Wtrampolines)
        add_compiler_flags(-Wzero-as-null-pointer-constant)
        add_compiler_flags(-Wuseless-cast)
        add_compiler_flags(-Wvector-operation-performance)
    endif()
    
    if(CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 6.0 OR CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 6.0)
        add_compiler_flags(-Wshift-overflow=2)
        add_compiler_flags(-Wnull-dereference)
        add_compiler_flags(-Wduplicated-cond)
    endif()
    
    if(CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 7.0 OR CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7.0)
        add_compiler_flags(-Walloc-zero)
        add_compiler_flags(-Walloca)
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compiler_flags(-Weverything)
    add_compiler_flags(-Qunused-arguments -fcolor-diagnostics) # needed for ccache integration on travis
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compiler_flags(/std:c++latest) # for post c++14 updates in MSVC
    add_compiler_flags(/WX)
    add_compiler_flags(/W4) # /Wall is too aggressive - even the standard C headers give thousands of errors...
endif()

# add a custom target that assembles the single header when any of the parts are touched

set(cat_cmd "cat")
set(doctest_include_folder "${CURRENT_LIST_DIR_CACHED}/../../doctest/")
set(doctest_parts_folder "${CURRENT_LIST_DIR_CACHED}/../../doctest/parts/")
if(WIN32)
    set(cat_cmd "type")
    STRING(REGEX REPLACE "/" "\\\\" doctest_include_folder ${doctest_include_folder})
    STRING(REGEX REPLACE "/" "\\\\" doctest_parts_folder ${doctest_parts_folder})
endif()

add_custom_command(
    OUTPUT ${doctest_include_folder}doctest.h
    DEPENDS
        ${doctest_parts_folder}doctest_fwd.h
        ${doctest_parts_folder}doctest_impl.h
    COMMAND ${CMAKE_COMMAND} -P ${CURRENT_LIST_DIR_CACHED}/asemble_single_header.cmake
    COMMENT "assembling the single header")

add_custom_target(assemble_single_header ALL DEPENDS ${doctest_include_folder}doctest.h)

# override add_executable() to add a dependency on the header assembly target
function(add_executable name)
    _add_executable(${name} ${ARGN})
    add_dependencies(${name} assemble_single_header)
endfunction()

# override add_library() to add a dependency on the header assembly target
function(add_library name)
    _add_library(${name} ${ARGN})
    add_dependencies(${name} assemble_single_header)
endfunction()
