#ifndef DOCTEST_PARTS_PRIVATE_REPORTERS_XML
#define DOCTEST_PARTS_PRIVATE_REPORTERS_XML

#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/xml.h"
#include "doctest/parts/private/reporter.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {

struct XmlReporter : public IReporter
{
    detail::XmlWriter xml;
    DOCTEST_DECLARE_MUTEX(mutex)

    // caching pointers/references to objects of these types - safe to do
    const ContextOptions& opt;
    const TestCaseData*   tc = nullptr;

    XmlReporter(const ContextOptions& co);

    void log_contexts();

    unsigned line(unsigned l) const;

    void test_case_start_impl(const TestCaseData& in);

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

    void subcase_start(const SubcaseSignature& in) override;

    void subcase_end() override;

    void log_assert(const AssertData& rb) override;

    void log_message(const MessageData& mb) override;

    void test_case_skipped(const TestCaseData& in) override;
};

DOCTEST_REGISTER_REPORTER("xml", 0, XmlReporter);

} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP

#endif // DOCTEST_PARTS_PRIVATE_REPORTERS_XML
