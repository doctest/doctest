#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/timer.h"
#include "doctest/parts/private/atomic.h"

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

    // this holds both parameters from the command line and runtime data for tests
    struct ContextState : ContextOptions, TestRunStats, CurrentTestCaseStats
    {
        MultiLaneAtomic<int> numAssertsCurrentTest_atomic;
        MultiLaneAtomic<int> numAssertsFailedCurrentTest_atomic;

        std::vector<std::vector<String>> filters = decltype(filters)(9); // 9 different filters

        std::vector<IReporter*> reporters_currently_used;

        assert_handler ah = nullptr;

        Timer timer;

        std::vector<String> stringifiedContexts; // logging from INFO() due to an exception

        // stuff for subcases
        bool reachedLeaf;
        std::vector<SubcaseSignature> subcaseStack;
        std::vector<SubcaseSignature> nextSubcaseStack;
        std::unordered_set<unsigned long long> fullyTraversedSubcases;
        size_t currentSubcaseDepth;
        Atomic<bool> shouldLogCurrentException;

        void resetRunData() {
            numTestCases                = 0;
            numTestCasesPassingFilters  = 0;
            numTestSuitesPassingFilters = 0;
            numTestCasesFailed          = 0;
            numAsserts                  = 0;
            numAssertsFailed            = 0;
            numAssertsCurrentTest       = 0;
            numAssertsFailedCurrentTest = 0;
        }

        void finalizeTestCaseData() {
            seconds = timer.getElapsedSeconds();

            // update the non-atomic counters
            numAsserts += numAssertsCurrentTest_atomic;
            numAssertsFailed += numAssertsFailedCurrentTest_atomic;
            numAssertsCurrentTest       = numAssertsCurrentTest_atomic;
            numAssertsFailedCurrentTest = numAssertsFailedCurrentTest_atomic;

            if(numAssertsFailedCurrentTest)
                failure_flags |= TestCaseFailureReason::AssertFailure;

            if(Approx(currentTest->m_timeout).epsilon(DBL_EPSILON) != 0 &&
               Approx(seconds).epsilon(DBL_EPSILON) > currentTest->m_timeout)
                failure_flags |= TestCaseFailureReason::Timeout;

            if(currentTest->m_should_fail) {
                if(failure_flags) {
                    failure_flags |= TestCaseFailureReason::ShouldHaveFailedAndDid;
                } else {
                    failure_flags |= TestCaseFailureReason::ShouldHaveFailedButDidnt;
                }
            } else if(failure_flags && currentTest->m_may_fail) {
                failure_flags |= TestCaseFailureReason::CouldHaveFailedAndDid;
            } else if(currentTest->m_expected_failures > 0) {
                if(numAssertsFailedCurrentTest == currentTest->m_expected_failures) {
                    failure_flags |= TestCaseFailureReason::FailedExactlyNumTimes;
                } else {
                    failure_flags |= TestCaseFailureReason::DidntFailExactlyNumTimes;
                }
            }

            bool ok_to_fail = (TestCaseFailureReason::ShouldHaveFailedAndDid & failure_flags) ||
                              (TestCaseFailureReason::CouldHaveFailedAndDid & failure_flags) ||
                              (TestCaseFailureReason::FailedExactlyNumTimes & failure_flags);

            // if any subcase has failed - the whole test case has failed
            testCaseSuccess = !(failure_flags && !ok_to_fail);
            if(!testCaseSuccess)
                numTestCasesFailed++;
        }
    };

    ContextState* g_cs = nullptr;

    // used to avoid locks for the debug output
    // TODO: figure out if this is indeed necessary/correct - seems like either there still
    // could be a race or that there wouldn't be a race even if using the context directly
    DOCTEST_THREAD_LOCAL bool g_no_colors;

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE
