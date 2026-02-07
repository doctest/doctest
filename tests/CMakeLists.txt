add_subdirectory(common)

function(doctest_add_unit_test)
    set(options            WITH_MAIN)
    set(one_value_keywords SOURCE TEST_NAME IF EXCEPT)
    set(multi_value_keywords)
    cmake_parse_arguments(ARG "${options}" "${one_value_keywords}" "${multi_value_keywords}" ${ARGN})

    if(NOT ARG_SOURCE)
        message(FATAL_ERROR "must specify a SOURCE")
    endif()

    if(NOT ARG_TEST_NAME)
        set(ARG_TEST_NAME ${ARG_SOURCE})
    endif()
    string(MAKE_C_IDENTIFIER "${ARG_TEST_NAME}" EXE_NAME)

    if(DEFINED ARG_IF AND NOT ARG_IF)
        return()
    endif()

    if(DEFINED ARG_EXCEPT AND ARG_EXCEPT)
        return()
    endif()

    add_executable(${EXE_NAME} ${ARG_SOURCE})
    target_link_libraries(${EXE_NAME} common)

    if(ARG_WITH_MAIN)
        target_link_libraries(${EXE_NAME} doctest_with_main)
    else()
        target_link_libraries(${EXE_NAME} doctest)
    endif()

    set(TEST_ARGS)
    if(NOT DOCTEST_INTERNAL_COVERAGE)
        # Stringification is mostly compiler-specific, so these are excluded for now
        set(TEST_ARGS
            "-tce=Stringification*"
            "-tse=Stringification*,Type stringification*,Value stringification*")
    endif()
    add_test(NAME "${ARG_SOURCE}" COMMAND ${EXE_NAME} ${TEST_ARGS})

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

set(GCC_4_8 OFF)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "4.8" AND
       CMAKE_CXX_COMPILER_VERSION VERSION_LESS          "4.9")
        set(GCC_4_8 ON)
    endif()
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
doctest_add_unit_test(WITH_MAIN SOURCE doctest/test_exception_translator.cpp)
doctest_add_unit_test(WITH_MAIN SOURCE doctest/test_filters.cpp)
doctest_add_unit_test(WITH_MAIN SOURCE doctest/test_path.cpp)
doctest_add_unit_test(WITH_MAIN SOURCE doctest/test_string.cpp)
doctest_add_unit_test(WITH_MAIN SOURCE doctest/test_xml.cpp EXCEPT ${GCC_4_8})
