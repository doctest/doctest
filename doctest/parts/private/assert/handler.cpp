#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/context_state.h"

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

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
