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

        XmlReporter(const ContextOptions& co)
                : xml(*co.cout)
                , opt(co) {}

        void log_contexts() {
            int num_contexts = get_num_active_contexts();
            if(num_contexts) {
                auto              contexts = get_active_contexts();
                std::stringstream ss;
                for(int i = 0; i < num_contexts; ++i) {
                    contexts[i]->stringify(&ss);
                    xml.scopedElement("Info").writeText(ss.str());
                    ss.str("");
                }
            }
        }

        unsigned line(unsigned l) const { return opt.no_line_numbers ? 0 : l; }

        void test_case_start_impl(const TestCaseData& in) {
            bool open_ts_tag = false;
            if(tc != nullptr) { // we have already opened a test suite
                if(std::strcmp(tc->m_test_suite, in.m_test_suite) != 0) {
                    xml.endElement();
                    open_ts_tag = true;
                }
            }
            else {
                open_ts_tag = true; // first test case ==> first test suite
            }

            if(open_ts_tag) {
                xml.startElement("TestSuite");
                xml.writeAttribute("name", in.m_test_suite);
            }

            tc = &in;
            xml.startElement("TestCase")
                    .writeAttribute("name", in.m_name)
                    .writeAttribute("filename", skipPathFromFilename(in.m_file.c_str()))
                    .writeAttribute("line", line(in.m_line))
                    .writeAttribute("description", in.m_description);

            if(Approx(in.m_timeout) != 0)
                xml.writeAttribute("timeout", in.m_timeout);
            if(in.m_may_fail)
                xml.writeAttribute("may_fail", true);
            if(in.m_should_fail)
                xml.writeAttribute("should_fail", true);
        }

        // =========================================================================================
        // WHAT FOLLOWS ARE OVERRIDES OF THE VIRTUAL METHODS OF THE REPORTER INTERFACE
        // =========================================================================================

        void report_query(const QueryData& in) override {
            test_run_start();
            if(opt.list_reporters) {
                for(auto& curr : getListeners())
                    xml.scopedElement("Listener")
                            .writeAttribute("priority", curr.first.first)
                            .writeAttribute("name", curr.first.second);
                for(auto& curr : getReporters())
                    xml.scopedElement("Reporter")
                            .writeAttribute("priority", curr.first.first)
                            .writeAttribute("name", curr.first.second);
            } else if(opt.count || opt.list_test_cases) {
                for(unsigned i = 0; i < in.num_data; ++i) {
                    xml.scopedElement("TestCase").writeAttribute("name", in.data[i]->m_name)
                        .writeAttribute("testsuite", in.data[i]->m_test_suite)
                        .writeAttribute("filename", skipPathFromFilename(in.data[i]->m_file.c_str()))
                        .writeAttribute("line", line(in.data[i]->m_line))
                        .writeAttribute("skipped", in.data[i]->m_skip);
                }
                xml.scopedElement("OverallResultsTestCases")
                        .writeAttribute("unskipped", in.run_stats->numTestCasesPassingFilters);
            } else if(opt.list_test_suites) {
                for(unsigned i = 0; i < in.num_data; ++i)
                    xml.scopedElement("TestSuite").writeAttribute("name", in.data[i]->m_test_suite);
                xml.scopedElement("OverallResultsTestCases")
                        .writeAttribute("unskipped", in.run_stats->numTestCasesPassingFilters);
                xml.scopedElement("OverallResultsTestSuites")
                        .writeAttribute("unskipped", in.run_stats->numTestSuitesPassingFilters);
            }
            xml.endElement();
        }

        void test_run_start() override {
            xml.writeDeclaration();

            // remove .exe extension - mainly to have the same output on UNIX and Windows
            std::string binary_name = skipPathFromFilename(opt.binary_name.c_str());
#ifdef DOCTEST_PLATFORM_WINDOWS
            if(binary_name.rfind(".exe") != std::string::npos)
                binary_name = binary_name.substr(0, binary_name.length() - 4);
#endif // DOCTEST_PLATFORM_WINDOWS

            xml.startElement("doctest").writeAttribute("binary", binary_name);
            if(opt.no_version == false)
                xml.writeAttribute("version", DOCTEST_VERSION_STR);

            // only the consequential ones (TODO: filters)
            xml.scopedElement("Options")
                    .writeAttribute("order_by", opt.order_by.c_str())
                    .writeAttribute("rand_seed", opt.rand_seed)
                    .writeAttribute("first", opt.first)
                    .writeAttribute("last", opt.last)
                    .writeAttribute("abort_after", opt.abort_after)
                    .writeAttribute("subcase_filter_levels", opt.subcase_filter_levels)
                    .writeAttribute("case_sensitive", opt.case_sensitive)
                    .writeAttribute("no_throw", opt.no_throw)
                    .writeAttribute("no_skip", opt.no_skip);
        }

        void test_run_end(const TestRunStats& p) override {
            if(tc) // the TestSuite tag - only if there has been at least 1 test case
                xml.endElement();

            xml.scopedElement("OverallResultsAsserts")
                    .writeAttribute("successes", p.numAsserts - p.numAssertsFailed)
                    .writeAttribute("failures", p.numAssertsFailed);

            xml.startElement("OverallResultsTestCases")
                    .writeAttribute("successes",
                                    p.numTestCasesPassingFilters - p.numTestCasesFailed)
                    .writeAttribute("failures", p.numTestCasesFailed);
            if(opt.no_skipped_summary == false)
                xml.writeAttribute("skipped", p.numTestCases - p.numTestCasesPassingFilters);
            xml.endElement();

            xml.endElement();
        }

        void test_case_start(const TestCaseData& in) override {
            test_case_start_impl(in);
            xml.ensureTagClosed();
        }

        void test_case_reenter(const TestCaseData&) override {}

        void test_case_end(const CurrentTestCaseStats& st) override {
            xml.startElement("OverallResultsAsserts")
                    .writeAttribute("successes",
                                    st.numAssertsCurrentTest - st.numAssertsFailedCurrentTest)
                    .writeAttribute("failures", st.numAssertsFailedCurrentTest)
                    .writeAttribute("test_case_success", st.testCaseSuccess);
            if(opt.duration)
                xml.writeAttribute("duration", st.seconds);
            if(tc->m_expected_failures)
                xml.writeAttribute("expected_failures", tc->m_expected_failures);
            xml.endElement();

            xml.endElement();
        }

        void test_case_exception(const TestCaseException& e) override {
            DOCTEST_LOCK_MUTEX(mutex)

            xml.scopedElement("Exception")
                    .writeAttribute("crash", e.is_crash)
                    .writeText(e.error_string.c_str());
        }

        void subcase_start(const SubcaseSignature& in) override {
            xml.startElement("SubCase")
                    .writeAttribute("name", in.m_name)
                    .writeAttribute("filename", skipPathFromFilename(in.m_file))
                    .writeAttribute("line", line(in.m_line));
            xml.ensureTagClosed();
        }

        void subcase_end() override { xml.endElement(); }

        void log_assert(const AssertData& rb) override {
            if(!rb.m_failed && !opt.success)
                return;

            DOCTEST_LOCK_MUTEX(mutex)

            xml.startElement("Expression")
                    .writeAttribute("success", !rb.m_failed)
                    .writeAttribute("type", assertString(rb.m_at))
                    .writeAttribute("filename", skipPathFromFilename(rb.m_file))
                    .writeAttribute("line", line(rb.m_line));

            xml.scopedElement("Original").writeText(rb.m_expr);

            if(rb.m_threw)
                xml.scopedElement("Exception").writeText(rb.m_exception.c_str());

            if(rb.m_at & assertType::is_throws_as)
                xml.scopedElement("ExpectedException").writeText(rb.m_exception_type);
            if(rb.m_at & assertType::is_throws_with)
                xml.scopedElement("ExpectedExceptionString").writeText(rb.m_exception_string.c_str());
            if((rb.m_at & assertType::is_normal) && !rb.m_threw)
                xml.scopedElement("Expanded").writeText(rb.m_decomp.c_str());

            log_contexts();

            xml.endElement();
        }

        void log_message(const MessageData& mb) override {
            DOCTEST_LOCK_MUTEX(mutex)

            xml.startElement("Message")
                    .writeAttribute("type", failureString(mb.m_severity))
                    .writeAttribute("filename", skipPathFromFilename(mb.m_file))
                    .writeAttribute("line", line(mb.m_line));

            xml.scopedElement("Text").writeText(mb.m_string.c_str());

            log_contexts();

            xml.endElement();
        }

        void test_case_skipped(const TestCaseData& in) override {
            if(opt.no_skipped_summary == false) {
                test_case_start_impl(in);
                xml.writeAttribute("skipped", "true");
                xml.endElement();
            }
        }
    };

    DOCTEST_REGISTER_REPORTER("xml", 0, XmlReporter);

} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP

#endif // DOCTEST_PARTS_PRIVATE_REPORTERS_XML
