#include "parts/doctest_fwd.h"

TEST_CASE("captain obvious") {
    bool b = false;
    CHECK(b);

    CHECK_UNARY_FALSE(true);

    WARN_GT(false, false);
    CHECK_EQ(false, true);
    REQUIRE_EQ(false, true);
}
