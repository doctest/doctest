#include "doctest.h"

template<typename T>
static int conditional_throw(bool in, const T& ex) {
    if(in)
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
        throw ex;
#else // DOCTEST_CONFIG_NO_EXCEPTIONS
        ((void)ex);
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS
    return 42;
}

using doctest::Approx;

TEST_SUITE("meaningless macros") {
    TEST_CASE("an empty test that will succeed") {}
    
    TEST_CASE("an empty test that will fail because of an exception") { conditional_throw(true, 0); }
}

TEST_SUITE_BEGIN("meaningless macros");

TEST_CASE("an empty test that will fail because of a std::exception") { conditional_throw(true, std::runtime_error("whops!")); }

TEST_SUITE_END();

TEST_CASE("normal macros") {
    int a = 5;
    int b = 5;

    CHECK(conditional_throw(true, std::runtime_error("whops!")) == 42);

    CHECK_FALSE(!(a == b));

    REQUIRE(a == b);

    CHECK_EQ(a, b);

    FAST_CHECK_EQ(a, b);

    // commented out because 32 vs 64 bit builds will fail when the output is compared
    //WARN(reinterpret_cast<void*>(1000) == reinterpret_cast<void*>(1004));

    CHECK(Approx(0.1000001) == 0.1000002);
    CHECK(Approx(0.502) == 0.501);

    conditional_throw(true, 0);
}

TEST_CASE("normal macros with std::exception") {
    int a = 5;
    int b = 5;

    CHECK(conditional_throw(true, 0) == 42);

    CHECK_FALSE(!(a == b));

    REQUIRE(a == b);

    CHECK_EQ(a, b);

    FAST_CHECK_EQ(a, b);

    // commented out because 32 vs 64 bit builds will fail when the output is compared
    //WARN(reinterpret_cast<void*>(1000) == reinterpret_cast<void*>(1004));

    CHECK(Approx(0.1000001) == 0.1000002);
    CHECK(Approx(0.502) == 0.501);

    conditional_throw(true, 0);
}

TEST_CASE("exceptions-related macros") {
    CHECK_THROWS(conditional_throw(false, 0));
    CHECK_THROWS_AS(conditional_throw(false, 0), int);
    CHECK_THROWS_AS(conditional_throw(true, 0), int);
    CHECK_THROWS_AS(conditional_throw(true, 0), char);

    CHECK_NOTHROW(conditional_throw(true, 0));
}

TEST_CASE("exceptions-related macros for std::exception") {
    CHECK_THROWS(conditional_throw(false, 0));
    CHECK_THROWS_AS(conditional_throw(false, std::runtime_error("whops!")), std::exception);
    CHECK_THROWS_AS(conditional_throw(true, std::runtime_error("whops!")), std::exception);
    CHECK_THROWS_AS(conditional_throw(true, std::runtime_error("whops!")), int);

    REQUIRE_NOTHROW(conditional_throw(true, std::runtime_error("whops!")));
}
