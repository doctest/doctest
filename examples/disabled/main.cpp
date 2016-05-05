#define DOCTEST_CONFIG_DISABLE

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstdio>
#include <exception>

TEST_CASE("name1") {
    printf("Anyone there?\n");
}

TEST_SUITE("the testsuite!");

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

TEST_CASE("ops") {
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

TEST_SUITE_END;

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Weffc++"
#endif

struct Empty
{};

TEST_CASE_FIXTURE(Empty, "name") {
    printf("Help?\n");
}

TEST_CASE_FIXTURE(Empty, "ops") {
    printf("Help?\n");
}
