#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <vector>
#include <mutex>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

DOCTEST_CLANG_SUPPRESS_WARNING("-Wweak-vtables")

DOCTEST_GCC_SUPPRESS_WARNING("-Weffc++")
DOCTEST_GCC_SUPPRESS_WARNING("-Wpedantic")

DOCTEST_MSVC_SUPPRESS_WARNING(5026) // move constructor was implicitly defined as deleted
DOCTEST_MSVC_SUPPRESS_WARNING(4625) // copy constructor was implicitly defined as deleted
DOCTEST_MSVC_SUPPRESS_WARNING(4626) // assignment operator was implicitly defined as deleted
DOCTEST_MSVC_SUPPRESS_WARNING(5027) // move assignment operator was implicitly defined as deleted

using namespace doctest;

struct MyXmlReporter : public IReporter
{
    // caching pointers/references to objects of these types - safe to do
    std::ostream&         stdout_stream;
    const ContextOptions& opt;
    const TestCaseData*   tc;
    std::mutex            mutex;

    // constructor has to accept the ContextOptions by ref as a single argument
    MyXmlReporter(const ContextOptions& in)
            : stdout_stream(*in.cout)
            , opt(in)
            , tc(nullptr) {}

    void report_query(const QueryData& /*in*/) override {}

    void test_run_start() override {}

    void test_run_end(const TestRunStats& /*in*/) override {}

    void test_case_start(const TestCaseData& in) override { tc = &in; }

    void test_case_end(const CurrentTestCaseStats& /*in*/) override {}

    void test_case_exception(const TestCaseException& /*in*/) override {}

    void subcase_start(const SubcaseSignature& /*in*/) override {
        std::lock_guard<std::mutex> lock(mutex);
    }

    void subcase_end() override {
        std::lock_guard<std::mutex> lock(mutex);
    }

    void log_assert(const AssertData& in) override {
        // don't include successful asserts by default - this is done here
        // instead of in the framework itself because doctest doesn't know
        // if/when a reporter/listener cares about successful results
        if(!in.m_failed && !opt.success)
            return;

        // make sure there are no races - this is done here instead of in the
        // framework itself because doctest doesn't know if reporters/listeners
        // care about successful asserts and thus doesn't lock a mutex unnecessarily
        std::lock_guard<std::mutex> lock(mutex);

        // ...
    }

    void log_message(const MessageData& /*in*/) override {
        // messages too can be used in a multi-threaded context - like asserts
        std::lock_guard<std::mutex> lock(mutex);

        // ...
    }

    void test_case_skipped(const TestCaseData& /*in*/) override {}
};

// "1" is the priority - used for ordering when multiple reporters/listeners are used
REGISTER_REPORTER("my_xml", 1, MyXmlReporter);
