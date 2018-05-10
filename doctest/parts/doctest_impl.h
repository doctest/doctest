#if defined(DOCTEST_CONFIG_IMPLEMENT) || !defined(DOCTEST_SINGLE_HEADER)
#ifndef DOCTEST_LIBRARY_IMPLEMENTATION
#define DOCTEST_LIBRARY_IMPLEMENTATION

#ifndef DOCTEST_SINGLE_HEADER
#include "doctest_fwd.h"
#endif // DOCTEST_SINGLE_HEADER

DOCTEST_CLANG_SUPPRESS_WARNING_PUSH
DOCTEST_CLANG_SUPPRESS_WARNING("-Wunknown-pragmas")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wpadded")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wglobal-constructors")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wexit-time-destructors")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wmissing-prototypes")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wsign-conversion")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wshorten-64-to-32")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wmissing-variable-declarations")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wswitch")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wswitch-enum")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wcovered-switch-default")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wmissing-noreturn")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wunused-local-typedef")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wdisabled-macro-expansion")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wmissing-braces")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wmissing-field-initializers")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wc++11-long-long")
#if DOCTEST_CLANG && DOCTEST_CLANG_HAS_WARNING("-Wzero-as-null-pointer-constant")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wzero-as-null-pointer-constant")
#endif // clang - 0 as null

DOCTEST_GCC_SUPPRESS_WARNING_PUSH
DOCTEST_GCC_SUPPRESS_WARNING("-Wunknown-pragmas")
DOCTEST_GCC_SUPPRESS_WARNING("-Wpragmas")
DOCTEST_GCC_SUPPRESS_WARNING("-Wconversion")
DOCTEST_GCC_SUPPRESS_WARNING("-Weffc++")
DOCTEST_GCC_SUPPRESS_WARNING("-Wsign-conversion")
DOCTEST_GCC_SUPPRESS_WARNING("-Wstrict-overflow")
DOCTEST_GCC_SUPPRESS_WARNING("-Wmissing-field-initializers")
DOCTEST_GCC_SUPPRESS_WARNING("-Wmissing-braces")
DOCTEST_GCC_SUPPRESS_WARNING("-Wmissing-declarations")
DOCTEST_GCC_SUPPRESS_WARNING("-Winline")
DOCTEST_GCC_SUPPRESS_WARNING("-Wswitch")
DOCTEST_GCC_SUPPRESS_WARNING("-Wswitch-enum")
DOCTEST_GCC_SUPPRESS_WARNING("-Wswitch-default")
DOCTEST_GCC_SUPPRESS_WARNING("-Wunsafe-loop-optimizations")
DOCTEST_GCC_SUPPRESS_WARNING("-Wlong-long")
DOCTEST_GCC_SUPPRESS_WARNING("-Wold-style-cast")
DOCTEST_GCC_SUPPRESS_WARNING("-Wzero-as-null-pointer-constant")
DOCTEST_GCC_SUPPRESS_WARNING("-Wunused-local-typedefs")
DOCTEST_GCC_SUPPRESS_WARNING("-Wuseless-cast")

DOCTEST_MSVC_SUPPRESS_WARNING_PUSH
DOCTEST_MSVC_SUPPRESS_WARNING(4616) // invalid compiler warning
DOCTEST_MSVC_SUPPRESS_WARNING(4619) // invalid compiler warning
DOCTEST_MSVC_SUPPRESS_WARNING(4996) // The compiler encountered a deprecated declaration
DOCTEST_MSVC_SUPPRESS_WARNING(4267) // 'var' : conversion from 'x' to 'y', possible loss of data
DOCTEST_MSVC_SUPPRESS_WARNING(4706) // assignment within conditional expression
DOCTEST_MSVC_SUPPRESS_WARNING(4512) // 'class' : assignment operator could not be generated
DOCTEST_MSVC_SUPPRESS_WARNING(4127) // conditional expression is constant
DOCTEST_MSVC_SUPPRESS_WARNING(4530) // C++ exception handler used, but unwind semantics not enabled
DOCTEST_MSVC_SUPPRESS_WARNING(4577) // 'noexcept' used with no exception handling mode specified
DOCTEST_MSVC_SUPPRESS_WARNING(4774) // format string expected in argument is not a string literal
DOCTEST_MSVC_SUPPRESS_WARNING(4365) // conversion from 'int' to 'unsigned', signed/unsigned mismatch
DOCTEST_MSVC_SUPPRESS_WARNING(4820) // padding in structs
DOCTEST_MSVC_SUPPRESS_WARNING(4640) // construction of local static object is not thread-safe
DOCTEST_MSVC_SUPPRESS_WARNING(5039) // pointer to potentially throwing function passed to extern C
DOCTEST_MSVC_SUPPRESS_WARNING(5045) // Spectre mitigation stuff

#if defined(DOCTEST_NO_CPP11_COMPAT)
DOCTEST_CLANG_SUPPRESS_WARNING("-Wc++98-compat")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wc++98-compat-pedantic")
#endif // DOCTEST_NO_CPP11_COMPAT

#define DOCTEST_LOG_START(s)                                                                       \
    do {                                                                                           \
        if(!contextState->hasLoggedCurrentTestStart) {                                             \
            logTestStart(s, *contextState->currentTest);                                           \
            DOCTEST_PRINT_TO_OUTPUT_WINDOW_IN_IDE_BEGIN;                                           \
            logTestStart(oss, *contextState->currentTest);                                         \
            DOCTEST_PRINT_TO_OUTPUT_WINDOW_IN_IDE_END;                                             \
            contextState->hasLoggedCurrentTestStart = true;                                        \
        }                                                                                          \
    } while(false)

#define DOCTEST_PRINT_TO_OUTPUT_WINDOW_IN_IDE_BEGIN                                                \
    if(isDebuggerActive()) {                                                                       \
        ContextState* p_cs     = contextState;                                                     \
        bool          with_col = p_cs->no_colors;                                                  \
        p_cs->no_colors        = false;                                                            \
    std::ostringstream oss

#define DOCTEST_PRINT_TO_OUTPUT_WINDOW_IN_IDE_END                                                  \
    printToDebugConsole(oss.str().c_str());                                                        \
    p_cs->no_colors = with_col;                                                                    \
    }                                                                                              \
    ((void)0)

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN

// required includes - will go only in one translation unit!
#include <ctime>
#include <cmath>
// borland (Embarcadero) compiler requires math.h and not cmath - https://github.com/onqtam/doctest/pull/37
#ifdef __BORLANDC__
#include <math.h>
#endif // __BORLANDC__
#include <new>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <utility>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <set>
#include <exception>
#include <stdexcept>
#include <csignal>
#include <cfloat>
#include <cctype>
#if !DOCTEST_MSVC
#include <stdint.h>
#endif // !MSVC

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

namespace doctest
{
namespace detail
{
    // case insensitive strcmp
    int stricmp(char const* a, char const* b) {
        for(;; a++, b++) {
            const int d = tolower(*a) - tolower(*b);
            if(d != 0 || !*a)
                return d;
        }
    }

    void my_memcpy(void* dest, const void* src, unsigned num) { memcpy(dest, src, num); }

    template <typename T>
    String fpToString(T value, int precision) {
        std::ostringstream oss;
        oss << std::setprecision(precision) << std::fixed << value;
        std::string d = oss.str();
        size_t      i = d.find_last_not_of('0');
        if(i != std::string::npos && i != d.size() - 1) {
            if(d[i] == '.')
                i++;
            d = d.substr(0, i + 1);
        }
        return d.c_str();
    }

    struct Endianness
    {
        enum Arch
        {
            Big,
            Little
        };

        static Arch which() {
            union _
            {
                int  asInt;
                char asChar[sizeof(int)];
            } u;

            u.asInt = 1;                                            // NOLINT
            return (u.asChar[sizeof(int) - 1] == 1) ? Big : Little; // NOLINT
        }
    };

    String rawMemoryToString(const void* object, unsigned size) {
        // Reverse order for little endian architectures
        int i = 0, end = static_cast<int>(size), inc = 1;
        if(Endianness::which() == Endianness::Little) {
            i   = end - 1;
            end = inc = -1;
        }

        unsigned char const* bytes = static_cast<unsigned char const*>(object);
        std::ostringstream   oss;
        oss << "0x" << std::setfill('0') << std::hex;
        for(; i != end; i += inc)
            oss << std::setw(2) << static_cast<unsigned>(bytes[i]);
        return oss.str().c_str();
    }

    std::ostream* createStream() { return new std::ostringstream(); }
    String        getStreamResult(std::ostream* s) {
        return static_cast<std::ostringstream*>(s)->str().c_str(); // NOLINT
    }
    void freeStream(std::ostream* s) { delete s; }

#ifndef DOCTEST_CONFIG_DISABLE

    // this holds both parameters for the command line and runtime data for tests
    struct ContextState : TestAccessibleContextState //!OCLINT too many fields
    {
        // == parameters from the command line

        std::vector<std::vector<String> > filters;

        String   order_by;  // how tests should be ordered
        unsigned rand_seed; // the seed for rand ordering

        unsigned first; // the first (matching) test to be executed
        unsigned last;  // the last (matching) test to be executed

        int  abort_after;           // stop tests after this many failed assertions
        int  subcase_filter_levels; // apply the subcase filters for the first N levels
        bool case_sensitive;        // if filtering should be case sensitive
        bool exit;          // if the program should be exited after the tests are ran/whatever
        bool duration;      // print the time duration of each test case
        bool no_exitcode;   // if the framework should return 0 as the exitcode
        bool no_run;        // to not run the tests at all (can be done with an "*" exclude)
        bool no_version;    // to not print the version of the framework
        bool no_colors;     // if output to the console should be colorized
        bool force_colors;  // forces the use of colors even when a tty cannot be detected
        bool no_breaks;     // to not break into the debugger
        bool no_skip;       // don't skip test cases which are marked to be skipped
        bool gnu_file_line; // if line numbers should be surrounded with :x: and not (x):
        bool no_path_in_filenames; // if the path to files should be removed from the output
        bool no_line_numbers;      // if source code line numbers should be omitted from the output
        bool no_skipped_summary;   // don't print "skipped" in the summary !!! UNDOCUMENTED !!!

        bool help;             // to print the help
        bool version;          // to print the version
        bool count;            // if only the count of matching tests is to be retreived
        bool list_test_cases;  // to list all tests matching the filters
        bool list_test_suites; // to list all suites matching the filters

        // == data for the tests being ran

        unsigned        numTestsPassingFilters;
        unsigned        numTestSuitesPassingFilters;
        unsigned        numFailed;
        const TestCase* currentTest;
        bool            hasLoggedCurrentTestStart;
        int             numAssertionsForCurrentTestcase;
        int             numAssertions;
        int             numFailedAssertionsForCurrentTestcase;
        int             numFailedAssertions;
        bool            hasCurrentTestFailed;

        std::vector<IContextScope*> contexts;            // for logging with INFO() and friends
        std::vector<std::string>    exceptionalContexts; // logging from INFO() due to an exception

        // stuff for subcases
        std::set<SubcaseSignature> subcasesPassed;
        std::set<int>              subcasesEnteredLevels;
        std::vector<Subcase>       subcasesStack;
        int                        subcasesCurrentLevel;
        bool                       subcasesHasSkipped;

        void resetRunData() {
            numTestsPassingFilters                = 0;
            numTestSuitesPassingFilters           = 0;
            numFailed                             = 0;
            numAssertions                         = 0;
            numFailedAssertions                   = 0;
            numFailedAssertionsForCurrentTestcase = 0;
        }

        // cppcheck-suppress uninitMemberVar
        ContextState()
                : filters(8) // 8 different filters total
        {
            resetRunData();
        }
    };

    ContextState* contextState = 0;
#endif // DOCTEST_CONFIG_DISABLE
} // namespace detail

void String::copy(const String& other) {
    if(other.isOnStack()) {
        memcpy(buf, other.buf, len);
    } else {
        setOnHeap();
        data.size     = other.data.size;
        data.capacity = data.size + 1;
        data.ptr      = new char[data.capacity];
        memcpy(data.ptr, other.data.ptr, data.size + 1);
    }
}

String::String(const char* in) {
    unsigned in_len = strlen(in);
    if(in_len <= last) {
        memcpy(buf, in, in_len + 1);
        setLast(last - in_len);
    } else {
        setOnHeap();
        data.size     = in_len;
        data.capacity = data.size + 1;
        data.ptr      = new char[data.capacity];
        memcpy(data.ptr, in, in_len + 1);
    }
}

String& String::operator+=(const String& other) {
    const unsigned my_old_size = size();
    const unsigned other_size  = other.size();
    const unsigned total_size  = my_old_size + other_size;
    if(isOnStack()) {
        if(total_size < len) {
            // append to the current stack space
            memcpy(buf + my_old_size, other.c_str(), other_size + 1);
            setLast(last - total_size);
        } else {
            // alloc new chunk
            char* temp = new char[total_size + 1];
            // copy current data to new location before writing in the union
            memcpy(temp, buf, my_old_size); // skip the +1 ('\0') for speed
            // update data in union
            setOnHeap();
            data.size     = total_size;
            data.capacity = data.size + 1;
            data.ptr      = temp;
            // transfer the rest of the data
            memcpy(data.ptr + my_old_size, other.c_str(), other_size + 1);
        }
    } else {
        if(data.capacity > total_size) {
            // append to the current heap block
            data.size = total_size;
            memcpy(data.ptr + my_old_size, other.c_str(), other_size + 1);
        } else {
            // resize
            data.capacity *= 2;
            if(data.capacity <= total_size)
                data.capacity = total_size + 1;
            // alloc new chunk
            char* temp = new char[data.capacity];
            // copy current data to new location before releasing it
            memcpy(temp, data.ptr, my_old_size); // skip the +1 ('\0') for speed
            // release old chunk
            delete[] data.ptr;
            // update the rest of the union members
            data.size = total_size;
            data.ptr  = temp;
            // transfer the rest of the data
            memcpy(data.ptr + my_old_size, other.c_str(), other_size + 1);
        }
    }

    return *this;
}

#ifdef DOCTEST_CONFIG_WITH_RVALUE_REFERENCES
String::String(String&& other) {
    memcpy(buf, other.buf, len);
    other.buf[0] = '\0';
    other.setLast();
}

String& String::operator=(String&& other) {
    if(this != &other) {
        if(!isOnStack())
            delete[] data.ptr;
        memcpy(buf, other.buf, len);
        other.buf[0] = '\0';
        other.setLast();
    }
    return *this;
}
#endif // DOCTEST_CONFIG_WITH_RVALUE_REFERENCES

int String::compare(const char* other, bool no_case) const {
    if(no_case)
        return detail::stricmp(c_str(), other);
    return std::strcmp(c_str(), other);
}

int String::compare(const String& other, bool no_case) const {
    return compare(other.c_str(), no_case);
}

std::ostream& operator<<(std::ostream& s, const String& in) { return s << in.c_str(); }

Approx::Approx(double value)
        : m_epsilon(static_cast<double>(std::numeric_limits<float>::epsilon()) * 100)
        , m_scale(1.0)
        , m_value(value) {}

bool operator==(double lhs, Approx const& rhs) {
    // Thanks to Richard Harris for his help refining this formula
    return std::fabs(lhs - rhs.m_value) <
           rhs.m_epsilon * (rhs.m_scale + std::max(std::fabs(lhs), std::fabs(rhs.m_value)));
}

String Approx::toString() const { return String("Approx( ") + doctest::toString(m_value) + " )"; }

#ifdef DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
String toString(char* in) { return toString(static_cast<const char*>(in)); }
String toString(const char* in) { return String("\"") + (in ? in : "{null string}") + "\""; }
#endif // DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
String toString(bool in) { return in ? "true" : "false"; }
String toString(float in) { return detail::fpToString(in, 5) + "f"; }
String toString(double in) { return detail::fpToString(in, 10); }
String toString(double long in) { return detail::fpToString(in, 15); }

String toString(char in) {
    char buf[64];
    std::sprintf(buf, "%d", in);
    return buf;
}

String toString(char signed in) {
    char buf[64];
    std::sprintf(buf, "%d", in);
    return buf;
}

String toString(char unsigned in) {
    char buf[64];
    std::sprintf(buf, "%ud", in);
    return buf;
}

String toString(int short in) {
    char buf[64];
    std::sprintf(buf, "%d", in);
    return buf;
}

String toString(int short unsigned in) {
    char buf[64];
    std::sprintf(buf, "%u", in);
    return buf;
}

String toString(int in) {
    char buf[64];
    std::sprintf(buf, "%d", in);
    return buf;
}

String toString(int unsigned in) {
    char buf[64];
    std::sprintf(buf, "%u", in);
    return buf;
}

String toString(int long in) {
    char buf[64];
    std::sprintf(buf, "%ld", in);
    return buf;
}

String toString(int long unsigned in) {
    char buf[64];
    std::sprintf(buf, "%lu", in);
    return buf;
}

#ifdef DOCTEST_CONFIG_WITH_LONG_LONG
String toString(int long long in) {
    char buf[64];
    std::sprintf(buf, "%lld", in);
    return buf;
}
String toString(int long long unsigned in) {
    char buf[64];
    std::sprintf(buf, "%llu", in);
    return buf;
}
#endif // DOCTEST_CONFIG_WITH_LONG_LONG

#ifdef DOCTEST_CONFIG_WITH_NULLPTR
String toString(std::nullptr_t) { return "nullptr"; }
#endif // DOCTEST_CONFIG_WITH_NULLPTR

} // namespace doctest

#ifdef DOCTEST_CONFIG_DISABLE
namespace doctest
{
bool isRunningInTest() { return false; }
Context::Context(int, const char* const*) {}
Context::~Context() {}
void Context::applyCommandLine(int, const char* const*) {}
void Context::addFilter(const char*, const char*) {}
void Context::clearFilters() {}
void Context::setOption(const char*, int) {}
void Context::setOption(const char*, const char*) {}
bool Context::shouldExit() { return false; }
int  Context::run() { return 0; }
} // namespace doctest
#else // DOCTEST_CONFIG_DISABLE

#if !defined(DOCTEST_CONFIG_COLORS_NONE)
#if !defined(DOCTEST_CONFIG_COLORS_WINDOWS) && !defined(DOCTEST_CONFIG_COLORS_ANSI)
#ifdef DOCTEST_PLATFORM_WINDOWS
#define DOCTEST_CONFIG_COLORS_WINDOWS
#else // linux
#define DOCTEST_CONFIG_COLORS_ANSI
#endif // platform
#endif // DOCTEST_CONFIG_COLORS_WINDOWS && DOCTEST_CONFIG_COLORS_ANSI
#endif // DOCTEST_CONFIG_COLORS_NONE

#if DOCTEST_MSVC || defined(__MINGW32__)
#if DOCTEST_MSVC >= DOCTEST_COMPILER(17, 0, 0)
#define DOCTEST_WINDOWS_SAL_IN_OPT _In_opt_
#else // MSVC
#define DOCTEST_WINDOWS_SAL_IN_OPT
#endif // MSVC
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(
        DOCTEST_WINDOWS_SAL_IN_OPT const char*);
extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent();
#endif // MSVC || __MINGW32__

#ifdef DOCTEST_CONFIG_COLORS_ANSI
#include <unistd.h>
#endif // DOCTEST_CONFIG_COLORS_ANSI

#ifdef DOCTEST_PLATFORM_WINDOWS

// defines for a leaner windows.h
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN
#ifndef VC_EXTRA_LEAN
#define VC_EXTRA_LEAN
#endif // VC_EXTRA_LEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN

// not sure what AfxWin.h is for - here I do what Catch does
#ifdef __AFXDLL
#include <AfxWin.h>
#else
#include <windows.h>
#endif
#include <io.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

#else // DOCTEST_PLATFORM_WINDOWS

#include <sys/time.h>

#endif // DOCTEST_PLATFORM_WINDOWS

namespace doctest_detail_test_suite_ns
{
// holds the current test suite
doctest::detail::TestSuite& getCurrentTestSuite() {
    static doctest::detail::TestSuite data;
    return data;
}
} // namespace doctest_detail_test_suite_ns

namespace doctest
{
namespace detail
{
    TestCase::TestCase(funcType test, const char* file, unsigned line, const TestSuite& test_suite,
                       const char* type, int template_id)
            : m_test(test)
            , m_name(0)
            , m_type(type)
            , m_test_suite(test_suite.m_test_suite)
            , m_description(test_suite.m_description)
            , m_skip(test_suite.m_skip)
            , m_may_fail(test_suite.m_may_fail)
            , m_should_fail(test_suite.m_should_fail)
            , m_expected_failures(test_suite.m_expected_failures)
            , m_timeout(test_suite.m_timeout)
            , m_file(file)
            , m_line(line)
            , m_template_id(template_id) {}

    TestCase& TestCase::operator*(const char* in) {
        m_name = in;
        // make a new name with an appended type for templated test case
        if(m_template_id != -1) {
            m_full_name = String(m_name) + m_type;
            // redirect the name to point to the newly constructed full name
            m_name = m_full_name.c_str();
        }
        return *this;
    }

    TestCase& TestCase::operator=(const TestCase& other) {
        m_test              = other.m_test;
        m_full_name         = other.m_full_name;
        m_name              = other.m_name;
        m_type              = other.m_type;
        m_test_suite        = other.m_test_suite;
        m_description       = other.m_description;
        m_skip              = other.m_skip;
        m_may_fail          = other.m_may_fail;
        m_should_fail       = other.m_should_fail;
        m_expected_failures = other.m_expected_failures;
        m_timeout           = other.m_timeout;
        m_file              = other.m_file;
        m_line              = other.m_line;
        m_template_id       = other.m_template_id;

        if(m_template_id != -1)
            m_name = m_full_name.c_str();
        return *this;
    }

    bool TestCase::operator<(const TestCase& other) const {
        if(m_line != other.m_line)
            return m_line < other.m_line;
        const int file_cmp = std::strcmp(m_file, other.m_file);
        if(file_cmp != 0)
            return file_cmp < 0;
        return m_template_id < other.m_template_id;
    }

    const char* assertString(assertType::Enum val) {
        DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(
                4062) // enumerator 'x' in switch of enum 'y' is not handled
        switch(val) { //!OCLINT missing default in switch statements
            // clang-format off
            case assertType::DT_WARN                    : return "WARN";
            case assertType::DT_CHECK                   : return "CHECK";
            case assertType::DT_REQUIRE                 : return "REQUIRE";

            case assertType::DT_WARN_FALSE              : return "WARN_FALSE";
            case assertType::DT_CHECK_FALSE             : return "CHECK_FALSE";
            case assertType::DT_REQUIRE_FALSE           : return "REQUIRE_FALSE";

            case assertType::DT_WARN_THROWS             : return "WARN_THROWS";
            case assertType::DT_CHECK_THROWS            : return "CHECK_THROWS";
            case assertType::DT_REQUIRE_THROWS          : return "REQUIRE_THROWS";

            case assertType::DT_WARN_THROWS_AS          : return "WARN_THROWS_AS";
            case assertType::DT_CHECK_THROWS_AS         : return "CHECK_THROWS_AS";
            case assertType::DT_REQUIRE_THROWS_AS       : return "REQUIRE_THROWS_AS";

            case assertType::DT_WARN_NOTHROW            : return "WARN_NOTHROW";
            case assertType::DT_CHECK_NOTHROW           : return "CHECK_NOTHROW";
            case assertType::DT_REQUIRE_NOTHROW         : return "REQUIRE_NOTHROW";

            case assertType::DT_WARN_EQ                 : return "WARN_EQ";
            case assertType::DT_CHECK_EQ                : return "CHECK_EQ";
            case assertType::DT_REQUIRE_EQ              : return "REQUIRE_EQ";
            case assertType::DT_WARN_NE                 : return "WARN_NE";
            case assertType::DT_CHECK_NE                : return "CHECK_NE";
            case assertType::DT_REQUIRE_NE              : return "REQUIRE_NE";
            case assertType::DT_WARN_GT                 : return "WARN_GT";
            case assertType::DT_CHECK_GT                : return "CHECK_GT";
            case assertType::DT_REQUIRE_GT              : return "REQUIRE_GT";
            case assertType::DT_WARN_LT                 : return "WARN_LT";
            case assertType::DT_CHECK_LT                : return "CHECK_LT";
            case assertType::DT_REQUIRE_LT              : return "REQUIRE_LT";
            case assertType::DT_WARN_GE                 : return "WARN_GE";
            case assertType::DT_CHECK_GE                : return "CHECK_GE";
            case assertType::DT_REQUIRE_GE              : return "REQUIRE_GE";
            case assertType::DT_WARN_LE                 : return "WARN_LE";
            case assertType::DT_CHECK_LE                : return "CHECK_LE";
            case assertType::DT_REQUIRE_LE              : return "REQUIRE_LE";

            case assertType::DT_WARN_UNARY              : return "WARN_UNARY";
            case assertType::DT_CHECK_UNARY             : return "CHECK_UNARY";
            case assertType::DT_REQUIRE_UNARY           : return "REQUIRE_UNARY";
            case assertType::DT_WARN_UNARY_FALSE        : return "WARN_UNARY_FALSE";
            case assertType::DT_CHECK_UNARY_FALSE       : return "CHECK_UNARY_FALSE";
            case assertType::DT_REQUIRE_UNARY_FALSE     : return "REQUIRE_UNARY_FALSE";

            case assertType::DT_FAST_WARN_EQ            : return "FAST_WARN_EQ";
            case assertType::DT_FAST_CHECK_EQ           : return "FAST_CHECK_EQ";
            case assertType::DT_FAST_REQUIRE_EQ         : return "FAST_REQUIRE_EQ";
            case assertType::DT_FAST_WARN_NE            : return "FAST_WARN_NE";
            case assertType::DT_FAST_CHECK_NE           : return "FAST_CHECK_NE";
            case assertType::DT_FAST_REQUIRE_NE         : return "FAST_REQUIRE_NE";
            case assertType::DT_FAST_WARN_GT            : return "FAST_WARN_GT";
            case assertType::DT_FAST_CHECK_GT           : return "FAST_CHECK_GT";
            case assertType::DT_FAST_REQUIRE_GT         : return "FAST_REQUIRE_GT";
            case assertType::DT_FAST_WARN_LT            : return "FAST_WARN_LT";
            case assertType::DT_FAST_CHECK_LT           : return "FAST_CHECK_LT";
            case assertType::DT_FAST_REQUIRE_LT         : return "FAST_REQUIRE_LT";
            case assertType::DT_FAST_WARN_GE            : return "FAST_WARN_GE";
            case assertType::DT_FAST_CHECK_GE           : return "FAST_CHECK_GE";
            case assertType::DT_FAST_REQUIRE_GE         : return "FAST_REQUIRE_GE";
            case assertType::DT_FAST_WARN_LE            : return "FAST_WARN_LE";
            case assertType::DT_FAST_CHECK_LE           : return "FAST_CHECK_LE";
            case assertType::DT_FAST_REQUIRE_LE         : return "FAST_REQUIRE_LE";

            case assertType::DT_FAST_WARN_UNARY         : return "FAST_WARN_UNARY";
            case assertType::DT_FAST_CHECK_UNARY        : return "FAST_CHECK_UNARY";
            case assertType::DT_FAST_REQUIRE_UNARY      : return "FAST_REQUIRE_UNARY";
            case assertType::DT_FAST_WARN_UNARY_FALSE   : return "FAST_WARN_UNARY_FALSE";
            case assertType::DT_FAST_CHECK_UNARY_FALSE  : return "FAST_CHECK_UNARY_FALSE";
            case assertType::DT_FAST_REQUIRE_UNARY_FALSE: return "FAST_REQUIRE_UNARY_FALSE";
                // clang-format on
        }
        DOCTEST_MSVC_SUPPRESS_WARNING_POP
        return "";
    }

    bool checkIfShouldThrow(assertType::Enum at) {
        if(at & assertType::is_require) //!OCLINT bitwise operator in conditional
            return true;

        if((at & assertType::is_check) //!OCLINT bitwise operator in conditional
           && contextState->abort_after > 0 &&
           contextState->numFailedAssertions >= contextState->abort_after)
            return true;

        return false;
    }
    void throwException() {
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
        throw TestFailureException();
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS
    }
    void fastAssertThrowIfFlagSet(int flags) {
        if(flags & assertAction::shouldthrow) //!OCLINT bitwise operator in conditional
            throwException();
    }

    // matching of a string against a wildcard mask (case sensitivity configurable) taken from
    // https://www.codeproject.com/Articles/1088/Wildcard-string-compare-globbing
    int wildcmp(const char* str, const char* wild, bool caseSensitive) {
        const char* cp = 0;
        const char* mp = 0;

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

    //// C string hash function (djb2) - taken from http://www.cse.yorku.ca/~oz/hash.html
    //unsigned hashStr(unsigned const char* str) {
    //    unsigned long hash = 5381;
    //    char          c;
    //    while((c = *str++))
    //        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    //    return hash;
    //}

    // checks if the name matches any of the filters (and can be configured what to do when empty)
    bool matchesAny(const char* name, const std::vector<String>& filters, int matchEmpty,
                    bool caseSensitive) {
        if(filters.empty() && matchEmpty)
            return true;
        for(unsigned i = 0; i < filters.size(); ++i)
            if(wildcmp(name, filters[i].c_str(), caseSensitive))
                return true;
        return false;
    }

#ifdef DOCTEST_PLATFORM_WINDOWS

    typedef unsigned long long UInt64;

    UInt64 getCurrentTicks() {
        static UInt64 hz = 0, hzo = 0;
        if(!hz) {
            QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&hz));
            QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&hzo));
        }
        UInt64 t;
        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&t));
        return ((t - hzo) * 1000000) / hz;
    }
#else  // DOCTEST_PLATFORM_WINDOWS

    typedef uint64_t UInt64;

    UInt64 getCurrentTicks() {
        timeval t;
        gettimeofday(&t, 0);
        return static_cast<UInt64>(t.tv_sec) * 1000000 + static_cast<UInt64>(t.tv_usec);
    }
#endif // DOCTEST_PLATFORM_WINDOWS

    class Timer
    {
    public:
        Timer()
                : m_ticks(0) {}
        void         start() { m_ticks = getCurrentTicks(); }
        unsigned int getElapsedMicroseconds() const {
            return static_cast<unsigned int>(getCurrentTicks() - m_ticks);
        }
        unsigned int getElapsedMilliseconds() const {
            return static_cast<unsigned int>(getElapsedMicroseconds() / 1000);
        }
        double getElapsedSeconds() const { return getElapsedMicroseconds() / 1000000.0; }

    private:
        UInt64 m_ticks;
    };

    TestAccessibleContextState* getTestsContextState() { return contextState; }

    // TODO: remove this from here
    void logTestEnd();

    bool SubcaseSignature::operator<(const SubcaseSignature& other) const {
        if(m_line != other.m_line)
            return m_line < other.m_line;
        if(std::strcmp(m_file, other.m_file) != 0)
            return std::strcmp(m_file, other.m_file) < 0;
        return std::strcmp(m_name, other.m_name) < 0;
    }

    Subcase::Subcase(const char* name, const char* file, int line)
            : m_signature(name, file, line)
            , m_entered(false) {
        ContextState* s = contextState;

        // if we have already completed it
        if(s->subcasesPassed.count(m_signature) != 0)
            return;

        // check subcase filters
        if(s->subcasesCurrentLevel < s->subcase_filter_levels) {
            if(!matchesAny(m_signature.m_name, s->filters[6], 1, s->case_sensitive))
                return;
            if(matchesAny(m_signature.m_name, s->filters[7], 0, s->case_sensitive))
                return;
        }

        // if a Subcase on the same level has already been entered
        if(s->subcasesEnteredLevels.count(s->subcasesCurrentLevel) != 0) {
            s->subcasesHasSkipped = true;
            return;
        }

        s->subcasesStack.push_back(*this);
        if(s->hasLoggedCurrentTestStart)
            logTestEnd();
        s->hasLoggedCurrentTestStart = false;

        s->subcasesEnteredLevels.insert(s->subcasesCurrentLevel++);
        m_entered = true;
    }

    Subcase::Subcase(const Subcase& other)
            : m_signature(other.m_signature.m_name, other.m_signature.m_file,
                          other.m_signature.m_line)
            , m_entered(other.m_entered) {}

    Subcase::~Subcase() {
        if(m_entered) {
            ContextState* s = contextState;

            s->subcasesCurrentLevel--;
            // only mark the subcase as passed if no subcases have been skipped
            if(s->subcasesHasSkipped == false)
                s->subcasesPassed.insert(m_signature);

            if(!s->subcasesStack.empty())
                s->subcasesStack.pop_back();
            if(s->hasLoggedCurrentTestStart)
                logTestEnd();
            s->hasLoggedCurrentTestStart = false;
        }
    }

    Result::~Result() {}

    Result& Result::operator=(const Result& other) {
        m_passed        = other.m_passed;
        m_decomposition = other.m_decomposition;

        return *this;
    }

    // for sorting tests by file/line
    int fileOrderComparator(const void* a, const void* b) {
        const TestCase* lhs = *static_cast<TestCase* const*>(a);
        const TestCase* rhs = *static_cast<TestCase* const*>(b);
#if DOCTEST_MSVC
        // this is needed because MSVC gives different case for drive letters
        // for __FILE__ when evaluated in a header and a source file
        const int res = stricmp(lhs->m_file, rhs->m_file);
#else  // MSVC
        const int res = std::strcmp(lhs->m_file, rhs->m_file);
#endif // MSVC
        if(res != 0)
            return res;
        return static_cast<int>(lhs->m_line - rhs->m_line);
    }

    // for sorting tests by suite/file/line
    int suiteOrderComparator(const void* a, const void* b) {
        const TestCase* lhs = *static_cast<TestCase* const*>(a);
        const TestCase* rhs = *static_cast<TestCase* const*>(b);

        const int res = std::strcmp(lhs->m_test_suite, rhs->m_test_suite);
        if(res != 0)
            return res;
        return fileOrderComparator(a, b);
    }

    // for sorting tests by name/suite/file/line
    int nameOrderComparator(const void* a, const void* b) {
        const TestCase* lhs = *static_cast<TestCase* const*>(a);
        const TestCase* rhs = *static_cast<TestCase* const*>(b);

        const int res_name = std::strcmp(lhs->m_name, rhs->m_name);
        if(res_name != 0)
            return res_name;
        return suiteOrderComparator(a, b);
    }

    // sets the current test suite
    int setTestSuite(const TestSuite& ts) {
        doctest_detail_test_suite_ns::getCurrentTestSuite() = ts;
        return 0;
    }

    // all the registered tests
    std::set<TestCase>& getRegisteredTests() {
        static std::set<TestCase> data;
        return data;
    }

    // used by the macros for registering tests
    int regTest(const TestCase& tc) {
        getRegisteredTests().insert(tc);
        return 0;
    }

    namespace Color
    {
        enum Code
        {
            None = 0,
            White,
            Red,
            Green,
            Blue,
            Cyan,
            Yellow,
            Grey,

            Bright = 0x10,

            BrightRed   = Bright | Red,
            BrightGreen = Bright | Green,
            LightGrey   = Bright | Grey,
            BrightWhite = Bright | White
        };

#ifdef DOCTEST_CONFIG_COLORS_WINDOWS
        HANDLE g_stdoutHandle;
        WORD   g_originalForegroundAttributes;
        WORD   g_originalBackgroundAttributes;
        bool   g_attrsInitted = false;
#endif // DOCTEST_CONFIG_COLORS_WINDOWS

        void init() {
#ifdef DOCTEST_CONFIG_COLORS_WINDOWS
            if(!g_attrsInitted) {
                g_stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
                g_attrsInitted = true;
                CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
                GetConsoleScreenBufferInfo(g_stdoutHandle, &csbiInfo);
                g_originalForegroundAttributes =
                        csbiInfo.wAttributes & ~(BACKGROUND_GREEN | BACKGROUND_RED |
                                                 BACKGROUND_BLUE | BACKGROUND_INTENSITY);
                g_originalBackgroundAttributes =
                        csbiInfo.wAttributes & ~(FOREGROUND_GREEN | FOREGROUND_RED |
                                                 FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            }
#endif // DOCTEST_CONFIG_COLORS_WINDOWS
        }

        std::ostream& operator<<(std::ostream&            s, Color::Code
#ifndef DOCTEST_CONFIG_COLORS_NONE
                                                          code
#endif // DOCTEST_CONFIG_COLORS_NONE
        ) {
            const ContextState* p = contextState;
            if(p->no_colors)
                return s;
#ifdef DOCTEST_CONFIG_COLORS_ANSI
            if(isatty(STDOUT_FILENO) == false && p->force_colors == false)
                return s;

            const char* col = "";
            // clang-format off
            switch(code) { //!OCLINT missing break in switch statement / unnecessary default statement in covered switch statement
                case Color::Red:         col = "[0;31m"; break;
                case Color::Green:       col = "[0;32m"; break;
                case Color::Blue:        col = "[0;34m"; break;
                case Color::Cyan:        col = "[0;36m"; break;
                case Color::Yellow:      col = "[0;33m"; break;
                case Color::Grey:        col = "[1;30m"; break;
                case Color::LightGrey:   col = "[0;37m"; break;
                case Color::BrightRed:   col = "[1;31m"; break;
                case Color::BrightGreen: col = "[1;32m"; break;
                case Color::BrightWhite: col = "[1;37m"; break;
                case Color::Bright: // invalid
                case Color::None:
                case Color::White:
                default:                 col = "[0m";
            }
            // clang-format on
            s << "\033" << col;
#endif // DOCTEST_CONFIG_COLORS_ANSI

#ifdef DOCTEST_CONFIG_COLORS_WINDOWS
            if(isatty(fileno(stdout)) == false && p->force_colors == false)
                return s;

#define DOCTEST_SET_ATTR(x)                                                                        \
    SetConsoleTextAttribute(g_stdoutHandle, x | g_originalBackgroundAttributes)

            // clang-format off
            switch (code) {
                case Color::White:       DOCTEST_SET_ATTR(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE); break;
                case Color::Red:         DOCTEST_SET_ATTR(FOREGROUND_RED);                                      break;
                case Color::Green:       DOCTEST_SET_ATTR(FOREGROUND_GREEN);                                    break;
                case Color::Blue:        DOCTEST_SET_ATTR(FOREGROUND_BLUE);                                     break;
                case Color::Cyan:        DOCTEST_SET_ATTR(FOREGROUND_BLUE | FOREGROUND_GREEN);                  break;
                case Color::Yellow:      DOCTEST_SET_ATTR(FOREGROUND_RED | FOREGROUND_GREEN);                   break;
                case Color::Grey:        DOCTEST_SET_ATTR(0);                                                   break;
                case Color::LightGrey:   DOCTEST_SET_ATTR(FOREGROUND_INTENSITY);                                break;
                case Color::BrightRed:   DOCTEST_SET_ATTR(FOREGROUND_INTENSITY | FOREGROUND_RED);               break;
                case Color::BrightGreen: DOCTEST_SET_ATTR(FOREGROUND_INTENSITY | FOREGROUND_GREEN);             break;
                case Color::BrightWhite: DOCTEST_SET_ATTR(FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE); break;
                case Color::None:
                case Color::Bright: // invalid
                default:                 DOCTEST_SET_ATTR(g_originalForegroundAttributes);
            }
                // clang-format on
#undef DOCTEST_SET_ATTR
#endif // DOCTEST_CONFIG_COLORS_WINDOWS
            return s;
        }
    } // namespace Color

    std::vector<const IExceptionTranslator*>& getExceptionTranslators() {
        static std::vector<const IExceptionTranslator*> data;
        return data;
    }

    void registerExceptionTranslatorImpl(const IExceptionTranslator* translateFunction) {
        if(std::find(getExceptionTranslators().begin(), getExceptionTranslators().end(),
                     translateFunction) == getExceptionTranslators().end())
            getExceptionTranslators().push_back(translateFunction);
    }

    String translateActiveException() {
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
        String                                    res;
        std::vector<const IExceptionTranslator*>& translators = getExceptionTranslators();
        for(size_t i = 0; i < translators.size(); ++i)
            if(translators[i]->translate(res))
                return res;
        // clang-format off
        try {
            throw;
        } catch(std::exception& ex) {
            return ex.what();
        } catch(std::string& msg) {
            return msg.c_str();
        } catch(const char* msg) {
            return msg;
        } catch(...) {
            return "unknown exception";
        }
// clang-format on
#else  // DOCTEST_CONFIG_NO_EXCEPTIONS
        return "";
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS
    }

    void writeStringToStream(std::ostream* s, const String& str) { *s << str; }

#ifdef DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
    void toStream(std::ostream* s, char* in) { *s << in; }
    void toStream(std::ostream* s, const char* in) { *s << in; }
#endif // DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
    void toStream(std::ostream* s, bool in) { *s << std::boolalpha << in << std::noboolalpha; }
    void toStream(std::ostream* s, float in) { *s << in; }
    void toStream(std::ostream* s, double in) { *s << in; }
    void toStream(std::ostream* s, double long in) { *s << in; }

    void toStream(std::ostream* s, char in) { *s << in; }
    void toStream(std::ostream* s, char signed in) { *s << in; }
    void toStream(std::ostream* s, char unsigned in) { *s << in; }
    void toStream(std::ostream* s, int short in) { *s << in; }
    void toStream(std::ostream* s, int short unsigned in) { *s << in; }
    void toStream(std::ostream* s, int in) { *s << in; }
    void toStream(std::ostream* s, int unsigned in) { *s << in; }
    void toStream(std::ostream* s, int long in) { *s << in; }
    void toStream(std::ostream* s, int long unsigned in) { *s << in; }

#ifdef DOCTEST_CONFIG_WITH_LONG_LONG
    void toStream(std::ostream* s, int long long in) { *s << in; }
    void toStream(std::ostream* s, int long long unsigned in) { *s << in; }
#endif // DOCTEST_CONFIG_WITH_LONG_LONG

    void addToContexts(IContextScope* ptr) { contextState->contexts.push_back(ptr); }
    void popFromContexts() { contextState->contexts.pop_back(); }
    DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4996) // std::uncaught_exception is deprecated in C++17
    DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH("-Wdeprecated-declarations")
    void useContextIfExceptionOccurred(IContextScope* ptr) {
        if(std::uncaught_exception()) {
            std::ostringstream s;
            ptr->build(&s);
            contextState->exceptionalContexts.push_back(s.str());
        }
    }
    DOCTEST_GCC_SUPPRESS_WARNING_POP
    DOCTEST_MSVC_SUPPRESS_WARNING_POP

    void printSummary(std::ostream& s);

#if !defined(DOCTEST_CONFIG_POSIX_SIGNALS) && !defined(DOCTEST_CONFIG_WINDOWS_SEH)
    void reportFatal(const std::string&) {}
    struct FatalConditionHandler
    {
        void reset() {}
    };
#else // DOCTEST_CONFIG_POSIX_SIGNALS || DOCTEST_CONFIG_WINDOWS_SEH

    void reportFatal(const std::string&);

#ifdef DOCTEST_PLATFORM_WINDOWS

    struct SignalDefs
    {
        DWORD       id;
        const char* name;
    };
    // There is no 1-1 mapping between signals and windows exceptions.
    // Windows can easily distinguish between SO and SigSegV,
    // but SigInt, SigTerm, etc are handled differently.
    SignalDefs signalDefs[] = {
            {EXCEPTION_ILLEGAL_INSTRUCTION, "SIGILL - Illegal instruction signal"},
            {EXCEPTION_STACK_OVERFLOW, "SIGSEGV - Stack overflow"},
            {EXCEPTION_ACCESS_VIOLATION, "SIGSEGV - Segmentation violation signal"},
            {EXCEPTION_INT_DIVIDE_BY_ZERO, "Divide by zero error"},
    };

    struct FatalConditionHandler
    {
        static LONG CALLBACK handleVectoredException(PEXCEPTION_POINTERS ExceptionInfo) {
            for(size_t i = 0; i < DOCTEST_COUNTOF(signalDefs); ++i) {
                if(ExceptionInfo->ExceptionRecord->ExceptionCode == signalDefs[i].id) {
                    reportFatal(signalDefs[i].name);
                }
            }
            // If its not an exception we care about, pass it along.
            // This stops us from eating debugger breaks etc.
            return EXCEPTION_CONTINUE_SEARCH;
        }

        FatalConditionHandler() {
            isSet = true;
            // 32k seems enough for doctest to handle stack overflow,
            // but the value was found experimentally, so there is no strong guarantee
            guaranteeSize          = 32 * 1024;
            exceptionHandlerHandle = 0;
            // Register as first handler in current chain
            exceptionHandlerHandle = AddVectoredExceptionHandler(1, handleVectoredException);
            // Pass in guarantee size to be filled
            SetThreadStackGuarantee(&guaranteeSize);
        }

        static void reset() {
            if(isSet) {
                // Unregister handler and restore the old guarantee
                RemoveVectoredExceptionHandler(exceptionHandlerHandle);
                SetThreadStackGuarantee(&guaranteeSize);
                exceptionHandlerHandle = 0;
                isSet                  = false;
            }
        }

        ~FatalConditionHandler() { reset(); }

    private:
        static bool  isSet;
        static ULONG guaranteeSize;
        static PVOID exceptionHandlerHandle;
    };

    bool  FatalConditionHandler::isSet                  = false;
    ULONG FatalConditionHandler::guaranteeSize          = 0;
    PVOID FatalConditionHandler::exceptionHandlerHandle = 0;

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
        static char             altStackMem[4 * SIGSTKSZ];

        static void handleSignal(int sig) {
            std::string name = "<unknown signal>";
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

        FatalConditionHandler() {
            isSet = true;
            stack_t sigStack;
            sigStack.ss_sp    = altStackMem;
            sigStack.ss_size  = sizeof(altStackMem);
            sigStack.ss_flags = 0;
            sigaltstack(&sigStack, &oldSigStack);
            struct sigaction sa = {0};

            sa.sa_handler = handleSignal; // NOLINT
            sa.sa_flags   = SA_ONSTACK;
            for(std::size_t i = 0; i < DOCTEST_COUNTOF(signalDefs); ++i) {
                sigaction(signalDefs[i].id, &sa, &oldSigActions[i]);
            }
        }

        ~FatalConditionHandler() { reset(); }
        static void reset() {
            if(isSet) {
                // Set signals back to previous values -- hopefully nobody overwrote them in the meantime
                for(std::size_t i = 0; i < DOCTEST_COUNTOF(signalDefs); ++i) {
                    sigaction(signalDefs[i].id, &oldSigActions[i], 0);
                }
                // Return the old stack
                sigaltstack(&oldSigStack, 0);
                isSet = false;
            }
        }
    };

    bool             FatalConditionHandler::isSet                                      = false;
    struct sigaction FatalConditionHandler::oldSigActions[DOCTEST_COUNTOF(signalDefs)] = {};
    stack_t          FatalConditionHandler::oldSigStack                                = {};
    char             FatalConditionHandler::altStackMem[]                              = {};

#endif // DOCTEST_PLATFORM_WINDOWS
#endif // DOCTEST_CONFIG_POSIX_SIGNALS || DOCTEST_CONFIG_WINDOWS_SEH

    void separator_to_stream(std::ostream& s) {
        s << Color::Yellow
          << "===============================================================================\n";
    }

    // depending on the current options this will remove the path of filenames
    const char* fileForOutput(const char* file) {
        if(contextState->no_path_in_filenames) {
            const char* back    = std::strrchr(file, '\\');
            const char* forward = std::strrchr(file, '/');
            if(back || forward) {
                if(back > forward)
                    forward = back;
                return forward + 1;
            }
        }
        return file;
    }

    void file_line_to_stream(std::ostream& s, const char* file, int line, const char* tail = "") {
        s << Color::LightGrey << fileForOutput(file) << (contextState->gnu_file_line ? ":" : "(")
          << (contextState->no_line_numbers ? 0 : line) // 0 or the real num depending on the option
          << (contextState->gnu_file_line ? ":" : "):") << tail;
    }

    const char* getSuccessOrFailString(bool success, assertType::Enum at, const char* success_str) {
        if(success)
            return success_str;
        if(at & assertType::is_warn) //!OCLINT bitwise operator in conditional
            return "WARNING: ";
        if(at & assertType::is_check) //!OCLINT bitwise operator in conditional
            return "ERROR: ";
        if(at & assertType::is_require) //!OCLINT bitwise operator in conditional
            return "FATAL ERROR: ";
        return "";
    }

    Color::Code getSuccessOrFailColor(bool success, assertType::Enum at) {
        return success ? Color::BrightGreen :
                         (at & assertType::is_warn) ? Color::Yellow : Color::Red;
    }

    void successOrFailColoredStringToStream(std::ostream& s, bool success, assertType::Enum at,
                                            const char* success_str = "SUCCESS: ") {
        s << getSuccessOrFailColor(success, at) << getSuccessOrFailString(success, at, success_str);
    }

#ifdef DOCTEST_PLATFORM_MAC
#include <sys/types.h>
#include <unistd.h>
#include <sys/sysctl.h>
    // The following function is taken directly from the following technical note:
    // http://developer.apple.com/library/mac/#qa/qa2004/qa1361.html
    // Returns true if the current process is being debugged (either
    // running under the debugger or has a debugger attached post facto).
    bool isDebuggerActive() {
        int        mib[4];
        kinfo_proc info;
        size_t     size;
        // Initialize the flags so that, if sysctl fails for some bizarre
        // reason, we get a predictable result.
        info.kp_proc.p_flag = 0;
        // Initialize mib, which tells sysctl the info we want, in this case
        // we're looking for information about a specific process ID.
        mib[0] = CTL_KERN;
        mib[1] = KERN_PROC;
        mib[2] = KERN_PROC_PID;
        mib[3] = getpid();
        // Call sysctl.
        size = sizeof(info);
        if(sysctl(mib, DOCTEST_COUNTOF(mib), &info, &size, 0, 0) != 0) {
            fprintf(stderr, "\n** Call to sysctl failed - unable to determine if debugger is "
                            "active **\n\n");
            return false;
        }
        // We're being debugged if the P_TRACED flag is set.
        return ((info.kp_proc.p_flag & P_TRACED) != 0);
    }
#elif DOCTEST_MSVC || defined(__MINGW32__)
    bool  isDebuggerActive() { return ::IsDebuggerPresent() != 0; }
#else
    bool isDebuggerActive() { return false; }
#endif // Platform

#ifdef DOCTEST_PLATFORM_WINDOWS
    void myOutputDebugString(const String& text) { ::OutputDebugStringA(text.c_str()); }
#else
    // TODO: integration with XCode and other IDEs
    void myOutputDebugString(const String&) {}
#endif // Platform

    void printToDebugConsole(const String& text) {
        if(isDebuggerActive())
            myOutputDebugString(text);
    }

    void addFailedAssert(assertType::Enum at) {
        if((at & assertType::is_warn) == 0) { //!OCLINT bitwise operator in conditional
            contextState->numFailedAssertions++;
            contextState->numFailedAssertionsForCurrentTestcase++;
            contextState->hasCurrentTestFailed = true;
        }
    }

    std::ostream& operator<<(std::ostream& s, const std::vector<IContextScope*>& contexts) {
        if(!contexts.empty())
            s << Color::None << "  logged: ";
        for(size_t i = 0; i < contexts.size(); ++i) {
            s << (i == 0 ? "" : "          ");
            contexts[i]->build(&s);
            s << "\n";
        }
        s << "\n";
        return s;
    }

    void logTestStart(std::ostream& s, const TestCase& tc) {
        separator_to_stream(s);
        file_line_to_stream(s, tc.m_file, tc.m_line, "\n");
        if(tc.m_description)
            s << Color::Yellow << "DESCRIPTION: " << Color::None << tc.m_description << "\n";
        if(tc.m_test_suite && tc.m_test_suite[0] != '\0')
            s << Color::Yellow << "TEST SUITE: " << Color::None << tc.m_test_suite << "\n";
        if(strncmp(tc.m_name, "  Scenario:", 11) != 0)
            s << Color::None << "TEST CASE:  ";
        s << Color::None << tc.m_name << "\n";

        std::vector<Subcase>& subcasesStack = contextState->subcasesStack;
        for(unsigned i = 0; i < subcasesStack.size(); ++i)
            if(subcasesStack[i].m_signature.m_name[0] != '\0')
                s << "  " << subcasesStack[i].m_signature.m_name << "\n";

        s << "\n";
    }

    void logTestEnd() {}

    void logTestException_impl(std::ostream& s, const TestCase& tc, const String& str, bool crash) {
        file_line_to_stream(s, tc.m_file, tc.m_line, " ");
        successOrFailColoredStringToStream(s, false,
                                           crash ? assertType::is_require : assertType::is_check);
        s << Color::Red << (crash ? "test case CRASHED: " : "test case THREW exception: ")
          << Color::Cyan << str << "\n";

        if(!contextState->exceptionalContexts.empty()) {
            s << Color::None << "  logged: ";
            for(size_t i = contextState->exceptionalContexts.size(); i > 0; --i)
                s << (i == contextState->exceptionalContexts.size() ? "" : "          ")
                  << contextState->exceptionalContexts[i - 1] << "\n";
        }
        s << "\n";
    }

    void logTestException(const TestCase& tc, const String& what, bool crash) {
        logTestException_impl(std::cout, tc, what, crash);
        DOCTEST_PRINT_TO_OUTPUT_WINDOW_IN_IDE_BEGIN;
        logTestException_impl(oss, tc, what, crash);
        DOCTEST_PRINT_TO_OUTPUT_WINDOW_IN_IDE_END;
    }

#if defined(DOCTEST_CONFIG_POSIX_SIGNALS) || defined(DOCTEST_CONFIG_WINDOWS_SEH)
    void reportFatal(const std::string& message) {
        DOCTEST_LOG_START(std::cout);

        contextState->numAssertions += contextState->numAssertionsForCurrentTestcase;
        logTestException(*contextState->currentTest, message.c_str(), true);
        logTestEnd();
        contextState->numFailed++;

        printSummary(std::cout);
    }
#endif // DOCTEST_CONFIG_POSIX_SIGNALS || DOCTEST_CONFIG_WINDOWS_SEH

    void logAssert_impl(std::ostream& s, bool passed, const String& dec, bool threw,
                        const String& ex, const char* expr, assertType::Enum at, const char* file,
                        int line) {
        file_line_to_stream(s, file, line, " ");
        successOrFailColoredStringToStream(s, passed, at);
        s << Color::Cyan << assertString(at) << "( " << expr << " ) " << Color::None
          << (threw ? "THREW exception: " : (passed ? "is correct!\n" : "is NOT correct!\n"));
        if(threw)
            s << ex << "\n";
        else
            s << "  values: " << assertString(at) << "( " << dec << " )\n";
        s << contextState->contexts;
    }

    void logAssert(bool passed, const String& dec, bool threw, const String& ex, const char* expr,
                   assertType::Enum at, const char* file, int line) {
        logAssert_impl(std::cout, passed, dec, threw, ex, expr, at, file, line);
        DOCTEST_PRINT_TO_OUTPUT_WINDOW_IN_IDE_BEGIN;
        logAssert_impl(oss, passed, dec, threw, ex, expr, at, file, line);
        DOCTEST_PRINT_TO_OUTPUT_WINDOW_IN_IDE_END;
    }

    void logAssertThrows_impl(std::ostream& s, bool threw, const char* expr, assertType::Enum at,
                              const char* file, int line) {
        file_line_to_stream(s, file, line, " ");
        successOrFailColoredStringToStream(s, threw, at);
        s << Color::Cyan << assertString(at) << "( " << expr << " ) " << Color::None
          << (threw ? "threw as expected!" : "did NOT throw at all!") << "\n";
        s << contextState->contexts;
    }

    void logAssertThrows(bool threw, const char* expr, assertType::Enum at, const char* file,
                         int line) {
        logAssertThrows_impl(std::cout, threw, expr, at, file, line);
        DOCTEST_PRINT_TO_OUTPUT_WINDOW_IN_IDE_BEGIN;
        logAssertThrows_impl(oss, threw, expr, at, file, line);
        DOCTEST_PRINT_TO_OUTPUT_WINDOW_IN_IDE_END;
    }

    void logAssertThrowsAs_impl(std::ostream& s, bool threw, bool threw_as, const char* as,
                                const String& ex, const char* expr, assertType::Enum at,
                                const char* file, int line) {
        file_line_to_stream(s, file, line, " ");
        successOrFailColoredStringToStream(s, threw_as, at);
        s << Color::Cyan << assertString(at) << "( " << expr << ", " << as << " ) " << Color::None
          << (threw ? (threw_as ? "threw as expected!" : "threw a DIFFERENT exception: ") :
                      "did NOT throw at all!")
          << Color::Cyan << ex << "\n";
        s << contextState->contexts;
    }

    void logAssertThrowsAs(bool threw, bool threw_as, const char* as, const String& ex,
                           const char* expr, assertType::Enum at, const char* file, int line) {
        logAssertThrowsAs_impl(std::cout, threw, threw_as, as, ex, expr, at, file, line);
        DOCTEST_PRINT_TO_OUTPUT_WINDOW_IN_IDE_BEGIN;
        logAssertThrowsAs_impl(oss, threw, threw_as, as, ex, expr, at, file, line);
        DOCTEST_PRINT_TO_OUTPUT_WINDOW_IN_IDE_END;
    }

    void logAssertNothrow_impl(std::ostream& s, bool threw, const String& ex, const char* expr,
                               assertType::Enum at, const char* file, int line) {
        file_line_to_stream(s, file, line, " ");
        successOrFailColoredStringToStream(s, !threw, at);
        s << Color::Cyan << assertString(at) << "( " << expr << " ) " << Color::None
          << (threw ? "THREW exception: " : "didn't throw!") << Color::Cyan << ex << "\n";
        s << contextState->contexts;
    }

    void logAssertNothrow(bool threw, const String& ex, const char* expr, assertType::Enum at,
                          const char* file, int line) {
        logAssertNothrow_impl(std::cout, threw, ex, expr, at, file, line);
        DOCTEST_PRINT_TO_OUTPUT_WINDOW_IN_IDE_BEGIN;
        logAssertNothrow_impl(oss, threw, ex, expr, at, file, line);
        DOCTEST_PRINT_TO_OUTPUT_WINDOW_IN_IDE_END;
    }

    ResultBuilder::ResultBuilder(assertType::Enum at, const char* file, int line, const char* expr,
                                 const char* exception_type)
            : m_assert_type(at)
            , m_file(file)
            , m_line(line)
            , m_expr(expr)
            , m_exception_type(exception_type)
            , m_threw(false)
            , m_threw_as(false)
            , m_failed(false) {
#if DOCTEST_MSVC
        if(m_expr[0] == ' ') // this happens when variadic macros are disabled under MSVC
            ++m_expr;
#endif // MSVC
    }

    ResultBuilder::~ResultBuilder() {}

    void ResultBuilder::unexpectedExceptionOccurred() {
        m_threw = true;

        m_exception = translateActiveException();
    }

    bool ResultBuilder::log() {
        if((m_assert_type & assertType::is_warn) == 0) //!OCLINT bitwise operator in conditional
            contextState->numAssertionsForCurrentTestcase++;

        if(m_assert_type & assertType::is_throws) { //!OCLINT bitwise operator in conditional
            m_failed = !m_threw;
        } else if(m_assert_type & //!OCLINT bitwise operator in conditional
                  assertType::is_throws_as) {
            m_failed = !m_threw_as;
        } else if(m_assert_type & //!OCLINT bitwise operator in conditional
                  assertType::is_nothrow) {
            m_failed = m_threw;
        } else {
            m_failed = m_result;
        }

        if(m_failed || contextState->success) {
            DOCTEST_LOG_START(std::cout);

            if(m_assert_type & assertType::is_throws) { //!OCLINT bitwise operator in conditional
                logAssertThrows(m_threw, m_expr, m_assert_type, m_file, m_line);
            } else if(m_assert_type & //!OCLINT bitwise operator in conditional
                      assertType::is_throws_as) {
                logAssertThrowsAs(m_threw, m_threw_as, m_exception_type, m_exception, m_expr,
                                  m_assert_type, m_file, m_line);
            } else if(m_assert_type & //!OCLINT bitwise operator in conditional
                      assertType::is_nothrow) {
                logAssertNothrow(m_threw, m_exception, m_expr, m_assert_type, m_file, m_line);
            } else {
                logAssert(m_result.m_passed, m_result.m_decomposition, m_threw, m_exception, m_expr,
                          m_assert_type, m_file, m_line);
            }
        }

        if(m_failed)
            addFailedAssert(m_assert_type);

        return m_failed && isDebuggerActive() && !contextState->no_breaks; // break into debugger
    }

    void ResultBuilder::react() const {
        if(m_failed && checkIfShouldThrow(m_assert_type))
            throwException();
    }

    MessageBuilder::MessageBuilder(const char* file, int line, assertType::Enum severity)
            : m_stream(createStream())
            , m_file(file)
            , m_line(line)
            , m_severity(severity) {}

    void MessageBuilder::log(std::ostream& s) {
        file_line_to_stream(s, m_file, m_line, " ");
        s << getSuccessOrFailColor(false, m_severity)
          << getSuccessOrFailString(m_severity & assertType::is_warn, m_severity, "MESSAGE: ");
        s << Color::None << getStreamResult(m_stream) << "\n";
        s << contextState->contexts;
    }

    bool MessageBuilder::log() {
        DOCTEST_LOG_START(std::cout);

        log(std::cout);
        DOCTEST_PRINT_TO_OUTPUT_WINDOW_IN_IDE_BEGIN;
        log(oss);
        DOCTEST_PRINT_TO_OUTPUT_WINDOW_IN_IDE_END;
        const bool isWarn = m_severity & assertType::is_warn;

        // warn is just a message in this context so we dont treat it as an assert
        if(!isWarn) {
            contextState->numAssertionsForCurrentTestcase++;
            addFailedAssert(m_severity);
        }

        return isDebuggerActive() && !contextState->no_breaks && !isWarn; // break into debugger
    }

    void MessageBuilder::react() {
        if(m_severity & assertType::is_require) //!OCLINT bitwise operator in conditional
            throwException();
    }

    MessageBuilder::~MessageBuilder() { freeStream(m_stream); }

    // the implementation of parseFlag()
    bool parseFlagImpl(int argc, const char* const* argv, const char* pattern) {
        for(int i = argc - 1; i >= 0; --i) {
            const char* temp = std::strstr(argv[i], pattern);
            if(temp && strlen(temp) == strlen(pattern)) {
                // eliminate strings in which the chars before the option are not '-'
                bool noBadCharsFound = true; //!OCLINT prefer early exits and continue
                while(temp != argv[i]) {
                    if(*--temp != '-') {
                        noBadCharsFound = false;
                        break;
                    }
                }
                if(noBadCharsFound && argv[i][0] == '-')
                    return true;
            }
        }
        return false;
    }

    // locates a flag on the command line
    bool parseFlag(int argc, const char* const* argv, const char* pattern) {
#ifndef DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS
        if(!parseFlagImpl(argc, argv, pattern))
            return parseFlagImpl(argc, argv, pattern + 3); // 3 for "dt-"
        return true;
#else  // DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS
        return parseFlagImpl(argc, argv, pattern);
#endif // DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS
    }

    // the implementation of parseOption()
    bool parseOptionImpl(int argc, const char* const* argv, const char* pattern, String& res) {
        for(int i = argc - 1; i >= 0; --i) {
            const char* temp = std::strstr(argv[i], pattern);
            if(temp) { //!OCLINT prefer early exits and continue
                // eliminate matches in which the chars before the option are not '-'
                bool        noBadCharsFound = true;
                const char* curr            = argv[i];
                while(curr != temp) {
                    if(*curr++ != '-') {
                        noBadCharsFound = false;
                        break;
                    }
                }
                if(noBadCharsFound && argv[i][0] == '-') {
                    temp += strlen(pattern);
                    const unsigned len = strlen(temp);
                    if(len) {
                        res = temp;
                        return true;
                    }
                }
            }
        }
        return false;
    }

    // parses an option and returns the string after the '=' character
    bool parseOption(int argc, const char* const* argv, const char* pattern, String& res,
                     const String& defaultVal = String()) {
        res = defaultVal;
#ifndef DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS
        if(!parseOptionImpl(argc, argv, pattern, res))
            return parseOptionImpl(argc, argv, pattern + 3, res); // 3 for "dt-"
        return true;
#else  // DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS
        return parseOptionImpl(argc, argv, pattern, res);
#endif // DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS
    }

    // parses a comma separated list of words after a pattern in one of the arguments in argv
    bool parseCommaSepArgs(int argc, const char* const* argv, const char* pattern,
                           std::vector<String>& res) {
        String filtersString;
        if(parseOption(argc, argv, pattern, filtersString)) {
            // tokenize with "," as a separator
            // cppcheck-suppress strtokCalled
            char* pch = std::strtok(filtersString.c_str(), ","); // modifies the string
            while(pch != 0) {
                if(strlen(pch))
                    res.push_back(pch);
                // uses the strtok() internal state to go to the next token
                // cppcheck-suppress strtokCalled
                pch = std::strtok(0, ",");
            }
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
        if(!parseOption(argc, argv, pattern, parsedValue))
            return false;

        if(type == 0) {
            // boolean
            const char positive[][5] = {"1", "true", "on", "yes"};  // 5 - strlen("true") + 1
            const char negative[][6] = {"0", "false", "off", "no"}; // 6 - strlen("false") + 1

            // if the value matches any of the positive/negative possibilities
            for(unsigned i = 0; i < 4; i++) {
                if(parsedValue.compare(positive[i], true) == 0) {
                    res = 1; //!OCLINT parameter reassignment
                    return true;
                }
                if(parsedValue.compare(negative[i], true) == 0) {
                    res = 0; //!OCLINT parameter reassignment
                    return true;
                }
            }
        } else {
            // integer
            int theInt = std::atoi(parsedValue.c_str()); // NOLINT
            if(theInt != 0) {
                res = theInt; //!OCLINT parameter reassignment
                return true;
            }
        }
        return false;
    }

    void printVersion(std::ostream& s) {
        if(contextState->no_version == false)
            s << Color::Cyan << "[doctest] " << Color::None << "doctest version is \""
              << DOCTEST_VERSION_STR << "\"\n";
    }

    void printHelp(std::ostream& s) {
        printVersion(s);
        // clang-format off
        s << Color::Cyan << "[doctest]\n" << Color::None;
        s << Color::Cyan << "[doctest] " << Color::None;
        s << "boolean values: \"1/on/yes/true\" or \"0/off/no/false\"\n";
        s << Color::Cyan << "[doctest] " << Color::None;
        s << "filter  values: \"str1,str2,str3\" (comma separated strings)\n";
        s << Color::Cyan << "[doctest]\n" << Color::None;
        s << Color::Cyan << "[doctest] " << Color::None;
        s << "filters use wildcards for matching strings\n";
        s << Color::Cyan << "[doctest] " << Color::None;
        s << "something passes a filter if any of the strings in a filter matches\n";
        s << Color::Cyan << "[doctest]\n" << Color::None;
        s << Color::Cyan << "[doctest] " << Color::None;
        s << "ALL FLAGS, OPTIONS AND FILTERS ALSO AVAILABLE WITH A \"dt-\" PREFIX!!!\n";
        s << Color::Cyan << "[doctest]\n" << Color::None;
        s << Color::Cyan << "[doctest] " << Color::None;
        s << "Query flags - the program quits after them. Available:\n\n";
        s << " -?,   --help, -h                      prints this message\n";
        s << " -v,   --version                       prints the version\n";
        s << " -c,   --count                         prints the number of matching tests\n";
        s << " -ltc, --list-test-cases               lists all matching tests by name\n";
        s << " -lts, --list-test-suites              lists all matching test suites\n\n";
        // ================================================================================== << 79
        s << Color::Cyan << "[doctest] " << Color::None;
        s << "The available <int>/<string> options/filters are:\n\n";
        s << " -tc,  --test-case=<filters>           filters     tests by their name\n";
        s << " -tce, --test-case-exclude=<filters>   filters OUT tests by their name\n";
        s << " -sf,  --source-file=<filters>         filters     tests by their file\n";
        s << " -sfe, --source-file-exclude=<filters> filters OUT tests by their file\n";
        s << " -ts,  --test-suite=<filters>          filters     tests by their test suite\n";
        s << " -tse, --test-suite-exclude=<filters>  filters OUT tests by their test suite\n";
        s << " -sc,  --subcase=<filters>             filters     subcases by their name\n";
        s << " -sce, --subcase-exclude=<filters>     filters OUT subcases by their name\n";
        s << " -ob,  --order-by=<string>             how the tests should be ordered\n";
        s << "                                       <string> - by [file/suite/name/rand]\n";
        s << " -rs,  --rand-seed=<int>               seed for random ordering\n";
        s << " -f,   --first=<int>                   the first test passing the filters to\n";
        s << "                                       execute - for range-based execution\n";
        s << " -l,   --last=<int>                    the last test passing the filters to\n";
        s << "                                       execute - for range-based execution\n";
        s << " -aa,  --abort-after=<int>             stop after <int> failed assertions\n";
        s << " -scfl,--subcase-filter-levels=<int>   apply filters for the first <int> levels\n";
        s << Color::Cyan << "\n[doctest] " << Color::None;
        s << "Bool options - can be used like flags and true is assumed. Available:\n\n";
        s << " -s,   --success=<bool>                include successful assertions in output\n";
        s << " -cs,  --case-sensitive=<bool>         filters being treated as case sensitive\n";
        s << " -e,   --exit=<bool>                   exits after the tests finish\n";
        s << " -d,   --duration=<bool>               prints the time duration of each test\n";
        s << " -nt,  --no-throw=<bool>               skips exceptions-related assert checks\n";
        s << " -ne,  --no-exitcode=<bool>            returns (or exits) always with success\n";
        s << " -nr,  --no-run=<bool>                 skips all runtime doctest operations\n";
        s << " -nv,  --no-version=<bool>             omit the framework version in the output\n";
        s << " -nc,  --no-colors=<bool>              disables colors in output\n";
        s << " -fc,  --force-colors=<bool>           use colors even when not in a tty\n";
        s << " -nb,  --no-breaks=<bool>              disables breakpoints in debuggers\n";
        s << " -ns,  --no-skip=<bool>                don't skip test cases marked as skip\n";
        s << " -gfl, --gnu-file-line=<bool>          :n: vs (n): for line numbers in output\n";
        s << " -npf, --no-path-filenames=<bool>      only filenames and no paths in output\n";
        s << " -nln, --no-line-numbers=<bool>        0 instead of real line numbers in output\n";
        // ================================================================================== << 79
        // clang-format on

        s << Color::Cyan << "\n[doctest] " << Color::None;
        s << "for more information visit the project documentation\n\n";
    }

    void printSummary(std::ostream& s) {
        const ContextState* p = contextState;

        separator_to_stream(s);

        if(p->count || p->list_test_cases) {
            s << Color::Cyan << "[doctest] " << Color::None
              << "unskipped test cases passing the current filters: " << p->numTestsPassingFilters
              << "\n";
        } else if(p->list_test_suites) {
            s << Color::Cyan << "[doctest] " << Color::None
              << "unskipped test cases passing the current filters: " << p->numTestsPassingFilters
              << "\n";
            s << Color::Cyan << "[doctest] " << Color::None
              << "test suites with unskipped test cases passing the current filters: "
              << p->numTestSuitesPassingFilters << "\n";
        } else {
            const bool anythingFailed = p->numFailed > 0 || p->numFailedAssertions > 0;
            s << Color::Cyan << "[doctest] " << Color::None << "test cases: " << std::setw(6)
              << p->numTestsPassingFilters << " | "
              << ((p->numTestsPassingFilters == 0 || anythingFailed) ? Color::None : Color::Green)
              << std::setw(6) << p->numTestsPassingFilters - p->numFailed << " passed"
              << Color::None << " | " << (p->numFailed > 0 ? Color::Red : Color::None)
              << std::setw(6) << p->numFailed << " failed" << Color::None << " | ";
            if(p->no_skipped_summary == false) {
                const int numSkipped = static_cast<unsigned>(getRegisteredTests().size()) -
                                       p->numTestsPassingFilters;
                s << (numSkipped == 0 ? Color::None : Color::Yellow) << std::setw(6) << numSkipped
                  << " skipped" << Color::None;
            }
            s << "\n";
            s << Color::Cyan << "[doctest] " << Color::None << "assertions: " << std::setw(6)
              << p->numAssertions << " | "
              << ((p->numAssertions == 0 || anythingFailed) ? Color::None : Color::Green)
              << std::setw(6) << (p->numAssertions - p->numFailedAssertions) << " passed"
              << Color::None << " | " << (p->numFailedAssertions > 0 ? Color::Red : Color::None)
              << std::setw(6) << p->numFailedAssertions << " failed" << Color::None << " |\n";
            s << Color::Cyan << "[doctest] " << Color::None
              << "Status: " << (p->numFailed > 0 ? Color::Red : Color::Green)
              << ((p->numFailed > 0) ? "FAILURE!\n" : "SUCCESS!\n");
        }

        // remove any coloring
        s << Color::None;
    }
} // namespace detail

bool isRunningInTest() { return detail::contextState != 0; }

Context::Context(int argc, const char* const* argv)
        : p(new detail::ContextState) {
    parseArgs(argc, argv, true);
}

Context::~Context() { delete p; }

void Context::applyCommandLine(int argc, const char* const* argv) { parseArgs(argc, argv); }

// parses args
void Context::parseArgs(int argc, const char* const* argv, bool withDefaults) {
    using namespace detail;

    // clang-format off
    parseCommaSepArgs(argc, argv, "dt-source-file=",        p->filters[0]);
    parseCommaSepArgs(argc, argv, "dt-sf=",                 p->filters[0]);
    parseCommaSepArgs(argc, argv, "dt-source-file-exclude=",p->filters[1]);
    parseCommaSepArgs(argc, argv, "dt-sfe=",                p->filters[1]);
    parseCommaSepArgs(argc, argv, "dt-test-suite=",         p->filters[2]);
    parseCommaSepArgs(argc, argv, "dt-ts=",                 p->filters[2]);
    parseCommaSepArgs(argc, argv, "dt-test-suite-exclude=", p->filters[3]);
    parseCommaSepArgs(argc, argv, "dt-tse=",                p->filters[3]);
    parseCommaSepArgs(argc, argv, "dt-test-case=",          p->filters[4]);
    parseCommaSepArgs(argc, argv, "dt-tc=",                 p->filters[4]);
    parseCommaSepArgs(argc, argv, "dt-test-case-exclude=",  p->filters[5]);
    parseCommaSepArgs(argc, argv, "dt-tce=",                p->filters[5]);
    parseCommaSepArgs(argc, argv, "dt-subcase=",            p->filters[6]);
    parseCommaSepArgs(argc, argv, "dt-sc=",                 p->filters[6]);
    parseCommaSepArgs(argc, argv, "dt-subcase-exclude=",    p->filters[7]);
    parseCommaSepArgs(argc, argv, "dt-sce=",                p->filters[7]);
    // clang-format on

    int    intRes = 0;
    String strRes;

#define DOCTEST_PARSE_AS_BOOL_OR_FLAG(name, sname, var, default)                                   \
    if(parseIntOption(argc, argv, name "=", option_bool, intRes) ||                                \
       parseIntOption(argc, argv, sname "=", option_bool, intRes))                                 \
        p->var = !!intRes;                                                                         \
    else if(parseFlag(argc, argv, name) || parseFlag(argc, argv, sname))                           \
        p->var = true;                                                                             \
    else if(withDefaults)                                                                          \
    p->var = default

#define DOCTEST_PARSE_INT_OPTION(name, sname, var, default)                                        \
    if(parseIntOption(argc, argv, name "=", option_int, intRes) ||                                 \
       parseIntOption(argc, argv, sname "=", option_int, intRes))                                  \
        p->var = intRes;                                                                           \
    else if(withDefaults)                                                                          \
    p->var = default

#define DOCTEST_PARSE_STR_OPTION(name, sname, var, default)                                        \
    if(parseOption(argc, argv, name "=", strRes, default) ||                                       \
       parseOption(argc, argv, sname "=", strRes, default) || withDefaults)                        \
    p->var = strRes

    // clang-format off
    DOCTEST_PARSE_STR_OPTION("dt-order-by", "dt-ob", order_by, "file");
    DOCTEST_PARSE_INT_OPTION("dt-rand-seed", "dt-rs", rand_seed, 0);

    DOCTEST_PARSE_INT_OPTION("dt-first", "dt-f", first, 1);
    DOCTEST_PARSE_INT_OPTION("dt-last", "dt-l", last, 0);

    DOCTEST_PARSE_INT_OPTION("dt-abort-after", "dt-aa", abort_after, 0);
    DOCTEST_PARSE_INT_OPTION("dt-subcase-filter-levels", "dt-scfl", subcase_filter_levels, 2000000000);

    DOCTEST_PARSE_AS_BOOL_OR_FLAG("dt-success", "dt-s", success, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("dt-case-sensitive", "dt-cs", case_sensitive, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("dt-exit", "dt-e", exit, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("dt-duration", "dt-d", duration, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("dt-no-throw", "dt-nt", no_throw, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("dt-no-exitcode", "dt-ne", no_exitcode, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("dt-no-run", "dt-nr", no_run, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("dt-no-version", "dt-nv", no_version, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("dt-no-colors", "dt-nc", no_colors, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("dt-force-colors", "dt-fc", force_colors, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("dt-no-breaks", "dt-nb", no_breaks, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("dt-no-skip", "dt-ns", no_skip, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("dt-gnu-file-line", "dt-gfl", gnu_file_line, bool(DOCTEST_GCC) || bool(DOCTEST_CLANG));
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("dt-no-path-filenames", "dt-npf", no_path_in_filenames, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("dt-no-line-numbers", "dt-nln", no_line_numbers, false);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG("dt-no-skipped-summary", "dt-nss", no_skipped_summary, false);
    // clang-format on

#undef DOCTEST_PARSE_STR_OPTION
#undef DOCTEST_PARSE_INT_OPTION
#undef DOCTEST_PARSE_AS_BOOL_OR_FLAG

    if(withDefaults) {
        p->help             = false;
        p->version          = false;
        p->count            = false;
        p->list_test_cases  = false;
        p->list_test_suites = false;
    }
    if(parseFlag(argc, argv, "dt-help") || parseFlag(argc, argv, "dt-h") ||
       parseFlag(argc, argv, "dt-?")) {
        p->help = true;
        p->exit = true;
    }
    if(parseFlag(argc, argv, "dt-version") || parseFlag(argc, argv, "dt-v")) {
        p->version = true;
        p->exit    = true;
    }
    if(parseFlag(argc, argv, "dt-count") || parseFlag(argc, argv, "dt-c")) {
        p->count = true;
        p->exit  = true;
    }
    if(parseFlag(argc, argv, "dt-list-test-cases") || parseFlag(argc, argv, "dt-ltc")) {
        p->list_test_cases = true;
        p->exit            = true;
    }
    if(parseFlag(argc, argv, "dt-list-test-suites") || parseFlag(argc, argv, "dt-lts")) {
        p->list_test_suites = true;
        p->exit             = true;
    }
}

// allows the user to add procedurally to the filters from the command line
void Context::addFilter(const char* filter, const char* value) { setOption(filter, value); }

// allows the user to clear all filters from the command line
void Context::clearFilters() {
    for(unsigned i = 0; i < p->filters.size(); ++i)
        p->filters[i].clear();
}

// allows the user to override procedurally the int/bool options from the command line
void Context::setOption(const char* option, int value) {
    setOption(option, toString(value).c_str());
}

// allows the user to override procedurally the string options from the command line
void Context::setOption(const char* option, const char* value) {
    String      argv   = String("-") + option + "=" + value;
    const char* lvalue = argv.c_str();
    parseArgs(1, &lvalue);
}

// users should query this in their main() and exit the program if true
bool Context::shouldExit() { return p->exit; }

// the main function that does all the filtering and test running
int Context::run() {
    using namespace detail;

    Color::init();

    contextState = p;
    p->resetRunData();

    // handle version, help and no_run
    if(p->no_run || p->version || p->help) {
        if(p->version)
            printVersion(std::cout);
        if(p->help)
            printHelp(std::cout);

        contextState = 0;

        return EXIT_SUCCESS;
    }

    printVersion(std::cout);
    std::cout << Color::Cyan << "[doctest] " << Color::None << "run with \"--help\" for options\n";

    unsigned i = 0; // counter used for loops - here for VC6

    std::set<TestCase>& registeredTests = getRegisteredTests();

    std::vector<const TestCase*> testArray;
    for(std::set<TestCase>::iterator it = registeredTests.begin(); it != registeredTests.end();
        ++it)
        testArray.push_back(&(*it));

    // sort the collected records
    if(!testArray.empty()) {
        if(p->order_by.compare("file", true) == 0) {
            std::qsort(&testArray[0], testArray.size(), sizeof(TestCase*), fileOrderComparator);
        } else if(p->order_by.compare("suite", true) == 0) {
            std::qsort(&testArray[0], testArray.size(), sizeof(TestCase*), suiteOrderComparator);
        } else if(p->order_by.compare("name", true) == 0) {
            std::qsort(&testArray[0], testArray.size(), sizeof(TestCase*), nameOrderComparator);
        } else if(p->order_by.compare("rand", true) == 0) {
            std::srand(p->rand_seed);

            // random_shuffle implementation
            const TestCase** first = &testArray[0];
            for(i = testArray.size() - 1; i > 0; --i) {
                int idxToSwap = std::rand() % (i + 1); // NOLINT

                const TestCase* temp = first[i];

                first[i]         = first[idxToSwap];
                first[idxToSwap] = temp;
            }
        }
    }

    if(p->list_test_cases) {
        std::cout << Color::Cyan << "[doctest] " << Color::None << "listing all test case names\n";
        separator_to_stream(std::cout);
    }

    std::set<String> testSuitesPassingFilters;
    if(p->list_test_suites) {
        std::cout << Color::Cyan << "[doctest] " << Color::None << "listing all test suites\n";
        separator_to_stream(std::cout);
    }

    // invoke the registered functions if they match the filter criteria (or just count them)
    for(i = 0; i < testArray.size(); i++) {
        const TestCase& data = *testArray[i];

        if(data.m_skip && !p->no_skip)
            continue;

        if(!matchesAny(data.m_file, p->filters[0], 1, p->case_sensitive))
            continue;
        if(matchesAny(data.m_file, p->filters[1], 0, p->case_sensitive))
            continue;
        if(!matchesAny(data.m_test_suite, p->filters[2], 1, p->case_sensitive))
            continue;
        if(matchesAny(data.m_test_suite, p->filters[3], 0, p->case_sensitive))
            continue;
        if(!matchesAny(data.m_name, p->filters[4], 1, p->case_sensitive))
            continue;
        if(matchesAny(data.m_name, p->filters[5], 0, p->case_sensitive))
            continue;

        p->numTestsPassingFilters++;

        // do not execute the test if we are to only count the number of filter passing tests
        if(p->count)
            continue;

        // print the name of the test and don't execute it
        if(p->list_test_cases) {
            std::cout << Color::None << data.m_name << "\n";
            continue;
        }

        // print the name of the test suite if not done already and don't execute it
        if(p->list_test_suites) {
            if((testSuitesPassingFilters.count(data.m_test_suite) == 0) &&
               data.m_test_suite[0] != '\0') {
                std::cout << Color::None << data.m_test_suite << "\n";
                testSuitesPassingFilters.insert(data.m_test_suite);
                p->numTestSuitesPassingFilters++;
            }
            continue;
        }

        // skip the test if it is not in the execution range
        if((p->last < p->numTestsPassingFilters && p->first <= p->last) ||
           (p->first > p->numTestsPassingFilters))
            continue;

        // execute the test if it passes all the filtering
        {
            p->currentTest = &data;

            bool failed                              = false;
            p->hasLoggedCurrentTestStart             = false;
            p->numFailedAssertionsForCurrentTestcase = 0;
            p->subcasesPassed.clear();
            double duration = 0;
            Timer  timer;
            timer.start();
            do {
                // if the start has been logged from a previous iteration of this loop
                if(p->hasLoggedCurrentTestStart)
                    logTestEnd();
                p->hasLoggedCurrentTestStart = false;

                // if logging successful tests - force the start log
                if(p->success)
                    DOCTEST_LOG_START(std::cout);

                // reset the assertion state
                p->numAssertionsForCurrentTestcase = 0;
                p->hasCurrentTestFailed            = false;

                // reset some of the fields for subcases (except for the set of fully passed ones)
                p->subcasesHasSkipped   = false;
                p->subcasesCurrentLevel = 0;
                p->subcasesEnteredLevels.clear();

                // reset stuff for logging with INFO()
                p->exceptionalContexts.clear();

// execute the test
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
                try {
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS
                    FatalConditionHandler fatalConditionHandler; // Handle signals
                    data.m_test();
                    fatalConditionHandler.reset();
                    if(contextState->hasCurrentTestFailed)
                        failed = true;
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
                } catch(const TestFailureException&) { failed = true; } catch(...) {
                    DOCTEST_LOG_START(std::cout);
                    logTestException(*contextState->currentTest, translateActiveException(), false);
                    failed = true;
                }
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS

                p->numAssertions += p->numAssertionsForCurrentTestcase;

                // exit this loop if enough assertions have failed
                if(p->abort_after > 0 && p->numFailedAssertions >= p->abort_after) {
                    p->subcasesHasSkipped = false;
                    std::cout << Color::Red << "Aborting - too many failed asserts!\n";
                }

            } while(p->subcasesHasSkipped == true);

            duration = timer.getElapsedSeconds();

            if(Approx(p->currentTest->m_timeout).epsilon(DBL_EPSILON) != 0 &&
               Approx(duration).epsilon(DBL_EPSILON) > p->currentTest->m_timeout) {
                failed = true;
                DOCTEST_LOG_START(std::cout);
                std::cout << Color::Red << "Test case exceeded time limit of "
                          << std::setprecision(6) << std::fixed << p->currentTest->m_timeout
                          << "!\n";
            }

            if(p->duration)
                std::cout << Color::None << std::setprecision(6) << std::fixed << duration
                          << " s: " << p->currentTest->m_name << "\n";

            if(data.m_should_fail) {
                DOCTEST_LOG_START(std::cout);
                if(failed)
                    std::cout << Color::Yellow
                              << "Failed as expected so marking it as not failed\n";
                else
                    std::cout << Color::Red
                              << "Should have failed but didn't! Marking it as failed!\n";
                failed = !failed;
            } else if(failed && data.m_may_fail) {
                DOCTEST_LOG_START(std::cout);
                failed = false;
                std::cout << Color::Yellow << "Allowed to fail so marking it as not failed\n";
            } else if(data.m_expected_failures > 0) {
                DOCTEST_LOG_START(std::cout);
                if(p->numFailedAssertionsForCurrentTestcase == data.m_expected_failures) {
                    failed = false;
                    std::cout << Color::Yellow << "Failed exactly " << data.m_expected_failures
                              << " times as expected so marking it as not failed!\n";
                } else {
                    failed = true;
                    std::cout << Color::Red << "Didn't fail exactly " << data.m_expected_failures
                              << " times so marking it as failed!\n";
                }
            }
            std::cout << Color::None;

            if(p->hasLoggedCurrentTestStart)
                logTestEnd();

            if(failed) // if any subcase has failed - the whole test case has failed
                p->numFailed++;

            // stop executing tests if enough assertions have failed
            if(p->abort_after > 0 && p->numFailedAssertions >= p->abort_after)
                break;
        }
    }

    printSummary(std::cout);

    contextState = 0;

    if(p->numFailed && !p->no_exitcode)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

#ifdef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
int main(int argc, char** argv) { return doctest::Context(argc, argv).run(); }
#endif // DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

DOCTEST_CLANG_SUPPRESS_WARNING_POP
DOCTEST_MSVC_SUPPRESS_WARNING_POP
DOCTEST_GCC_SUPPRESS_WARNING_POP

#endif // DOCTEST_LIBRARY_IMPLEMENTATION
#endif // DOCTEST_CONFIG_IMPLEMENT
