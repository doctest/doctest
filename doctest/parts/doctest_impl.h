#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wshorten-64-to-32"
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#pragma clang diagnostic ignored "-Wswitch"
#pragma clang diagnostic ignored "-Wswitch-enum"
#pragma clang diagnostic ignored "-Wcovered-switch-default"
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#pragma clang diagnostic ignored "-Wunused-local-typedef"
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#pragma clang diagnostic ignored "-Wmissing-braces"
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#endif // __clang__

#if defined(__GNUC__) && !defined(__clang__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 6)
#pragma GCC diagnostic push
#endif // > gcc 4.6
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wstrict-overflow"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wmissing-braces"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Winline"
#pragma GCC diagnostic ignored "-Wswitch"
#pragma GCC diagnostic ignored "-Wswitch-enum"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wunsafe-loop-optimizations"
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 6)
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif // > gcc 4.6
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 7)
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#endif // > gcc 4.7
#if __GNUC__ > 5 || (__GNUC__ == 5 && __GNUC_MINOR__ > 3)
#pragma GCC diagnostic ignored "-Wuseless-cast"
#endif // > gcc 5.3
#endif // __GNUC__

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996) // The compiler encountered a deprecated declaration
#pragma warning(disable : 4267) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#pragma warning(disable : 4706) // assignment within conditional expression
#pragma warning(disable : 4512) // 'class' : assignment operator could not be generated
#pragma warning(disable : 4127) // conditional expression is constant
#pragma warning(disable : 4530) // C++ exception handler used, but unwind semantics are not enabled
#pragma warning(disable : 4577) // 'noexcept' used with no exception handling mode specified
#endif                          // _MSC_VER

#if defined(DOCTEST_CONFIG_IMPLEMENT) || !defined(DOCTEST_SINGLE_HEADER)
#ifndef DOCTEST_LIBRARY_IMPLEMENTATION
#define DOCTEST_LIBRARY_IMPLEMENTATION

#ifndef DOCTEST_SINGLE_HEADER
#include "doctest_fwd.h"
#endif // DOCTEST_SINGLE_HEADER

#if defined(__clang__) && defined(DOCTEST_NO_CPP11_COMPAT)
#pragma clang diagnostic ignored "-Wc++98-compat"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif // __clang__ && DOCTEST_NO_CPP11_COMPAT

// snprintf() not in the C++98 standard
#ifdef _MSC_VER
#define DOCTEST_SNPRINTF _snprintf
#else
#define DOCTEST_SNPRINTF std::snprintf
#endif

#undef DOCTEST_GCS
#define DOCTEST_GCS() (*doctest::detail::getContextState())

#define DOCTEST_LOG_START()                                                                        \
    do {                                                                                           \
        if(!DOCTEST_GCS().hasLoggedCurrentTestStart) {                                             \
            doctest::detail::logTestStart(DOCTEST_GCS().currentTest->m_name,                       \
                                          DOCTEST_GCS().currentTest->m_file,                       \
                                          DOCTEST_GCS().currentTest->m_line);                      \
            DOCTEST_GCS().hasLoggedCurrentTestStart = true;                                        \
        }                                                                                          \
    } while(false)

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
#include <iomanip>
#include <vector>
#include <set>
#include <exception>
#include <stdexcept>
#include <csignal>

namespace doctest
{
namespace detail
{
    // not using std::strlen() because of valgrind errors when optimizations are turned on
    // 'Invalid read of size 4' when the test suite len (with '\0') is not a multiple of 4
    // for details see http://stackoverflow.com/questions/35671155
    size_t my_strlen(const char* in) {
        const char* temp = in;
        while(temp && *temp)
            ++temp;
        return temp - in;
    }

    // lowers ascii letters
    char tolower(const char c) { return ((c >= 'A' && c <= 'Z') ? static_cast<char>(c + 32) : c); }

    template <typename T>
    T my_max(const T& lhs, const T& rhs) {
        return lhs > rhs ? lhs : rhs;
    }

    // case insensitive strcmp
    int stricmp(char const* a, char const* b) {
        for(;; a++, b++) {
            int d = tolower(*a) - tolower(*b);
            if(d != 0 || !*a)
                return d;
        }
    }

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

            u.asInt = 1;
            return (u.asChar[sizeof(int) - 1] == 1) ? Big : Little;
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
        std::ostringstream   os;
        os << "0x" << std::setfill('0') << std::hex;
        for(; i != end; i += inc)
            os << std::setw(2) << static_cast<unsigned>(bytes[i]);
        return os.str().c_str();
    }

    std::ostream* createStream() { return new std::ostringstream(); }
    String getStreamResult(std::ostream* in) {
        return static_cast<std::ostringstream*>(in)->str().c_str();
    }
    void freeStream(std::ostream* in) { delete in; }

#ifndef DOCTEST_CONFIG_DISABLE

    // a struct defining a registered test callback
    struct TestData
    {
        // not used for determining uniqueness
        const char* m_suite; // the test suite in which the test was added
        const char* m_name;  // name of the test function
        funcType    m_f;     // a function pointer to the test function
        String m_full_name; // contains the name (only for templated test cases!) + the template type

        // fields by which uniqueness of test cases shall be determined
        const char* m_file; // the file in which the test was registered
        unsigned    m_line; // the line where the test was registered
        int m_template_id; // an ID used to distinguish between the different versions of a templated test case

        TestData(const char* suite, const char* name, funcType f, const char* file, unsigned line,
                 const char* type_name, int template_id)
                : m_suite(suite)
                , m_name(name)
                , m_f(f)
                , m_file(file)
                , m_line(line)
                , m_template_id(template_id) {
            if(m_template_id != -1) {
                m_full_name = String(name) + type_name;
                // redirect the name to point to the newly constructed full name
                m_name = m_full_name.c_str();
            }
        }

        TestData(const TestData& other) { *this = other; }

        TestData& operator=(const TestData& other) {
            m_suite       = other.m_suite;
            m_name        = other.m_name;
            m_f           = other.m_f;
            m_full_name   = other.m_full_name;
            m_file        = other.m_file;
            m_line        = other.m_line;
            m_template_id = other.m_template_id;
            if(m_template_id != -1)
                m_name = m_full_name.c_str();
            return *this;
        }

        bool operator<(const TestData& other) const;
    };

    // this holds both parameters for the command line and runtime data for tests
    struct ContextState : TestAccessibleContextState
    {
        // == parameters from the command line

        std::vector<std::vector<String> > filters;

        String   order_by;  // how tests should be ordered
        unsigned rand_seed; // the seed for rand ordering

        unsigned first; // the first (matching) test to be executed
        unsigned last;  // the last (matching) test to be executed

        int  abort_after;           // stop tests after this many failed assertions
        int  subcase_filter_levels; // apply the subcase filters for the first N levels
        bool success;               // include successful assertions in output
        bool case_sensitive;        // if filtering should be case sensitive
        bool exit;         // if the program should be exited after the tests are ran/whatever
        bool no_exitcode;  // if the framework should return 0 as the exitcode
        bool no_run;       // to not run the tests at all (can be done with an "*" exclude)
        bool no_version;   // to not print the version of the framework
        bool no_colors;    // if output to the console should be colorized
        bool force_colors; // forces the use of colors even when a tty cannot be detected
        bool no_breaks;    // to not break into the debugger
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
        unsigned        numFailed;
        const TestData* currentTest;
        bool            hasLoggedCurrentTestStart;
        int             numAssertionsForCurrentTestcase;
        int             numAssertions;
        int             numFailedAssertions;
        int             hasCurrentTestFailed;

        std::vector<IContextScope*> contexts;            // for logging with INFO() and friends
        std::vector<std::string>    exceptionalContexts; // logging from INFO() due to an exception

        // stuff for subcases
        std::set<SubcaseSignature> subcasesPassed;
        std::set<int>              subcasesEnteredLevels;
        std::vector<Subcase>       subcasesStack;
        int                        subcasesCurrentLevel;
        bool                       subcasesHasSkipped;

        void resetRunData() {
            numTestsPassingFilters = 0;
            numFailed              = 0;
            numAssertions          = 0;
            numFailedAssertions    = 0;
        }

        ContextState()
                : filters(8) // 8 different filters total
        {
            resetRunData();
        }
    };

    ContextState*& getContextState();
#endif // DOCTEST_CONFIG_DISABLE
} // namespace detail

String::String(const char* in)
        : m_str(static_cast<char*>(std::malloc(detail::my_strlen(in) + 1))) {
    if(in)
        std::strcpy(m_str, in);
    else
        m_str[0] = '\0';
}

String::String(const String& other)
        : m_str(0) {
    copy(other);
}

void String::copy(const String& other) {
    if(m_str)
        std::free(m_str);
    m_str = static_cast<char*>(std::malloc(detail::my_strlen(other.m_str) + 1));
    std::strcpy(m_str, other.m_str);
}

String::~String() { std::free(m_str); }

String& String::operator=(const String& other) {
    if(this != &other)
        copy(other);
    return *this;
}

String String::operator+(const String& other) const { return String(m_str) += other; }

String& String::operator+=(const String& other) {
    using namespace detail;
    if(other.m_str != 0) {
        char* newStr =
                static_cast<char*>(std::malloc(my_strlen(m_str) + my_strlen(other.m_str) + 1));
        std::strcpy(newStr, m_str);
        std::strcpy(newStr + my_strlen(m_str), other.m_str);
        std::free(m_str);
        m_str = newStr;
    }
    return *this;
}

unsigned String::size() const { return m_str ? detail::my_strlen(m_str) : 0; }
unsigned String::length() const { return size(); }

int String::compare(const char* other, bool no_case) const {
    if(no_case)
        return detail::stricmp(m_str, other);
    return std::strcmp(m_str, other);
}

int String::compare(const String& other, bool no_case) const {
    return compare(other.c_str(), no_case);
}

std::ostream& operator<<(std::ostream& stream, const String& in) {
    stream << in.c_str();
    return stream;
}

Approx::Approx(double value)
        : m_epsilon(static_cast<double>(std::numeric_limits<float>::epsilon()) * 100)
        , m_scale(1.0)
        , m_value(value) {}

bool operator==(double lhs, Approx const& rhs) {
    // Thanks to Richard Harris for his help refining this formula
    return std::fabs(lhs - rhs.m_value) <
           rhs.m_epsilon * (rhs.m_scale + detail::my_max(std::fabs(lhs), std::fabs(rhs.m_value)));
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

#if defined(DOCTEST_CONFIG_DISABLE)
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

#define DOCTEST_PRINTF_COLORED(buffer, color)                                                      \
    do {                                                                                           \
        if(buffer[0] != 0) {                                                                       \
            doctest::detail::Color col(color);                                                     \
            std::printf("%s", buffer);                                                             \
        }                                                                                          \
    } while(false)

// the buffer size used for snprintf() calls
#if !defined(DOCTEST_SNPRINTF_BUFFER_LENGTH)
#define DOCTEST_SNPRINTF_BUFFER_LENGTH 1024
#endif // DOCTEST_SNPRINTF_BUFFER_LENGTH

#if defined(_MSC_VER) || defined(__MINGW32__)
#if defined(_MSC_VER) && _MSC_VER >= 1700
#define DOCTEST_WINDOWS_SAL_IN_OPT _In_opt_
#else // _MSC_VER
#define DOCTEST_WINDOWS_SAL_IN_OPT
#endif // _MSC_VER
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(
        DOCTEST_WINDOWS_SAL_IN_OPT const char*);
extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent();
#endif // _MSC_VER || __MINGW32__

#ifdef DOCTEST_CONFIG_COLORS_ANSI
#include <unistd.h>
#endif // DOCTEST_CONFIG_COLORS_ANSI

#ifdef _WIN32

// defines for a leaner windows.h
#ifndef WIN32_MEAN_AND_LEAN
#define WIN32_MEAN_AND_LEAN
#endif // WIN32_MEAN_AND_LEAN
#ifndef VC_EXTRA_LEAN
#define VC_EXTRA_LEAN
#endif // VC_EXTRA_LEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX

// not sure what AfxWin.h is for - here I do what Catch does
#ifdef __AFXDLL
#include <AfxWin.h>
#else
#include <windows.h>
#endif
#include <io.h>

#endif // _WIN32

namespace doctest_detail_test_suite_ns
{
// holds the current test suite
const char*& getCurrentTestSuite() {
    static const char* data = 0;
    return data;
}
} // namespace doctest_detail_test_suite_ns

namespace doctest
{
namespace detail
{
    bool TestData::operator<(const TestData& other) const {
        if(m_line != other.m_line)
            return m_line < other.m_line;
        int file_cmp = std::strcmp(m_file, other.m_file);
        if(file_cmp != 0)
            return file_cmp < 0;
        return m_template_id < other.m_template_id;
    }

    const char* getAssertString(assertType::Enum val) {
        switch(val) {
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
        return "";
    }

    bool checkIfShouldThrow(assertType::Enum assert_type) {
        if(assert_type & assertType::is_require)
            return true;

        if((assert_type & assertType::is_check) && getContextState()->abort_after > 0) {
            if(getContextState()->numFailedAssertions >= getContextState()->abort_after)
                return true;
        }

        return false;
    }
    void fastAssertThrowIfFlagSet(int flags) {
        if(flags & assertAction::shouldthrow)
            throwException();
    }
    void throwException() {
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
        throw TestFailureException();
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS
    }

    void my_memcpy(void* dest, void* src, int num) {
        char* csrc  = static_cast<char*>(src);
        char* cdest = static_cast<char*>(dest);
        for(int i    = 0; i < num; ++i)
            cdest[i] = csrc[i];
    }

    // matching of a string against a wildcard mask (case sensitivity configurable) taken from
    // http://www.emoticode.net/c/simple-wildcard-string-compare-globbing-function.html
    int wildcmp(const char* str, const char* wild, bool caseSensitive) {
        const char* cp = 0;
        const char* mp = 0;

        // rolled my own tolower() to not include more headers
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
                wild = mp;
                str  = cp++;
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
    int matchesAny(const char* name, std::vector<String> filters, int matchEmpty,
                   bool caseSensitive) {
        if(filters.size() == 0 && matchEmpty)
            return 1;
        for(unsigned i = 0; i < filters.size(); ++i)
            if(wildcmp(name, filters[i].c_str(), caseSensitive))
                return 1;
        return 0;
    }

    // the current ContextState with which tests are being executed
    ContextState*& getContextState() {
        static ContextState* data = 0;
        return data;
    }

    TestAccessibleContextState* getTestsContextState() { return getContextState(); }

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
        ContextState* s = getContextState();

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
            ContextState* s = getContextState();

            s->subcasesCurrentLevel--;
            // only mark the subcase as passed if no subcases have been skipped
            if(s->subcasesHasSkipped == false)
                s->subcasesPassed.insert(m_signature);

            if(s->subcasesStack.size() > 0)
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
        const TestData* lhs = *static_cast<TestData* const*>(a);
        const TestData* rhs = *static_cast<TestData* const*>(b);
#ifdef _MSC_VER
        // this is needed because MSVC gives different case for drive letters
        // for __FILE__ when evaluated in a header and a source file
        int res = stricmp(lhs->m_file, rhs->m_file);
#else  // _MSC_VER
        int res = std::strcmp(lhs->m_file, rhs->m_file);
#endif // _MSC_VER
        if(res != 0)
            return res;
        return static_cast<int>(lhs->m_line - rhs->m_line);
    }

    // for sorting tests by suite/file/line
    int suiteOrderComparator(const void* a, const void* b) {
        const TestData* lhs = *static_cast<TestData* const*>(a);
        const TestData* rhs = *static_cast<TestData* const*>(b);

        int res = std::strcmp(lhs->m_suite, rhs->m_suite);
        if(res != 0)
            return res;
        return fileOrderComparator(a, b);
    }

    // for sorting tests by name/suite/file/line
    int nameOrderComparator(const void* a, const void* b) {
        const TestData* lhs = *static_cast<TestData* const*>(a);
        const TestData* rhs = *static_cast<TestData* const*>(b);

        int res_name = std::strcmp(lhs->m_name, rhs->m_name);
        if(res_name != 0)
            return res_name;
        return suiteOrderComparator(a, b);
    }

    // sets the current test suite
    int setTestSuiteName(const char* name) {
        doctest_detail_test_suite_ns::getCurrentTestSuite() = name;
        return 0;
    }

    // all the registered tests
    std::set<TestData>& getRegisteredTests() {
        static std::set<TestData> data;
        return data;
    }

    // used by the macros for registering tests
    int regTest(funcType f, unsigned line, const char* file, const char* name, const char* suite,
                const char* type, int template_id) {
        getRegisteredTests().insert(TestData(suite, name, f, file, line, type, template_id));
        return 0;
    }

    struct Color
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
        Color(Code code) { use(code); }
        ~Color() { use(None); }

        void use(Code code);

    private:
        Color(Color const& other);
    };

    void Color::use(Code
#ifndef DOCTEST_CONFIG_COLORS_NONE
                            code
#endif // DOCTEST_CONFIG_COLORS_NONE
                    ) {
        ContextState* p = getContextState();
        if(p->no_colors)
            return;
#ifdef DOCTEST_CONFIG_COLORS_ANSI
        if(isatty(STDOUT_FILENO) == false && p->force_colors == false)
            return;

        const char* col = "";
        // clang-format off
        switch(code) {
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
        std::printf("\033%s", col);
#endif // DOCTEST_CONFIG_COLORS_ANSI

#ifdef DOCTEST_CONFIG_COLORS_WINDOWS
        if(isatty(fileno(stdout)) == false && p->force_colors == false)
            return;

        static HANDLE stdoutHandle(GetStdHandle(STD_OUTPUT_HANDLE));
        static WORD   originalForegroundAttributes;
        static WORD   originalBackgroundAttributes;
        static bool   attrsInitted = false;
        if(!attrsInitted) {
            attrsInitted = true;
            CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
            GetConsoleScreenBufferInfo(stdoutHandle, &csbiInfo);
            originalForegroundAttributes =
                    csbiInfo.wAttributes &
                    ~(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
            originalBackgroundAttributes =
                    csbiInfo.wAttributes &
                    ~(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        }

#define DOCTEST_SET_ATTR(x) SetConsoleTextAttribute(stdoutHandle, x | originalBackgroundAttributes)

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
            default:                 DOCTEST_SET_ATTR(originalForegroundAttributes);
        }
// clang-format on
#undef DOCTEST_SET_ATTR
#endif // DOCTEST_CONFIG_COLORS_WINDOWS
    }

    IExceptionTranslator::~IExceptionTranslator() {}

    std::vector<const IExceptionTranslator*>& getExceptionTranslators() {
        static std::vector<const IExceptionTranslator*> data;
        return data;
    }

    void registerExceptionTranslatorImpl(const IExceptionTranslator* translateFunction) {
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

    void writeStringToStream(std::ostream* stream, const String& str) { *stream << str; }

    void addToContexts(IContextScope* ptr) { getContextState()->contexts.push_back(ptr); }
    void                              popFromContexts() { getContextState()->contexts.pop_back(); }
    void useContextIfExceptionOccurred(IContextScope* ptr) {
        if(std::uncaught_exception()) {
            std::ostringstream stream;
            ptr->build(&stream);
            getContextState()->exceptionalContexts.push_back(stream.str());
        }
    }

    void printSummary();
    void reportFatal(std::string const& message) {
        DOCTEST_LOG_START();

        detail::ContextState* p = getContextState();
        p->numAssertions += p->numAssertionsForCurrentTestcase;
        logTestException(message.c_str(), true);
        logTestEnd();
        p->numFailed++;

        printSummary();
    }

#if !defined(DOCTEST_CONFIG_POSIX_SIGNALS) && !defined(DOCTEST_CONFIG_WINDOWS_SEH)
    struct FatalConditionHandler
    {
        void reset() {}
    };
#else // DOCTEST_CONFIG_POSIX_SIGNALS || DOCTEST_CONFIG_WINDOWS_SEH
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
            for(size_t i = 0; i < sizeof(signalDefs) / sizeof(SignalDefs); ++i) {
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
        static struct sigaction oldSigActions[sizeof(signalDefs) / sizeof(SignalDefs)];
        static stack_t          oldSigStack;
        static char             altStackMem[SIGSTKSZ];

        static void handleSignal(int sig) {
            std::string name = "<unknown signal>";
            for(std::size_t i = 0; i < sizeof(signalDefs) / sizeof(SignalDefs); ++i) {
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
            sigStack.ss_size  = SIGSTKSZ;
            sigStack.ss_flags = 0;
            sigaltstack(&sigStack, &oldSigStack);
            struct sigaction sa = {0};

            sa.sa_handler = handleSignal;
            sa.sa_flags   = SA_ONSTACK;
            for(std::size_t i = 0; i < sizeof(signalDefs) / sizeof(SignalDefs); ++i) {
                sigaction(signalDefs[i].id, &sa, &oldSigActions[i]);
            }
        }

        ~FatalConditionHandler() { reset(); }
        static void reset() {
            if(isSet) {
                // Set signals back to previous values -- hopefully nobody overwrote them in the meantime
                for(std::size_t i = 0; i < sizeof(signalDefs) / sizeof(SignalDefs); ++i) {
                    sigaction(signalDefs[i].id, &oldSigActions[i], 0);
                }
                // Return the old stack
                sigaltstack(&oldSigStack, 0);
                isSet = false;
            }
        }
    };

    bool             FatalConditionHandler::isSet = false;
    struct sigaction FatalConditionHandler::oldSigActions[sizeof(signalDefs) / sizeof(SignalDefs)] =
            {};
    stack_t FatalConditionHandler::oldSigStack           = {};
    char    FatalConditionHandler::altStackMem[SIGSTKSZ] = {};

#endif // DOCTEST_PLATFORM_WINDOWS
#endif // DOCTEST_CONFIG_POSIX_SIGNALS || DOCTEST_CONFIG_WINDOWS_SEH

    // depending on the current options this will remove the path of filenames
    const char* fileForOutput(const char* file) {
        if(getContextState()->no_path_in_filenames) {
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

    // depending on the current options this will substitute the line numbers with 0
    int lineForOutput(int line) {
        if(getContextState()->no_line_numbers)
            return 0;
        return line;
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
        if(sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, 0, 0) != 0) {
            fprintf(stderr, "\n** Call to sysctl failed - unable to determine if debugger is "
                            "active **\n\n");
            return false;
        }
        // We're being debugged if the P_TRACED flag is set.
        return ((info.kp_proc.p_flag & P_TRACED) != 0);
    }
#elif defined(_MSC_VER) || defined(__MINGW32__)
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

    const char* getSeparator() {
        return "===============================================================================\n";
    }

    const char* getTestCaseSeparator() {
        return "== TEST CASE ==================================================================\n";
    }

    void printToDebugConsole(const String& text) {
        if(isDebuggerActive())
            myOutputDebugString(text.c_str());
    }

    void addFailedAssert(assertType::Enum assert_type) {
        if((assert_type & assertType::is_warn) == 0) {
            getContextState()->hasCurrentTestFailed = true;
            getContextState()->numFailedAssertions++;
        }
    }

    void logTestStart(const char* name, const char* file, unsigned line) {
        char loc[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(loc, DOCTEST_COUNTOF(loc), "%s(%d)\n", fileForOutput(file),
                         lineForOutput(line));

        char msg[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(msg, DOCTEST_COUNTOF(msg), "%s\n", name);

        DOCTEST_PRINTF_COLORED(getTestCaseSeparator(), Color::Yellow);
        DOCTEST_PRINTF_COLORED(loc, Color::LightGrey);
        DOCTEST_PRINTF_COLORED(msg, Color::None);

        String                subcaseStuff  = "";
        std::vector<Subcase>& subcasesStack = getContextState()->subcasesStack;
        for(unsigned i = 0; i < subcasesStack.size(); ++i) {
            if(subcasesStack[i].m_signature.m_name[0] != '\0') {
                char subcase[DOCTEST_SNPRINTF_BUFFER_LENGTH];
                DOCTEST_SNPRINTF(subcase, DOCTEST_COUNTOF(loc), "  %s\n",
                                 subcasesStack[i].m_signature.m_name);
                DOCTEST_PRINTF_COLORED(subcase, Color::None);
                subcaseStuff += subcase;
            }
        }

        DOCTEST_PRINTF_COLORED("\n", Color::None);

        printToDebugConsole(String(getTestCaseSeparator()) + loc + msg + subcaseStuff.c_str() +
                            "\n");
    }

    void logTestEnd() {}

    void logTestException(const String& what, bool crash) {
        char msg[DOCTEST_SNPRINTF_BUFFER_LENGTH];

        DOCTEST_SNPRINTF(msg, DOCTEST_COUNTOF(msg), "TEST CASE FAILED!\n");

        char info1[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        char info2[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        info1[0] = 0;
        info2[0] = 0;
        DOCTEST_SNPRINTF(info1, DOCTEST_COUNTOF(info1),
                         crash ? "crashed:\n" : "threw exception:\n");
        DOCTEST_SNPRINTF(info2, DOCTEST_COUNTOF(info2), "  %s\n", what.c_str());

        std::string contextStr;

        ContextState*& cs = getContextState();
        if(cs->exceptionalContexts.size()) {
            contextStr += "with context:\n";
            for(size_t i = cs->exceptionalContexts.size(); i > 0; --i) {
                contextStr += "  ";
                contextStr += cs->exceptionalContexts[i - 1];
                contextStr += "\n";
            }
        }

        DOCTEST_PRINTF_COLORED(msg, Color::Red);
        DOCTEST_PRINTF_COLORED(info1, Color::None);
        DOCTEST_PRINTF_COLORED(info2, Color::Cyan);
        DOCTEST_PRINTF_COLORED(contextStr.c_str(), Color::None);
        DOCTEST_PRINTF_COLORED("\n", Color::None);

        printToDebugConsole(String(msg) + info1 + info2 + contextStr.c_str() + "\n");
    }

    String logContext() {
        std::ostringstream           stream;
        std::vector<IContextScope*>& contexts = getContextState()->contexts;
        if(contexts.size() > 0)
            stream << "with context:\n";
        for(size_t i = 0; i < contexts.size(); ++i) {
            stream << "  ";
            contexts[i]->build(&stream);
            stream << "\n";
        }
        return stream.str().c_str();
    }

    const char* getFailString(assertType::Enum assert_type) {
        if(assert_type & assertType::is_warn)
            return "WARNING";
        if(assert_type & assertType::is_check)
            return "ERROR";
        if(assert_type & assertType::is_require)
            return "FATAL ERROR";
        return "";
    }

    void logAssert(bool passed, const char* decomposition, bool threw, const String& exception,
                   const char* expr, assertType::Enum assert_type, const char* file, int line) {
        char loc[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(loc, DOCTEST_COUNTOF(loc), "%s(%d)", fileForOutput(file),
                         lineForOutput(line));

        char msg[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(msg, DOCTEST_COUNTOF(msg), " %s!\n",
                         passed ? "PASSED" : getFailString(assert_type));

        char info1[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(info1, DOCTEST_COUNTOF(info1), "  %s( %s )\n",
                         getAssertString(assert_type), expr);

        char info2[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        char info3[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        info2[0] = 0;
        info3[0] = 0;
        if(!threw) {
            DOCTEST_SNPRINTF(info2, DOCTEST_COUNTOF(info2), "with expansion:\n");
            DOCTEST_SNPRINTF(info3, DOCTEST_COUNTOF(info3), "  %s( %s )\n",
                             getAssertString(assert_type), decomposition);
        } else {
            DOCTEST_SNPRINTF(info2, DOCTEST_COUNTOF(info2), "threw exception:\n");
            DOCTEST_SNPRINTF(info3, DOCTEST_COUNTOF(info3), "  %s\n", exception.c_str());
        }

        DOCTEST_PRINTF_COLORED(loc, Color::LightGrey);
        DOCTEST_PRINTF_COLORED(msg,
                               passed ? Color::BrightGreen :
                                        (assert_type & assertType::is_warn) ? Color::Yellow :
                                                                              Color::Red);
        DOCTEST_PRINTF_COLORED(info1, Color::Cyan);
        DOCTEST_PRINTF_COLORED(info2, Color::None);
        DOCTEST_PRINTF_COLORED(info3, Color::Cyan);
        String context = logContext();
        DOCTEST_PRINTF_COLORED(context.c_str(), Color::None);
        DOCTEST_PRINTF_COLORED("\n", Color::None);

        printToDebugConsole(String(loc) + msg + info1 + info2 + info3 + context.c_str() + "\n");
    }

    void logAssertThrows(bool threw, const char* expr, assertType::Enum assert_type,
                         const char* file, int line) {
        char loc[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(loc, DOCTEST_COUNTOF(loc), "%s(%d)", fileForOutput(file),
                         lineForOutput(line));

        char msg[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(msg, DOCTEST_COUNTOF(msg), " %s!\n",
                         threw ? "PASSED" : getFailString(assert_type));

        char info1[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(info1, DOCTEST_COUNTOF(info1), "  %s( %s )\n",
                         getAssertString(assert_type), expr);

        char info2[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        info2[0] = 0;

        if(!threw)
            DOCTEST_SNPRINTF(info2, DOCTEST_COUNTOF(info2), "didn't throw at all\n");

        DOCTEST_PRINTF_COLORED(loc, Color::LightGrey);
        DOCTEST_PRINTF_COLORED(msg,
                               threw ? Color::BrightGreen :
                                       (assert_type & assertType::is_warn) ? Color::Yellow :
                                                                             Color::Red);
        DOCTEST_PRINTF_COLORED(info1, Color::Cyan);
        DOCTEST_PRINTF_COLORED(info2, Color::None);
        String context = logContext();
        DOCTEST_PRINTF_COLORED(context.c_str(), Color::None);
        DOCTEST_PRINTF_COLORED("\n", Color::None);

        printToDebugConsole(String(loc) + msg + info1 + info2 + context.c_str() + "\n");
    }

    void logAssertThrowsAs(bool threw, bool threw_as, const char* as, const String& exception,
                           const char* expr, assertType::Enum assert_type, const char* file,
                           int line) {
        char loc[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(loc, DOCTEST_COUNTOF(loc), "%s(%d)", fileForOutput(file),
                         lineForOutput(line));

        char msg[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(msg, DOCTEST_COUNTOF(msg), " %s!\n",
                         threw_as ? "PASSED" : getFailString(assert_type));

        char info1[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(info1, DOCTEST_COUNTOF(info1), "  %s( %s, %s )\n",
                         getAssertString(assert_type), expr, as);

        char info2[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        char info3[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        info2[0] = 0;
        info3[0] = 0;

        if(!threw) {
            DOCTEST_SNPRINTF(info2, DOCTEST_COUNTOF(info2), "didn't throw at all\n");
        } else if(!threw_as) {
            DOCTEST_SNPRINTF(info2, DOCTEST_COUNTOF(info2), "threw a different exception:\n");
            DOCTEST_SNPRINTF(info3, DOCTEST_COUNTOF(info3), "  %s\n", exception.c_str());
        }

        DOCTEST_PRINTF_COLORED(loc, Color::LightGrey);
        DOCTEST_PRINTF_COLORED(msg,
                               threw_as ? Color::BrightGreen :
                                          (assert_type & assertType::is_warn) ? Color::Yellow :
                                                                                Color::Red);
        DOCTEST_PRINTF_COLORED(info1, Color::Cyan);
        DOCTEST_PRINTF_COLORED(info2, Color::None);
        DOCTEST_PRINTF_COLORED(info3, Color::Cyan);
        String context = logContext();
        DOCTEST_PRINTF_COLORED(context.c_str(), Color::None);
        DOCTEST_PRINTF_COLORED("\n", Color::None);

        printToDebugConsole(String(loc) + msg + info1 + info2 + info3 + context.c_str() + "\n");
    }

    void logAssertNothrow(bool threw, const String& exception, const char* expr,
                          assertType::Enum assert_type, const char* file, int line) {
        char loc[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(loc, DOCTEST_COUNTOF(loc), "%s(%d)", fileForOutput(file),
                         lineForOutput(line));

        char msg[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(msg, DOCTEST_COUNTOF(msg), " %s!\n",
                         !threw ? "PASSED" : getFailString(assert_type));

        char info1[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(info1, DOCTEST_COUNTOF(info1), "  %s( %s )\n",
                         getAssertString(assert_type), expr);

        char info2[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        char info3[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        info2[0] = 0;
        info3[0] = 0;
        if(threw) {
            DOCTEST_SNPRINTF(info2, DOCTEST_COUNTOF(info2), "threw exception:\n");
            DOCTEST_SNPRINTF(info3, DOCTEST_COUNTOF(info3), "  %s\n", exception.c_str());
        }

        DOCTEST_PRINTF_COLORED(loc, Color::LightGrey);
        DOCTEST_PRINTF_COLORED(msg,
                               !threw ? Color::BrightGreen :
                                        (assert_type & assertType::is_warn) ? Color::Yellow :
                                                                              Color::Red);
        DOCTEST_PRINTF_COLORED(info1, Color::Cyan);
        DOCTEST_PRINTF_COLORED(info2, Color::None);
        DOCTEST_PRINTF_COLORED(info3, Color::Cyan);
        String context = logContext();
        DOCTEST_PRINTF_COLORED(context.c_str(), Color::None);
        DOCTEST_PRINTF_COLORED("\n", Color::None);

        printToDebugConsole(String(loc) + msg + info1 + info2 + info3 + context.c_str() + "\n");
    }

    ResultBuilder::ResultBuilder(assertType::Enum assert_type, const char* file, int line,
                                 const char* expr, const char* exception_type)
            : m_assert_type(assert_type)
            , m_file(file)
            , m_line(line)
            , m_expr(expr)
            , m_exception_type(exception_type)
            , m_threw(false)
            , m_threw_as(false)
            , m_failed(false) {
        if(m_expr[0] == ' ') // this happens when variadic macros are disabled
            ++m_expr;
    }

    ResultBuilder::~ResultBuilder() {}

    void ResultBuilder::setResult(const Result& res) {
        m_result = res;
        if(m_assert_type & assertType::is_false)
            m_result.invert();
    }

    void ResultBuilder::unexpectedExceptionOccurred() {
        m_threw = true;

        m_exception = translateActiveException();
    }

    bool ResultBuilder::log() {
        if((m_assert_type & assertType::is_warn) == 0)
            DOCTEST_GCS().numAssertionsForCurrentTestcase++;

        if(m_assert_type & assertType::is_throws) {
            m_failed = !m_threw;
        } else if(m_assert_type & assertType::is_throws_as) {
            m_failed = !m_threw_as;
        } else if(m_assert_type & assertType::is_nothrow) {
            m_failed = m_threw;
        } else {
            m_failed = m_result;
        }

        if(m_failed || DOCTEST_GCS().success) {
            DOCTEST_LOG_START();

            if(m_assert_type & assertType::is_throws) {
                logAssertThrows(m_threw, m_expr, m_assert_type, m_file, m_line);
            } else if(m_assert_type & assertType::is_throws_as) {
                logAssertThrowsAs(m_threw, m_threw_as, m_exception_type, m_exception, m_expr,
                                  m_assert_type, m_file, m_line);
            } else if(m_assert_type & assertType::is_nothrow) {
                logAssertNothrow(m_threw, m_exception, m_expr, m_assert_type, m_file, m_line);
            } else {
                logAssert(m_result.m_passed, m_result.m_decomposition.c_str(), m_threw, m_exception,
                          m_expr, m_assert_type, m_file, m_line);
            }
        }

        if(m_failed) {
            addFailedAssert(m_assert_type);
            if(isDebuggerActive() && !DOCTEST_GCS().no_breaks)
                return true; // should break into the debugger
        }
        return false;
    }

    void ResultBuilder::react() const {
        if(m_failed && checkIfShouldThrow(m_assert_type))
            throwException();
    }

    MessageBuilder::MessageBuilder(const char* file, int line,
                                   doctest::detail::assertType::Enum severity)
            : m_stream(createStream())
            , m_file(file)
            , m_line(line)
            , m_severity(severity) {}

    bool MessageBuilder::log() {
        DOCTEST_LOG_START();

        bool is_warn = m_severity & doctest::detail::assertType::is_warn;

        if(!is_warn)
            getContextState()->hasCurrentTestFailed = true;

        {
            char loc[DOCTEST_SNPRINTF_BUFFER_LENGTH];
            DOCTEST_SNPRINTF(loc, DOCTEST_COUNTOF(loc), "%s(%d)", fileForOutput(m_file),
                             lineForOutput(m_line));
            char msg[DOCTEST_SNPRINTF_BUFFER_LENGTH];
            DOCTEST_SNPRINTF(msg, DOCTEST_COUNTOF(msg), " %s!\n",
                             is_warn ? "MESSAGE" : getFailString(m_severity));

            DOCTEST_PRINTF_COLORED(loc, Color::LightGrey);
            DOCTEST_PRINTF_COLORED(msg, is_warn ? Color::Yellow : Color::Red);

            String info = getStreamResult(m_stream);
            if(info.length()) {
                DOCTEST_PRINTF_COLORED("  ", Color::None);
                DOCTEST_PRINTF_COLORED(info.c_str(), Color::None);
                DOCTEST_PRINTF_COLORED("\n", Color::None);
            }
            String context = logContext();
            DOCTEST_PRINTF_COLORED(context.c_str(), Color::None);
            DOCTEST_PRINTF_COLORED("\n", Color::None);

            printToDebugConsole(String(loc) + msg + "  " + info.c_str() + "\n" + context.c_str() +
                                "\n");
        }

        if(isDebuggerActive() && !DOCTEST_GCS().no_breaks && !is_warn)
            return true; // should break into the debugger
        return false;
    }

    void MessageBuilder::react() {
        if(m_severity & doctest::detail::assertType::is_require)
            throwException();
    }

    MessageBuilder::~MessageBuilder() { freeStream(m_stream); }

    // the implementation of parseFlag()
    bool parseFlagImpl(int argc, const char* const* argv, const char* pattern) {
        for(int i = argc - 1; i >= 0; --i) {
            const char* temp = std::strstr(argv[i], pattern);
            if(temp && my_strlen(temp) == my_strlen(pattern)) {
                // eliminate strings in which the chars before the option are not '-'
                bool noBadCharsFound = true;
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
            if(temp) {
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
                    temp += my_strlen(pattern);
                    unsigned len = my_strlen(temp);
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
#else // DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS
        return parseOptionImpl(argc, argv, pattern, res);
#endif // DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS
    }

    // parses a comma separated list of words after a pattern in one of the arguments in argv
    bool parseCommaSepArgs(int argc, const char* const* argv, const char* pattern,
                           std::vector<String>& res) {
        String filtersString;
        if(parseOption(argc, argv, pattern, filtersString)) {
            // tokenize with "," as a separator
            char* pch = std::strtok(filtersString.c_str(), ","); // modifies the string
            while(pch != 0) {
                if(my_strlen(pch))
                    res.push_back(pch);
                // uses the strtok() internal state to go to the next token
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
        if(parseOption(argc, argv, pattern, parsedValue)) {
            if(type == 0) {
                // boolean
                const char positive[][5] = {"1", "true", "on", "yes"};  // 5 - strlen("true") + 1
                const char negative[][6] = {"0", "false", "off", "no"}; // 6 - strlen("false") + 1

                // if the value matches any of the positive/negative possibilities
                for(unsigned i = 0; i < 4; i++) {
                    if(parsedValue.compare(positive[i], true) == 0) {
                        res = 1;
                        return true;
                    }
                    if(parsedValue.compare(negative[i], true) == 0) {
                        res = 0;
                        return true;
                    }
                }
            } else {
                // integer
                int theInt = std::atoi(parsedValue.c_str());
                if(theInt != 0) {
                    res = theInt;
                    return true;
                }
            }
        }
        return false;
    }

    void printVersion() {
        if(getContextState()->no_version == false) {
            DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
            std::printf("doctest version is \"%s\"\n", DOCTEST_VERSION_STR);
        }
    }

    void printHelp() {
        printVersion();
        DOCTEST_PRINTF_COLORED("[doctest]\n", Color::Cyan);
        DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
        std::printf("boolean values: \"1/on/yes/true\" or \"0/off/no/false\"\n");
        DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
        std::printf("filter  values: \"str1,str2,str3\" (comma separated strings)\n");
        DOCTEST_PRINTF_COLORED("[doctest]\n", Color::Cyan);
        DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
        std::printf("filters use wildcards for matching strings\n");
        DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
        std::printf("something passes a filter if any of the strings in a filter matches\n");
        DOCTEST_PRINTF_COLORED("[doctest]\n", Color::Cyan);
        DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
        std::printf("ALL FLAGS, OPTIONS AND FILTERS ALSO AVAILABLE WITH A \"dt-\" PREFIX!!!\n");
        DOCTEST_PRINTF_COLORED("[doctest]\n", Color::Cyan);
        DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
        std::printf("Query flags - the program quits after them. Available:\n\n");
        std::printf(" -?,   --help, -h                      prints this message\n");
        std::printf(" -v,   --version                       prints the version\n");
        std::printf(" -c,   --count                         prints the number of matching tests\n");
        std::printf(" -ltc, --list-test-cases               lists all matching tests by name\n");
        std::printf(" -lts, --list-test-suites              lists all matching test suites\n\n");
        // ==================================================================================== << 79
        DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
        std::printf("The available <int>/<string> options/filters are:\n\n");
        std::printf(" -tc,  --test-case=<filters>           filters     tests by their name\n");
        std::printf(" -tce, --test-case-exclude=<filters>   filters OUT tests by their name\n");
        std::printf(" -sf,  --source-file=<filters>         filters     tests by their file\n");
        std::printf(" -sfe, --source-file-exclude=<filters> filters OUT tests by their file\n");
        std::printf(
                " -ts,  --test-suite=<filters>          filters     tests by their test suite\n");
        std::printf(
                " -tse, --test-suite-exclude=<filters>  filters OUT tests by their test suite\n");
        std::printf(" -sc,  --subcase=<filters>             filters     subcases by their name\n");
        std::printf(" -sce, --subcase-exclude=<filters>     filters OUT subcases by their name\n");
        std::printf(" -ob,  --order-by=<string>             how the tests should be ordered\n");
        std::printf(
                "                                       <string> - by [file/suite/name/rand]\n");
        std::printf(" -rs,  --rand-seed=<int>               seed for random ordering\n");
        std::printf(
                " -f,   --first=<int>                   the first test passing the filters to\n");
        std::printf("                                       execute - for range-based execution\n");
        std::printf(
                " -l,   --last=<int>                    the last test passing the filters to\n");
        std::printf("                                       execute - for range-based execution\n");
        std::printf(" -aa,  --abort-after=<int>             stop after <int> failed assertions\n");
        std::printf(" -scfl,--subcase-filter-levels=<int>   apply filters for the first <int> "
                    "levels\n");
        DOCTEST_PRINTF_COLORED("\n[doctest] ", Color::Cyan);
        std::printf("Bool options - can be used like flags and true is assumed. Available:\n\n");
        std::printf(
                " -s,   --success=<bool>                include successful assertions in output\n");
        std::printf(
                " -cs,  --case-sensitive=<bool>         filters being treated as case sensitive\n");
        std::printf(" -e,   --exit=<bool>                   exits after the tests finish\n");
        std::printf(
                " -nt,  --no-throw=<bool>               skips exceptions-related assert checks\n");
        std::printf(
                " -ne,  --no-exitcode=<bool>            returns (or exits) always with success\n");
        std::printf(
                " -nr,  --no-run=<bool>                 skips all runtime doctest operations\n");
        std::printf(" -nv,  --no-version=<bool>             omit the framework version in the "
                    "output\n");
        std::printf(" -nc,  --no-colors=<bool>              disables colors in output\n");
        std::printf(" -fc,  --force-colors=<bool>           use colors even when not in a tty\n");
        std::printf(" -nb,  --no-breaks=<bool>              disables breakpoints in debuggers\n");
        std::printf(
                " -npf, --no-path-filenames=<bool>      only filenames and no paths in output\n");
        std::printf(" -nln, --no-line-numbers=<bool>        0 instead of real line numbers in "
                    "output\n");
        // ==================================================================================== << 79

        DOCTEST_PRINTF_COLORED("\n[doctest] ", Color::Cyan);
        std::printf("for more information visit the project documentation\n\n");
    }

    void printSummary() {
        detail::ContextState* p = getContextState();

        DOCTEST_PRINTF_COLORED(getSeparator(), Color::Yellow);
        if(p->count || p->list_test_cases || p->list_test_suites) {
            DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
            std::printf("number of tests passing the current filters: %d\n",
                        p->numTestsPassingFilters);
        } else {
            char buff[DOCTEST_SNPRINTF_BUFFER_LENGTH];

            DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);

            DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), "test cases: %4d",
                             p->numTestsPassingFilters);
            DOCTEST_PRINTF_COLORED(buff, Color::None);
            DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), " | ");
            DOCTEST_PRINTF_COLORED(buff, Color::None);
            DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), "%4d passed",
                             p->numTestsPassingFilters - p->numFailed);
            DOCTEST_PRINTF_COLORED(buff, p->numFailed > 0 ? Color::None : Color::Green);
            DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), " | ");
            DOCTEST_PRINTF_COLORED(buff, Color::None);
            DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), "%4d failed", p->numFailed);
            DOCTEST_PRINTF_COLORED(buff, p->numFailed > 0 ? Color::Red : Color::None);

            DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), " | ");
            DOCTEST_PRINTF_COLORED(buff, Color::None);
            if(p->no_skipped_summary == false) {
                DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), "%4d skipped",
                                 static_cast<unsigned>(getRegisteredTests().size()) -
                                         p->numTestsPassingFilters);
                DOCTEST_PRINTF_COLORED(buff, Color::None);
            }
            DOCTEST_PRINTF_COLORED("\n", Color::None);

            DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);

            DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), "assertions: %4d", p->numAssertions);
            DOCTEST_PRINTF_COLORED(buff, Color::None);
            DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), " | ");
            DOCTEST_PRINTF_COLORED(buff, Color::None);
            DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), "%4d passed",
                             p->numAssertions - p->numFailedAssertions);
            DOCTEST_PRINTF_COLORED(buff, p->numFailed > 0 ? Color::None : Color::Green);
            DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), " | ");
            DOCTEST_PRINTF_COLORED(buff, Color::None);
            DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), "%4d failed", p->numFailedAssertions);
            DOCTEST_PRINTF_COLORED(buff, p->numFailedAssertions > 0 ? Color::Red : Color::None);

            DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), " |\n");
            DOCTEST_PRINTF_COLORED(buff, Color::None);
        }

        // remove any coloring
        DOCTEST_PRINTF_COLORED("", Color::None);
    }
} // namespace detail

bool isRunningInTest() { return detail::getContextState() != 0; }

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
    if(parseIntOption(argc, argv, DOCTEST_STR_CONCAT_TOSTR(name, =), option_bool, intRes) ||       \
       parseIntOption(argc, argv, DOCTEST_STR_CONCAT_TOSTR(sname, =), option_bool, intRes))        \
        p->var = !!intRes;                                                                         \
    else if(parseFlag(argc, argv, #name) || parseFlag(argc, argv, #sname))                         \
        p->var = 1;                                                                                \
    else if(withDefaults)                                                                          \
    p->var = default

#define DOCTEST_PARSE_INT_OPTION(name, sname, var, default)                                        \
    if(parseIntOption(argc, argv, DOCTEST_STR_CONCAT_TOSTR(name, =), option_int, intRes) ||        \
       parseIntOption(argc, argv, DOCTEST_STR_CONCAT_TOSTR(sname, =), option_int, intRes))         \
        p->var = intRes;                                                                           \
    else if(withDefaults)                                                                          \
    p->var = default

#define DOCTEST_PARSE_STR_OPTION(name, sname, var, default)                                        \
    if(parseOption(argc, argv, DOCTEST_STR_CONCAT_TOSTR(name, =), strRes, default) ||              \
       parseOption(argc, argv, DOCTEST_STR_CONCAT_TOSTR(sname, =), strRes, default) ||             \
       withDefaults)                                                                               \
    p->var = strRes

    // clang-format off
    DOCTEST_PARSE_STR_OPTION(dt-order-by, dt-ob, order_by, "file");
    DOCTEST_PARSE_INT_OPTION(dt-rand-seed, dt-rs, rand_seed, 0);

    DOCTEST_PARSE_INT_OPTION(dt-first, dt-f, first, 1);
    DOCTEST_PARSE_INT_OPTION(dt-last, dt-l, last, 0);

    DOCTEST_PARSE_INT_OPTION(dt-abort-after, dt-aa, abort_after, 0);
    DOCTEST_PARSE_INT_OPTION(dt-subcase-filter-levels, dt-scfl, subcase_filter_levels, 2000000000);

    DOCTEST_PARSE_AS_BOOL_OR_FLAG(dt-success, dt-s, success, 0);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG(dt-case-sensitive, dt-cs, case_sensitive, 0);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG(dt-exit, dt-e, exit, 0);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG(dt-no-throw, dt-nt, no_throw, 0);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG(dt-no-exitcode, dt-ne, no_exitcode, 0);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG(dt-no-run, dt-nr, no_run, 0);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG(dt-no-version, dt-nv, no_version, 0);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG(dt-no-colors, dt-nc, no_colors, 0);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG(dt-force-colors, dt-fc, force_colors, 0);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG(dt-no-breaks, dt-nb, no_breaks, 0);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG(dt-no-path-filenames, dt-npf, no_path_in_filenames, 0);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG(dt-no-line-numbers, dt-nln, no_line_numbers, 0);
    DOCTEST_PARSE_AS_BOOL_OR_FLAG(dt-no-skipped-summary, dt-no-skipped-summary, no_skipped_summary, 0);
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

    getContextState() = p;
    p->resetRunData();

    // handle version, help and no_run
    if(p->no_run || p->version || p->help) {
        if(p->version)
            printVersion();
        if(p->help)
            printHelp();

        getContextState() = 0;

        return EXIT_SUCCESS;
    }

    printVersion();
    DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
    std::printf("run with \"--help\" for options\n");

    unsigned i = 0; // counter used for loops - here for VC6

    std::set<TestData>& registeredTests = getRegisteredTests();

    std::vector<const TestData*> testArray;
    for(std::set<TestData>::iterator it = registeredTests.begin(); it != registeredTests.end();
        ++it)
        testArray.push_back(&(*it));

    // sort the collected records
    if(testArray.size() > 0) {
        if(p->order_by.compare("file", true) == 0) {
            std::qsort(&testArray[0], testArray.size(), sizeof(TestData*), fileOrderComparator);
        } else if(p->order_by.compare("suite", true) == 0) {
            std::qsort(&testArray[0], testArray.size(), sizeof(TestData*), suiteOrderComparator);
        } else if(p->order_by.compare("name", true) == 0) {
            std::qsort(&testArray[0], testArray.size(), sizeof(TestData*), nameOrderComparator);
        } else if(p->order_by.compare("rand", true) == 0) {
            std::srand(p->rand_seed);

            // random_shuffle implementation
            const TestData** first = &testArray[0];
            for(i = testArray.size() - 1; i > 0; --i) {
                int idxToSwap = std::rand() % (i + 1);

                const TestData* temp = first[i];

                first[i]         = first[idxToSwap];
                first[idxToSwap] = temp;
            }
        }
    }

    if(p->list_test_cases) {
        DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
        std::printf("listing all test case names\n");
    }

    std::set<String> testSuitesPassingFilters;
    if(p->list_test_suites) {
        DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
        std::printf("listing all test suites\n");
    }

    // invoke the registered functions if they match the filter criteria (or just count them)
    for(i = 0; i < testArray.size(); i++) {
        const TestData& data = *testArray[i];
        if(!matchesAny(data.m_file, p->filters[0], 1, p->case_sensitive))
            continue;
        if(matchesAny(data.m_file, p->filters[1], 0, p->case_sensitive))
            continue;
        if(!matchesAny(data.m_suite, p->filters[2], 1, p->case_sensitive))
            continue;
        if(matchesAny(data.m_suite, p->filters[3], 0, p->case_sensitive))
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
            std::printf("%s\n", data.m_name);
            continue;
        }

        // print the name of the test suite if not done already and don't execute it
        if(p->list_test_suites) {
            if(testSuitesPassingFilters.count(data.m_suite) == 0) {
                std::printf("%s\n", data.m_suite);
                testSuitesPassingFilters.insert(data.m_suite);
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

            // if logging successful tests - force the start log
            p->hasLoggedCurrentTestStart = false;
            if(p->success)
                DOCTEST_LOG_START();

            bool failed = false;
            p->subcasesPassed.clear();
            do {
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
                    data.m_f();
                    fatalConditionHandler.reset();
                    if(getContextState()->hasCurrentTestFailed)
                        failed = true;
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
                } catch(const TestFailureException&) { failed = true; } catch(...) {
                    DOCTEST_LOG_START();
                    logTestException(translateActiveException());
                    failed = true;
                }
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS

                p->numAssertions += p->numAssertionsForCurrentTestcase;

                // exit this loop if enough assertions have failed
                if(p->abort_after > 0 && p->numFailedAssertions >= p->abort_after)
                    p->subcasesHasSkipped = false;

                // if the start has been logged
                if(p->hasLoggedCurrentTestStart)
                    logTestEnd();
                p->hasLoggedCurrentTestStart = false;

            } while(p->subcasesHasSkipped == true);

            if(failed) // if any subcase has failed - the whole test case has failed
                p->numFailed++;

            // stop executing tests if enough assertions have failed
            if(p->abort_after > 0 && p->numFailedAssertions >= p->abort_after)
                break;
        }
    }

    printSummary();

    getContextState() = 0;

    if(p->numFailed && !p->no_exitcode)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

#ifdef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
int main(int argc, char** argv) { return doctest::Context(argc, argv).run(); }
#endif // DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#endif // DOCTEST_LIBRARY_IMPLEMENTATION
#endif // DOCTEST_CONFIG_IMPLEMENT

#if defined(__clang__)
#pragma clang diagnostic pop
#endif // __clang__

#if defined(__GNUC__) && !defined(__clang__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 6)
#pragma GCC diagnostic pop
#endif // > gcc 4.6
#endif // __GNUC__

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER
