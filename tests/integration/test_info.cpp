#include <doctest/doctest.h>
#include <doctest/parts/private/reporters/junit.h> // see issue #1035
#include <doctest/parts/private/reporters/xml.h>
#include <utils/test_case.h>
#include <utils/text.h>
using namespace utils;


TEST_CASE("No interlaced assertions" * doctest::test_suite("INTERNAL")) {
    INFO("Before");
    INFO("Middle");
    INFO("After");
}

TEST_CASE("No interlaced assertions") {
    auto tc   = test_case::current().internal();
    auto opts = test_case::options();

    SUBCASE("Console reporting (without --success)") {
        const auto result = tc.run(opts.success(false));

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == text::dedent(R"(
            ===============================================================================
            [doctest] test cases: 1 | 1 passed | 0 failed |
            [doctest] assertions: 0 | 0 passed | 0 failed |
            [doctest] Status: SUCCESS!
        )"));
    }

    SUBCASE("Console reporting (with --success)") {
        const auto result = tc.run(opts.success(true));

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == text::dedent(R"(
            ===============================================================================
            [doctest] test cases: 1 | 1 passed | 0 failed |
            [doctest] assertions: 0 | 0 passed | 0 failed |
            [doctest] Status: SUCCESS!
        )"));
    }

    SUBCASE("JUnit reporting") {
        const auto result = tc.run(opts.junit(true));

        CHECK(result.exit_code == 0);
        CHECK(result.err.empty());
        CHECK(result.out == text::dedent(R"(
            <?xml version="1.0" encoding="UTF-8"?>
            <testsuites>
              <testsuite name="integration_test_info_cpp" errors="0" failures="0" tests="0">
                <testcase classname="test_info.cpp" name="No interlaced assertions" status="run"/>
              </testsuite>
            </testsuites>
        )"));
    }

    SUBCASE("XML reporting") {
        const auto result = tc.run(opts.xml(true));

        CHECK(result.exit_code == 0);
        CHECK(result.err.empty());
        CHECK(result.out == text::dedent(R"(
            <?xml version="1.0" encoding="UTF-8"?>
            <doctest binary="integration_test_info_cpp">
              <Options order_by="file" rand_seed="0" first="0" last="4294967295" abort_after="0" subcase_filter_levels="2147483647" case_sensitive="false" no_throw="false" no_skip="false"/>
              <TestSuite name="INTERNAL">
                <TestCase name="No interlaced assertions" filename="test_info.cpp" line="9">
                  <OverallResultsAsserts successes="0" failures="0" test_case_success="true"/>
                </TestCase>
              </TestSuite>
              <OverallResultsAsserts successes="0" failures="0"/>
              <OverallResultsTestCases successes="1" failures="0"/>
            </doctest>
        )"));
    }
}


TEST_CASE("Failing WARN-like assertion" * doctest::test_suite("INTERNAL")) {
    INFO("Before");
    WARN(1 + 1 == 2);
    INFO("Middle");
    WARN(2 + 2 == 5);
    INFO("After");
}

TEST_CASE("Failing WARN-like assertion") {
    auto tc   = test_case::current().internal();
    auto opts = test_case::options();

    SUBCASE("Console reporting (without --success)") {
        const auto result = tc.run(opts.success(false));

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == text::dedent(R"(
            ===============================================================================
            test_info.cpp:82:
            TEST SUITE: INTERNAL
            TEST CASE:  Failing WARN-like assertion

            test_info.cpp:86: WARNING: WARN( 2 + 2 == 5 ) is NOT correct!
              values: WARN( 4 == 5 )
              logged: Before
                      Middle

            ===============================================================================
            [doctest] test cases: 1 | 1 passed | 0 failed |
            [doctest] assertions: 0 | 0 passed | 0 failed |
            [doctest] Status: SUCCESS!
        )"));
    }

    SUBCASE("Console reporting (with --success)") {
        const auto result = tc.run(opts.success(true));

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == text::dedent(R"(
            ===============================================================================
            test_info.cpp:82:
            TEST SUITE: INTERNAL
            TEST CASE:  Failing WARN-like assertion

            test_info.cpp:84: SUCCESS: WARN( 1 + 1 == 2 ) is correct!
              values: WARN( 2 == 2 )
              logged: Before

            test_info.cpp:86: WARNING: WARN( 2 + 2 == 5 ) is NOT correct!
              values: WARN( 4 == 5 )
              logged: Before
                      Middle

            ===============================================================================
            [doctest] test cases: 1 | 1 passed | 0 failed |
            [doctest] assertions: 0 | 0 passed | 0 failed |
            [doctest] Status: SUCCESS!
        )"));
    }

    SUBCASE("JUnit reporting") {
        const auto result = tc.run(opts.junit(true));

        CHECK(result.exit_code == 0);
        CHECK(result.err.empty());
        CHECK(result.out == text::dedent(R"(
            <?xml version="1.0" encoding="UTF-8"?>
            <testsuites>
              <testsuite name="integration_test_info_cpp" errors="0" failures="1" tests="0">
                <testcase classname="test_info.cpp" name="Failing WARN-like assertion" status="run">
                  <failure message="4 == 5" type="WARN">
            test_info.cpp:86:
            WARN( 2 + 2 == 5 ) is NOT correct!
              values: WARN( 4 == 5 )
              logged: Before
                      Middle

                  </failure>
                </testcase>
              </testsuite>
            </testsuites>
        )"));
    }

    SUBCASE("XML reporting") {
        const auto result = tc.run(opts.xml(true));

        CHECK(result.exit_code == 0);
        CHECK(result.err.empty());
        CHECK(result.out == text::dedent(R"(
            <?xml version="1.0" encoding="UTF-8"?>
            <doctest binary="integration_test_info_cpp">
              <Options order_by="file" rand_seed="0" first="0" last="4294967295" abort_after="0" subcase_filter_levels="2147483647" case_sensitive="false" no_throw="false" no_skip="false"/>
              <TestSuite name="INTERNAL">
                <TestCase name="Failing WARN-like assertion" filename="test_info.cpp" line="82">
                  <Expression success="false" type="WARN" filename="test_info.cpp" line="86">
                    <Original>
                      2 + 2 == 5
                    </Original>
                    <Expanded>
                      4 == 5
                    </Expanded>
                    <Info>
                      Before
                    </Info>
                    <Info>
                      Middle
                    </Info>
                  </Expression>
                  <OverallResultsAsserts successes="0" failures="0" test_case_success="true"/>
                </TestCase>
              </TestSuite>
              <OverallResultsAsserts successes="0" failures="0"/>
              <OverallResultsTestCases successes="1" failures="0"/>
            </doctest>
        )"));
    }
}


TEST_CASE("Failing CHECK-like assertion" * doctest::test_suite("INTERNAL")) {
    INFO("Before");
    CHECK(1 + 1 == 2);
    INFO("Middle");
    CHECK(2 + 2 == 5);
    INFO("After");
}

TEST_CASE("Failing CHECK-like assertion") {
    auto tc   = test_case::current().internal();
    auto opts = test_case::options();

    SUBCASE("Console reporting (without --success)") {
        const auto result = tc.run(opts.success(false));

        CHECK(result.exit_code != 0);
        CHECK(result.err == "");
        CHECK(result.out == text::dedent(R"(
            ===============================================================================
            test_info.cpp:204:
            TEST SUITE: INTERNAL
            TEST CASE:  Failing CHECK-like assertion

            test_info.cpp:208: ERROR: CHECK( 2 + 2 == 5 ) is NOT correct!
              values: CHECK( 4 == 5 )
              logged: Before
                      Middle

            ===============================================================================
            [doctest] test cases: 1 | 0 passed | 1 failed |
            [doctest] assertions: 2 | 1 passed | 1 failed |
            [doctest] Status: FAILURE!
        )"));
    }

    SUBCASE("Console reporting (with --success)") {
        const auto result = tc.run(opts.success(true));

        CHECK(result.exit_code != 0);
        CHECK(result.err == "");
        CHECK(result.out == text::dedent(R"(
            ===============================================================================
            test_info.cpp:204:
            TEST SUITE: INTERNAL
            TEST CASE:  Failing CHECK-like assertion

            test_info.cpp:206: SUCCESS: CHECK( 1 + 1 == 2 ) is correct!
              values: CHECK( 2 == 2 )
              logged: Before

            test_info.cpp:208: ERROR: CHECK( 2 + 2 == 5 ) is NOT correct!
              values: CHECK( 4 == 5 )
              logged: Before
                      Middle

            ===============================================================================
            [doctest] test cases: 1 | 0 passed | 1 failed |
            [doctest] assertions: 2 | 1 passed | 1 failed |
            [doctest] Status: FAILURE!
        )"));
    }

    SUBCASE("JUnit reporting") {
        const auto result = tc.run(opts.junit(true));

        CHECK(result.exit_code != 0);
        CHECK(result.err.empty());
        CHECK(result.out == text::dedent(R"(
            <?xml version="1.0" encoding="UTF-8"?>
            <testsuites>
              <testsuite name="integration_test_info_cpp" errors="0" failures="1" tests="2">
                <testcase classname="test_info.cpp" name="Failing CHECK-like assertion" status="run">
                  <failure message="4 == 5" type="CHECK">
            test_info.cpp:208:
            CHECK( 2 + 2 == 5 ) is NOT correct!
              values: CHECK( 4 == 5 )
              logged: Before
                      Middle

                  </failure>
                </testcase>
              </testsuite>
            </testsuites>
        )"));
    }

    SUBCASE("XML reporting") {
        const auto result = tc.run(opts.xml(true));

        CHECK(result.exit_code != 0);
        CHECK(result.err.empty());
        CHECK(result.out == text::dedent(R"(
            <?xml version="1.0" encoding="UTF-8"?>
            <doctest binary="integration_test_info_cpp">
              <Options order_by="file" rand_seed="0" first="0" last="4294967295" abort_after="0" subcase_filter_levels="2147483647" case_sensitive="false" no_throw="false" no_skip="false"/>
              <TestSuite name="INTERNAL">
                <TestCase name="Failing CHECK-like assertion" filename="test_info.cpp" line="204">
                  <Expression success="false" type="CHECK" filename="test_info.cpp" line="208">
                    <Original>
                      2 + 2 == 5
                    </Original>
                    <Expanded>
                      4 == 5
                    </Expanded>
                    <Info>
                      Before
                    </Info>
                    <Info>
                      Middle
                    </Info>
                  </Expression>
                  <OverallResultsAsserts successes="1" failures="1" test_case_success="false"/>
                </TestCase>
              </TestSuite>
              <OverallResultsAsserts successes="1" failures="1"/>
              <OverallResultsTestCases successes="0" failures="1"/>
            </doctest>
        )"));
    }
}


TEST_CASE("Failing REQUIRE-like assertion" * doctest::test_suite("INTERNAL")) {
    INFO("Before");
    REQUIRE(1 + 1 == 2);
    INFO("Middle");
    REQUIRE(2 + 2 == 5);
    INFO("After");
}

TEST_CASE("Failing REQUIRE-like assertion") {
    auto tc   = test_case::current().internal();
    auto opts = test_case::options();

    SUBCASE("Console reporting (without --success)") {
        const auto result = tc.run(opts.success(false));

        CHECK(result.exit_code != 0);
        CHECK(result.err == "");
        CHECK(result.out == text::dedent(R"(
            ===============================================================================
            test_info.cpp:326:
            TEST SUITE: INTERNAL
            TEST CASE:  Failing REQUIRE-like assertion

            test_info.cpp:330: FATAL ERROR: REQUIRE( 2 + 2 == 5 ) is NOT correct!
              values: REQUIRE( 4 == 5 )
              logged: Before
                      Middle

            ===============================================================================
            [doctest] test cases: 1 | 0 passed | 1 failed |
            [doctest] assertions: 2 | 1 passed | 1 failed |
            [doctest] Status: FAILURE!
        )"));
    }

    SUBCASE("Console reporting (with --success)") {
        const auto result = tc.run(opts.success(true));

        CHECK(result.exit_code != 0);
        CHECK(result.err == "");
        CHECK(result.out == text::dedent(R"(
            ===============================================================================
            test_info.cpp:326:
            TEST SUITE: INTERNAL
            TEST CASE:  Failing REQUIRE-like assertion

            test_info.cpp:328: SUCCESS: REQUIRE( 1 + 1 == 2 ) is correct!
              values: REQUIRE( 2 == 2 )
              logged: Before

            test_info.cpp:330: FATAL ERROR: REQUIRE( 2 + 2 == 5 ) is NOT correct!
              values: REQUIRE( 4 == 5 )
              logged: Before
                      Middle

            ===============================================================================
            [doctest] test cases: 1 | 0 passed | 1 failed |
            [doctest] assertions: 2 | 1 passed | 1 failed |
            [doctest] Status: FAILURE!
        )"));
    }

    SUBCASE("JUnit reporting") {
        const auto result = tc.run(opts.junit(true));

        CHECK(result.exit_code != 0);
        CHECK(result.err.empty());
        CHECK(result.out == text::dedent(R"(
            <?xml version="1.0" encoding="UTF-8"?>
            <testsuites>
              <testsuite name="integration_test_info_cpp" errors="0" failures="1" tests="2">
                <testcase classname="test_info.cpp" name="Failing REQUIRE-like assertion" status="run">
                  <failure message="4 == 5" type="REQUIRE">
            test_info.cpp:330:
            REQUIRE( 2 + 2 == 5 ) is NOT correct!
              values: REQUIRE( 4 == 5 )
              logged: Before
                      Middle

                  </failure>
                </testcase>
              </testsuite>
            </testsuites>
        )"));
    }

    SUBCASE("XML reporting") {
        const auto result = tc.run(opts.xml(true));

        CHECK(result.exit_code != 0);
        CHECK(result.err.empty());
        CHECK(result.out == text::dedent(R"(
            <?xml version="1.0" encoding="UTF-8"?>
            <doctest binary="integration_test_info_cpp">
              <Options order_by="file" rand_seed="0" first="0" last="4294967295" abort_after="0" subcase_filter_levels="2147483647" case_sensitive="false" no_throw="false" no_skip="false"/>
              <TestSuite name="INTERNAL">
                <TestCase name="Failing REQUIRE-like assertion" filename="test_info.cpp" line="326">
                  <Expression success="false" type="REQUIRE" filename="test_info.cpp" line="330">
                    <Original>
                      2 + 2 == 5
                    </Original>
                    <Expanded>
                      4 == 5
                    </Expanded>
                    <Info>
                      Before
                    </Info>
                    <Info>
                      Middle
                    </Info>
                  </Expression>
                  <OverallResultsAsserts successes="1" failures="1" test_case_success="false"/>
                </TestCase>
              </TestSuite>
              <OverallResultsAsserts successes="1" failures="1"/>
              <OverallResultsTestCases successes="0" failures="1"/>
            </doctest>
        )"));
    }
}


DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wmissing-noreturn")
DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4702)
TEST_CASE("Abnormal termination" * doctest::test_suite("INTERNAL")) {
    INFO("Before");
    CHECK(1 + 1 == 2);
    INFO("Middle");
    std::abort();
    INFO("After");
}
DOCTEST_CLANG_SUPPRESS_WARNING_POP
DOCTEST_MSVC_SUPPRESS_WARNING_POP

TEST_CASE("Abnormal termination" * doctest::skip()) {
    /** Currently triggers tsan violations */
    auto tc   = test_case::current().internal();
    auto opts = test_case::options();

    SUBCASE("Console reporting (without --success)") {
        const auto result = tc.run(opts.success(false));

        CHECK(result.exit_code != 0);
        CHECK(result.err == "");
        CHECK(result.out == text::dedent(R"(
            ===============================================================================
            test_info.cpp:450:
            TEST SUITE: INTERNAL
            TEST CASE:  Abnormal termination

            test_info.cpp:450: FATAL ERROR: test case CRASHED: SIGABRT - Abort (abnormal termination) signal

            ===============================================================================
            [doctest] test cases: 1 | 0 passed | 1 failed |
            [doctest] assertions: 1 | 1 passed | 0 failed |
            [doctest] Status: FAILURE!
        )"));
    }

    SUBCASE("Console reporting (with --success)") {
        const auto result = tc.run(opts.success(true));

        CHECK(result.exit_code != 0);
        CHECK(result.err == "");
        CHECK(result.out == text::dedent(R"(
            ===============================================================================
            test_info.cpp:450:
            TEST SUITE: INTERNAL
            TEST CASE:  Abnormal termination

            test_info.cpp:452: SUCCESS: CHECK( 1 + 1 == 2 ) is correct!
              values: CHECK( 2 == 2 )
              logged: Before

            test_info.cpp:450: FATAL ERROR: test case CRASHED: SIGABRT - Abort (abnormal termination) signal

            ===============================================================================
            [doctest] test cases: 1 | 0 passed | 1 failed |
            [doctest] assertions: 1 | 1 passed | 0 failed |
            [doctest] Status: FAILURE!
        )"));
    }

    SUBCASE("JUnit reporting") {
        const auto result = tc.run(opts.junit(true));

        CHECK(result.exit_code != 0);
        CHECK(result.err.empty());
        CHECK(result.out == text::dedent(R"(
            <?xml version="1.0" encoding="UTF-8"?>
            <testsuites>
              <testsuite name="integration_test_info_cpp" errors="1" failures="0" tests="1">
                <testcase classname="test_info.cpp" name="Abnormal termination" status="run">
                  <error message="exception">
                    SIGABRT - Abort (abnormal termination) signal
                  </error>
                </testcase>
              </testsuite>
            </testsuites>
        )"));
    }

    SUBCASE("XML reporting") {
        const auto result = tc.run(opts.xml(true));

        CHECK(result.exit_code != 0);
        CHECK(result.err.empty());
        CHECK(result.out == text::dedent(R"(
            <?xml version="1.0" encoding="UTF-8"?>
            <doctest binary="integration_test_info_cpp">
              <Options order_by="file" rand_seed="0" first="0" last="4294967295" abort_after="0" subcase_filter_levels="2147483647" case_sensitive="false" no_throw="false" no_skip="false"/>
              <TestSuite name="INTERNAL">
                <TestCase name="Abnormal termination" filename="test_info.cpp" line="450">
                  <Exception crash="true">
                    SIGABRT - Abort (abnormal termination) signal
                  </Exception>
                  <OverallResultsAsserts successes="1" failures="0" test_case_success="false"/>
                </TestCase>
              </TestSuite>
              <OverallResultsAsserts successes="1" failures="0"/>
              <OverallResultsTestCases successes="0" failures="1"/>
            </doctest>
        )"));
    }
}
