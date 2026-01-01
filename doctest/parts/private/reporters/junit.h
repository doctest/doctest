#ifndef DOCTEST_PARTS_PRIVATE_REPORTERS_JUNIT
#define DOCTEST_PARTS_PRIVATE_REPORTERS_JUNIT

#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/xml.h"
#include "doctest/parts/private/timer.h"
#include "doctest/parts/private/reporters/common.h"

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
            static std::string getCurrentTimestamp() {
                // Beware, this is not reentrant because of backward compatibility issues
                // Also, UTC only, again because of backward compatibility (%z is C++11)
                time_t rawtime;
                std::time(&rawtime);
                auto const timeStampSize = sizeof("2017-01-16T17:06:45Z");

                std::tm timeInfo;
#ifdef DOCTEST_PLATFORM_WINDOWS
                gmtime_s(&timeInfo, &rawtime);
#else // DOCTEST_PLATFORM_WINDOWS
                gmtime_r(&rawtime, &timeInfo);
#endif // DOCTEST_PLATFORM_WINDOWS

                char timeStamp[timeStampSize];
                const char* const fmt = "%Y-%m-%dT%H:%M:%SZ";

                std::strftime(timeStamp, timeStampSize, fmt, &timeInfo);
                return std::string(timeStamp);
            }

            struct JUnitTestMessage
            {
                JUnitTestMessage(const std::string& _message, const std::string& _type, const std::string& _details)
                    : message(_message), type(_type), details(_details) {}

                JUnitTestMessage(const std::string& _message, const std::string& _details)
                    : message(_message), type(), details(_details) {}

                std::string message, type, details;
            };

            struct JUnitTestCase
            {
                JUnitTestCase(const std::string& _classname, const std::string& _name)
                    : classname(_classname), name(_name), time(0), failures() {}

                std::string classname, name;
                double time;
                std::vector<JUnitTestMessage> failures, errors;
            };

            void add(const std::string& classname, const std::string& name) {
                testcases.emplace_back(classname, name);
            }

            void appendSubcaseNamesToLastTestcase(std::vector<String> nameStack) {
                for(auto& curr: nameStack)
                    if(curr.size())
                        testcases.back().name += std::string("/") + curr.c_str();
            }

            void addTime(double time) {
                if(time < 1e-4)
                    time = 0;
                testcases.back().time = time;
                totalSeconds += time;
            }

            void addFailure(const std::string& message, const std::string& type, const std::string& details) {
                testcases.back().failures.emplace_back(message, type, details);
                ++totalFailures;
            }

            void addError(const std::string& message, const std::string& details) {
                testcases.back().errors.emplace_back(message, details);
                ++totalErrors;
            }

            std::vector<JUnitTestCase> testcases;
            double totalSeconds = 0;
            int totalErrors = 0, totalFailures = 0;
        };

        JUnitTestCaseData testCaseData;

        // caching pointers/references to objects of these types - safe to do
        const ContextOptions& opt;
        const TestCaseData*   tc = nullptr;

        JUnitReporter(const ContextOptions& co)
                : xml(*co.cout)
                , opt(co) {}

        unsigned line(unsigned l) const { return opt.no_line_numbers ? 0 : l; }

        // =========================================================================================
        // WHAT FOLLOWS ARE OVERRIDES OF THE VIRTUAL METHODS OF THE REPORTER INTERFACE
        // =========================================================================================

        void report_query(const QueryData&) override {
            xml.writeDeclaration();
        }

        void test_run_start() override {
            xml.writeDeclaration();
        }

        void test_run_end(const TestRunStats& p) override {
            // remove .exe extension - mainly to have the same output on UNIX and Windows
            std::string binary_name = skipPathFromFilename(opt.binary_name.c_str());
#ifdef DOCTEST_PLATFORM_WINDOWS
            if(binary_name.rfind(".exe") != std::string::npos)
                binary_name = binary_name.substr(0, binary_name.length() - 4);
#endif // DOCTEST_PLATFORM_WINDOWS
            xml.startElement("testsuites");
            xml.startElement("testsuite").writeAttribute("name", binary_name)
                    .writeAttribute("errors", testCaseData.totalErrors)
                    .writeAttribute("failures", testCaseData.totalFailures)
                    .writeAttribute("tests", p.numAsserts);
            if(opt.no_time_in_output == false) {
                xml.writeAttribute("time", testCaseData.totalSeconds);
                xml.writeAttribute("timestamp", JUnitTestCaseData::getCurrentTimestamp());
            }
            if(opt.no_version == false)
                xml.writeAttribute("doctest_version", DOCTEST_VERSION_STR);

            for(const auto& testCase : testCaseData.testcases) {
                xml.startElement("testcase")
                    .writeAttribute("classname", testCase.classname)
                    .writeAttribute("name", testCase.name);
                if(opt.no_time_in_output == false)
                    xml.writeAttribute("time", testCase.time);
                // This is not ideal, but it should be enough to mimic gtest's junit output.
                xml.writeAttribute("status", "run");

                for(const auto& failure : testCase.failures) {
                    xml.scopedElement("failure")
                        .writeAttribute("message", failure.message)
                        .writeAttribute("type", failure.type)
                        .writeText(failure.details, false);
                }

                for(const auto& error : testCase.errors) {
                    xml.scopedElement("error")
                        .writeAttribute("message", error.message)
                        .writeText(error.details);
                }

                xml.endElement();
            }
            xml.endElement();
            xml.endElement();
        }

        void test_case_start(const TestCaseData& in) override {
            testCaseData.add(skipPathFromFilename(in.m_file.c_str()), in.m_name);
            timer.start();
        }

        void test_case_reenter(const TestCaseData& in) override {
            testCaseData.addTime(timer.getElapsedSeconds());
            testCaseData.appendSubcaseNamesToLastTestcase(deepestSubcaseStackNames);
            deepestSubcaseStackNames.clear();

            timer.start();
            testCaseData.add(skipPathFromFilename(in.m_file.c_str()), in.m_name);
        }

        void test_case_end(const CurrentTestCaseStats&) override {
            testCaseData.addTime(timer.getElapsedSeconds());
            testCaseData.appendSubcaseNamesToLastTestcase(deepestSubcaseStackNames);
            deepestSubcaseStackNames.clear();
        }

        void test_case_exception(const TestCaseException& e) override {
            DOCTEST_LOCK_MUTEX(mutex)
            testCaseData.addError("exception", e.error_string.c_str());
        }

        void subcase_start(const SubcaseSignature& in) override {
            deepestSubcaseStackNames.push_back(in.m_name);
        }

        void subcase_end() override {}

        void log_assert(const AssertData& rb) override {
            if(!rb.m_failed) // report only failures & ignore the `success` option
                return;

            DOCTEST_LOCK_MUTEX(mutex)

            std::ostringstream os;
            os << skipPathFromFilename(rb.m_file) << (opt.gnu_file_line ? ":" : "(")
              << line(rb.m_line) << (opt.gnu_file_line ? ":" : "):") << std::endl;

            fulltext_log_assert_to_stream(os, rb);
            log_contexts(os);
            testCaseData.addFailure(rb.m_decomp.c_str(), assertString(rb.m_at), os.str());
        }

        void log_message(const MessageData& mb) override {
            if(mb.m_severity & assertType::is_warn) // report only failures
                return;

            DOCTEST_LOCK_MUTEX(mutex)

            std::ostringstream os;
            os << skipPathFromFilename(mb.m_file) << (opt.gnu_file_line ? ":" : "(")
              << line(mb.m_line) << (opt.gnu_file_line ? ":" : "):") << std::endl;

            os << mb.m_string.c_str() << "\n";
            log_contexts(os);

            testCaseData.addFailure(mb.m_string.c_str(),
                mb.m_severity & assertType::is_check ? "FAIL_CHECK" : "FAIL", os.str());
        }

        void test_case_skipped(const TestCaseData&) override {}

        void log_contexts(std::ostringstream& s) {
            int num_contexts = get_num_active_contexts();
            if(num_contexts) {
                auto contexts = get_active_contexts();

                s << "  logged: ";
                for(int i = 0; i < num_contexts; ++i) {
                    s << (i == 0 ? "" : "          ");
                    contexts[i]->stringify(&s);
                    s << std::endl;
                }
            }
        }
    };

    DOCTEST_REGISTER_REPORTER("junit", 0, JUnitReporter);

} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP

#endif // DOCTEST_PARTS_PRIVATE_REPORTERS_JUNIT
