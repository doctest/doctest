//#define DOCTEST_CONFIG_SUPER_FAST_ASSERTS
#include "parts/doctest_fwd.h"

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <iostream>
using namespace std;
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

void some_func();
void some_func() {
    FAST_CHECK_EQ(true, false);
    FAST_CHECK_UNARY(false);
    FAST_CHECK_UNARY_FALSE(true);

    CHECK_EQ(true, false);
    CHECK_UNARY(false);
    CHECK_UNARY_FALSE(true);

    CHECK(false);
    CHECK_THROWS(((void)false));
}

TEST_CASE("some test case lololo!") { some_func(); }
