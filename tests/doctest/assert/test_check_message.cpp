#include <doctest/doctest.h>

#include <functional>

TEST_CASE("CHECK_MESSAGE with lambda and MESSAGE") {
    CHECK_MESSAGE(
        []() {
            MESSAGE("message!!");
            return true;
        }(),
        "lambda Failed!!");
}
