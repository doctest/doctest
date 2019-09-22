#ifndef DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES
#define DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES
#endif // DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES

// no guard for including the doctest header itself because it should support multiple inclusion
#include <doctest/doctest.h>

#ifndef MY_PROXY_MACROS
#define MY_PROXY_MACROS

#define my_testcase                 DOCTEST_TEST_CASE
#define my_testcase_class           DOCTEST_TEST_CASE_CLASS
#define my_testcase_fixture         DOCTEST_TEST_CASE_FIXTURE
#define my_subcase                  DOCTEST_SUBCASE
#define my_testsuite                DOCTEST_TEST_SUITE
#define my_testsuite_begin          DOCTEST_TEST_SUITE_BEGIN
#define my_testsuite_end            DOCTEST_TEST_SUITE_END
#define my_warn                     DOCTEST_WARN
#define my_warn_false               DOCTEST_WARN_FALSE
#define my_warn_throws              DOCTEST_WARN_THROWS
#define my_warn_throws_as           DOCTEST_WARN_THROWS_AS
#define my_warn_throws_with         DOCTEST_WARN_THROWS_WITH
#define my_warn_throws_with_as      DOCTEST_WARN_THROWS_WITH_AS
#define my_warn_nothrow             DOCTEST_WARN_NOTHROW
#define my_check                    DOCTEST_CHECK
#define my_check_false              DOCTEST_CHECK_FALSE
#define my_check_throws             DOCTEST_CHECK_THROWS
#define my_check_throws_as          DOCTEST_CHECK_THROWS_AS
#define my_check_throws_with        DOCTEST_CHECK_THROWS_WITH
#define my_check_throws_with_as     DOCTEST_CHECK_THROWS_WITH_AS
#define my_check_nothrow            DOCTEST_CHECK_NOTHROW
#define my_require                  DOCTEST_REQUIRE
#define my_require_false            DOCTEST_REQUIRE_FALSE
#define my_require_throws           DOCTEST_REQUIRE_THROWS
#define my_require_throws_as        DOCTEST_REQUIRE_THROWS_AS
#define my_require_throws_with_as   DOCTEST_REQUIRE_THROWS_WITH_AS
#define my_require_nothrow          DOCTEST_REQUIRE_NOTHROW

#define my_scenario                 DOCTEST_SCENARIO
#define my_given                    DOCTEST_GIVEN
#define my_when                     DOCTEST_WHEN
#define my_and_when                 DOCTEST_AND_WHEN
#define my_then                     DOCTEST_THEN
#define my_and_then                 DOCTEST_AND_THEN

#define my_warn_eq                  DOCTEST_WARN_EQ
#define my_check_eq                 DOCTEST_CHECK_EQ
#define my_require_eq               DOCTEST_REQUIRE_EQ
#define my_warn_ne                  DOCTEST_WARN_NE
#define my_check_ne                 DOCTEST_CHECK_NE
#define my_require_ne               DOCTEST_REQUIRE_NE
#define my_warn_gt                  DOCTEST_WARN_GT
#define my_check_gt                 DOCTEST_CHECK_GT
#define my_require_gt               DOCTEST_REQUIRE_GT
#define my_warn_lt                  DOCTEST_WARN_LT
#define my_check_lt                 DOCTEST_CHECK_LT
#define my_require_lt               DOCTEST_REQUIRE_LT
#define my_warn_ge                  DOCTEST_WARN_GE
#define my_check_ge                 DOCTEST_CHECK_GE
#define my_require_ge               DOCTEST_REQUIRE_GE
#define my_warn_le                  DOCTEST_WARN_LE
#define my_check_le                 DOCTEST_CHECK_LE
#define my_require_le               DOCTEST_REQUIRE_LE
#define my_warn_unary               DOCTEST_WARN_UNARY
#define my_check_unary              DOCTEST_CHECK_UNARY
#define my_require_unary            DOCTEST_REQUIRE_UNARY
#define my_warn_unary_false         DOCTEST_WARN_UNARY_FALSE
#define my_check_unary_false        DOCTEST_CHECK_UNARY_FALSE
#define my_require_unary_false      DOCTEST_REQUIRE_UNARY_FALSE

#endif // MY_PROXY_MACROS
