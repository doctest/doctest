#ifndef DOCTEST_PARTS_PUBLIC_ASSERT_RESULT
#define DOCTEST_PARTS_PUBLIC_ASSERT_RESULT

#include "doctest/parts/public/assert/type.h"
#include "doctest/parts/public/assert/comparator.h"
#include "doctest/parts/public/assert/data.h"
#include "doctest/parts/public/matchers/contains.h"
#include "doctest/parts/public/string.h"

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

// more checks could be added - like in Catch:
// https://github.com/catchorg/Catch2/pull/1480/files
// https://github.com/catchorg/Catch2/pull/1481/files
#define DOCTEST_FORBIT_EXPRESSION(rt, op)                                                          \
    template <typename R>                                                                          \
    rt& operator op(const R&) {                                                                    \
        static_assert(deferred_false<R>::value,                                                    \
                      "Expression Too Complex Please Rewrite As Binary Comparison!");              \
        return *this;                                                                              \
    }

    struct DOCTEST_INTERFACE Result // NOLINT(*-member-init)
    {
        bool   m_passed;
        String m_decomp;

        Result() = default; // TODO: Why do we need this? (To remove NOLINT)
        Result(bool passed, const String& decomposition = String());

        // forbidding some expressions based on this table: https://en.cppreference.com/w/cpp/language/operator_precedence
        DOCTEST_FORBIT_EXPRESSION(Result, &)
        DOCTEST_FORBIT_EXPRESSION(Result, ^)
        DOCTEST_FORBIT_EXPRESSION(Result, |)
        DOCTEST_FORBIT_EXPRESSION(Result, &&)
        DOCTEST_FORBIT_EXPRESSION(Result, ||)
        DOCTEST_FORBIT_EXPRESSION(Result, ==)
        DOCTEST_FORBIT_EXPRESSION(Result, !=)
        DOCTEST_FORBIT_EXPRESSION(Result, <)
        DOCTEST_FORBIT_EXPRESSION(Result, >)
        DOCTEST_FORBIT_EXPRESSION(Result, <=)
        DOCTEST_FORBIT_EXPRESSION(Result, >=)
        DOCTEST_FORBIT_EXPRESSION(Result, =)
        DOCTEST_FORBIT_EXPRESSION(Result, +=)
        DOCTEST_FORBIT_EXPRESSION(Result, -=)
        DOCTEST_FORBIT_EXPRESSION(Result, *=)
        DOCTEST_FORBIT_EXPRESSION(Result, /=)
        DOCTEST_FORBIT_EXPRESSION(Result, %=)
        DOCTEST_FORBIT_EXPRESSION(Result, <<=)
        DOCTEST_FORBIT_EXPRESSION(Result, >>=)
        DOCTEST_FORBIT_EXPRESSION(Result, &=)
        DOCTEST_FORBIT_EXPRESSION(Result, ^=)
        DOCTEST_FORBIT_EXPRESSION(Result, |=)
    };

    struct DOCTEST_INTERFACE ResultBuilder : public AssertData
    {
        ResultBuilder(assertType::Enum at, const char* file, int line, const char* expr,
                      const char* exception_type = "", const String& exception_string = "");

        ResultBuilder(assertType::Enum at, const char* file, int line, const char* expr,
                      const char* exception_type, const Contains& exception_string);

        void setResult(const Result& res);

        template <int comparison, typename L, typename R>
        DOCTEST_NOINLINE bool binary_assert(const DOCTEST_REF_WRAP(L) lhs,
                                            const DOCTEST_REF_WRAP(R) rhs) {
            m_failed = !RelationalComparator<comparison, L, R>()(lhs, rhs);
            if (m_failed || getContextOptions()->success) {
                m_decomp = stringifyBinaryExpr(lhs, ", ", rhs);
            }
            return !m_failed;
        }

        template <typename L>
        DOCTEST_NOINLINE bool unary_assert(const DOCTEST_REF_WRAP(L) val) {
            m_failed = !val;

            if (m_at & assertType::is_false) { //!OCLINT bitwise operator in conditional
                m_failed = !m_failed;
            }

            if (m_failed || getContextOptions()->success) {
                m_decomp = (DOCTEST_STRINGIFY(val));
            }

            return !m_failed;
        }

        void translateException();

        bool log();
        void react() const;
    };

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_POP

#endif // DOCTEST_PARTS_PUBLIC_ASSERT_RESULT
