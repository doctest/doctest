#include "parts/doctest_fwd.h"
#include "parts/doctest_fast.h"

TEST_CASE("captain obvious") {
    //CHECK(true == false);

    WARN_EQ(false, true);
    CHECK_EQ(false, true);
    REQUIRE_EQ(false, true);
}
