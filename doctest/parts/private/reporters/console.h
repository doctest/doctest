#ifndef DOCTEST_PARTS_PRIVATE_REPORTERS_CONSOLE
#define DOCTEST_PARTS_PRIVATE_REPORTERS_CONSOLE

#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/reporters/common.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifdef DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS
#define DOCTEST_OPTIONS_PREFIX_DISPLAY DOCTEST_CONFIG_OPTIONS_PREFIX
#else
#define DOCTEST_OPTIONS_PREFIX_DISPLAY ""
#endif

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {

struct Whitespace
{
    int nrSpaces;
    explicit Whitespace(int nr);
};

std::ostream& operator<<(std::ostream& out, const Whitespace& ws);

struct ConsoleReporter : public IReporter
{
    std::ostream&                 s;
    bool                          hasLoggedCurrentTestStart;
    std::vector<SubcaseSignature> subcasesStack;
    size_t                        currentSubcaseLevel;
    DOCTEST_DECLARE_MUTEX(mutex)

    // caching pointers/references to objects of these types - safe to do
    const ContextOptions& opt;
    const TestCaseData*   tc;

    ConsoleReporter(const ContextOptions& co);

    ConsoleReporter(const ContextOptions& co, std::ostream& ostr);

    // =========================================================================================
    // WHAT FOLLOWS ARE HELPERS USED BY THE OVERRIDES OF THE VIRTUAL METHODS OF THE INTERFACE
    // =========================================================================================

    void separator_to_stream();

    const char* getSuccessOrFailString(bool success, assertType::Enum at, const char* success_str);

    Color::Enum getSuccessOrFailColor(bool success, assertType::Enum at);

    void successOrFailColoredStringToStream(bool success, assertType::Enum at, const char* success_str = "SUCCESS");

    void log_contexts();

    // this was requested to be made virtual so users could override it
    virtual void file_line_to_stream(const char* file, int line, const char* tail = "");

    void logTestStart();

    void printVersion();

    void printIntro();

    void printHelp();

    void printRegisteredReporters();

    // =========================================================================================
    // WHAT FOLLOWS ARE OVERRIDES OF THE VIRTUAL METHODS OF THE REPORTER INTERFACE
    // =========================================================================================

    void report_query(const QueryData& in) override;

    void test_run_start() override;

    void test_run_end(const TestRunStats& p) override;

    void test_case_start(const TestCaseData& in) override;

    void test_case_reenter(const TestCaseData&) override;

    void test_case_end(const CurrentTestCaseStats& st) override;

    void test_case_exception(const TestCaseException& e) override;

    void subcase_start(const SubcaseSignature& subc) override;

    void subcase_end() override;

    void log_assert(const AssertData& rb) override;

    void log_message(const MessageData& mb) override;

    void test_case_skipped(const TestCaseData&) override;
};

DOCTEST_REGISTER_REPORTER("console", 0, ConsoleReporter);

} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP

#endif // DOCTEST_PARTS_PRIVATE_REPORTERS_CONSOLE
