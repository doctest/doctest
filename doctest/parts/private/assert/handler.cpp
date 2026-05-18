#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/context_state.h"
#include "doctest/parts/private/reporter.h"
#include "doctest/parts/private/assert/handler.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

void addAssert(assertType::Enum at) {
    if ((at & assertType::is_warn) == 0)
        g_cs->numAssertsCurrentTest_atomic++;
}

void addFailedAssert(assertType::Enum at) {
    if ((at & assertType::is_warn) == 0)
        g_cs->numAssertsFailedCurrentTest_atomic++;
}

void failed_out_of_a_testing_context(const AssertData &ad) {
    if (g_cs->ah)
        g_cs->ah(ad);
    else
        std::abort();
}

bool decomp_assert(assertType::Enum at, const char *file, int line, const char *expr, const Result &result) {
    const bool failed = !result.m_passed;

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
