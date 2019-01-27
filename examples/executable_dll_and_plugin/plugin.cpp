#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <cstdio>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

TEST_CASE("plugin") {
    printf("I am a test from the plugin!\n");
}

// set an exception translator for char
REGISTER_EXCEPTION_TRANSLATOR(char& e) {
    return doctest::String("char: ") + doctest::toString(e);
}
