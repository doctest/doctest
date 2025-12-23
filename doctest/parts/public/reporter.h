namespace doctest {

namespace TestCaseFailureReason {
    enum Enum
    {
        None                     = 0,
        AssertFailure            = 1,   // an assertion has failed in the test case
        Exception                = 2,   // test case threw an exception
        Crash                    = 4,   // a crash...
        TooManyFailedAsserts     = 8,   // the abort-after option
        Timeout                  = 16,  // see the timeout decorator
        ShouldHaveFailedButDidnt = 32,  // see the should_fail decorator
        ShouldHaveFailedAndDid   = 64,  // see the should_fail decorator
        DidntFailExactlyNumTimes = 128, // see the expected_failures decorator
        FailedExactlyNumTimes    = 256, // see the expected_failures decorator
        CouldHaveFailedAndDid    = 512  // see the may_fail decorator
    };
} // namespace TestCaseFailureReason

    struct DOCTEST_INTERFACE CurrentTestCaseStats
    {
        int    numAssertsCurrentTest;
        int    numAssertsFailedCurrentTest;
        double seconds;
        int    failure_flags; // use TestCaseFailureReason::Enum
        bool   testCaseSuccess;
    };

    struct DOCTEST_INTERFACE TestCaseException
    {
        String error_string;
        bool   is_crash;
    };

    struct DOCTEST_INTERFACE TestRunStats
    {
        unsigned numTestCases;
        unsigned numTestCasesPassingFilters;
        unsigned numTestSuitesPassingFilters;
        unsigned numTestCasesFailed;
        int      numAsserts;
        int      numAssertsFailed;
    };

    struct QueryData
    {
        const TestRunStats*  run_stats = nullptr;
        const TestCaseData** data      = nullptr;
        unsigned             num_data  = 0;
    };

    struct DOCTEST_INTERFACE IReporter
    {
        // The constructor has to accept "const ContextOptions&" as a single argument
        // which has most of the options for the run + a pointer to the stdout stream
        // Reporter(const ContextOptions& in)

        // called when a query should be reported (listing test cases, printing the version, etc.)
        virtual void report_query(const QueryData&) = 0;

        // called when the whole test run starts
        virtual void test_run_start() = 0;
        // called when the whole test run ends (caching a pointer to the input doesn't make sense here)
        virtual void test_run_end(const TestRunStats&) = 0;

        // called when a test case is started (safe to cache a pointer to the input)
        virtual void test_case_start(const TestCaseData&) = 0;
        // called when a test case is reentered because of unfinished subcases (safe to cache a pointer to the input)
        virtual void test_case_reenter(const TestCaseData&) = 0;
        // called when a test case has ended
        virtual void test_case_end(const CurrentTestCaseStats&) = 0;

        // called when an exception is thrown from the test case (or it crashes)
        virtual void test_case_exception(const TestCaseException&) = 0;

        // called whenever a subcase is entered (don't cache pointers to the input)
        virtual void subcase_start(const SubcaseSignature&) = 0;
        // called whenever a subcase is exited (don't cache pointers to the input)
        virtual void subcase_end() = 0;

        // called for each assert (don't cache pointers to the input)
        virtual void log_assert(const AssertData&) = 0;
        // called for each message (don't cache pointers to the input)
        virtual void log_message(const MessageData&) = 0;

        // called when a test case is skipped either because it doesn't pass the filters, has a skip decorator
        // or isn't in the execution range (between first and last) (safe to cache a pointer to the input)
        virtual void test_case_skipped(const TestCaseData&) = 0;

        DOCTEST_DECLARE_INTERFACE(IReporter)

        // can obtain all currently active contexts and stringify them if one wishes to do so
        static int                         get_num_active_contexts();
        static const IContextScope* const* get_active_contexts();

        // can iterate through contexts which have been stringified automatically in their destructors when an exception has been thrown
        static int           get_num_stringified_contexts();
        static const String* get_stringified_contexts();
    };

namespace detail {
    using reporterCreatorFunc =  IReporter* (*)(const ContextOptions&);

    DOCTEST_INTERFACE void registerReporterImpl(const char* name, int prio, reporterCreatorFunc c, bool isReporter);

    template <typename Reporter>
    IReporter* reporterCreator(const ContextOptions& o) {
        return new Reporter(o);
    }
} // namespace detail

    template <typename Reporter>
    int registerReporter(const char* name, int priority, bool isReporter) {
        detail::registerReporterImpl(name, priority, detail::reporterCreator<Reporter>, isReporter);
        return 0;
    }
} // namespace doctest
