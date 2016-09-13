#include "parts/doctest_fwd.h"

#include <cstdio>

TEST_CASE("custom macros") {
    printf("Hello world!\n");

    CHECK(1 == 1);

    SUBCASE("bar") {
        SUBCASE("foo") {
            printf("foo\n"); //
        }
        SUBCASE("baz") {
            printf("baz\n"); //
        }

        printf("end\n"); //
    }
}

//TEST_CASE("captain obvious") {
//    bool b = false;
//    CHECK(b);
//
//    CHECK_UNARY_FALSE(true);
//
//    WARN_GT(false, false);
//    CHECK_EQ(false, true);
//    REQUIRE_EQ(false, true);
//}
