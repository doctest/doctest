// note that DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL should not be defined here
#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <cstdio>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

TEST_CASE("implementation_2") {
    printf("I am a test from the implementation_2!\n");
}
