#ifndef DOCTEST_PARTS_PRIVATE_REPORTERS_JUNIT
#define DOCTEST_PARTS_PRIVATE_REPORTERS_JUNIT

#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/xml.h"
#include "doctest/parts/private/timer.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {

    // TODO:
    // - log_message()
    // - respond to queries
    // - honor remaining options
    // - more attributes in tags
    struct JUnitReporter : public IReporter
    {
        detail::XmlWriter xml;
        DOCTEST_DECLARE_MUTEX(mutex)
        detail::Timer timer;
        std::vector<String> deepestSubcaseStackNames;

        struct JUnitTestCaseData
        {
            static std::string getCurrentTimestamp();

            struct JUnitTestMessage
            {
                JUnitTestMessage(const std::string& _message, const std::string& _type, const std::string& _details);

                JUnitTestMessage(const std::string& _message, const std::string& _details);

                std::string message, type, details;
            };

            struct JUnitTestCase
            {
                JUnitTestCase(const std::string& _classname, const std::string& _name);

                std::string classname, name;
                double time;
                std::vector<JUnitTestMessage> failures, errors;
            };

            void add(const std::string& classname, const std::string& name);

            void appendSubcaseNamesToLastTestcase(std::vector<String> nameStack);

            void addTime(double time);

            void addFailure(const std::string& message, const std::string& type, const std::string& details);

            void addError(const std::string& message, const std::string& details);

            std::vector<JUnitTestCase> testcases;
            double totalSeconds = 0;
            int totalErrors = 0, totalFailures = 0;
        };

        JUnitTestCaseData testCaseData;

        // caching pointers/references to objects of these types - safe to do
        const ContextOptions& opt;
        const TestCaseData*   tc = nullptr;

        JUnitReporter(const ContextOptions& co);

        unsigned line(unsigned l) const;

        // =========================================================================================
        // WHAT FOLLOWS ARE OVERRIDES OF THE VIRTUAL METHODS OF THE REPORTER INTERFACE
        // =========================================================================================

        void report_query(const QueryData&) override;

        void test_run_start() override;

        void test_run_end(const TestRunStats& p) override;

        void test_case_start(const TestCaseData& in) override;

        void test_case_reenter(const TestCaseData& in) override;

        void test_case_end(const CurrentTestCaseStats&) override;

        void test_case_exception(const TestCaseException& e) override;

        void subcase_start(const SubcaseSignature& in) override;

        void subcase_end() override;

        void log_assert(const AssertData& rb) override;

        void log_message(const MessageData& mb) override;

        void test_case_skipped(const TestCaseData&) override;

        void log_contexts(std::ostringstream& s);
    };

    DOCTEST_REGISTER_REPORTER("junit", 0, JUnitReporter);

} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP

#endif // DOCTEST_PARTS_PRIVATE_REPORTERS_JUNIT
