#ifndef DOCTEST_PARTS_PUBLIC_ASSERT_EXPRESSION
#define DOCTEST_PARTS_PUBLIC_ASSERT_EXPRESSION

#include "doctest/parts/public/assert/result.h"
#include "doctest/parts/public/assert/type.h"
#include "doctest/parts/public/context/options.h"
#include "doctest/parts/public/string.h"

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

#if DOCTEST_CLANG && DOCTEST_CLANG < DOCTEST_COMPILER(3, 6, 0)
DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wunused-comparison")
#endif

// This will check if there is any way it could find a operator like member or friend and uses it.
// If not it doesn't find the operator or if the operator at global scope is defined after
// this template, the template won't be instantiated due to SFINAE. Once the template is not
// instantiated it can look for global operator using normal conversions.
#ifdef __NVCC__
#define SFINAE_OP(ret,op) ret
#else
#define SFINAE_OP(ret,op) decltype((void)(doctest::detail::declval<L>() op doctest::detail::declval<R>()),ret{})
#endif

#define DOCTEST_DO_BINARY_EXPRESSION_COMPARISON(op, op_str, op_macro)                              \
    template <typename R>                                                                          \
    DOCTEST_NOINLINE SFINAE_OP(Result,op) operator op(R&& rhs) {                                   \
    bool res = op_macro(doctest::detail::forward<const L>(lhs), doctest::detail::forward<R>(rhs)); \
        if(m_at & assertType::is_false)                                                            \
            res = !res;                                                                            \
        if(!res || doctest::getContextOptions()->success)                                          \
            return Result(res, stringifyBinaryExpr(lhs, op_str, rhs));                             \
        return Result(res);                                                                        \
    }

#ifndef DOCTEST_CONFIG_NO_COMPARISON_WARNING_SUPPRESSION

    DOCTEST_CLANG_SUPPRESS_WARNING_PUSH
    DOCTEST_CLANG_SUPPRESS_WARNING("-Wsign-conversion")
    DOCTEST_CLANG_SUPPRESS_WARNING("-Wsign-compare")
    //DOCTEST_CLANG_SUPPRESS_WARNING("-Wdouble-promotion")
    //DOCTEST_CLANG_SUPPRESS_WARNING("-Wconversion")
    //DOCTEST_CLANG_SUPPRESS_WARNING("-Wfloat-equal")

    DOCTEST_GCC_SUPPRESS_WARNING_PUSH
    DOCTEST_GCC_SUPPRESS_WARNING("-Wsign-conversion")
    DOCTEST_GCC_SUPPRESS_WARNING("-Wsign-compare")
    //DOCTEST_GCC_SUPPRESS_WARNING("-Wdouble-promotion")
    //DOCTEST_GCC_SUPPRESS_WARNING("-Wconversion")
    //DOCTEST_GCC_SUPPRESS_WARNING("-Wfloat-equal")

    DOCTEST_MSVC_SUPPRESS_WARNING_PUSH
    // https://stackoverflow.com/questions/39479163 what's the difference between 4018 and 4389
    DOCTEST_MSVC_SUPPRESS_WARNING(4388) // signed/unsigned mismatch
    DOCTEST_MSVC_SUPPRESS_WARNING(4389) // 'operator' : signed/unsigned mismatch
    DOCTEST_MSVC_SUPPRESS_WARNING(4018) // 'expression' : signed/unsigned mismatch
    //DOCTEST_MSVC_SUPPRESS_WARNING(4805) // 'operation' : unsafe mix of type 'type' and type 'type' in operation

#endif // DOCTEST_CONFIG_NO_COMPARISON_WARNING_SUPPRESSION

template <typename L>
// cppcheck-suppress copyCtorAndEqOperator
struct Expression_lhs
{
    L                lhs;
    assertType::Enum m_at;

    explicit Expression_lhs(L&& in, assertType::Enum at)
            : lhs(static_cast<L&&>(in))
            , m_at(at) {}

    DOCTEST_NOINLINE operator Result() {
// this is needed only for MSVC 2015
DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4800) // 'int': forcing value to bool
        bool res = static_cast<bool>(lhs);
DOCTEST_MSVC_SUPPRESS_WARNING_POP
        if(m_at & assertType::is_false) { //!OCLINT bitwise operator in conditional
            res = !res;
        }

        if(!res || getContextOptions()->success) {
            return { res, (DOCTEST_STRINGIFY(lhs)) };
        }
        return { res };
    }

    /* This is required for user-defined conversions from Expression_lhs to L */
    operator L() const { return lhs; }

    // clang-format off
    DOCTEST_DO_BINARY_EXPRESSION_COMPARISON(==, " == ", DOCTEST_CMP_EQ) //!OCLINT bitwise operator in conditional
    DOCTEST_DO_BINARY_EXPRESSION_COMPARISON(!=, " != ", DOCTEST_CMP_NE) //!OCLINT bitwise operator in conditional
    DOCTEST_DO_BINARY_EXPRESSION_COMPARISON(>,  " >  ", DOCTEST_CMP_GT) //!OCLINT bitwise operator in conditional
    DOCTEST_DO_BINARY_EXPRESSION_COMPARISON(<,  " <  ", DOCTEST_CMP_LT) //!OCLINT bitwise operator in conditional
    DOCTEST_DO_BINARY_EXPRESSION_COMPARISON(>=, " >= ", DOCTEST_CMP_GE) //!OCLINT bitwise operator in conditional
    DOCTEST_DO_BINARY_EXPRESSION_COMPARISON(<=, " <= ", DOCTEST_CMP_LE) //!OCLINT bitwise operator in conditional
    // clang-format on

    // forbidding some expressions based on this table: https://en.cppreference.com/w/cpp/language/operator_precedence
    DOCTEST_FORBIT_EXPRESSION(Expression_lhs, &)
    DOCTEST_FORBIT_EXPRESSION(Expression_lhs, ^)
    DOCTEST_FORBIT_EXPRESSION(Expression_lhs, |)
    DOCTEST_FORBIT_EXPRESSION(Expression_lhs, &&)
    DOCTEST_FORBIT_EXPRESSION(Expression_lhs, ||)
    DOCTEST_FORBIT_EXPRESSION(Expression_lhs, =)
    DOCTEST_FORBIT_EXPRESSION(Expression_lhs, +=)
    DOCTEST_FORBIT_EXPRESSION(Expression_lhs, -=)
    DOCTEST_FORBIT_EXPRESSION(Expression_lhs, *=)
    DOCTEST_FORBIT_EXPRESSION(Expression_lhs, /=)
    DOCTEST_FORBIT_EXPRESSION(Expression_lhs, %=)
    DOCTEST_FORBIT_EXPRESSION(Expression_lhs, <<=)
    DOCTEST_FORBIT_EXPRESSION(Expression_lhs, >>=)
    DOCTEST_FORBIT_EXPRESSION(Expression_lhs, &=)
    DOCTEST_FORBIT_EXPRESSION(Expression_lhs, ^=)
    DOCTEST_FORBIT_EXPRESSION(Expression_lhs, |=)
    // these 2 are unfortunate because they should be allowed - they have higher precedence over the comparisons, but the
    // ExpressionDecomposer class uses the left shift operator to capture the left operand of the binary expression...
    DOCTEST_FORBIT_EXPRESSION(Expression_lhs, <<)
    DOCTEST_FORBIT_EXPRESSION(Expression_lhs, >>)
};

#ifndef DOCTEST_CONFIG_NO_COMPARISON_WARNING_SUPPRESSION

    DOCTEST_CLANG_SUPPRESS_WARNING_POP
    DOCTEST_MSVC_SUPPRESS_WARNING_POP
    DOCTEST_GCC_SUPPRESS_WARNING_POP

#endif // DOCTEST_CONFIG_NO_COMPARISON_WARNING_SUPPRESSION

#if DOCTEST_CLANG && DOCTEST_CLANG < DOCTEST_COMPILER(3, 6, 0)
DOCTEST_CLANG_SUPPRESS_WARNING_POP
#endif

struct DOCTEST_INTERFACE ExpressionDecomposer
{
    assertType::Enum m_at;

    ExpressionDecomposer(assertType::Enum at);

    // The right operator for capturing expressions is "<=" instead of "<<" (based on the operator precedence table)
    // but then there will be warnings from GCC about "-Wparentheses" and since "_Pragma()" is problematic this will stay for now...
    // https://github.com/catchorg/Catch2/issues/870
    // https://github.com/catchorg/Catch2/issues/565
    template <typename L>
    Expression_lhs<const L&&> operator<<(const L&& operand) { //bitfields bind to universal ref but not const rvalue ref
        return Expression_lhs<const L&&>(static_cast<const L&&>(operand), m_at);
    }

    template <typename L,typename types::enable_if<!doctest::detail::types::is_rvalue_reference<L>::value,void >::type* = nullptr>
    Expression_lhs<const L&> operator<<(const L &operand) {
        return Expression_lhs<const L&>(operand, m_at);
    }
};

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_POP

#endif // DOCTEST_PARTS_PUBLIC_ASSERT_EXPRESSION
