#include "doctest.h"

using doctest::Approx;

static int throws(bool in) {
    if(in)
        throw "whops!";
    return 42;
}

TEST_SUITE("ts1");

TEST_CASE("assertions") {
    //int iVar = 0;
    //int* a = &iVar;
    //int* b = a - 1;
    //CHECK(a == b);
    CHECK(1 == false);
    CHECK(Approx(0.1) == 0.2);

    CHECK_THROWS(throws(true));
    CHECK_NOTHROW(throws(false));
    CHECK_THROWS_AS(throws(true), bool);
}

TEST_CASE("throws") {
    throws(true);
}

TEST_SUITE_END();

TEST_SUITE("ts2");

TEST_CASE("subcases") {
    SUBCASE("1") {
        SUBCASE("1.1") {}
        SUBCASE("1.2") {}
    }
    SUBCASE("2") {}
    SUBCASE("3") {}
}

TEST_SUITE_END();