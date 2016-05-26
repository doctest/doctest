#ifndef DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES
#define DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES
#endif // DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES

// no guard for including the doctest header itself because it should support multiple inclusion
#include "doctest.h"

#ifndef MY_PROXY_MACROS
#define MY_PROXY_MACROS

#define my_testcase            DOCTEST_TEST_CASE
#define my_testcase_fixture    DOCTEST_TEST_CASE_FIXTURE
#define my_subcase             DOCTEST_SUBCASE
#define my_testsuite           DOCTEST_TEST_SUITE
#define my_testsuite_end       DOCTEST_TEST_SUITE_END
#define my_warn                DOCTEST_WARN
#define my_warn_false          DOCTEST_WARN_FALSE
#define my_warn_throws         DOCTEST_WARN_THROWS
#define my_warn_throws_as      DOCTEST_WARN_THROWS_AS
#define my_warn_nothrow        DOCTEST_WARN_NOTHROW
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

#define my_scenario DOCTEST_SCENARIO
#define my_given DOCTEST_GIVEN
#define my_when DOCTEST_WHEN
#define my_and_when DOCTEST_AND_WHEN
#define my_then DOCTEST_THEN
#define my_and_then DOCTEST_AND_THEN

#endif // MY_PROXY_MACROS
