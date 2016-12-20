#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

static int throws(bool in) {
    if(in)
        throw 5;
    return 42;
}

static int throws_stdexcept(bool in) {
    if(in)
        throw std::runtime_error("Exception");
    return 42;
}

using doctest::Approx;

TEST_SUITE("meaningless macros");

TEST_CASE("an empty test that will succeed") {}

TEST_CASE("an empty test that will fail because of an exception") { throws(true); }

TEST_CASE("an empty test that will fail because of a std::exception") { throws_stdexcept(true); }

TEST_SUITE_END();

TEST_CASE("normal macros") {
    int a = 5;
    int b = 5;

    CHECK(throws(true) == 42);

    CHECK_FALSE(!(a == b));

    REQUIRE(a == b);

    CHECK_EQ(a, b);

    FAST_CHECK_EQ(a, b);

    // commented out because 32 vs 64 bit builds will fail when the output is compared
    //WARN(reinterpret_cast<void*>(1000) == reinterpret_cast<void*>(1004));

    CHECK(Approx(0.1000001) == 0.1000002);
    CHECK(Approx(0.502) == 0.501);

    throws(true);
}

TEST_CASE("normal macros with std::exception") {
    int a = 5;
    int b = 5;

    CHECK(throws_stdexcept(true) == 42);

    CHECK_FALSE(!(a == b));

    REQUIRE(a == b);

    CHECK_EQ(a, b);

    FAST_CHECK_EQ(a, b);

    // commented out because 32 vs 64 bit builds will fail when the output is compared
    //WARN(reinterpret_cast<void*>(1000) == reinterpret_cast<void*>(1004));

    CHECK(Approx(0.1000001) == 0.1000002);
    CHECK(Approx(0.502) == 0.501);

    throws(true);
}

TEST_CASE("exceptions-related macros") {
    CHECK_THROWS(throws(false));
    CHECK_THROWS_AS(throws(false), int);
    CHECK_THROWS_AS(throws(true), int);
    CHECK_THROWS_AS(throws(true), char);

    CHECK_NOTHROW(throws(true));
}

TEST_CASE("exceptions-related macros for std::exception") {
    CHECK_THROWS(throws_stdexcept(false));
    CHECK_THROWS_AS(throws_stdexcept(false), std::exception);
    CHECK_THROWS_AS(throws_stdexcept(true), std::exception);
    CHECK_THROWS_AS(throws_stdexcept(true), int);

    REQUIRE_NOTHROW(throws_stdexcept(true));
}
