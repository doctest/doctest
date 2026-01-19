#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/context_state.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

ContextState* g_cs = nullptr;
DOCTEST_THREAD_LOCAL bool g_no_colors;

void ContextState::resetRunData() {
    numTestCases                = 0;
    numTestCasesPassingFilters  = 0;
    numTestSuitesPassingFilters = 0;
    numTestCasesFailed          = 0;
    numAsserts                  = 0;
    numAssertsFailed            = 0;
    numAssertsCurrentTest       = 0;
    numAssertsFailedCurrentTest = 0;
}

void ContextState::finalizeTestCaseData() {
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


} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP
