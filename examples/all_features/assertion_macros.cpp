#include <doctest/doctest.h>

#include "header.h"

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <stdexcept>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

TEST_CASE("normal macros") {
    int a = 5;
    int b = 5;

    CHECK(throw_if(true, std::runtime_error("whops!")) == 42);

    CHECK_FALSE(!(a == b));

    REQUIRE(a == b);

    CHECK_EQ(a, b);

    CHECK(doctest::Approx(0.1000001) == 0.1000002);
    CHECK(doctest::Approx(0.502) == 0.501);
}

TEST_CASE("expressions should be evaluated only once") {
    int a = 5;
    REQUIRE(++a == 6);
    REQUIRE_EQ(++a, 7);
}

TEST_CASE("exceptions-related macros") {
    CHECK_THROWS(throw_if(true, 0));
    CHECK_THROWS(throw_if(false, 0)); // fails
    CHECK_THROWS_AS(throw_if(true, 0), int);
    CHECK_THROWS_AS(throw_if(true, 0), char); // fails
    CHECK_THROWS_AS(throw_if(false, 0), int); // fails

    CHECK_THROWS_WITH(throw_if(true, "whops!"), "whops! no match!"); // fails

    CHECK_NOTHROW(throw_if(true, 0)); // fails
    CHECK_NOTHROW(throw_if(false, 0));
}

TEST_CASE("exceptions-related macros for std::exception") {
    CHECK_THROWS(throw_if(false, 0));
    CHECK_THROWS_AS(throw_if(false, std::runtime_error("whops!")), std::exception);
    CHECK_THROWS_AS(throw_if(true, std::runtime_error("whops!")), const std::exception&);
    CHECK_THROWS_AS(throw_if(true, std::runtime_error("whops!")), int);

    CHECK_THROWS_WITH(throw_if(false, ""), "whops!");

    REQUIRE_NOTHROW(throw_if(true, std::runtime_error("whops!")));
}

// =================================================================================================
// == TESTING (ALMOST) ALL ASSERTS THAT THEY ACT ACCORDINGLY - not interesting examples...
// =================================================================================================

TEST_CASE("WARN level of asserts don't fail the test case") {
    WARN(0);
    WARN_FALSE(1);
    WARN_THROWS(throw_if(false, 0));
    WARN_THROWS_WITH(throw_if(true, ""), "whops!");
    WARN_THROWS_WITH(throw_if(false, ""), "whops!");
    WARN_THROWS_AS(throw_if(false, 0), bool);
    WARN_THROWS_AS(throw_if(true, 0), bool);
    WARN_NOTHROW(throw_if(true, 0));

    WARN_EQ(1, 0);
    WARN_UNARY(0);
    WARN_UNARY_FALSE(1);
}

TEST_CASE("CHECK level of asserts fail the test case but don't abort it") {
    CHECK(0);
    CHECK_FALSE(1);
    CHECK_THROWS(throw_if(false, 0));
    CHECK_THROWS_AS(throw_if(false, 0), bool);
    CHECK_THROWS_AS(throw_if(true, 0), bool);
    CHECK_THROWS_WITH(throw_if(true, 0), "unrecognized");
    CHECK_NOTHROW(throw_if(true, 0));

    CHECK_EQ(1, 0);
    CHECK_UNARY(0);
    CHECK_UNARY_FALSE(1);

    MESSAGE("reached!");
}

TEST_CASE("REQUIRE level of asserts fail and abort the test case - 1") {
    REQUIRE(0);
    MESSAGE("should not be reached!");
}
TEST_CASE("REQUIRE level of asserts fail and abort the test case - 2") {
    REQUIRE_FALSE(1);
    MESSAGE("should not be reached!");
}
TEST_CASE("REQUIRE level of asserts fail and abort the test case - 3") {
    REQUIRE_THROWS(throw_if(false, 0));
    MESSAGE("should not be reached!");
}
TEST_CASE("REQUIRE level of asserts fail and abort the test case - 4") {
    REQUIRE_THROWS_AS(throw_if(false, 0), bool);
    MESSAGE("should not be reached!");
}
TEST_CASE("REQUIRE level of asserts fail and abort the test case - 5") {
    REQUIRE_THROWS_AS(throw_if(true, 0), bool);
    MESSAGE("should not be reached!");
}
TEST_CASE("REQUIRE level of asserts fail and abort the test case - 4") {
	REQUIRE_THROWS_WITH(throw_if(false, ""), "whops!");
    MESSAGE("should not be reached!");
}
TEST_CASE("REQUIRE level of asserts fail and abort the test case - 5") {
	REQUIRE_THROWS_WITH(throw_if(true, ""), "whops!");
    MESSAGE("should not be reached!");
}
TEST_CASE("REQUIRE level of asserts fail and abort the test case - 6") {
    REQUIRE_NOTHROW(throw_if(true, 0));
    MESSAGE("should not be reached!");
}
TEST_CASE("REQUIRE level of asserts fail and abort the test case - 7") {
    REQUIRE_EQ(1, 0);
    MESSAGE("should not be reached!");
}
TEST_CASE("REQUIRE level of asserts fail and abort the test case - 8") {
    REQUIRE_UNARY(0);
    MESSAGE("should not be reached!");
}
TEST_CASE("REQUIRE level of asserts fail and abort the test case - 9") {
    REQUIRE_UNARY_FALSE(1);
    MESSAGE("should not be reached!");
}

TEST_CASE("all binary assertions") {
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
}

static void someAssertsInFunction() {
    int a = 5;
    int b = 5;
    CHECK(a == b);
    CHECK_FALSE(a != b);
    CHECK_THROWS(throw_if(true, 0));
    CHECK_THROWS_AS(throw_if(true, 0), int);
    CHECK_THROWS_WITH(throw_if(true, false), "unknown exception");
    CHECK_NOTHROW(throw_if(false, 0));

    CHECK_EQ(a, b);
    CHECK_UNARY(a == b);
    CHECK_UNARY_FALSE(a != b);
}

TEST_CASE("some asserts used in a function called by a test case") {
    someAssertsInFunction();
}
