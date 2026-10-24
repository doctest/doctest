#include "doctest/doctest.h"

TEST_CASE("current_test_name returns the active test case name") {
    CHECK(std::strcmp(doctest::current_test_name(), "current_test_name returns the active test case name") == 0);
    CHECK(std::strcmp(doctest::current_subcase_name(), "") == 0);

    SUBCASE("inner") {
        CHECK(std::strcmp(doctest::current_subcase_name(), "inner") == 0);
    }
}
