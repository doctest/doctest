#define DOCTEST_DISABLE

#define DOCTEST_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstdio>
#include <exception>

TESTCASE("name1") {
    printf("Anyone there?\n");
}

TESTSUITE("the testsuite!");

// to silence GCC "-Wmissing-declarations"
// and the attribute is to silence "-Wmissing-noreturn" on clang
#ifdef __clang__
void throws() __attribute__((noreturn));
#else
void throws();
#endif

void throws() { throw std::exception(); }
void nothrows(); // to silence GCC "-Wmissing-declarations"
void nothrows() {}

TESTCASE("ops") {
    printf("Anyone there?\n");

    CHECK(1 == 0);
    CHECK_FALSE(1 == 0);

    CHECK(1 == 1);
    REQUIRE(1 == 1);

    CHECK_FALSE(0);
    REQUIRE_FALSE(0);

    CHECK_THROWS(throws());
    REQUIRE_THROWS(throws());

    CHECK_THROWS_AS(throws(), std::exception);
    REQUIRE_THROWS_AS(throws(), std::exception);

    CHECK_NOTHROW(nothrows());
    REQUIRE_NOTHROW(nothrows());

    SUBCASE("") {}
}

TESTSUITE_END;

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Weffc++"
#endif

struct Empty
{};

TESTCASE_FIXTURE(Empty, "name") {
    printf("Help?\n");
}

TESTCASE_FIXTURE(Empty, "ops") {
    printf("Help?\n");
}
