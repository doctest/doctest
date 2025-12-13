#include "doctest/parts/private/prelude.h"

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

    Result::Result(bool passed, const String& decomposition)
            : m_passed(passed)
            , m_decomp(decomposition) {}

    ResultBuilder::ResultBuilder(assertType::Enum at, const char* file, int line, const char* expr,
                                 const char* exception_type, const String& exception_string)
        : AssertData(at, file, line, expr, exception_type, exception_string) { }

    ResultBuilder::ResultBuilder(assertType::Enum at, const char* file, int line, const char* expr,
        const char* exception_type, const Contains& exception_string)
        : AssertData(at, file, line, expr, exception_type, exception_string) { }

    void ResultBuilder::setResult(const Result& res) {
        m_decomp = res.m_decomp;
        m_failed = !res.m_passed;
    }

    /* TODO: ResultBuilder::translateException */
    /* TODO: ResultBuilder::log */

    void ResultBuilder::react() const {
        if(m_failed && checkIfShouldThrow(m_at))
            throwException();
    }

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE
