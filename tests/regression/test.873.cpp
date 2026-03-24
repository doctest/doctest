#define DOCTEST_CONFIG_DISABLE
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

namespace {

DOCTEST_MSVC_SUPPRESS_WARNING_PUSH
DOCTEST_MSVC_SUPPRESS_WARNING(4625)
DOCTEST_MSVC_SUPPRESS_WARNING(5026)
DOCTEST_MSVC_SUPPRESS_WARNING(4626)
DOCTEST_MSVC_SUPPRESS_WARNING(5027)
class reporter : public doctest::IReporter {
public:
    inline void report_query(const doctest::QueryData &) override {}
    inline void test_run_start() override {}
    inline void test_run_end(const doctest::TestRunStats &) override {}
    inline void test_case_start(const doctest::TestCaseData &) override {}
    inline void test_case_reenter(const doctest::TestCaseData &) override {}
    inline void test_case_end(const doctest::CurrentTestCaseStats &) override {}
    inline void test_case_exception(const doctest::TestCaseException &) override {}
    inline void subcase_start(const doctest::SubcaseSignature &) override {}
    inline void subcase_end() override {}
    inline void log_assert(const doctest::AssertData &) override {}
    inline void log_message(const doctest::MessageData &) override {}
    inline void test_case_skipped(const doctest::TestCaseData &) override {}
};
DOCTEST_MSVC_SUPPRESS_WARNING_POP

} // namespace

int main() {
    // NOLINTNEXTLINE(misc-const-correctness)
    reporter r;
    static_cast<void>(r);

    // Required to fix issues with xcode optimizing away `main`
    const volatile int result = 0;
    return result;
}
