#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_SUITE("meaningless macros");

TEST_CASE("an empty test that will succeed") {}

TEST_CASE("an empty test that will fail because of an exception") { throw "omgomgomg"; }

TEST_SUITE_END();

static int throws(bool in) {
    if(in)
        throw 5;
    return 42;
}

using doctest::Approx;

TEST_CASE("normal macros") {
    int a = 5;
    int b = 5;

    CHECK(throws(true) == 42);

    CHECK_FALSE(!(a == b));

    REQUIRE(a == b);
    WARN(&a == &b);

    CHECK(Approx(0.1000001) == 0.1000002);
    CHECK(Approx(0.502) == 0.501);

    const char* c_string = "test_test";

    CHECK(c_string == "test_test");

    throws(true);
}

TEST_CASE("exceptions-related macros") {
    CHECK_THROWS(throws(false));
    CHECK_THROWS_AS(throws(false), int);
    CHECK_THROWS_AS(throws(true), int);
    CHECK_THROWS_AS(throws(true), char);

    REQUIRE_NOTHROW(throws(true));
}
