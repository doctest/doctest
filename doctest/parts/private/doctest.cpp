#include "doctest/parts/private/prelude.h"

#ifndef DOCTEST_CONFIG_OPTIONS_PREFIX
#define DOCTEST_CONFIG_OPTIONS_PREFIX "dt-"
#endif

#ifndef DOCTEST_CONFIG_OPTIONS_FILE_PREFIX_SEPARATOR
#define DOCTEST_CONFIG_OPTIONS_FILE_PREFIX_SEPARATOR ':'
#endif

#include "doctest/parts/private/exceptions.h"
#include "doctest/parts/private/timer.h"
#include "doctest/parts/private/atomic.h"
#include "doctest/parts/private/context_state.h"
#include "doctest/parts/private/test_case.h"
#include "doctest/parts/private/exception_translator.h"
#include "doctest/parts/private/context_scope.h"
#include "doctest/parts/private/reporter.h"

namespace doctest {

bool is_running_in_test = false;

DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wnull-dereference")
DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH("-Wnull-dereference")
// depending on the current options this will remove the path of filenames
const char* skipPathFromFilename(const char* file) {
#ifndef DOCTEST_CONFIG_DISABLE
    if(getContextOptions()->no_path_in_filenames) {
        auto back    = std::strrchr(file, '\\');
        auto forward = std::strrchr(file, '/');
        if(back || forward) {
            if(back > forward)
                forward = back;
            return forward + 1;
        }
    } else {
        const auto prefixes = getContextOptions()->strip_file_prefixes;
        const char separator = DOCTEST_CONFIG_OPTIONS_FILE_PREFIX_SEPARATOR;
        String::size_type longest_match = 0U;
        for(String::size_type pos = 0U; pos < prefixes.size(); ++pos)
        {
            const auto prefix_start = pos;
            pos = std::min(prefixes.find(separator, prefix_start), prefixes.size());

            const auto prefix_size = pos - prefix_start;
            if(prefix_size > longest_match)
            {
                // TODO under DOCTEST_MSVC: does the comparison need strnicmp() to work with drive letter capitalization?
                if(0 == std::strncmp(prefixes.c_str() + prefix_start, file, prefix_size))
                {
                    longest_match = prefix_size;
                }
            }
        }
        return &file[longest_match];
    }
#endif // DOCTEST_CONFIG_DISABLE
    return file;
}
DOCTEST_CLANG_SUPPRESS_WARNING_POP
DOCTEST_GCC_SUPPRESS_WARNING_POP

} // namespace doctest

#ifdef DOCTEST_CONFIG_DISABLE
namespace doctest {
Context::Context(int, const char* const*) {}
Context::~Context() = default;
void Context::applyCommandLine(int, const char* const*) {}
void Context::addFilter(const char*, const char*) {}
void Context::clearFilters() {}
void Context::setOption(const char*, bool) {}
void Context::setOption(const char*, int) {}
void Context::setOption(const char*, const char*) {}
bool Context::shouldExit() { return false; }
void Context::setAsDefaultForAssertsOutOfTestCases() {}
void Context::setAssertHandler(detail::assert_handler) {}
void Context::setCout(std::ostream*) {}
int  Context::run() { return 0; }
} // namespace doctest
#else // DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace {
    using namespace detail;
    // matching of a string against a wildcard mask (case sensitivity configurable) taken from
    // https://www.codeproject.com/Articles/1088/Wildcard-string-compare-globbing
    int wildcmp(const char* str, const char* wild, bool caseSensitive) {
        const char* cp = str;
        const char* mp = wild;

        while((*str) && (*wild != '*')) {
            if((caseSensitive ? (*wild != *str) : (tolower(*wild) != tolower(*str))) &&
               (*wild != '?')) {
                return 0;
            }
            wild++;
            str++;
        }

        while(*str) {
            if(*wild == '*') {
                if(!*++wild) {
                    return 1;
                }
                mp = wild;
                cp = str + 1;
            } else if((caseSensitive ? (*wild == *str) : (tolower(*wild) == tolower(*str))) ||
                      (*wild == '?')) {
                wild++;
                str++;
            } else {
                wild = mp;   //!OCLINT parameter reassignment
                str  = cp++; //!OCLINT parameter reassignment
            }
        }

        while(*wild == '*') {
            wild++;
        }
        return !*wild;
    }

    // checks if the name matches any of the filters (and can be configured what to do when empty)
    bool matchesAny(const char* name, const std::vector<String>& filters, bool matchEmpty,
        bool caseSensitive) {
        if (filters.empty() && matchEmpty)
            return true;
        for (auto& curr : filters)
            if (wildcmp(name, curr.c_str(), caseSensitive))
                return true;
        return false;
    }

    DOCTEST_NO_SANITIZE_INTEGER
    unsigned long long hash(unsigned long long a, unsigned long long b) {
        return (a << 5) + b;
    }

    // C string hash function (djb2) - taken from http://www.cse.yorku.ca/~oz/hash.html
    DOCTEST_NO_SANITIZE_INTEGER
    unsigned long long hash(const char* str) {
        unsigned long long hash = 5381;
        char c;
        while ((c = *str++))
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        return hash;
    }

    unsigned long long hash(const SubcaseSignature& sig) {
        return hash(hash(hash(sig.m_file), hash(sig.m_name.c_str())), sig.m_line);
    }

    unsigned long long hash(const std::vector<SubcaseSignature>& sigs, size_t count) {
        unsigned long long running = 0;
        auto end = sigs.begin() + count;
        for (auto it = sigs.begin(); it != end; it++) {
            running = hash(running, hash(*it));
        }
        return running;
    }

    unsigned long long hash(const std::vector<SubcaseSignature>& sigs) {
        unsigned long long running = 0;
        for (const SubcaseSignature& sig : sigs) {
            running = hash(running, hash(sig));
        }
        return running;
    }
} // namespace
namespace {
    using namespace detail;
    // for sorting tests by file/line
    bool fileOrderComparator(const TestCase* lhs, const TestCase* rhs) {
        // this is needed because MSVC gives different case for drive letters
        // for __FILE__ when evaluated in a header and a source file
        const int res = lhs->m_file.compare(rhs->m_file, bool(DOCTEST_MSVC));
        if(res != 0)
            return res < 0;
        if(lhs->m_line != rhs->m_line)
            return lhs->m_line < rhs->m_line;
        return lhs->m_template_id < rhs->m_template_id;
    }

    // for sorting tests by suite/file/line
    bool suiteOrderComparator(const TestCase* lhs, const TestCase* rhs) {
        const int res = std::strcmp(lhs->m_test_suite, rhs->m_test_suite);
        if(res != 0)
            return res < 0;
        return fileOrderComparator(lhs, rhs);
    }

    // for sorting tests by name/suite/file/line
    bool nameOrderComparator(const TestCase* lhs, const TestCase* rhs) {
        const int res = std::strcmp(lhs->m_name, rhs->m_name);
        if(res != 0)
            return res < 0;
        return suiteOrderComparator(lhs, rhs);
    }

} // namespace

namespace detail {


} // namespace detail
namespace {
    using namespace detail;

#if !defined(DOCTEST_CONFIG_POSIX_SIGNALS) && !defined(DOCTEST_CONFIG_WINDOWS_SEH)
    struct FatalConditionHandler
    {
        static void reset() {}
        static void allocateAltStackMem() {}
        static void freeAltStackMem() {}
    };
#else // DOCTEST_CONFIG_POSIX_SIGNALS || DOCTEST_CONFIG_WINDOWS_SEH

    void reportFatal(const std::string&);

#ifdef DOCTEST_PLATFORM_WINDOWS

    struct SignalDefs
    {
        DWORD id;
        const char* name;
    };
    // There is no 1-1 mapping between signals and windows exceptions.
    // Windows can easily distinguish between SO and SigSegV,
    // but SigInt, SigTerm, etc are handled differently.
    SignalDefs signalDefs[] = {
            {static_cast<DWORD>(EXCEPTION_ILLEGAL_INSTRUCTION),
             "SIGILL - Illegal instruction signal"},
            {static_cast<DWORD>(EXCEPTION_STACK_OVERFLOW), "SIGSEGV - Stack overflow"},
            {static_cast<DWORD>(EXCEPTION_ACCESS_VIOLATION),
             "SIGSEGV - Segmentation violation signal"},
            {static_cast<DWORD>(EXCEPTION_INT_DIVIDE_BY_ZERO), "Divide by zero error"},
    };

    struct FatalConditionHandler
    {
        static LONG CALLBACK handleException(PEXCEPTION_POINTERS ExceptionInfo) {
            // Multiple threads may enter this filter/handler at once. We want the error message to be printed on the
            // console just once no matter how many threads have crashed.
            DOCTEST_DECLARE_STATIC_MUTEX(mutex)
            static bool execute = true;
            {
                DOCTEST_LOCK_MUTEX(mutex)
                if(execute) {
                    bool reported = false;
                    for(size_t i = 0; i < DOCTEST_COUNTOF(signalDefs); ++i) {
                        if(ExceptionInfo->ExceptionRecord->ExceptionCode == signalDefs[i].id) {
                            reportFatal(signalDefs[i].name);
                            reported = true;
                            break;
                        }
                    }
                    if(reported == false)
                        reportFatal("Unhandled SEH exception caught");
                    if(isDebuggerActive() && !g_cs->no_breaks)
                        DOCTEST_BREAK_INTO_DEBUGGER();
                }
                execute = false;
            }
            std::exit(EXIT_FAILURE);
        }

        static void allocateAltStackMem() {}
        static void freeAltStackMem() {}

        FatalConditionHandler() {
            isSet = true;
            // 32k seems enough for doctest to handle stack overflow,
            // but the value was found experimentally, so there is no strong guarantee
            guaranteeSize = 32 * 1024;
            // Register an unhandled exception filter
            previousTop = SetUnhandledExceptionFilter(handleException);
            // Pass in guarantee size to be filled
            SetThreadStackGuarantee(&guaranteeSize);

            // On Windows uncaught exceptions from another thread, exceptions from
            // destructors, or calls to std::terminate are not a SEH exception

            // The terminal handler gets called when:
            // - std::terminate is called FROM THE TEST RUNNER THREAD
            // - an exception is thrown from a destructor FROM THE TEST RUNNER THREAD
            original_terminate_handler = std::get_terminate();
            std::set_terminate([]() DOCTEST_NOEXCEPT {
                reportFatal("Terminate handler called");
                if(isDebuggerActive() && !g_cs->no_breaks)
                    DOCTEST_BREAK_INTO_DEBUGGER();
                std::exit(EXIT_FAILURE); // explicitly exit - otherwise the SIGABRT handler may be called as well
            });

            // SIGABRT is raised when:
            // - std::terminate is called FROM A DIFFERENT THREAD
            // - an exception is thrown from a destructor FROM A DIFFERENT THREAD
            // - an uncaught exception is thrown FROM A DIFFERENT THREAD
            prev_sigabrt_handler = std::signal(SIGABRT, [](int signal) DOCTEST_NOEXCEPT {
                if(signal == SIGABRT) {
                    reportFatal("SIGABRT - Abort (abnormal termination) signal");
                    if(isDebuggerActive() && !g_cs->no_breaks)
                        DOCTEST_BREAK_INTO_DEBUGGER();
                    std::exit(EXIT_FAILURE);
                }
            });

            // The following settings are taken from google test, and more
            // specifically from UnitTest::Run() inside of gtest.cc

            // the user does not want to see pop-up dialogs about crashes
            prev_error_mode_1 = SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOALIGNMENTFAULTEXCEPT |
                                             SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
            // This forces the abort message to go to stderr in all circumstances.
            prev_error_mode_2 = _set_error_mode(_OUT_TO_STDERR);
            // In the debug version, Visual Studio pops up a separate dialog
            // offering a choice to debug the aborted program - we want to disable that.
            prev_abort_behavior = _set_abort_behavior(0x0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
            // In debug mode, the Windows CRT can crash with an assertion over invalid
            // input (e.g. passing an invalid file descriptor). The default handling
            // for these assertions is to pop up a dialog and wait for user input.
            // Instead ask the CRT to dump such assertions to stderr non-interactively.
            prev_report_mode = _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
            prev_report_file = _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
        }

        static void reset() {
            if(isSet) {
                // Unregister handler and restore the old guarantee
                SetUnhandledExceptionFilter(previousTop);
                SetThreadStackGuarantee(&guaranteeSize);
                std::set_terminate(original_terminate_handler);
                std::signal(SIGABRT, prev_sigabrt_handler);
                SetErrorMode(prev_error_mode_1);
                _set_error_mode(prev_error_mode_2);
                _set_abort_behavior(prev_abort_behavior, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
                static_cast<void>(_CrtSetReportMode(_CRT_ASSERT, prev_report_mode));
                static_cast<void>(_CrtSetReportFile(_CRT_ASSERT, prev_report_file));
                isSet = false;
            }
        }

        ~FatalConditionHandler() { reset(); }

    private:
        static UINT         prev_error_mode_1;
        static int          prev_error_mode_2;
        static unsigned int prev_abort_behavior;
        static int          prev_report_mode;
        static _HFILE       prev_report_file;
        static void (DOCTEST_CDECL *prev_sigabrt_handler)(int);
        static std::terminate_handler original_terminate_handler;
        static bool isSet;
        static ULONG guaranteeSize;
        static LPTOP_LEVEL_EXCEPTION_FILTER previousTop;
    };

    UINT         FatalConditionHandler::prev_error_mode_1;
    int          FatalConditionHandler::prev_error_mode_2;
    unsigned int FatalConditionHandler::prev_abort_behavior;
    int          FatalConditionHandler::prev_report_mode;
    _HFILE       FatalConditionHandler::prev_report_file;
    void (DOCTEST_CDECL *FatalConditionHandler::prev_sigabrt_handler)(int);
    std::terminate_handler FatalConditionHandler::original_terminate_handler;
    bool FatalConditionHandler::isSet = false;
    ULONG FatalConditionHandler::guaranteeSize = 0;
    LPTOP_LEVEL_EXCEPTION_FILTER FatalConditionHandler::previousTop = nullptr;

#else // DOCTEST_PLATFORM_WINDOWS

    struct SignalDefs
    {
        int         id;
        const char* name;
    };
    SignalDefs signalDefs[] = {{SIGINT, "SIGINT - Terminal interrupt signal"},
                               {SIGILL, "SIGILL - Illegal instruction signal"},
                               {SIGFPE, "SIGFPE - Floating point error signal"},
                               {SIGSEGV, "SIGSEGV - Segmentation violation signal"},
                               {SIGTERM, "SIGTERM - Termination request signal"},
                               {SIGABRT, "SIGABRT - Abort (abnormal termination) signal"}};

    struct FatalConditionHandler
    {
        static bool             isSet;
        static struct sigaction oldSigActions[DOCTEST_COUNTOF(signalDefs)];
        static stack_t          oldSigStack;
        static size_t           altStackSize;
        static char*            altStackMem;

        static void handleSignal(int sig) {
            const char* name = "<unknown signal>";
            for(std::size_t i = 0; i < DOCTEST_COUNTOF(signalDefs); ++i) {
                SignalDefs& def = signalDefs[i];
                if(sig == def.id) {
                    name = def.name;
                    break;
                }
            }
            reset();
            reportFatal(name);
            raise(sig);
        }

        static void allocateAltStackMem() {
            altStackMem = new char[altStackSize];
        }

        static void freeAltStackMem() {
            delete[] altStackMem;
        }

        FatalConditionHandler() {
            isSet = true;
            stack_t sigStack;
            sigStack.ss_sp    = altStackMem;
            sigStack.ss_size  = altStackSize;
            sigStack.ss_flags = 0;
            sigaltstack(&sigStack, &oldSigStack);
            struct sigaction sa = {};
            sa.sa_handler       = handleSignal;
            sa.sa_flags         = SA_ONSTACK;
            for(std::size_t i = 0; i < DOCTEST_COUNTOF(signalDefs); ++i) {
                sigaction(signalDefs[i].id, &sa, &oldSigActions[i]);
            }
        }

        ~FatalConditionHandler() { reset(); }
        static void reset() {
            if(isSet) {
                // Set signals back to previous values -- hopefully nobody overwrote them in the meantime
                for(std::size_t i = 0; i < DOCTEST_COUNTOF(signalDefs); ++i) {
                    sigaction(signalDefs[i].id, &oldSigActions[i], nullptr);
                }
                // Return the old stack
                sigaltstack(&oldSigStack, nullptr);
                isSet = false;
            }
        }
    };

    bool             FatalConditionHandler::isSet = false;
    struct sigaction FatalConditionHandler::oldSigActions[DOCTEST_COUNTOF(signalDefs)] = {};
    stack_t          FatalConditionHandler::oldSigStack = {};
    size_t           FatalConditionHandler::altStackSize = 4 * SIGSTKSZ;
    char*            FatalConditionHandler::altStackMem = nullptr;

#endif // DOCTEST_PLATFORM_WINDOWS
#endif // DOCTEST_CONFIG_POSIX_SIGNALS || DOCTEST_CONFIG_WINDOWS_SEH

} // namespace

namespace {
    using namespace detail;

    void addAssert(assertType::Enum at) {
        if((at & assertType::is_warn) == 0) //!OCLINT bitwise operator in conditional
            g_cs->numAssertsCurrentTest_atomic++;
    }

    void addFailedAssert(assertType::Enum at) {
        if((at & assertType::is_warn) == 0) //!OCLINT bitwise operator in conditional
            g_cs->numAssertsFailedCurrentTest_atomic++;
    }

#if defined(DOCTEST_CONFIG_POSIX_SIGNALS) || defined(DOCTEST_CONFIG_WINDOWS_SEH)
    void reportFatal(const std::string& message) {
        g_cs->failure_flags |= TestCaseFailureReason::Crash;

        DOCTEST_ITERATE_THROUGH_REPORTERS(test_case_exception, {message.c_str(), true});

        while (g_cs->subcaseStack.size()) {
            g_cs->subcaseStack.pop_back();
            DOCTEST_ITERATE_THROUGH_REPORTERS(subcase_end, DOCTEST_EMPTY);
        }

        g_cs->finalizeTestCaseData();

        DOCTEST_ITERATE_THROUGH_REPORTERS(test_case_end, *g_cs);

        DOCTEST_ITERATE_THROUGH_REPORTERS(test_run_end, *g_cs);
    }
#endif // DOCTEST_CONFIG_POSIX_SIGNALS || DOCTEST_CONFIG_WINDOWS_SEH
} // namespace

namespace detail {

    void ResultBuilder::translateException() {
        m_threw     = true;
        m_exception = translateActiveException();
    }

    bool ResultBuilder::log() {
        if(m_at & assertType::is_throws) { //!OCLINT bitwise operator in conditional
            m_failed = !m_threw;
        } else if((m_at & assertType::is_throws_as) && (m_at & assertType::is_throws_with)) { //!OCLINT
            m_failed = !m_threw_as || !m_exception_string.check(m_exception);
        } else if(m_at & assertType::is_throws_as) { //!OCLINT bitwise operator in conditional
            m_failed = !m_threw_as;
        } else if(m_at & assertType::is_throws_with) { //!OCLINT bitwise operator in conditional
            m_failed = !m_exception_string.check(m_exception);
        } else if(m_at & assertType::is_nothrow) { //!OCLINT bitwise operator in conditional
            m_failed = m_threw;
        }

        if(m_exception.size())
            m_exception = "\"" + m_exception + "\"";

        if(is_running_in_test) {
            addAssert(m_at);
            DOCTEST_ITERATE_THROUGH_REPORTERS(log_assert, *this);

            if(m_failed)
                addFailedAssert(m_at);
        } else if(m_failed) {
            failed_out_of_a_testing_context(*this);
        }

        return m_failed && isDebuggerActive() && !getContextOptions()->no_breaks &&
            (g_cs->currentTest == nullptr || !g_cs->currentTest->m_no_breaks); // break into debugger
    }

    MessageBuilder::MessageBuilder(const char* file, int line, assertType::Enum severity) {
        m_stream   = tlssPush();
        m_file     = file;
        m_line     = line;
        m_severity = severity;
    }

    MessageBuilder::~MessageBuilder() {
        if (!logged)
            tlssPop();
    }

    bool MessageBuilder::log() {
        if (!logged) {
            m_string = tlssPop();
            logged = true;
        }

        DOCTEST_ITERATE_THROUGH_REPORTERS(log_message, *this);

        const bool isWarn = m_severity & assertType::is_warn;

        // warn is just a message in this context so we don't treat it as an assert
        if(!isWarn) {
            addAssert(m_severity);
            addFailedAssert(m_severity);
        }

        return isDebuggerActive() && !getContextOptions()->no_breaks && !isWarn &&
            (g_cs->currentTest == nullptr || !g_cs->currentTest->m_no_breaks); // break into debugger
    }

    void MessageBuilder::react() {
        if(m_severity & assertType::is_require) //!OCLINT bitwise operator in conditional
            throwException();
    }
} // namespace detail
} // namespace doctest

#include "doctest/parts/private/xml.h"
#include "doctest/parts/private/reporters/xml.h"
#include "doctest/parts/private/reporters/junit.h"
#include "doctest/parts/private/reporters/console.h"
#include "doctest/parts/private/reporters/debug_output_window.h"

namespace doctest {
namespace {
    using namespace detail;

    // the implementation of parseOption()
    bool parseOptionImpl(int argc, const char* const* argv, const char* pattern, String* value) {
        // going from the end to the beginning and stopping on the first occurrence from the end
        for(int i = argc; i > 0; --i) {
            auto index = i - 1;
            auto temp = std::strstr(argv[index], pattern);
            if(temp && (value || strlen(temp) == strlen(pattern))) { //!OCLINT prefer early exits and continue
                // eliminate matches in which the chars before the option are not '-'
                bool noBadCharsFound = true;
                auto curr            = argv[index];
                while(curr != temp) {
                    if(*curr++ != '-') {
                        noBadCharsFound = false;
                        break;
                    }
                }
                if(noBadCharsFound && argv[index][0] == '-') {
                    if(value) {
                        // parsing the value of an option
                        temp += strlen(pattern);
                        const unsigned len = strlen(temp);
                        if(len) {
                            *value = temp;
                            return true;
                        }
                    } else {
                        // just a flag - no value
                        return true;
                    }
                }
            }
        }
        return false;
    }

    // parses an option and returns the string after the '=' character
    bool parseOption(int argc, const char* const* argv, const char* pattern, String* value = nullptr,
                     const String& defaultVal = String()) {
        if(value)
            *value = defaultVal;
#ifndef DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS
        // offset (normally 3 for "dt-") to skip prefix
        if(parseOptionImpl(argc, argv, pattern + strlen(DOCTEST_CONFIG_OPTIONS_PREFIX), value))
            return true;
#endif // DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS
        return parseOptionImpl(argc, argv, pattern, value);
    }

    // locates a flag on the command line
    bool parseFlag(int argc, const char* const* argv, const char* pattern) {
        return parseOption(argc, argv, pattern);
    }

    // parses a comma separated list of words after a pattern in one of the arguments in argv
    bool parseCommaSepArgs(int argc, const char* const* argv, const char* pattern,
                           std::vector<String>& res) {
        String filtersString;
        if(parseOption(argc, argv, pattern, &filtersString)) {
            // tokenize with "," as a separator, unless escaped with backslash
            std::ostringstream s;
            auto flush = [&s, &res]() {
                auto string = s.str();
                if(string.size() > 0) {
                    res.push_back(string.c_str());
                }
                s.str("");
            };

            bool seenBackslash = false;
            const char* current = filtersString.c_str();
            const char* end = current + strlen(current);
            while(current != end) {
                char character = *current++;
                if(seenBackslash) {
                    seenBackslash = false;
                    if(character == ',' || character == '\\') {
                        s.put(character);
                        continue;
                    }
                    s.put('\\');
                }
                if(character == '\\') {
                    seenBackslash = true;
                } else if(character == ',') {
                    flush();
                } else {
                    s.put(character);
                }
            }

            if(seenBackslash) {
                s.put('\\');
            }
            flush();
            return true;
        }
        return false;
    }

    enum optionType
    {
        option_bool,
        option_int
    };

    // parses an int/bool option from the command line
    bool parseIntOption(int argc, const char* const* argv, const char* pattern, optionType type,
                        int& res) {
        String parsedValue;
        if(!parseOption(argc, argv, pattern, &parsedValue))
            return false;

        if(type) {
            // integer
            // TODO: change this to use std::stoi or something else! currently it uses undefined behavior - assumes '0' on failed parse...
            int theInt = std::atoi(parsedValue.c_str());
            if (theInt != 0) {
                res = theInt; //!OCLINT parameter reassignment
                return true;
            }
        } else {
            // boolean
            const char positive[][5] = { "1", "true", "on", "yes" };  // 5 - strlen("true") + 1
            const char negative[][6] = { "0", "false", "off", "no" }; // 6 - strlen("false") + 1

            // if the value matches any of the positive/negative possibilities
            for (unsigned i = 0; i < 4; i++) {
                if (parsedValue.compare(positive[i], true) == 0) {
                    res = 1; //!OCLINT parameter reassignment
                    return true;
                }
                if (parsedValue.compare(negative[i], true) == 0) {
                    res = 0; //!OCLINT parameter reassignment
                    return true;
                }
            }
        }
        return false;
    }
} // namespace

Context::Context(int argc, const char* const* argv)
        : p(new detail::ContextState) {
    parseArgs(argc, argv, true);
    if(argc)
        p->binary_name = argv[0];
}

Context::~Context() {
    if(g_cs == p)
        g_cs = nullptr;
    delete p;
}

void Context::applyCommandLine(int argc, const char* const* argv) {
    parseArgs(argc, argv);
    if(argc)
        p->binary_name = argv[0];
}

// parses args
void Context::parseArgs(int argc, const char* const* argv, bool withDefaults) {
    using namespace detail;

    // clang-format off
    parseCommaSepArgs(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "source-file=",        p->filters[0]);
    parseCommaSepArgs(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "sf=",                 p->filters[0]);
    parseCommaSepArgs(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "source-file-exclude=",p->filters[1]);
    parseCommaSepArgs(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "sfe=",                p->filters[1]);
    parseCommaSepArgs(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "test-suite=",         p->filters[2]);
    parseCommaSepArgs(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "ts=",                 p->filters[2]);
    parseCommaSepArgs(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "test-suite-exclude=", p->filters[3]);
    parseCommaSepArgs(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "tse=",                p->filters[3]);
    parseCommaSepArgs(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "test-case=",          p->filters[4]);
    parseCommaSepArgs(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "tc=",                 p->filters[4]);
    parseCommaSepArgs(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "test-case-exclude=",  p->filters[5]);
    parseCommaSepArgs(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "tce=",                p->filters[5]);
    parseCommaSepArgs(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "subcase=",            p->filters[6]);
    parseCommaSepArgs(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "sc=",                 p->filters[6]);
    parseCommaSepArgs(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "subcase-exclude=",    p->filters[7]);
    parseCommaSepArgs(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "sce=",                p->filters[7]);
    parseCommaSepArgs(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "reporters=",          p->filters[8]);
    parseCommaSepArgs(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "r=",                  p->filters[8]);
    // clang-format on

    int    intRes = 0;
    String strRes;

#define DOCTEST_PARSE_AS_BOOL_OR_FLAG(name, sname, var, default)                                   \
    if(parseIntOption(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX name "=", option_bool, intRes) ||  \
       parseIntOption(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX sname "=", option_bool, intRes))   \
        p->var = static_cast<bool>(intRes);                                                        \
    else if(parseFlag(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX name) ||                           \
            parseFlag(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX sname))                            \
        p->var = true;                                                                             \
    else if(withDefaults)                                                                          \
    p->var = default

#define DOCTEST_PARSE_INT_OPTION(name, sname, var, default)                                        \
    if(parseIntOption(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX name "=", option_int, intRes) ||   \
       parseIntOption(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX sname "=", option_int, intRes))    \
        p->var = intRes;                                                                           \
    else if(withDefaults)                                                                          \
    p->var = default

#define DOCTEST_PARSE_STR_OPTION(name, sname, var, default)                                        \
    if(parseOption(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX name "=", &strRes, default) ||        \
       parseOption(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX sname "=", &strRes, default) ||       \
       withDefaults)                                                                               \
    p->var = strRes

    // clang-format off
    DOCTEST_PARSE_STR_OPTION("out", "o", out, "");
    DOCTEST_PARSE_STR_OPTION("order-by", "ob", order_by, "file");
    DOCTEST_PARSE_INT_OPTION("rand-seed", "rs", rand_seed, 0);

    DOCTEST_PARSE_INT_OPTION("first", "f", first, 0);
    DOCTEST_PARSE_INT_OPTION("last", "l", last, UINT_MAX);

    DOCTEST_PARSE_INT_OPTION("abort-after", "aa", abort_after, 0);
    DOCTEST_PARSE_INT_OPTION("subcase-filter-levels", "scfl", subcase_filter_levels, INT_MAX);

    DOCTEST_PARSE_AS_BOOL_OR_FLAG("success", "s", success, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("case-sensitive", "cs", case_sensitive, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("exit", "e", exit, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("duration", "d", duration, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("minimal", "m", minimal, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("quiet", "q", quiet, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("no-throw", "nt", no_throw, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("no-exitcode", "ne", no_exitcode, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("no-run", "nr", no_run, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("no-intro", "ni", no_intro, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("no-version", "nv", no_version, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("no-colors", "nc", no_colors, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("force-colors", "fc", force_colors, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("no-breaks", "nb", no_breaks, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("no-skip", "ns", no_skip, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("gnu-file-line", "gfl", gnu_file_line, !bool(DOCTEST_MSVC));
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("no-path-filenames", "npf", no_path_in_filenames, false);
    DOCTEST_PARSE_STR_OPTION("strip-file-prefixes", "sfp", strip_file_prefixes, "");
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("no-line-numbers", "nln", no_line_numbers, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("no-debug-output", "ndo", no_debug_output, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("no-skipped-summary", "nss", no_skipped_summary, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("no-time-in-output", "ntio", no_time_in_output, false);
    // clang-format on

    if(withDefaults) {
        p->help             = false;
        p->version          = false;
        p->count            = false;
        p->list_test_cases  = false;
        p->list_test_suites = false;
        p->list_reporters   = false;
    }
    if(parseFlag(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "help") ||
       parseFlag(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "h") ||
       parseFlag(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "?")) {
        p->help = true;
        p->exit = true;
    }
    if(parseFlag(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "version") ||
       parseFlag(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "v")) {
        p->version = true;
        p->exit    = true;
    }
    if(parseFlag(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "count") ||
       parseFlag(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "c")) {
        p->count = true;
        p->exit  = true;
    }
    if(parseFlag(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "list-test-cases") ||
       parseFlag(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "ltc")) {
        p->list_test_cases = true;
        p->exit            = true;
    }
    if(parseFlag(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "list-test-suites") ||
       parseFlag(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "lts")) {
        p->list_test_suites = true;
        p->exit             = true;
    }
    if(parseFlag(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "list-reporters") ||
       parseFlag(argc, argv, DOCTEST_CONFIG_OPTIONS_PREFIX "lr")) {
        p->list_reporters = true;
        p->exit           = true;
    }
}

// allows the user to add procedurally to the filters from the command line
void Context::addFilter(const char* filter, const char* value) { setOption(filter, value); }

// allows the user to clear all filters from the command line
void Context::clearFilters() {
    for(auto& curr : p->filters)
        curr.clear();
}

// allows the user to override procedurally the bool options from the command line
void Context::setOption(const char* option, bool value) {
    setOption(option, value ? "true" : "false");
}

// allows the user to override procedurally the int options from the command line
void Context::setOption(const char* option, int value) {
    setOption(option, toString(value).c_str());
}

// allows the user to override procedurally the string options from the command line
void Context::setOption(const char* option, const char* value) {
    auto argv   = String("-") + option + "=" + value;
    auto lvalue = argv.c_str();
    parseArgs(1, &lvalue);
}

// users should query this in their main() and exit the program if true
bool Context::shouldExit() { return p->exit; }

void Context::setAsDefaultForAssertsOutOfTestCases() { g_cs = p; }

void Context::setAssertHandler(detail::assert_handler ah) { p->ah = ah; }

void Context::setCout(std::ostream* out) { p->cout = out; }

static class DiscardOStream : public std::ostream
{
private:
    class : public std::streambuf
    {
    private:
        // allowing some buffering decreases the amount of calls to overflow
        char buf[1024];

    protected:
        std::streamsize xsputn(const char_type*, std::streamsize count) override { return count; }

        int_type overflow(int_type ch) override {
            setp(std::begin(buf), std::end(buf));
            return traits_type::not_eof(ch);
        }
    } discardBuf;

public:
    DiscardOStream()
            : std::ostream(&discardBuf) {}
} discardOut;

// the main function that does all the filtering and test running
int Context::run() {
    using namespace detail;

    // save the old context state in case such was setup - for using asserts out of a testing context
    auto old_cs = g_cs;
    // this is the current contest
    g_cs               = p;
    is_running_in_test = true;

    g_no_colors = p->no_colors;
    p->resetRunData();

    std::fstream fstr;
    if(p->cout == nullptr) {
        if(p->quiet) {
            p->cout = &discardOut;
        } else if(p->out.size()) {
            // to a file if specified
            fstr.open(p->out.c_str(), std::fstream::out);
            p->cout = &fstr;
        } else {
#ifndef DOCTEST_CONFIG_NO_INCLUDE_IOSTREAM
            // stdout by default
            p->cout = &std::cout;
#else // DOCTEST_CONFIG_NO_INCLUDE_IOSTREAM
            return EXIT_FAILURE;
#endif // DOCTEST_CONFIG_NO_INCLUDE_IOSTREAM
        }
    }

    FatalConditionHandler::allocateAltStackMem();

    auto cleanup_and_return = [&]() {
        FatalConditionHandler::freeAltStackMem();

        if(fstr.is_open())
            fstr.close();

        // restore context
        g_cs               = old_cs;
        is_running_in_test = false;

        // we have to free the reporters which were allocated when the run started
        for(auto& curr : p->reporters_currently_used)
            delete curr;
        p->reporters_currently_used.clear();

        if(p->numTestCasesFailed && !p->no_exitcode)
            return EXIT_FAILURE;
        return EXIT_SUCCESS;
    };

    // setup default reporter if none is given through the command line
    if(p->filters[8].empty())
        p->filters[8].push_back("console");

    // check to see if any of the registered reporters has been selected
    for(auto& curr : getReporters()) {
        if(matchesAny(curr.first.second.c_str(), p->filters[8], false, p->case_sensitive))
            p->reporters_currently_used.push_back(curr.second(*g_cs));
    }

    // TODO: check if there is nothing in reporters_currently_used

    // prepend all listeners
    for(auto& curr : getListeners())
        p->reporters_currently_used.insert(p->reporters_currently_used.begin(), curr.second(*g_cs));

#ifdef DOCTEST_PLATFORM_WINDOWS
    if(isDebuggerActive() && p->no_debug_output == false)
        p->reporters_currently_used.push_back(new DebugOutputWindowReporter(*g_cs));
#endif // DOCTEST_PLATFORM_WINDOWS

    // handle version, help and no_run
    if(p->no_run || p->version || p->help || p->list_reporters) {
        DOCTEST_ITERATE_THROUGH_REPORTERS(report_query, QueryData());

        return cleanup_and_return();
    }

    std::vector<const TestCase*> testArray;
    for(auto& curr : getRegisteredTests())
        testArray.push_back(&curr);
    p->numTestCases = testArray.size();

    // sort the collected records
    if(!testArray.empty()) {
        if(p->order_by.compare("file", true) == 0) {
            std::sort(testArray.begin(), testArray.end(), fileOrderComparator);
        } else if(p->order_by.compare("suite", true) == 0) {
            std::sort(testArray.begin(), testArray.end(), suiteOrderComparator);
        } else if(p->order_by.compare("name", true) == 0) {
            std::sort(testArray.begin(), testArray.end(), nameOrderComparator);
        } else if(p->order_by.compare("rand", true) == 0) {
            std::srand(p->rand_seed);

            // random_shuffle implementation
            const auto first = &testArray[0];
            for(size_t i = testArray.size() - 1; i > 0; --i) {
                int idxToSwap = std::rand() % (i + 1);

                const auto temp = first[i];

                first[i]         = first[idxToSwap];
                first[idxToSwap] = temp;
            }
        } else if(p->order_by.compare("none", true) == 0) {
            // means no sorting - beneficial for death tests which call into the executable
            // with a specific test case in mind - we don't want to slow down the startup times
        }
    }

    std::set<String> testSuitesPassingFilt;

    bool                             query_mode = p->count || p->list_test_cases || p->list_test_suites;
    std::vector<const TestCaseData*> queryResults;

    if(!query_mode)
        DOCTEST_ITERATE_THROUGH_REPORTERS(test_run_start, DOCTEST_EMPTY);

    // invoke the registered functions if they match the filter criteria (or just count them)
    for(auto& curr : testArray) {
        const auto& tc = *curr;

        bool skip_me = false;
        if(tc.m_skip && !p->no_skip)
            skip_me = true;

        if(!matchesAny(tc.m_file.c_str(), p->filters[0], true, p->case_sensitive))
            skip_me = true;
        if(matchesAny(tc.m_file.c_str(), p->filters[1], false, p->case_sensitive))
            skip_me = true;
        if(!matchesAny(tc.m_test_suite, p->filters[2], true, p->case_sensitive))
            skip_me = true;
        if(matchesAny(tc.m_test_suite, p->filters[3], false, p->case_sensitive))
            skip_me = true;
        if(!matchesAny(tc.m_name, p->filters[4], true, p->case_sensitive))
            skip_me = true;
        if(matchesAny(tc.m_name, p->filters[5], false, p->case_sensitive))
            skip_me = true;

        if(!skip_me)
            p->numTestCasesPassingFilters++;

        // skip the test if it is not in the execution range
        if((p->last < p->numTestCasesPassingFilters && p->first <= p->last) ||
           (p->first > p->numTestCasesPassingFilters))
            skip_me = true;

        if(skip_me) {
            if(!query_mode)
                DOCTEST_ITERATE_THROUGH_REPORTERS(test_case_skipped, tc);
            continue;
        }

        // do not execute the test if we are to only count the number of filter passing tests
        if(p->count)
            continue;

        // print the name of the test and don't execute it
        if(p->list_test_cases) {
            queryResults.push_back(&tc);
            continue;
        }

        // print the name of the test suite if not done already and don't execute it
        if(p->list_test_suites) {
            if((testSuitesPassingFilt.count(tc.m_test_suite) == 0) && tc.m_test_suite[0] != '\0') {
                queryResults.push_back(&tc);
                testSuitesPassingFilt.insert(tc.m_test_suite);
                p->numTestSuitesPassingFilters++;
            }
            continue;
        }

        // execute the test if it passes all the filtering
        {
            p->currentTest = &tc;

            p->failure_flags = TestCaseFailureReason::None;
            p->seconds       = 0;

            // reset atomic counters
            p->numAssertsFailedCurrentTest_atomic = 0;
            p->numAssertsCurrentTest_atomic       = 0;

            p->fullyTraversedSubcases.clear();

            DOCTEST_ITERATE_THROUGH_REPORTERS(test_case_start, tc);

            p->timer.start();

            bool run_test = true;

            do {
                // reset some of the fields for subcases (except for the set of fully passed ones)
                p->reachedLeaf = false;
                // May not be empty if previous subcase exited via exception.
                p->subcaseStack.clear();
                p->currentSubcaseDepth = 0;

                p->shouldLogCurrentException = true;

                // reset stuff for logging with INFO()
                p->stringifiedContexts.clear();

#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
                try {
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS
// MSVC 2015 diagnoses fatalConditionHandler as unused (because reset() is a static method)
DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4101) // unreferenced local variable
                    FatalConditionHandler fatalConditionHandler; // Handle signals
                    // execute the test
                    tc.m_test();
                    fatalConditionHandler.reset();
DOCTEST_MSVC_SUPPRESS_WARNING_POP
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
                } catch(const TestFailureException&) {
                    p->failure_flags |= TestCaseFailureReason::AssertFailure;
                } catch(...) {
                    DOCTEST_ITERATE_THROUGH_REPORTERS(test_case_exception,
                                                      {translateActiveException(), false});
                    p->failure_flags |= TestCaseFailureReason::Exception;
                }
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS

                // exit this loop if enough assertions have failed - even if there are more subcases
                if(p->abort_after > 0 &&
                   p->numAssertsFailed + p->numAssertsFailedCurrentTest_atomic >= p->abort_after) {
                    run_test = false;
                    p->failure_flags |= TestCaseFailureReason::TooManyFailedAsserts;
                }

                if(!p->nextSubcaseStack.empty() && run_test)
                    DOCTEST_ITERATE_THROUGH_REPORTERS(test_case_reenter, tc);
                if(p->nextSubcaseStack.empty())
                    run_test = false;
            } while(run_test);

            p->finalizeTestCaseData();

            DOCTEST_ITERATE_THROUGH_REPORTERS(test_case_end, *g_cs);

            p->currentTest = nullptr;

            // stop executing tests if enough assertions have failed
            if(p->abort_after > 0 && p->numAssertsFailed >= p->abort_after)
                break;
        }
    }

    if(!query_mode) {
        DOCTEST_ITERATE_THROUGH_REPORTERS(test_run_end, *g_cs);
    } else {
        QueryData qdata;
        qdata.run_stats = g_cs;
        qdata.data      = queryResults.data();
        qdata.num_data  = unsigned(queryResults.size());
        DOCTEST_ITERATE_THROUGH_REPORTERS(report_query, qdata);
    }

    return cleanup_and_return();
}
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

#ifdef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4007) // 'function' : must be 'attribute' - see issue #182
int main(int argc, char** argv) { return doctest::Context(argc, argv).run(); }
DOCTEST_MSVC_SUPPRESS_WARNING_POP
#endif // DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
