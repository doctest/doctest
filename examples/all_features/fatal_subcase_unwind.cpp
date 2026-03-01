#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <csignal>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

struct FatalTraceReporter : public doctest::IReporter {
    std::ostream& out;

    explicit FatalTraceReporter(const doctest::ContextOptions& in)
            : out(*in.cout) {}

    void report_query(const doctest::QueryData&) override {}
    void test_run_start() override {}
    void test_run_end(const doctest::TestRunStats&) override {}
    void test_case_start(const doctest::TestCaseData&) override {}
    void test_case_reenter(const doctest::TestCaseData&) override {}
    void test_case_skipped(const doctest::TestCaseData&) override {}
    void log_assert(const doctest::AssertData&) override {}
    void log_message(const doctest::MessageData&) override {}

    void test_case_exception(const doctest::TestCaseException& in) override {
        out << "exception:" << in.error_string.c_str() << ":crash=" << (in.is_crash ? 1 : 0) << '\n';
        out.flush();
    }

    void subcase_start(const doctest::SubcaseSignature& in) override {
        out << "subcase_start:" << in.m_name.c_str() << '\n';
        out.flush();
    }

    void subcase_end() override {
        out << "subcase_end\n";
        out.flush();
    }

    void test_case_end(const doctest::CurrentTestCaseStats&) override {
        out << "test_case_end\n";
        out.flush();
    }
};

REGISTER_REPORTER("fatal_trace", 0, FatalTraceReporter);

TEST_CASE("fatal unwind closes active subcases") {
    SUBCASE("outer") {
        SUBCASE("inner") {
            std::raise(SIGABRT);
        }
    }
}
