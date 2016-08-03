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
{};

static std::ostream& operator<<(std::ostream& stream, const myType&) {
    stream << "myType!";
    return stream;
}

static bool operator==(const myType&, const myType&) { return false; }

TEST_CASE("assertions") {
    CHECK(true);
    CHECK(1 == 0);
    CHECK_FALSE(1);
    CHECK(myType() == myType());
    CHECK(Approx(0.1) == 0.2);

    CHECK_THROWS(throws(true));
    CHECK_THROWS(throws(false));
    CHECK_NOTHROW(throws(false));
    CHECK_NOTHROW(throws(true));
    CHECK_THROWS_AS(throws(true), bool);
    REQUIRE_THROWS_AS(throws(false), bool);
}

TEST_CASE("throws") { throws(true); }

TEST_SUITE_END();
