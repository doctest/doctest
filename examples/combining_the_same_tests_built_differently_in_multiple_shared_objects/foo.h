#pragma once
#include "doctest/doctest.h"
#include <type_traits>

DOCTEST_GCC_SUPPRESS_WARNING("-Wmissing-declarations")

#ifdef RETURN_42
#define TEST_LABEL "[return42] "
#else
#define TEST_LABEL "[default] "
#endif

int bar() {
#ifdef RETURN_42
    return 42;
#else
    return 11;
#endif
}

#ifdef DOCTEST_LIBRARY_INCLUDED

#ifdef RETURN_42
TEST_CASE(TEST_LABEL "bartest"){
    INFO("Running " TEST_LABEL "bartest"); MESSAGE("");
    CHECK_EQ(42, bar());
}
#endif

#ifndef RETURN_42
TEST_CASE(TEST_LABEL "bartest"){
    INFO("Running " TEST_LABEL "bartest"); MESSAGE("");
    CHECK_EQ(11, bar());
}
#endif

TEST_CASE(TEST_LABEL "commontest"){
    INFO("Running " TEST_LABEL "commontest"); MESSAGE("");
    auto x = bar();
    CHECK(std::is_same<decltype(x), int>::value);
}
#endif // DOCTEST_LIBRARY_INCLUDED
