#include "doctest.h"

TEST_SUITE("ts2");

// intentionally here so there are subcases on the same lines in different files
TEST_CASE("subcases") {
    SUBCASE("1") {
        SUBCASE("1.1") {}
        SUBCASE("1.2") {}
    }
    SUBCASE("2") { CHECK(0); }
    SUBCASE("3") {}
}

TEST_SUITE_END();

using doctest::Approx;

static int throws(bool in) {
    if(in)
        throw false;
    return 42;
}

TEST_SUITE("ts1");

TEST_CASE("assertions") {
    //int iVar = 0;
    //int* a = &iVar;
    //int* b = a - 1;
    //CHECK(a == b);
    CHECK(1 == 0);
    CHECK_FALSE(1);
    CHECK(Approx(0.1) == 0.2);

    CHECK_THROWS(throws(true));
    CHECK_THROWS(throws(false));
    CHECK_NOTHROW(throws(false));
    CHECK_NOTHROW(throws(true));
    CHECK_THROWS_AS(throws(true), bool);
    CHECK_THROWS_AS(throws(false), bool);
}

TEST_CASE("throws") { throws(true); }

TEST_SUITE_END();
