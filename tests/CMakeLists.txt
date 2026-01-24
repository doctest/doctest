set(unit_test_sources
    doctest/assert/test_expression.cpp
    doctest/assert/test_message.cpp
    doctest/assert/test_type.cpp
    doctest/matchers/test_approx.cpp
    doctest/matchers/test_contains.cpp
    doctest/matchers/test_is_nan.cpp
    doctest/test_exception_translator.cpp
    doctest/test_path.cpp
    doctest/test_string.cpp)

add_executable(unit_tests ${unit_test_sources})
target_link_libraries(unit_tests doctest_with_main)

foreach(f ${unit_test_sources})
    set(args)
    if(NOT DOCTEST_INTERNAL_COVERAGE)
        # Stringification is mostly compiler-specific, so these are excluded for now
        set(args
            "-tce=Stringification*"
            "-tse=Stringification*,Type stringification*,Value stringification*")
    endif()

    add_test(NAME "${f}" COMMAND unit_tests "-sf=*${f}" ${args})
endforeach()

# For unit-tests, we don't want to enforce the same level of warning-correctness
# as with our examples. So, we inhibit some extras here
macro(add_compiler_flags)
    foreach(flag ${ARGV})
        target_compile_options(unit_tests PRIVATE ${flag})
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
