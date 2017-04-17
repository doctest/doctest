#pragma once

#include "doctest.h"

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

TEST_CASE_TEMPLATE("template 1", T, doctest::Types<char>) {
    FAIL("");
}

TEST_CASE_TEMPLATE_DEFINE("template 2", T, header_test) {
    FAIL("");
}

TEST_CASE_TEMPLATE_INSTANTIATE(header_test, doctest::Types<doctest::String>);

// to silence GCC warnings when inheriting from the class SomeFixture which has no virtual destructor
//#if defined(__GNUC__) && !defined(__clang__)
//#pragma GCC diagnostic ignored "-Weffc++"
//#endif // __GNUC__

struct SomeFixture
{
    int data;
    SomeFixture()
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
