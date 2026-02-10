#include <doctest/doctest.h>
#include <utils/test_case.h>
#include <utils/text.h>
using namespace utils;


SCENARIO("Basic BDD usage" * doctest::test_suite("INTERNAL")) {
  GIVEN("A precondition") {
      WHEN("An action") {
          THEN("An observation") { MESSAGE("Output"); }
      }
  }
}

SCENARIO("Basic BDD usage") {
    auto tc   = test_case::current().internal();
    auto opts = test_case::options();

    const auto result = tc.run(opts);
    CHECK(result.exit_code == 0);
    CHECK(result.err == "");
    CHECK(result.out == text::dedent(R"(
        ===============================================================================
        test_bdd.cpp:7:
        TEST SUITE: INTERNAL
          Scenario: Basic BDD usage
             Given: A precondition
              When: An action
              Then: An observation

        test_bdd.cpp:10: MESSAGE: Output

        ===============================================================================
        [doctest] test cases: 1 | 1 passed | 0 failed |
        [doctest] assertions: 0 | 0 passed | 0 failed |
        [doctest] Status: SUCCESS!
    )"));
}


SCENARIO("Nested BDD usage" * doctest::test_suite("INTERNAL")) {
    GIVEN("A precondition") {
        WHEN("An action") {
            THEN("First observation") { MESSAGE("Output"); }
            THEN("Second observation") { MESSAGE("Output"); }
            THEN("Third observation") { MESSAGE("Output"); }
        }
    }
}

SCENARIO("Nested BDD usage") {
    auto tc   = test_case::current().internal();
    auto opts = test_case::options();

    const auto result = tc.run(opts);
    CHECK(result.exit_code == 0);
    CHECK(result.err == "");
    CHECK(result.out == text::dedent(R"(
        ===============================================================================
        test_bdd.cpp:41:
        TEST SUITE: INTERNAL
          Scenario: Nested BDD usage
             Given: A precondition
              When: An action
              Then: First observation

        test_bdd.cpp:44: MESSAGE: Output

        ===============================================================================
        test_bdd.cpp:41:
        TEST SUITE: INTERNAL
          Scenario: Nested BDD usage
             Given: A precondition
              When: An action
              Then: Second observation

        test_bdd.cpp:45: MESSAGE: Output

        ===============================================================================
        test_bdd.cpp:41:
        TEST SUITE: INTERNAL
          Scenario: Nested BDD usage
             Given: A precondition
              When: An action
              Then: Third observation

        test_bdd.cpp:46: MESSAGE: Output

        ===============================================================================
        [doctest] test cases: 1 | 1 passed | 0 failed |
        [doctest] assertions: 0 | 0 passed | 0 failed |
        [doctest] Status: SUCCESS!
    )"));
}
