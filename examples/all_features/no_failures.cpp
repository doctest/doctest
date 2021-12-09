#include <doctest/doctest.h>

TEST_CASE("no checks") {}

TEST_CASE("simple check") {
    CHECK(1 == 1);
}

TEST_SUITE("some suite") {
    TEST_CASE("fails - and its allowed" * doctest::may_fail()) { FAIL(""); }
}

TEST_CASE("should fail and no output" * doctest::should_fail() * doctest::no_breaks() * doctest::no_output()) { FAIL(""); }
