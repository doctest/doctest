#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#include <doctest/doctest.h>

#include <cstdio>

// most of these are used here just to test that they compile successfully from within a plugin
TEST_SUITE("some test suite") {
    TEST_CASE("test case in a plugin") {
        SUBCASE("some subcase") {
            INFO("some info");
            MESSAGE("triggering the INFO above to be printed");
            CHECK(1 == 2);
            FAIL("certain death!");
        }
    }
}

// set an exception translator for char
REGISTER_EXCEPTION_TRANSLATOR(char &e) {
    return doctest::String("char: ") + doctest::toString(e);
}
