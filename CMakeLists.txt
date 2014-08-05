cmake_minimum_required(VERSION 2.8)

project(all)

enable_testing()

include(CMakeParseArguments)
include(scripts/flags.cmake)

set(TEST_MODE "NORMAL" CACHE STRING "Test mode - normal/run through valgrind/collect output/compare with output")
set_property(CACHE TEST_MODE PROPERTY STRINGS "NORMAL;VALGRIND;COLLECT;COMPARE")

# when on Travis CI force 64 bit for gcc 4.4 under OSX because -m32 fails
# saying that it cannot find 'std::exception' as a symbol (or others) for i386
if(DEFINED ENV{TRAVIS} AND APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.5)
    add_compiler_flags(-m64)
endif()

# setup coverage stuff only when COVERALLS_SERVICE_NAME is set (usually on travis CI)
if(DEFINED ENV{COVERALLS_SERVICE_NAME})
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
    string(STRIP ${the_command} the_command)
    
    list(APPEND ADDITIONAL_FLAGS -DTEST_OUTPUT_FILE=${CMAKE_CURRENT_SOURCE_DIR}/test_output/${ARG_NAME}.txt)
    list(APPEND ADDITIONAL_FLAGS -DTEST_TEMP_FILE=${CMAKE_SOURCE_DIR}/examples/temp_test_output.txt)
    
    if(${TEST_MODE} STREQUAL "COLLECT" OR ${TEST_MODE} STREQUAL "COMPARE")
        if(NOT ARG_NO_OUTPUT)
            file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test_output/)
            set(the_test_mode ${TEST_MODE})
        endif()
    endif()
    
    list(APPEND ADDITIONAL_FLAGS -DTEST_MODE=${the_test_mode})
    
    _add_test(NAME ${ARG_NAME} COMMAND ${CMAKE_COMMAND} -DCOMMAND=${the_command} ${ADDITIONAL_FLAGS} -P ${CMAKE_SOURCE_DIR}/scripts/exec_test.cmake)
endfunction()

include_directories("doctest/") # needed here so the coverage tools work - otherwise the "../../doctest" relative path fucks up

file(GLOB subdir_list "${CMAKE_SOURCE_DIR}/examples/*")
foreach(dir ${subdir_list})
    if(IS_DIRECTORY ${dir})
        get_filename_component(DIRNAME ${dir} NAME)
        add_subdirectory(examples/${DIRNAME})
    endif()
endforeach()
