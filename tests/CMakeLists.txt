add_subdirectory(common)

find_package(Threads REQUIRED)

function(doctest_add_unit_test)
    set(options              WITH_MAIN)
    set(one_value_keywords   SOURCE TEST_NAME IF EXCEPT)
    set(multi_value_keywords SOURCES DEFINES)
    cmake_parse_arguments(ARG "${options}" "${one_value_keywords}" "${multi_value_keywords}" ${ARGN})

    SET(ARG_SOURCES ${ARG_SOURCE} ${ARG_SOURCES})
    if(NOT ARG_SOURCES)
        message(FATAL_ERROR "must specify a SOURCE or multiple SOURCES")
    endif()

    if(NOT ARG_TEST_NAME)
        list(GET ARG_SOURCES 0 ARG_TEST_NAME)
    endif()
    string(MAKE_C_IDENTIFIER "${ARG_TEST_NAME}" EXE_NAME)

    if(DEFINED ARG_IF AND NOT ARG_IF)
        return()
    endif()

    if(DEFINED ARG_EXCEPT AND ARG_EXCEPT)
        return()
    endif()

    add_executable(${EXE_NAME} ${ARG_SOURCES})
    target_link_libraries(${EXE_NAME} common)
    target_link_libraries(${EXE_NAME} Threads::Threads)

    if(ARG_WITH_MAIN)
        target_link_libraries(${EXE_NAME} doctest_with_main)
    else()
        target_link_libraries(${EXE_NAME} doctest)
    endif()

    foreach(DEFINE ${ARG_DEFINES})
        target_compile_options(${EXE_NAME} PRIVATE -D${DEFINE})
    endforeach()

    set(TEST_ARGS)
    if(NOT DOCTEST_INTERNAL_COVERAGE)
        # Stringification is mostly compiler-specific, so these are excluded for now
        set(TEST_ARGS
            "-tce=Stringification*"
            "-tse=Stringification*,Type stringification*,Value stringification*")
    endif()
    add_test(NAME "${ARG_TEST_NAME}" COMMAND ${EXE_NAME} ${TEST_ARGS})

    # For unit-tests, we don't want to enforce the same level of warning-correctness
    # as with our examples. So, we inhibit some extras here
    macro(add_compiler_flags)
        foreach(flag ${ARGV})
            target_compile_options(${EXE_NAME} PRIVATE ${flag})
        endforeach()
    endmacro()

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        add_compiler_flags(-Wno-address)
        add_compiler_flags(-Wno-effc++)

        if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 9)
            add_compiler_flags(-Wno-pessimizing-move)
        endif()
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compiler_flags(-Wno-double-promotion)
        add_compiler_flags(-Wno-exit-time-destructors)
        add_compiler_flags(-Wno-global-constructors)
        add_compiler_flags(-Wno-missing-variable-declarations)
        add_compiler_flags(-Wno-padded)
        add_compiler_flags(-Wno-pessimizing-move)
    endif()

    if(MSVC)
        add_compiler_flags(/wd5263)
    endif()
endfunction()

set(MAC OFF)
if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(MAC ON)
endif()

doctest_add_unit_test(WITH_MAIN SOURCE doctest/assert/test_expression.cpp)
doctest_add_unit_test(WITH_MAIN SOURCE doctest/assert/test_message.cpp)
doctest_add_unit_test(WITH_MAIN SOURCE doctest/assert/test_type.cpp)
doctest_add_unit_test(WITH_MAIN SOURCE doctest/matchers/test_approx.cpp)
doctest_add_unit_test(WITH_MAIN SOURCE doctest/matchers/test_contains.cpp)
doctest_add_unit_test(WITH_MAIN SOURCE doctest/matchers/test_is_nan.cpp)
doctest_add_unit_test(          SOURCE doctest/test_color.ansi.cpp)
doctest_add_unit_test(          SOURCE doctest/test_color.none.cpp)
doctest_add_unit_test(          SOURCE doctest/test_color.win32.cpp)
doctest_add_unit_test(WITH_MAIN SOURCE doctest/test_debugger.cpp EXCEPT ${MAC})
doctest_add_unit_test(WITH_MAIN SOURCE doctest/test_exception_translator.cpp)
doctest_add_unit_test(WITH_MAIN SOURCE doctest/test_filters.cpp)
doctest_add_unit_test(WITH_MAIN SOURCE doctest/test_path.cpp)
doctest_add_unit_test(WITH_MAIN SOURCE doctest/test_platform.cpp DEFINES CMAKE_SYSTEM_NAME="${CMAKE_SYSTEM_NAME}")
doctest_add_unit_test(WITH_MAIN SOURCE doctest/test_string.cpp)
doctest_add_unit_test(WITH_MAIN SOURCE doctest/test_traversal.cpp)
doctest_add_unit_test(WITH_MAIN SOURCE doctest/test_xml.cpp)

# Regression tests
doctest_add_unit_test(
    TEST_NAME "regression/test.443" WITH_MAIN SOURCES
    regression/test.443/namespace1.cpp
    regression/test.443/namespace2.cpp
    regression/test.443/namespace3.cpp
    regression/test.443/namespace4.cpp
    regression/test.443/namespace5.cpp
    regression/test.443/namespace6.cpp
    regression/test.443/namespace7.cpp
    regression/test.443/namespace8.cpp
    regression/test.443/namespace9.cpp
)

doctest_add_unit_test(WITH_MAIN SOURCE regression/test.630.cpp)
doctest_add_unit_test(WITH_MAIN SOURCE regression/test.651.cpp)
doctest_add_unit_test(          SOURCE regression/test.782.cpp)
doctest_add_unit_test(WITH_MAIN SOURCE regression/test.786.cpp)
doctest_add_unit_test(          SOURCE regression/test.873.cpp)
doctest_add_unit_test(WITH_MAIN SOURCE regression/test.915.cpp)

set_tests_properties(
    regression/test.782.cpp
    PROPERTIES
        PASS_REGULAR_EXPRESSION
            ".*ERROR: test case THREW exception: #782 repro exception.*|Exceptions disabled, cannot run this test"
)
