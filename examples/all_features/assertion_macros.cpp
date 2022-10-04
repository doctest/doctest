#define DOCTEST_CONFIG_ASSERTS_RETURN_VALUES
#include <doctest/doctest.h>

#include "header.h"

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <stdexcept>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

TEST_CASE("normal macros") {
    int a = 5;
    int b = 5;

    CHECK(throw_if(true, std::runtime_error("whops!")) == 42);

    CHECK_FALSE(!(a == b));

    REQUIRE(a == b);

    CHECK_EQ(a, b);

    CHECK(doctest::Approx(0.1000001) == 0.1000002);
    CHECK(doctest::Approx(0.502) == 0.501);
}
