#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/context_state.h"
#include "doctest/parts/private/reporter.h"
#include "doctest/parts/private/assert/handler.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

    void addAssert(assertType::Enum at) {
        if((at & assertType::is_warn) == 0) //!OCLINT bitwise operator in conditional
            g_cs->numAssertsCurrentTest_atomic++;
    }

    void addFailedAssert(assertType::Enum at) {
        if((at & assertType::is_warn) == 0) //!OCLINT bitwise operator in conditional
            g_cs->numAssertsFailedCurrentTest_atomic++;
    }

#if defined(DOCTEST_CONFIG_POSIX_SIGNALS) || defined(DOCTEST_CONFIG_WINDOWS_SEH)
    void reportFatal(const std::string& message) {
        g_cs->failure_flags |= TestCaseFailureReason::Crash;

        DOCTEST_ITERATE_THROUGH_REPORTERS(test_case_exception, {message.c_str(), true});

        while (g_cs->subcaseStack.size()) {
            g_cs->subcaseStack.pop_back();
            DOCTEST_ITERATE_THROUGH_REPORTERS(subcase_end, DOCTEST_EMPTY);
        }

        g_cs->finalizeTestCaseData();

        DOCTEST_ITERATE_THROUGH_REPORTERS(test_case_end, *g_cs);

        DOCTEST_ITERATE_THROUGH_REPORTERS(test_run_end, *g_cs);
    }
#endif // DOCTEST_CONFIG_POSIX_SIGNALS || DOCTEST_CONFIG_WINDOWS_SEH


    void failed_out_of_a_testing_context(const AssertData& ad) {
        if(g_cs->ah)
            g_cs->ah(ad);
        else
            std::abort();
    }

    bool decomp_assert(assertType::Enum at, const char* file, int line, const char* expr,
                       const Result& result) {
        bool failed = !result.m_passed;

        // ###################################################################################
        // IF THE DEBUGGER BREAKS HERE - GO 1 LEVEL UP IN THE CALLSTACK FOR THE FAILING ASSERT
        // THIS IS THE EFFECT OF HAVING 'DOCTEST_CONFIG_SUPER_FAST_ASSERTS' DEFINED
        // ###################################################################################
        DOCTEST_ASSERT_OUT_OF_TESTS(result.m_decomp);
        DOCTEST_ASSERT_IN_TESTS(result.m_decomp);
        return !failed;
    }

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP
