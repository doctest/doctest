#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#include "doctest.h"

#include <cstdio>

TEST_CASE("plugin") {
    printf("I am a test from the plugin!\n");
}

// set an exception translator for char
REGISTER_EXCEPTION_TRANSLATOR(char& e) {
    return doctest::String("char: ") + doctest::toString(e);
}
