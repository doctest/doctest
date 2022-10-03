#define DOCTEST_CONFIG_ASSERTS_RETURN_VALUES
#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <iostream>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

#ifndef TEST_FLIP
#define TEST_FLIP 0
#endif

#define TEST_FAIL() std::cout << "FAILED ON: " << __LINE__ \
    << "(" << (TEST_FLIP ? "EVALUATED" : "DISABLED") << ")" << std::endl

static int test_disabled_var_ = [] { // NOLINT
    // none may return true
    if (TEST_FLIP ^ CHECK(0 == 0)) { TEST_FAIL(); }
    if (TEST_FLIP ^ CHECK_FALSE(0 != 0)) { TEST_FAIL(); }
    if (TEST_FLIP ^ CHECK_EQ(0, 0)) { TEST_FAIL(); }
    if (TEST_FLIP ^ CHECK_UNARY(true)) { TEST_FAIL(); }
    if (TEST_FLIP ^ CHECK_UNARY_FALSE(false)) { TEST_FAIL(); }
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
    int i = 2; (void)i;
    if (TEST_FLIP ^ CHECK_THROWS([=] { throw i; }())) { TEST_FAIL(); }
    if (TEST_FLIP ^ CHECK_THROWS_AS([=] { throw i; }(), int)) { TEST_FAIL(); }
    if (TEST_FLIP ^ CHECK_NOTHROW([=]{ }())) { TEST_FAIL(); }
#endif

    return 0;
}();
