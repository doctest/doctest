#include <doctest/doctest.h>

TEST_SUITE("doctest unit tests") {
    TEST_CASE("doctest::String::substr()") {
        const doctest::String abcde = "abcde";
        CHECK(abcde.substr(0, 3) == "abc");
        CHECK(abcde.substr(2, 3) == "cde");
        CHECK(abcde.substr(0, 5) == "abcde");
    }
}