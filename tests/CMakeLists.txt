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

# doctest_with_main currently has multiple definitions of various symbols
# due to them being implemented inline. Once this is resolved, we can drop
# main.cpp and just use doctest_with_main instead
add_executable(unit_tests ${unit_test_sources} main.cpp)
target_link_libraries(unit_tests doctest)

# While our unit-tests compile, there's sufficient errors that we can't really enable globally.
# For now, we're going to run the tests, and hopefully try to smooth over these errors later.
if(DOCTEST_INTERNAL_COVERAGE)
    add_test(NAME unit_tests COMMAND unit_tests)
endif()

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
