#include <doctest/doctest.h>

#ifdef _MSC_VER
#define __STDC_WANT_SECURE_LIB__ 1
#endif
#include <iostream>
#include <limits>


#ifndef TEST_FLIP
#define TEST_FLIP 0
#endif

#define TEST_FAIL() std::cout << "FAILED ON: " << __LINE__ \
    << " (" << (TEST_FLIP ? "EVALUATED" : "DISABLED") << ")\n";

static int test_disabled_var_ = [&] {
    (void)&test_disabled_var_;
    // none may return true
    if (TEST_FLIP ^ CHECK(0 == 0)) { TEST_FAIL(); }
    if (TEST_FLIP ^ CHECK_FALSE(0 != 0)) { TEST_FAIL(); }
    if (TEST_FLIP ^ CHECK_EQ(0, 0)) { TEST_FAIL(); }
    if (TEST_FLIP ^ CHECK_UNARY(true)) { TEST_FAIL(); }
    if (TEST_FLIP ^ CHECK_UNARY_FALSE(false)) { TEST_FAIL(); }
    if (TEST_FLIP ^ CHECK_THROWS([] { throw 2; }())) { TEST_FAIL(); }
    if (TEST_FLIP ^ CHECK_THROWS_AS([] { throw 2; }(), int)) { TEST_FAIL(); }
    if (TEST_FLIP ^ CHECK_NOTHROW([]{ }())) { TEST_FAIL(); }
    if (CHECK_THROWS_WITH([] { throw 2; }(), "2")) { TEST_FAIL(); }
    if (CHECK_NAN(std::numeric_limits<float>::quiet_NaN())) { TEST_FAIL(); }
    if (CHECK_NOT_NAN(2'2.)) { TEST_FAIL(); }

    return 0;
}();
