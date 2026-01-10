#define DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
#include <doctest/doctest.h>

TEST_CASE("Assertion stringification") {
    using namespace doctest;

    CHECK(assertString(assertType::DT_WARN) == "WARN");
    CHECK(assertString(assertType::DT_CHECK) == "CHECK");
    CHECK(assertString(assertType::DT_REQUIRE) == "REQUIRE");

    CHECK(assertString(assertType::DT_WARN_FALSE) == "WARN_FALSE");
    CHECK(assertString(assertType::DT_CHECK_FALSE) == "CHECK_FALSE");
    CHECK(assertString(assertType::DT_REQUIRE_FALSE) == "REQUIRE_FALSE");

    CHECK(assertString(assertType::DT_WARN_THROWS) == "WARN_THROWS");
    CHECK(assertString(assertType::DT_CHECK_THROWS) == "CHECK_THROWS");
    CHECK(assertString(assertType::DT_REQUIRE_THROWS) == "REQUIRE_THROWS");

    CHECK(assertString(assertType::DT_WARN_THROWS_AS) == "WARN_THROWS_AS");
    CHECK(assertString(assertType::DT_CHECK_THROWS_AS) == "CHECK_THROWS_AS");
    CHECK(assertString(assertType::DT_REQUIRE_THROWS_AS) == "REQUIRE_THROWS_AS");

    CHECK(assertString(assertType::DT_WARN_THROWS_WITH) == "WARN_THROWS_WITH");
    CHECK(assertString(assertType::DT_CHECK_THROWS_WITH) == "CHECK_THROWS_WITH");
    CHECK(assertString(assertType::DT_REQUIRE_THROWS_WITH) == "REQUIRE_THROWS_WITH");

    CHECK(assertString(assertType::DT_WARN_THROWS_WITH_AS) == "WARN_THROWS_WITH_AS");
    CHECK(assertString(assertType::DT_CHECK_THROWS_WITH_AS) == "CHECK_THROWS_WITH_AS");
    CHECK(assertString(assertType::DT_REQUIRE_THROWS_WITH_AS) == "REQUIRE_THROWS_WITH_AS");

    CHECK(assertString(assertType::DT_WARN_NOTHROW) == "WARN_NOTHROW");
    CHECK(assertString(assertType::DT_CHECK_NOTHROW) == "CHECK_NOTHROW");
    CHECK(assertString(assertType::DT_REQUIRE_NOTHROW) == "REQUIRE_NOTHROW");

    CHECK(assertString(assertType::DT_WARN_EQ) == "WARN_EQ");
    CHECK(assertString(assertType::DT_CHECK_EQ) == "CHECK_EQ");
    CHECK(assertString(assertType::DT_REQUIRE_EQ) == "REQUIRE_EQ");

    CHECK(assertString(assertType::DT_WARN_NE) == "WARN_NE");
    CHECK(assertString(assertType::DT_CHECK_NE) == "CHECK_NE");
    CHECK(assertString(assertType::DT_REQUIRE_NE) == "REQUIRE_NE");

    CHECK(assertString(assertType::DT_WARN_GT) == "WARN_GT");
    CHECK(assertString(assertType::DT_CHECK_GT) == "CHECK_GT");
    CHECK(assertString(assertType::DT_REQUIRE_GT) == "REQUIRE_GT");

    CHECK(assertString(assertType::DT_WARN_LT) == "WARN_LT");
    CHECK(assertString(assertType::DT_CHECK_LT) == "CHECK_LT");
    CHECK(assertString(assertType::DT_REQUIRE_LT) == "REQUIRE_LT");

    CHECK(assertString(assertType::DT_WARN_GE) == "WARN_GE");
    CHECK(assertString(assertType::DT_CHECK_GE) == "CHECK_GE");
    CHECK(assertString(assertType::DT_REQUIRE_GE) == "REQUIRE_GE");

    CHECK(assertString(assertType::DT_WARN_LE) == "WARN_LE");
    CHECK(assertString(assertType::DT_CHECK_LE) == "CHECK_LE");
    CHECK(assertString(assertType::DT_REQUIRE_LE) == "REQUIRE_LE");

    CHECK(assertString(assertType::DT_WARN_UNARY) == "WARN_UNARY");
    CHECK(assertString(assertType::DT_CHECK_UNARY) == "CHECK_UNARY");
    CHECK(assertString(assertType::DT_REQUIRE_UNARY) == "REQUIRE_UNARY");

    CHECK(assertString(assertType::DT_WARN_UNARY_FALSE) == "WARN_UNARY_FALSE");
    CHECK(assertString(assertType::DT_CHECK_UNARY_FALSE) == "CHECK_UNARY_FALSE");
    CHECK(assertString(assertType::DT_REQUIRE_UNARY_FALSE) == "REQUIRE_UNARY_FALSE");
}

TEST_CASE("Failure stringification") {
    using namespace doctest;

    CHECK(failureString(assertType::DT_WARN) == "WARNING");
    CHECK(failureString(assertType::DT_CHECK) == "ERROR");
    CHECK(failureString(assertType::DT_REQUIRE) == "FATAL ERROR");

    CHECK(failureString(assertType::DT_WARN_FALSE) == "WARNING");
    CHECK(failureString(assertType::DT_CHECK_FALSE) == "ERROR");
    CHECK(failureString(assertType::DT_REQUIRE_FALSE) == "FATAL ERROR");

    CHECK(failureString(assertType::DT_WARN_THROWS) == "WARNING");
    CHECK(failureString(assertType::DT_CHECK_THROWS) == "ERROR");
    CHECK(failureString(assertType::DT_REQUIRE_THROWS) == "FATAL ERROR");

    CHECK(failureString(assertType::DT_WARN_THROWS_AS) == "WARNING");
    CHECK(failureString(assertType::DT_CHECK_THROWS_AS) == "ERROR");
    CHECK(failureString(assertType::DT_REQUIRE_THROWS_AS) == "FATAL ERROR");

    CHECK(failureString(assertType::DT_WARN_THROWS_WITH) == "WARNING");
    CHECK(failureString(assertType::DT_CHECK_THROWS_WITH) == "ERROR");
    CHECK(failureString(assertType::DT_REQUIRE_THROWS_WITH) == "FATAL ERROR");

    CHECK(failureString(assertType::DT_WARN_THROWS_WITH_AS) == "WARNING");
    CHECK(failureString(assertType::DT_CHECK_THROWS_WITH_AS) == "ERROR");
    CHECK(failureString(assertType::DT_REQUIRE_THROWS_WITH_AS) == "FATAL ERROR");

    CHECK(failureString(assertType::DT_WARN_NOTHROW) == "WARNING");
    CHECK(failureString(assertType::DT_CHECK_NOTHROW) == "ERROR");
    CHECK(failureString(assertType::DT_REQUIRE_NOTHROW) == "FATAL ERROR");

    CHECK(failureString(assertType::DT_WARN_EQ) == "WARNING");
    CHECK(failureString(assertType::DT_CHECK_EQ) == "ERROR");
    CHECK(failureString(assertType::DT_REQUIRE_EQ) == "FATAL ERROR");

    CHECK(failureString(assertType::DT_WARN_NE) == "WARNING");
    CHECK(failureString(assertType::DT_CHECK_NE) == "ERROR");
    CHECK(failureString(assertType::DT_REQUIRE_NE) == "FATAL ERROR");

    CHECK(failureString(assertType::DT_WARN_GT) == "WARNING");
    CHECK(failureString(assertType::DT_CHECK_GT) == "ERROR");
    CHECK(failureString(assertType::DT_REQUIRE_GT) == "FATAL ERROR");

    CHECK(failureString(assertType::DT_WARN_LT) == "WARNING");
    CHECK(failureString(assertType::DT_CHECK_LT) == "ERROR");
    CHECK(failureString(assertType::DT_REQUIRE_LT) == "FATAL ERROR");

    CHECK(failureString(assertType::DT_WARN_GE) == "WARNING");
    CHECK(failureString(assertType::DT_CHECK_GE) == "ERROR");
    CHECK(failureString(assertType::DT_REQUIRE_GE) == "FATAL ERROR");

    CHECK(failureString(assertType::DT_WARN_LE) == "WARNING");
    CHECK(failureString(assertType::DT_CHECK_LE) == "ERROR");
    CHECK(failureString(assertType::DT_REQUIRE_LE) == "FATAL ERROR");

    CHECK(failureString(assertType::DT_WARN_UNARY) == "WARNING");
    CHECK(failureString(assertType::DT_CHECK_UNARY) == "ERROR");
    CHECK(failureString(assertType::DT_REQUIRE_UNARY) == "FATAL ERROR");

    CHECK(failureString(assertType::DT_WARN_UNARY_FALSE) == "WARNING");
    CHECK(failureString(assertType::DT_CHECK_UNARY_FALSE) == "ERROR");
    CHECK(failureString(assertType::DT_REQUIRE_UNARY_FALSE) == "FATAL ERROR");
}
