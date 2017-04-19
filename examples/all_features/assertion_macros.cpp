#include "doctest.h"

#include "header.h"

#include <stdexcept>

TEST_CASE("normal macros") {
    int a = 5;
    int b = 5;

    CHECK(throw_if(true, std::runtime_error("whops!")) == 42);

    CHECK_FALSE(!(a == b));

    REQUIRE(a == b);

    CHECK_EQ(a, b);

    FAST_CHECK_EQ(a, b);

    CHECK(doctest::Approx(0.1000001) == 0.1000002);
    CHECK(doctest::Approx(0.502) == 0.501);

    throw_if(true, 0);
}

TEST_CASE("exceptions-related macros") {
    CHECK_THROWS(throw_if(false, 0));
    CHECK_THROWS_AS(throw_if(false, 0), int);
    CHECK_THROWS_AS(throw_if(true, 0), int);
    CHECK_THROWS_AS(throw_if(true, 0), char);

    CHECK_NOTHROW(throw_if(true, 0));
}

TEST_CASE("exceptions-related macros for std::exception") {
    CHECK_THROWS(throw_if(false, 0));
    CHECK_THROWS_AS(throw_if(false, std::runtime_error("whops!")), std::exception&);
    CHECK_THROWS_AS(throw_if(true, std::runtime_error("whops!")), std::exception&);
    CHECK_THROWS_AS(throw_if(true, std::runtime_error("whops!")), int);

    REQUIRE_NOTHROW(throw_if(true, std::runtime_error("whops!")));
}
