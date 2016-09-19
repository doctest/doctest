#pragma once

#include "doctest.h"

// this test will be registered only once even though it is in a header file
TEST_CASE("hdr.h") {
    CHECK(1 == 0);
}

