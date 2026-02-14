#include <doctest/doctest.h>
#include <doctest/parts/private/reporters/junit.h> // see issue #1035
#include <doctest/parts/private/reporters/xml.h>
#include <utils/test_case.h>
#include <utils/text.h>
using namespace utils;

// CAPTURE(x) is just an alias to INFO(#x " := ", x)
// So, we don't really need to test it particularly thoroughly
// We can observe this aliasing best through the XML reporter,
// as it has <Info> blocks to record these messages

namespace {
  constexpr auto x = 1, y = 2, z = 3;
}

TEST_CASE("Failing CHECK-like assertion" * doctest::test_suite("INTERNAL")) {
    CAPTURE(x);
    CHECK(1 + 1 == 2);
    CAPTURE(y);
    CHECK(2 + 2 == 5);
    CAPTURE(z);
}

TEST_CASE("Failing CHECK-like assertion") {
    auto tc   = test_case::current().internal();
    auto opts = test_case::options();

    const auto result = tc.run(opts.xml(true));

    CHECK(result.exit_code != 0);
    CHECK(result.err == "");
    CHECK(result.out == text::dedent(R"(
        <?xml version="1.0" encoding="UTF-8"?>
        <doctest binary="integration_test_capture_cpp">
          <Options order_by="file" rand_seed="0" first="0" last="4294967295" abort_after="0" subcase_filter_levels="2147483647" case_sensitive="false" no_throw="false" no_skip="false"/>
          <TestSuite name="INTERNAL">
            <TestCase name="Failing CHECK-like assertion" filename="test_capture.cpp" line="17">
              <Expression success="false" type="CHECK" filename="test_capture.cpp" line="21">
                <Original>
                  2 + 2 == 5
                </Original>
                <Expanded>
                  4 == 5
                </Expanded>
                <Info>
                  x := 1
                </Info>
                <Info>
                  y := 2
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
