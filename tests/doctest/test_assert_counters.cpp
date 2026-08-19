#include <doctest/doctest.h>
#include <doctest/parts/private/context_state.h>
#include <doctest/parts/private/reporters/console.h>

#include <sstream>
#include <string>

DOCTEST_SUPPRESS_COMMON_WARNINGS_PUSH

namespace {

// The same type the assertion counters use, so this test follows them if the type changes.
using counter_t = decltype(doctest::TestRunStats::numAsserts);

// 2^30. Four of these add up to 2^32. Kept as long long so that a cast to the counter type
// is a real conversion whether that type is 32 or 64 bit wide, which keeps gcc
// -Wuseless-cast quiet in both cases.
const long long quarter_of_2_32 = 1073741824LL;

// Widening goes through a template so gcc does not see a cast to the type the operand
// already has, which -Wuseless-cast rejects.
template <typename T>
unsigned long long widen(T value) {
    return static_cast<unsigned long long>(value);
}

// A context state of its own. Seeding the counters of the live run would disturb the run that
// executes these test cases, and restoring them afterwards would also discard the record of a
// failed check, which is the only thing that makes the test binary exit non-zero.
class Counters {
public:
    doctest::detail::ContextState state{};

    Counters() {
        // finalizeTestCaseData() reads the timeout of the current test case
        state.currentTest = doctest::getContextOptions()->currentTest;
        state.timer.start();
    }

    // Every call adds to the same lane, so a 32 bit counter wraps within four calls.
    void addAsserts(long long amount) {
        state.numAssertsCurrentTest_atomic.fetch_add(static_cast<counter_t>(amount));
    }

    void addFailedAsserts(long long amount) {
        state.numAssertsFailedCurrentTest_atomic.fetch_add(static_cast<counter_t>(amount));
    }
};

TEST_CASE("the per test case assertion count is exact at 2^32") {
    Counters counters{};

    counters.addAsserts(quarter_of_2_32);
    counters.addAsserts(quarter_of_2_32);
    counters.addAsserts(quarter_of_2_32);
    counters.addAsserts(quarter_of_2_32);
    counters.state.finalizeTestCaseData();

    CHECK(widen(counters.state.numAssertsCurrentTest) == 4ull * widen(quarter_of_2_32));
}

TEST_CASE("the per test case failed assertion count is exact at 2^32") {
    Counters counters{};

    for (int i = 0; i < 4; ++i) {
        counters.addAsserts(quarter_of_2_32);
        counters.addFailedAsserts(quarter_of_2_32);
    }
    counters.state.finalizeTestCaseData();

    CHECK(widen(counters.state.numAssertsFailedCurrentTest) == 4ull * widen(quarter_of_2_32));
    CHECK(widen(counters.state.numAssertsCurrentTest) == 4ull * widen(quarter_of_2_32));
}

TEST_CASE("the run assertion count is exact past 2^32") {
    Counters counters{};

    counters.state.numAsserts = static_cast<counter_t>(4000000000LL);
    counters.addAsserts(quarter_of_2_32);
    counters.state.finalizeTestCaseData();

    // Past 2^32, so a 32 bit counter of either signedness reports the wrong number.
    CHECK(widen(counters.state.numAsserts) == 4000000000ull + widen(quarter_of_2_32));
}

TEST_CASE("the console reporter prints an assertion count above 2^32") {
    doctest::ContextOptions options = *doctest::getContextOptions();
    options.minimal = false;
    options.no_skipped_summary = true;

    std::ostringstream oss{};
    doctest::ConsoleReporter reporter{options, oss};

    doctest::TestRunStats stats{};
    stats.numTestCases = 1u;
    stats.numTestCasesPassingFilters = 1u;
    stats.numTestSuitesPassingFilters = 1u;
    stats.numTestCasesFailed = 0u;
    stats.numAsserts = static_cast<counter_t>(5000000000LL);
    stats.numAssertsFailed = 0u;

    reporter.test_run_end(stats);

    // The summary prints the number twice, as the total and as the passed count.
    CHECK(oss.str().find("5000000000") != std::string::npos);
}

} // namespace

DOCTEST_SUPPRESS_COMMON_WARNINGS_POP
