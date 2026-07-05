#include <doctest/doctest.h>
#include <doctest/parts/public/warnings.h>

DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH("-Wuseless-cast")

TEST_CASE("Comparing differently signed types") {
    using T = int;
    using UT = unsigned int;

    SUBCASE("operator==") {
        auto x = static_cast<T>(1);
        auto y = static_cast<UT>(1);
        CHECK(x == y);
        CHECK_EQ(x, y);
    }

    SUBCASE("operator!=") {
        auto x = static_cast<T>(1);
        auto y = static_cast<T>(2);
        CHECK(x != y);
        CHECK_NE(x, y);
    }

    SUBCASE("operator<") {
        auto x = static_cast<T>(1);
        auto y = static_cast<UT>(2);
        CHECK(x < y);
        CHECK_LT(x, y);
    }

    SUBCASE("operator>") {
        auto x = static_cast<T>(2);
        auto y = static_cast<UT>(1);
        CHECK(x > y);
        CHECK_GT(x, y);
    }

    SUBCASE("operator<=") {
        auto x = static_cast<T>(1);
        auto y = static_cast<UT>(2);
        CHECK(x <= y);
        CHECK_LE(x, y);
    }

    SUBCASE("operator>=") {
        auto x = static_cast<T>(2);
        auto y = static_cast<UT>(1);
        CHECK(x >= y);
        CHECK_GE(x, y);
    }
}

DOCTEST_GCC_SUPPRESS_WARNING_POP
