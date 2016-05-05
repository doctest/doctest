#ifndef DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES
#define DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES
#endif // DOCTEST_NO_SHORT_MACRO_NAMES

// no guard for including the doctest header itself because it should support multiple inclusion
#include "doctest.h"

#ifndef MY_PROXY_MACROS
#define MY_PROXY_MACROS

#define my_testcase            DOCTEST_TEST_CASE
#define my_testcase_fixture    DOCTEST_TEST_CASE_FIXTURE
#define my_subcase             DOCTEST_SUBCASE
#define my_testsuite           DOCTEST_TEST_SUITE
#define my_testsuite_end       DOCTEST_TEST_SUITE_END
#define my_check               DOCTEST_CHECK
#define my_check_false         DOCTEST_CHECK_FALSE
#define my_check_throws        DOCTEST_CHECK_THROWS
#define my_check_throws_as     DOCTEST_CHECK_THROWS_AS
#define my_check_nothrow       DOCTEST_CHECK_NOTHROW
#define my_require             DOCTEST_REQUIRE
#define my_require_false       DOCTEST_REQUIRE_FALSE
#define my_require_throws      DOCTEST_REQUIRE_THROWS
#define my_require_throws_as   DOCTEST_REQUIRE_THROWS_AS
#define my_require_nothrow     DOCTEST_REQUIRE_NOTHROW

#endif // MY_PROXY_MACROS
