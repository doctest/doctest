#include <doctest/doctest.h>
#include <doctest/parts/private/reporters/junit.h> // see issue #1035
#include <doctest/parts/private/reporters/xml.h>
#include <utils/test_case.h>
#include <utils/text.h>
using namespace utils;


TEST_CASE("Unconditional message" * doctest::test_suite("INTERNAL")) {
    MESSAGE("Data");
}

TEST_CASE("Unconditional message") {
    auto tc   = test_case::current().internal();
    auto opts = test_case::options();

    SUBCASE("Console reporting") {
        const auto result = tc.run(opts);

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == text::dedent(R"(
            ===============================================================================
            test_message.cpp:9:
            TEST SUITE: INTERNAL
            TEST CASE:  Unconditional message

            test_message.cpp:10: MESSAGE: Data

            ===============================================================================
            [doctest] test cases: 1 | 1 passed | 0 failed |
            [doctest] assertions: 0 | 0 passed | 0 failed |
            [doctest] Status: SUCCESS!
        )"));
    }

    SUBCASE("JUnit reporting") {
        const auto result = tc.run(opts.junit(true));

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == text::dedent(R"(
            <?xml version="1.0" encoding="UTF-8"?>
            <testsuites>
              <testsuite name="integration_test_message_cpp" errors="0" failures="0" tests="0">
                <testcase classname="test_message.cpp" name="Unconditional message" status="run"/>
              </testsuite>
            </testsuites>
        )"));
    }

    SUBCASE("XML reporting") {
        const auto result = tc.run(opts.xml(true));

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == text::dedent(R"(
            <?xml version="1.0" encoding="UTF-8"?>
            <doctest binary="integration_test_message_cpp">
              <Options order_by="file" rand_seed="0" first="0" last="4294967295" abort_after="0" subcase_filter_levels="2147483647" case_sensitive="false" no_throw="false" no_skip="false"/>
              <TestSuite name="INTERNAL">
                <TestCase name="Unconditional message" filename="test_message.cpp" line="9">
                  <Message type="WARNING" filename="test_message.cpp" line="10">
                    <Text>
                      Data
                    </Text>
                  </Message>
                  <OverallResultsAsserts successes="0" failures="0" test_case_success="true"/>
                </TestCase>
              </TestSuite>
              <OverallResultsAsserts successes="0" failures="0"/>
              <OverallResultsTestCases successes="1" failures="0"/>
            </doctest>
        )"));
    }
}
