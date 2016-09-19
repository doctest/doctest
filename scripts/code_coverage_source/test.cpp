#include "doctest.h"

#include <ostream>

// intentionally here so there are subcases on the same lines in different files
TEST_CASE("subcases") {
    SUBCASE("1") {
        SUBCASE("1.1") {}
        SUBCASE("1.2") {}
    }
    SUBCASE("2") { CHECK(0); }
    SUBCASE("3") {}
}

TEST_SUITE("ts1");

using doctest::Approx;

static int throws(bool in) {
    if(in)
        throw false;
    return 42;
}

struct myType
{
    myType() {}

private:
    myType(const myType&);            // non-copyable
    myType& operator=(const myType&); // non-assignable
};

static std::ostream& operator<<(std::ostream& stream, const myType&) {
    stream << "myType!";
    return stream;
}

static bool operator==(const myType&, const myType&) { return false; }

TEST_CASE("expressions should be evaluated only once") {
    int a = 5;
    REQUIRE(++a == 6);
    REQUIRE_EQ(++a, 7);
}

TEST_CASE("assertions") {
    CHECK(true);
    CHECK(1 == 0);
    CHECK_FALSE(1);
    myType a;
    myType b;
    CHECK(a == b);
    CHECK(Approx(0.1) == 0.2);

    CHECK_THROWS(throws(true));
    CHECK_THROWS(throws(false));
    CHECK_NOTHROW(throws(false));
    CHECK_NOTHROW(throws(true));
    CHECK_THROWS_AS(throws(true), bool);
    REQUIRE_THROWS_AS(throws(false), bool);
}

TEST_CASE("assertions - all of them") {
    WARN(true);
    CHECK(true);
    REQUIRE(true);
    WARN_FALSE(false);
    CHECK_FALSE(false);
    REQUIRE_FALSE(false);
    WARN_THROWS(throws(true));
    CHECK_THROWS(throws(true));
    REQUIRE_THROWS(throws(true));
    WARN_THROWS_AS(throws(true), bool);
    CHECK_THROWS_AS(throws(true), bool);
    REQUIRE_THROWS_AS(throws(true), bool);
    WARN_NOTHROW(throws(false));
    CHECK_NOTHROW(throws(false));
    REQUIRE_NOTHROW(throws(false));
    WARN_EQ(1, 1);
    CHECK_EQ(1, 1);
    REQUIRE_EQ(1, 1);
    WARN_NE(1, 0);
    CHECK_NE(1, 0);
    REQUIRE_NE(1, 0);
    WARN_GT(1, 0);
    CHECK_GT(1, 0);
    REQUIRE_GT(1, 0);
    WARN_LT(0, 1);
    CHECK_LT(0, 1);
    REQUIRE_LT(0, 1);
    WARN_GE(1, 1);
    CHECK_GE(1, 1);
    REQUIRE_GE(1, 1);
    WARN_LE(1, 1);
    CHECK_LE(1, 1);
    REQUIRE_LE(1, 1);
    WARN_UNARY(1);
    CHECK_UNARY(1);
    REQUIRE_UNARY(1);
    WARN_UNARY_FALSE(0);
    CHECK_UNARY_FALSE(0);
    REQUIRE_UNARY_FALSE(0);
    FAST_WARN_EQ(1, 1);
    FAST_CHECK_EQ(1, 1);
    FAST_REQUIRE_EQ(1, 1);
    FAST_WARN_NE(1, 0);
    FAST_CHECK_NE(1, 0);
    FAST_REQUIRE_NE(1, 0);
    FAST_WARN_GT(1, 0);
    FAST_CHECK_GT(1, 0);
    FAST_REQUIRE_GT(1, 0);
    FAST_WARN_LT(0, 1);
    FAST_CHECK_LT(0, 1);
    FAST_REQUIRE_LT(0, 1);
    FAST_WARN_GE(1, 1);
    FAST_CHECK_GE(1, 1);
    FAST_REQUIRE_GE(1, 1);
    FAST_WARN_LE(1, 1);
    FAST_CHECK_LE(1, 1);
    FAST_REQUIRE_LE(1, 1);
    FAST_WARN_UNARY(1);
    FAST_CHECK_UNARY(1);
    FAST_REQUIRE_UNARY(1);
    FAST_WARN_UNARY_FALSE(0);
    FAST_CHECK_UNARY_FALSE(0);
    FAST_REQUIRE_UNARY_FALSE(1);
}

TEST_CASE("throws") { throws(true); }

TEST_SUITE_END();
