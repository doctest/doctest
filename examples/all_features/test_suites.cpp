#include "doctest.h"

TEST_CASE("not part of a test suite") {
    FAIL("");
}

TEST_SUITE("scoped test suite") {
    TEST_CASE("part of scoped") {
        FAIL("");
    }

    TEST_CASE("part of scoped 2") {
        FAIL("");
    }
}

TEST_SUITE_BEGIN("some TS"); // begin "some TS"

TEST_CASE("part of some TS") {
    FAIL("");
}

TEST_SUITE_END(); // ends "some TS"

TEST_CASE("not part of a test suite 2") {
    FAIL("");
}
