#define DOCTEST_CONFIG_DISABLE

#include "doctest.h"

#ifdef _MSC_VER
#pragma warning(disable : 4505) // unreferenced local functions being removed
#endif                          // _MSC_VER

#ifdef __clang__
#pragma clang diagnostic ignored "-Wunneeded-internal-declaration"
#endif // __clang__

using doctest::Approx;

static int throws(bool in) {
    if(in)
        throw "whops!";
    return 42;
}

TEST_SUITE_BEGIN("the testsuite!");

#define DO_STUFF()                                                                                 \
    CHECK(1 == 0);                                                                                 \
    CHECK_FALSE(1 == 0);                                                                           \
    CHECK(Approx(0.502) == 0.501);                                                                 \
    CHECK(1 == 1);                                                                                 \
    REQUIRE(1 == 1);                                                                               \
    CHECK_FALSE(0);                                                                                \
    REQUIRE_FALSE(0);                                                                              \
    CHECK_THROWS(throws(true));                                                                    \
    REQUIRE_THROWS(throws(true));                                                                  \
    CHECK_THROWS_AS(throws(true), int);                                                            \
    REQUIRE_THROWS_AS(throws(true), char);                                                         \
    CHECK_NOTHROW(throws(false));                                                                  \
    REQUIRE_NOTHROW(throws(false));                                                                \
    SUBCASE("") {}

// in a separate function because the TEST_CASE() macro will expand to an uninstantiated template
// and we want to ensure this code is parsed (MSVC will not if it is in an uninstantiated template)
static void f() { DO_STUFF(); }

TEST_CASE("ops") {
    f();

    throws(false);

    DO_STUFF();
}

TEST_SUITE_END();

// to silence GCC warnings when inheriting from the class TheFixture which has no virtual destructor
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Weffc++"
#endif // __GNUC__

struct TheFixture
{
    int data;
    TheFixture()
            : data(42) {
        // setup here
    }

    ~TheFixture() {
        // teardown here
    }
};

TEST_SUITE("the testsuite!") {
    TEST_CASE_FIXTURE(TheFixture, "test with a fixture - 1") {
        data /= 2;
        CHECK(data == 21);
    }
}
