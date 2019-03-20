#pragma once

#include <doctest/doctest.h>

// helper for throwing exceptions
template <typename T>
int throw_if(bool in, const T& ex) {
    if(in)
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
        throw ex;
#else  // DOCTEST_CONFIG_NO_EXCEPTIONS
        ((void)ex);
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS
    return 42;
}

// stuff that should be fine when used in a header - test cases for example should be registered only once

TEST_SUITE("some TS") {
    TEST_CASE("in TS") {
        FAIL("");
    }
}

REGISTER_EXCEPTION_TRANSLATOR(int& in) {
    return doctest::toString(in);
}

TYPE_TO_STRING(doctest::String);

TEST_CASE_TEMPLATE("template 1", T, char) {
    FAIL("");
}

TEST_CASE_TEMPLATE_DEFINE("template 2", T, header_test) {
    FAIL("");
}

TEST_CASE_TEMPLATE_INVOKE(header_test, doctest::String);

// to silence GCC warnings when inheriting from some class which has no virtual destructor - happens only on gcc 4.7/4.8
#if DOCTEST_GCC >= DOCTEST_COMPILER(4, 7, 0) && DOCTEST_GCC < DOCTEST_COMPILER(4, 9, 0)
DOCTEST_GCC_SUPPRESS_WARNING("-Weffc++")
#endif // gcc 4.7 / 4.8
#if DOCTEST_GCC >= DOCTEST_COMPILER(5, 0, 0) && DOCTEST_GCC < DOCTEST_COMPILER(6, 0, 0)
DOCTEST_GCC_SUPPRESS_WARNING("-Wstrict-overflow")
#endif // gcc 5

struct SomeFixture
{
    int data;
    SomeFixture() noexcept
            : data(42) {
        // setup here
    }

    ~SomeFixture() {
        // teardown here
    }
};

TEST_CASE_FIXTURE(SomeFixture, "fixtured test") {
    data /= 2;
    CHECK(data == 21);
}
