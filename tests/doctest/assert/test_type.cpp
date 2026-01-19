#include <doctest/doctest.h>

TEST_CASE("Assertion stringification") {
    using namespace doctest;

    CHECK(assertString(assertType::DT_WARN) == doctest::String("WARN"));
    CHECK(assertString(assertType::DT_CHECK) == doctest::String("CHECK"));
    CHECK(assertString(assertType::DT_REQUIRE) == doctest::String("REQUIRE"));

    CHECK(assertString(assertType::DT_WARN_FALSE) == doctest::String("WARN_FALSE"));
    CHECK(assertString(assertType::DT_CHECK_FALSE) == doctest::String("CHECK_FALSE"));
    CHECK(assertString(assertType::DT_REQUIRE_FALSE) == doctest::String("REQUIRE_FALSE"));

    CHECK(assertString(assertType::DT_WARN_THROWS) == doctest::String("WARN_THROWS"));
    CHECK(assertString(assertType::DT_CHECK_THROWS) == doctest::String("CHECK_THROWS"));
    CHECK(assertString(assertType::DT_REQUIRE_THROWS) == doctest::String("REQUIRE_THROWS"));

    CHECK(assertString(assertType::DT_WARN_THROWS_AS) == doctest::String("WARN_THROWS_AS"));
    CHECK(assertString(assertType::DT_CHECK_THROWS_AS) == doctest::String("CHECK_THROWS_AS"));
    CHECK(assertString(assertType::DT_REQUIRE_THROWS_AS) == doctest::String("REQUIRE_THROWS_AS"));

    CHECK(assertString(assertType::DT_WARN_THROWS_WITH) == doctest::String("WARN_THROWS_WITH"));
    CHECK(assertString(assertType::DT_CHECK_THROWS_WITH) == doctest::String("CHECK_THROWS_WITH"));
    CHECK(assertString(assertType::DT_REQUIRE_THROWS_WITH) == doctest::String("REQUIRE_THROWS_WITH"));

    CHECK(assertString(assertType::DT_WARN_THROWS_WITH_AS) == doctest::String("WARN_THROWS_WITH_AS"));
    CHECK(assertString(assertType::DT_CHECK_THROWS_WITH_AS) == doctest::String("CHECK_THROWS_WITH_AS"));
    CHECK(assertString(assertType::DT_REQUIRE_THROWS_WITH_AS) == doctest::String("REQUIRE_THROWS_WITH_AS"));

    CHECK(assertString(assertType::DT_WARN_NOTHROW) == doctest::String("WARN_NOTHROW"));
    CHECK(assertString(assertType::DT_CHECK_NOTHROW) == doctest::String("CHECK_NOTHROW"));
    CHECK(assertString(assertType::DT_REQUIRE_NOTHROW) == doctest::String("REQUIRE_NOTHROW"));

    CHECK(assertString(assertType::DT_WARN_EQ) == doctest::String("WARN_EQ"));
    CHECK(assertString(assertType::DT_CHECK_EQ) == doctest::String("CHECK_EQ"));
    CHECK(assertString(assertType::DT_REQUIRE_EQ) == doctest::String("REQUIRE_EQ"));

    CHECK(assertString(assertType::DT_WARN_NE) == doctest::String("WARN_NE"));
    CHECK(assertString(assertType::DT_CHECK_NE) == doctest::String("CHECK_NE"));
    CHECK(assertString(assertType::DT_REQUIRE_NE) == doctest::String("REQUIRE_NE"));

    CHECK(assertString(assertType::DT_WARN_GT) == doctest::String("WARN_GT"));
    CHECK(assertString(assertType::DT_CHECK_GT) == doctest::String("CHECK_GT"));
    CHECK(assertString(assertType::DT_REQUIRE_GT) == doctest::String("REQUIRE_GT"));

    CHECK(assertString(assertType::DT_WARN_LT) == doctest::String("WARN_LT"));
    CHECK(assertString(assertType::DT_CHECK_LT) == doctest::String("CHECK_LT"));
    CHECK(assertString(assertType::DT_REQUIRE_LT) == doctest::String("REQUIRE_LT"));

    CHECK(assertString(assertType::DT_WARN_GE) == doctest::String("WARN_GE"));
    CHECK(assertString(assertType::DT_CHECK_GE) == doctest::String("CHECK_GE"));
    CHECK(assertString(assertType::DT_REQUIRE_GE) == doctest::String("REQUIRE_GE"));

    CHECK(assertString(assertType::DT_WARN_LE) == doctest::String("WARN_LE"));
    CHECK(assertString(assertType::DT_CHECK_LE) == doctest::String("CHECK_LE"));
    CHECK(assertString(assertType::DT_REQUIRE_LE) == doctest::String("REQUIRE_LE"));

    CHECK(assertString(assertType::DT_WARN_UNARY) == doctest::String("WARN_UNARY"));
    CHECK(assertString(assertType::DT_CHECK_UNARY) == doctest::String("CHECK_UNARY"));
    CHECK(assertString(assertType::DT_REQUIRE_UNARY) == doctest::String("REQUIRE_UNARY"));

    CHECK(assertString(assertType::DT_WARN_UNARY_FALSE) == doctest::String("WARN_UNARY_FALSE"));
    CHECK(assertString(assertType::DT_CHECK_UNARY_FALSE) == doctest::String("CHECK_UNARY_FALSE"));
    CHECK(assertString(assertType::DT_REQUIRE_UNARY_FALSE) == doctest::String("REQUIRE_UNARY_FALSE"));
}

TEST_CASE("Failure stringification") {
    using namespace doctest;

    CHECK(failureString(assertType::DT_WARN) == doctest::String("WARNING"));
    CHECK(failureString(assertType::DT_CHECK) == doctest::String("ERROR"));
    CHECK(failureString(assertType::DT_REQUIRE) == doctest::String("FATAL ERROR"));

    CHECK(failureString(assertType::DT_WARN_FALSE) == doctest::String("WARNING"));
    CHECK(failureString(assertType::DT_CHECK_FALSE) == doctest::String("ERROR"));
    CHECK(failureString(assertType::DT_REQUIRE_FALSE) == doctest::String("FATAL ERROR"));

    CHECK(failureString(assertType::DT_WARN_THROWS) == doctest::String("WARNING"));
    CHECK(failureString(assertType::DT_CHECK_THROWS) == doctest::String("ERROR"));
    CHECK(failureString(assertType::DT_REQUIRE_THROWS) == doctest::String("FATAL ERROR"));

    CHECK(failureString(assertType::DT_WARN_THROWS_AS) == doctest::String("WARNING"));
    CHECK(failureString(assertType::DT_CHECK_THROWS_AS) == doctest::String("ERROR"));
    CHECK(failureString(assertType::DT_REQUIRE_THROWS_AS) == doctest::String("FATAL ERROR"));

    CHECK(failureString(assertType::DT_WARN_THROWS_WITH) == doctest::String("WARNING"));
    CHECK(failureString(assertType::DT_CHECK_THROWS_WITH) == doctest::String("ERROR"));
    CHECK(failureString(assertType::DT_REQUIRE_THROWS_WITH) == doctest::String("FATAL ERROR"));

    CHECK(failureString(assertType::DT_WARN_THROWS_WITH_AS) == doctest::String("WARNING"));
    CHECK(failureString(assertType::DT_CHECK_THROWS_WITH_AS) == doctest::String("ERROR"));
    CHECK(failureString(assertType::DT_REQUIRE_THROWS_WITH_AS) == doctest::String("FATAL ERROR"));

    CHECK(failureString(assertType::DT_WARN_NOTHROW) == doctest::String("WARNING"));
    CHECK(failureString(assertType::DT_CHECK_NOTHROW) == doctest::String("ERROR"));
    CHECK(failureString(assertType::DT_REQUIRE_NOTHROW) == doctest::String("FATAL ERROR"));

    CHECK(failureString(assertType::DT_WARN_EQ) == doctest::String("WARNING"));
    CHECK(failureString(assertType::DT_CHECK_EQ) == doctest::String("ERROR"));
    CHECK(failureString(assertType::DT_REQUIRE_EQ) == doctest::String("FATAL ERROR"));

    CHECK(failureString(assertType::DT_WARN_NE) == doctest::String("WARNING"));
    CHECK(failureString(assertType::DT_CHECK_NE) == doctest::String("ERROR"));
    CHECK(failureString(assertType::DT_REQUIRE_NE) == doctest::String("FATAL ERROR"));

    CHECK(failureString(assertType::DT_WARN_GT) == doctest::String("WARNING"));
    CHECK(failureString(assertType::DT_CHECK_GT) == doctest::String("ERROR"));
    CHECK(failureString(assertType::DT_REQUIRE_GT) == doctest::String("FATAL ERROR"));

    CHECK(failureString(assertType::DT_WARN_LT) == doctest::String("WARNING"));
    CHECK(failureString(assertType::DT_CHECK_LT) == doctest::String("ERROR"));
    CHECK(failureString(assertType::DT_REQUIRE_LT) == doctest::String("FATAL ERROR"));

    CHECK(failureString(assertType::DT_WARN_GE) == doctest::String("WARNING"));
    CHECK(failureString(assertType::DT_CHECK_GE) == doctest::String("ERROR"));
    CHECK(failureString(assertType::DT_REQUIRE_GE) == doctest::String("FATAL ERROR"));

    CHECK(failureString(assertType::DT_WARN_LE) == doctest::String("WARNING"));
    CHECK(failureString(assertType::DT_CHECK_LE) == doctest::String("ERROR"));
    CHECK(failureString(assertType::DT_REQUIRE_LE) == doctest::String("FATAL ERROR"));

    CHECK(failureString(assertType::DT_WARN_UNARY) == doctest::String("WARNING"));
    CHECK(failureString(assertType::DT_CHECK_UNARY) == doctest::String("ERROR"));
    CHECK(failureString(assertType::DT_REQUIRE_UNARY) == doctest::String("FATAL ERROR"));

    CHECK(failureString(assertType::DT_WARN_UNARY_FALSE) == doctest::String("WARNING"));
    CHECK(failureString(assertType::DT_CHECK_UNARY_FALSE) == doctest::String("ERROR"));
    CHECK(failureString(assertType::DT_REQUIRE_UNARY_FALSE) == doctest::String("FATAL ERROR"));
}
