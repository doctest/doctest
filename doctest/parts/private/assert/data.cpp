#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/context_state.h"

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {

    using detail::g_cs;

    AssertData::AssertData(assertType::Enum at, const char* file, int line, const char* expr,
        const char* exception_type, const StringContains& exception_string)
        : m_test_case(g_cs->currentTest), m_at(at), m_file(file), m_line(line), m_expr(expr),
        m_failed(true), m_threw(false), m_threw_as(false), m_exception_type(exception_type),
        m_exception_string(exception_string) {
    #if DOCTEST_MSVC
        if (m_expr[0] == ' ') // this happens when variadic macros are disabled under MSVC
            ++m_expr;
    #endif // MSVC
    }

} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE
