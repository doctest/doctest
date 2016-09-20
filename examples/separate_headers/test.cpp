#include "parts/doctest_fwd.h"

#include <cstdio>

TEST_CASE("stuff") {
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
