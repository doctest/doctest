#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#include "doctest.h"

#include <cstdio>

TEST_CASE("dll") {
    printf("I am a test from the dll!\n");
}

DOCTEST_SYMBOL_EXPORT void from_dll();   // to silence "-Wmissing-declarations" with GCC
DOCTEST_SYMBOL_EXPORT void from_dll() {} // force the creation of a .lib file with MSVC
