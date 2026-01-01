#ifndef DOCTEST_PARTS_PRIVATE_REPORTERS_DEBUG_OUTPUT_WINDOW
#define DOCTEST_PARTS_PRIVATE_REPORTERS_DEBUG_OUTPUT_WINDOW

#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/reporters/console.h"
#include "doctest/parts/private/context_state.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace {

#ifdef DOCTEST_PLATFORM_WINDOWS
#define DOCTEST_OUTPUT_DEBUG_STRING(text) ::OutputDebugStringA(text)
#else
    // TODO: integration with XCode and other IDEs
#define DOCTEST_OUTPUT_DEBUG_STRING(text)
#endif // Platform

#ifdef DOCTEST_PLATFORM_WINDOWS
    struct DebugOutputWindowReporter : public ConsoleReporter
    {
        DOCTEST_THREAD_LOCAL static std::ostringstream oss;

        DebugOutputWindowReporter(const ContextOptions& co)
                : ConsoleReporter(co, oss) {}

#define DOCTEST_DEBUG_OUTPUT_REPORTER_OVERRIDE(func, type, arg)                                    \
    void func(type arg) override {                                                                 \
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
    };

    DOCTEST_THREAD_LOCAL std::ostringstream DebugOutputWindowReporter::oss;
#endif // DOCTEST_PLATFORM_WINDOWS

} // namespace
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP

#endif // DOCTEST_PARTS_PRIVATE_REPORTERS_DEBUG_OUTPUT_WINDOW
