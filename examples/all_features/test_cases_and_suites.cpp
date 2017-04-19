#include "doctest.h"

#include "header.h"

static void doStuff() {
    int a = 5;
    a += 2;
    if(doctest::isRunningInTest())
        CHECK(a == 7); // asserts and other doctest functionality can be used in user code if checked for a testing context
}

TEST_CASE("an empty test that will succeed - not part of a test suite") {}

TEST_CASE("should fail because of an exception") {
    doStuff();

    throw_if(true, 0);
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

TEST_CASE_FIXTURE(SomeFixture, "fixtured test - not part of a test suite") {
    data /= 2;
    CHECK(data == 85);
}
