// Sometimes, you just want to run a handful of tests, and you don't want to
// think of the correct command-line filter to use. In cases like this,
// you can use `doctest:focus()`. Like many other test frameworks,
// if there is at least one test marked as focused, then all non-focused
// tests are implicitly skipped.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_CASE("This test will be executed, as it is marked as focused" * doctest::focus()) {}

TEST_CASE("This test will not be executed, since at least one test is focused") {}

TEST_CASE("This test will not be executed, since skipping overrides focus" * doctest::focus() * doctest::skip()) {}

TEST_SUITE("Test cases will inherit focusing from their surrounding suite" * doctest::focus()) {

    TEST_CASE("This test will be excuted, as it is transitively marked as focused") {}
}
