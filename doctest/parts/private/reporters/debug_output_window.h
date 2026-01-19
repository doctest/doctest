#ifndef DOCTEST_PARTS_PRIVATE_REPORTERS_DEBUG_OUTPUT_WINDOW
#define DOCTEST_PARTS_PRIVATE_REPORTERS_DEBUG_OUTPUT_WINDOW

#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/reporters/console.h"
#include "doctest/parts/private/context_state.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

#ifdef DOCTEST_PLATFORM_WINDOWS
    struct DebugOutputWindowReporter : public ConsoleReporter
    {
        DOCTEST_THREAD_LOCAL static std::ostringstream oss;

        DebugOutputWindowReporter(const ContextOptions& co);

        void test_run_start() override;
        void test_run_end(const TestRunStats& in) override;
        void test_case_start(const TestCaseData& in) override;
        void test_case_reenter(const TestCaseData& in) override;
        void test_case_end(const CurrentTestCaseStats& in) override;
        void test_case_exception(const TestCaseException& in) override;
        void subcase_start(const SubcaseSignature& in) override;
        void subcase_end(DOCTEST_EMPTY DOCTEST_EMPTY) override;
        void log_assert(const AssertData& in) override;
        void log_message(const MessageData& in) override;
        void test_case_skipped(const TestCaseData& in) override;
    };
#endif // DOCTEST_PLATFORM_WINDOWS

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP

#endif // DOCTEST_PARTS_PRIVATE_REPORTERS_DEBUG_OUTPUT_WINDOW
