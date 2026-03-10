#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#include <doctest/doctest.h>

#include <iostream>

TEST_CASE("dll") {
    std::cout << "I am a test from the dll!\n";
    CHECK(true);
}

DOCTEST_SYMBOL_EXPORT void from_dll();   // to silence "-Wmissing-declarations" with GCC
DOCTEST_SYMBOL_EXPORT void from_dll() {} // force the creation of a .lib file with MSVC
