#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/reporters/debug_output_window.h"
#include "doctest/parts/private/context_state.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

#ifdef DOCTEST_PLATFORM_WINDOWS
#define DOCTEST_OUTPUT_DEBUG_STRING(text) ::OutputDebugStringA(text)
#endif // Platform

#ifdef DOCTEST_PLATFORM_WINDOWS

DOCTEST_THREAD_LOCAL std::ostringstream DebugOutputWindowReporter::oss;

DebugOutputWindowReporter::DebugOutputWindowReporter(const ContextOptions& co)
        : ConsoleReporter(co, oss) {}

#define DOCTEST_DEBUG_OUTPUT_REPORTER_OVERRIDE(func, type, arg)                                    \
    void DebugOutputWindowReporter::func(type arg) {                                                                 \
        using detail::g_no_colors;                                                                 \
        bool with_col = g_no_colors;                                                               \
        g_no_colors   = false;                                                                     \
        ConsoleReporter::func(arg);                                                                \
        if(oss.tellp() != std::streampos{}) {                                                      \
            DOCTEST_OUTPUT_DEBUG_STRING(oss.str().c_str());                                        \
            oss.str("");                                                                           \
        }                                                                                          \
        g_no_colors = with_col;                                                                    \
    }

DOCTEST_DEBUG_OUTPUT_REPORTER_OVERRIDE(test_run_start, DOCTEST_EMPTY, DOCTEST_EMPTY)
DOCTEST_DEBUG_OUTPUT_REPORTER_OVERRIDE(test_run_end, const TestRunStats&, in)
DOCTEST_DEBUG_OUTPUT_REPORTER_OVERRIDE(test_case_start, const TestCaseData&, in)
DOCTEST_DEBUG_OUTPUT_REPORTER_OVERRIDE(test_case_reenter, const TestCaseData&, in)
DOCTEST_DEBUG_OUTPUT_REPORTER_OVERRIDE(test_case_end, const CurrentTestCaseStats&, in)
DOCTEST_DEBUG_OUTPUT_REPORTER_OVERRIDE(test_case_exception, const TestCaseException&, in)
DOCTEST_DEBUG_OUTPUT_REPORTER_OVERRIDE(subcase_start, const SubcaseSignature&, in)
DOCTEST_DEBUG_OUTPUT_REPORTER_OVERRIDE(subcase_end, DOCTEST_EMPTY, DOCTEST_EMPTY)
DOCTEST_DEBUG_OUTPUT_REPORTER_OVERRIDE(log_assert, const AssertData&, in)
DOCTEST_DEBUG_OUTPUT_REPORTER_OVERRIDE(log_message, const MessageData&, in)
DOCTEST_DEBUG_OUTPUT_REPORTER_OVERRIDE(test_case_skipped, const TestCaseData&, in)

#endif // DOCTEST_PLATFORM_WINDOWS

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP
