#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

    DOCTEST_INTERFACE void failed_out_of_a_testing_context(const AssertData& ad);

    DOCTEST_INTERFACE bool decomp_assert(assertType::Enum at, const char* file, int line,
                                         const char* expr, const Result& result);

#define DOCTEST_ASSERT_OUT_OF_TESTS(decomp)                                                        \
    do {                                                                                           \
        if(!is_running_in_test) {                                                                  \
            if(failed) {                                                                           \
                ResultBuilder rb(at, file, line, expr);                                            \
                rb.m_failed = failed;                                                              \
                rb.m_decomp = decomp;                                                              \
                failed_out_of_a_testing_context(rb);                                               \
                if(isDebuggerActive() && !getContextOptions()->no_breaks)                          \
                    DOCTEST_BREAK_INTO_DEBUGGER();                                                 \
                if(checkIfShouldThrow(at))                                                         \
                    throwException();                                                              \
            }                                                                                      \
            return !failed;                                                                        \
        }                                                                                          \
    } while(false)

#define DOCTEST_ASSERT_IN_TESTS(decomp)                                                            \
    ResultBuilder rb(at, file, line, expr);                                                        \
    rb.m_failed = failed;                                                                          \
    if(rb.m_failed || getContextOptions()->success)                                                \
        rb.m_decomp = decomp;                                                                      \
    if(rb.log())                                                                                   \
        DOCTEST_BREAK_INTO_DEBUGGER();                                                             \
    if(rb.m_failed && checkIfShouldThrow(at))                                                      \
    throwException()

    template <int comparison, typename L, typename R>
    DOCTEST_NOINLINE bool binary_assert(assertType::Enum at, const char* file, int line,
                                        const char* expr, const DOCTEST_REF_WRAP(L) lhs,
                                        const DOCTEST_REF_WRAP(R) rhs) {
        bool failed = !RelationalComparator<comparison, L, R>()(lhs, rhs);

        // ###################################################################################
        // IF THE DEBUGGER BREAKS HERE - GO 1 LEVEL UP IN THE CALLSTACK FOR THE FAILING ASSERT
        // THIS IS THE EFFECT OF HAVING 'DOCTEST_CONFIG_SUPER_FAST_ASSERTS' DEFINED
        // ###################################################################################
        DOCTEST_ASSERT_OUT_OF_TESTS(stringifyBinaryExpr(lhs, ", ", rhs));
        DOCTEST_ASSERT_IN_TESTS(stringifyBinaryExpr(lhs, ", ", rhs));
        return !failed;
    }

    template <typename L>
    DOCTEST_NOINLINE bool unary_assert(assertType::Enum at, const char* file, int line,
                                       const char* expr, const DOCTEST_REF_WRAP(L) val) {
        bool failed = !val;

        if(at & assertType::is_false) //!OCLINT bitwise operator in conditional
            failed = !failed;

        // ###################################################################################
        // IF THE DEBUGGER BREAKS HERE - GO 1 LEVEL UP IN THE CALLSTACK FOR THE FAILING ASSERT
        // THIS IS THE EFFECT OF HAVING 'DOCTEST_CONFIG_SUPER_FAST_ASSERTS' DEFINED
        // ###################################################################################
        DOCTEST_ASSERT_OUT_OF_TESTS((DOCTEST_STRINGIFY(val)));
        DOCTEST_ASSERT_IN_TESTS((DOCTEST_STRINGIFY(val)));
        return !failed;
    }

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE
