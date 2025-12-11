namespace doctest {
namespace detail {
    struct DOCTEST_INTERFACE TestCase;
} // namespace detail

    struct ContextOptions //!OCLINT too many fields
    {
        std::ostream* cout = nullptr; // stdout stream
        String        binary_name;    // the test binary name

        const detail::TestCase* currentTest = nullptr;

        // == parameters from the command line
        String   out;       // output filename
        String   order_by;  // how tests should be ordered
        unsigned rand_seed; // the seed for rand ordering

        unsigned first; // the first (matching) test to be executed
        unsigned last;  // the last (matching) test to be executed

        int abort_after;           // stop tests after this many failed assertions
        int subcase_filter_levels; // apply the subcase filters for the first N levels

        bool success;              // include successful assertions in output
        bool case_sensitive;       // if filtering should be case sensitive
        bool exit;                 // if the program should be exited after the tests are ran/whatever
        bool duration;             // print the time duration of each test case
        bool minimal;              // minimal console output (only test failures)
        bool quiet;                // no console output
        bool no_throw;             // to skip exceptions-related assertion macros
        bool no_exitcode;          // if the framework should return 0 as the exitcode
        bool no_run;               // to not run the tests at all (can be done with an "*" exclude)
        bool no_intro;             // to not print the intro of the framework
        bool no_version;           // to not print the version of the framework
        bool no_colors;            // if output to the console should be colorized
        bool force_colors;         // forces the use of colors even when a tty cannot be detected
        bool no_breaks;            // to not break into the debugger
        bool no_skip;              // don't skip test cases which are marked to be skipped
        bool gnu_file_line;        // if line numbers should be surrounded with :x: and not (x):
        bool no_path_in_filenames; // if the path to files should be removed from the output
        String strip_file_prefixes;// remove the longest matching one of these prefixes from any file paths in the output
        bool no_line_numbers;      // if source code line numbers should be omitted from the output
        bool no_debug_output;      // no output in the debug console when a debugger is attached
        bool no_skipped_summary;   // don't print "skipped" in the summary !!! UNDOCUMENTED !!!
        bool no_time_in_output;    // omit any time/timestamps from output !!! UNDOCUMENTED !!!

        bool help;             // to print the help
        bool version;          // to print the version
        bool count;            // if only the count of matching tests is to be retrieved
        bool list_test_cases;  // to list all tests matching the filters
        bool list_test_suites; // to list all suites matching the filters
        bool list_reporters;   // lists all registered reporters
    };

    DOCTEST_INTERFACE const ContextOptions* getContextOptions();

} // namespace doctest
