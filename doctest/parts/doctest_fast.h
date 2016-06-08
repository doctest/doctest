#ifndef DOCTEST_EXT_INCLUDED
#define DOCTEST_EXT_INCLUDED

namespace doctest
{
namespace detail
{
    namespace fastAssertAction
    {
        enum fastAssertAction
        {
            nothing     = 0,
            dbgbreak    = 1,
            shouldthrow = 2
        };
    } // namespace fastAssertAction

    //template <typename L>
    //inline void evaluate_result_direct(const char* file, int line, const char* exp, const L& lhs) {}

    template <typename L, typename R>
    int fast_assert(const char* assert_name, const char* file, int line, const char* expr,
                    const L& lhs, R rhs) {
        ResultBuilder _DOCTEST_RB(assert_name, doctest::detail::assertType::normal, file, line,
                                  expr);
        try {
            _DOCTEST_RB.m_res.m_passed        = lhs == rhs;
            _DOCTEST_RB.m_res.m_decomposition = stringifyBinaryExpr(lhs, "==", rhs);
        } catch(...) { _DOCTEST_RB.m_threw = true; }

        int res = 0;

        if(_DOCTEST_RB.log())
            res |= fastAssertAction::dbgbreak;

        if(_DOCTEST_RB.m_failed && checkIfShouldThrow(assert_name))
            res |= fastAssertAction::shouldthrow;

        return res;
    }
} // namespace detail
} // namespace doctest

#define DOCTEST_FAST_ASSERTION(assert_name, lhs, rhs)                                              \
    do {                                                                                           \
        int res = doctest::detail::fast_assert(assert_name, __FILE__, __LINE__, #lhs " == " #rhs,  \
                                               lhs, rhs);                                          \
        if(res & doctest::detail::fastAssertAction::dbgbreak)                                      \
            DOCTEST_BREAK_INTO_DEBUGGER();                                                         \
        if(res & doctest::detail::fastAssertAction::shouldthrow)                                   \
            doctest::detail::throwException();                                                     \
    } while(doctest::detail::always_false())

#define WARN_EQ(lhs, rhs) DOCTEST_FAST_ASSERTION("WARN_EQ", lhs, rhs)
#define CHECK_EQ(lhs, rhs) DOCTEST_FAST_ASSERTION("CHECK_EQ", lhs, rhs)
#define REQUIRE_EQ(lhs, rhs) DOCTEST_FAST_ASSERTION("REQUIRE_EQ", lhs, rhs)

#endif // DOCTEST_EXT_INCLUDED
