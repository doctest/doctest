#if defined(DOCTEST_CONFIG_IMPLEMENT) || !defined(DOCTEST_SINGLE_HEADER)
#ifndef DOCTEST_LIBRARY_IMPLEMENTATION
#define DOCTEST_LIBRARY_IMPLEMENTATION

#ifndef DOCTEST_SINGLE_HEADER
#include "doctest_fwd.h"
#endif // DOCTEST_SINGLE_HEADER

DOCTEST_CLANG_SUPPRESS_WARNING_PUSH
DOCTEST_CLANG_SUPPRESS_WARNING("-Wunknown-pragmas")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wpadded")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wweak-vtables")
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
DOCTEST_CLANG_SUPPRESS_WARNING("-Wc++98-compat")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wc++98-compat-pedantic")

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
DOCTEST_MSVC_SUPPRESS_WARNING(4626) // assignment operator was implicitly defined as deleted
DOCTEST_MSVC_SUPPRESS_WARNING(5027) // move assignment operator was implicitly defined as deleted
DOCTEST_MSVC_SUPPRESS_WARNING(5026) // move constructor was implicitly defined as deleted
DOCTEST_MSVC_SUPPRESS_WARNING(4625) // copy constructor was implicitly defined as deleted
DOCTEST_MSVC_SUPPRESS_WARNING(4800) // forcing value to bool 'true' or 'false' (performance warning)
// static analysis
DOCTEST_MSVC_SUPPRESS_WARNING(26439) // This kind of function may not throw. Declare it 'noexcept'
DOCTEST_MSVC_SUPPRESS_WARNING(26495) // Always initialize a member variable
DOCTEST_MSVC_SUPPRESS_WARNING(26451) // Arithmetic overflow ...
DOCTEST_MSVC_SUPPRESS_WARNING(26444) // Avoid unnamed objects with custom construction and dtr...

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN

// required includes - will go only in one translation unit!
#include <ctime>
#include <cmath>
#include <climits>
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
#include <map>
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

    // this holds both parameters from the command line and runtime data for tests
    struct ContextState : ContextOptions, TestRunStats, CurrentTestCaseStats
    {
        std::vector<std::vector<String>> filters;

        std::vector<IReporter*> reporters_currently_used;

        const TestCase* currentTest;

        std::vector<IContextScope*> contexts;            // for logging with INFO() and friends
        std::vector<String>         stringifiedContexts; // logging from INFO() due to an exception

        // stuff for subcases
        std::set<SubcaseSignature> subcasesPassed;
        std::set<int>              subcasesEnteredLevels;
        int                        subcasesCurrentLevel;

        void resetRunData() {
            numTestCases                = 0;
            numTestCasesPassingFilters  = 0;
            numTestSuitesPassingFilters = 0;
            numTestCasesFailed          = 0;
            numAsserts                  = 0;
            numAssertsFailed            = 0;
        }

        // cppcheck-suppress uninitMemberVar
        ContextState()
                : filters(9) // 9 different filters total
        {}
    };

    ContextState* g_contextState = 0;
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

int String::compare(const char* other, bool no_case) const {
    if(no_case)
        return detail::stricmp(c_str(), other);
    return std::strcmp(c_str(), other);
}

int String::compare(const String& other, bool no_case) const {
    return compare(other.c_str(), no_case);
}

std::ostream& operator<<(std::ostream& s, const String& in) { return s << in.c_str(); }

namespace detail
{
    void color_to_stream(std::ostream&, Color::Enum) DOCTEST_BRANCH_ON_DISABLED({}, ;)
} // namespace detail

namespace Color
{
    std::ostream& operator<<(std::ostream& s, Color::Enum code) {
        detail::color_to_stream(s, code);
        return s;
    }
} // namespace Color

const char* assertString(assertType::Enum at) {
    DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(
            4062) // enumerator 'x' in switch of enum 'y' is not handled
    switch(at) {  //!OCLINT missing default in switch statements
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

bool SubcaseSignature::operator<(const SubcaseSignature& other) const {
    if(m_line != other.m_line)
        return m_line < other.m_line;
    if(std::strcmp(m_file, other.m_file) != 0)
        return std::strcmp(m_file, other.m_file) < 0;
    return std::strcmp(m_name, other.m_name) < 0;
}

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

String toString(std::nullptr_t) { return "NULL"; }

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

int                         IReporter::get_num_active_contexts() { return 0; }
const IContextScope* const* IReporter::get_active_contexts() { return 0; }
int                         IReporter::get_num_stringified_contexts() { return 0; }
const String*               IReporter::get_stringified_contexts() { return 0; }

int registerReporter(const char*, int, IReporter*) { return 0; }

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
    typedef std::map<std::pair<int, String>, IReporter*> reporterMap;
    reporterMap&                                         getReporters() {
        static reporterMap data;
        return data;
    }

#define DOCTEST_ITERATE_THROUGH_REPORTERS(function, args)                                          \
    for(size_t iii = 0; iii < g_contextState->reporters_currently_used.size(); ++iii)              \
    g_contextState->reporters_currently_used[iii]->function(args)

    TestCase::TestCase(funcType test, const char* file, unsigned line, const TestSuite& test_suite,
                       const char* type, int template_id) {
        m_file              = file;
        m_line              = line;
        m_name              = 0;
        m_test_suite        = test_suite.m_test_suite;
        m_description       = test_suite.m_description;
        m_skip              = test_suite.m_skip;
        m_may_fail          = test_suite.m_may_fail;
        m_should_fail       = test_suite.m_should_fail;
        m_expected_failures = test_suite.m_expected_failures;
        m_timeout           = test_suite.m_timeout;

        m_test        = test;
        m_type        = type;
        m_template_id = template_id;
    }

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

    DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(26434) // hides a non-virtual function
    TestCase& TestCase::operator=(const TestCase& other) {
        m_file              = other.m_file;
        m_line              = other.m_line;
        m_name              = other.m_name;
        m_test_suite        = other.m_test_suite;
        m_description       = other.m_description;
        m_skip              = other.m_skip;
        m_may_fail          = other.m_may_fail;
        m_should_fail       = other.m_should_fail;
        m_expected_failures = other.m_expected_failures;
        m_timeout           = other.m_timeout;

        m_test        = other.m_test;
        m_type        = other.m_type;
        m_template_id = other.m_template_id;
        m_full_name   = other.m_full_name;

        if(m_template_id != -1)
            m_name = m_full_name.c_str();
        return *this;
    }
    DOCTEST_MSVC_SUPPRESS_WARNING_POP

    bool TestCase::operator<(const TestCase& other) const {
        if(m_line != other.m_line)
            return m_line < other.m_line;
        const int file_cmp = std::strcmp(m_file, other.m_file);
        if(file_cmp != 0)
            return file_cmp < 0;
        return m_template_id < other.m_template_id;
    }

    bool checkIfShouldThrow(assertType::Enum at) {
        if(at & assertType::is_require) //!OCLINT bitwise operator in conditional
            return true;

        if((at & assertType::is_check) //!OCLINT bitwise operator in conditional
           && g_contextState->abort_after > 0 &&
           g_contextState->numAssertsFailed >= g_contextState->abort_after)
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
    bool matchesAny(const char* name, const std::vector<String>& filters, bool matchEmpty,
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

    DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH("-Wstrict-aliasing")
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
    DOCTEST_GCC_SUPPRESS_WARNING_POP
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

    Timer g_timer;

    const ContextOptions* getContextOptions() { return g_contextState; }

    Subcase::Subcase(const char* name, const char* file, int line)
            : m_signature(name, file, line)
            , m_entered(false) {
        ContextState* s = g_contextState;

        // if we have already completed it
        if(s->subcasesPassed.count(m_signature) != 0)
            return;

        // check subcase filters
        if(s->subcasesCurrentLevel < s->subcase_filter_levels) {
            if(!matchesAny(m_signature.m_name, s->filters[6], true, s->case_sensitive))
                return;
            if(matchesAny(m_signature.m_name, s->filters[7], false, s->case_sensitive))
                return;
        }

        // if a Subcase on the same level has already been entered
        if(s->subcasesEnteredLevels.count(s->subcasesCurrentLevel) != 0) {
            s->should_reenter = true;
            return;
        }

        s->subcasesEnteredLevels.insert(s->subcasesCurrentLevel++);
        m_entered = true;

        DOCTEST_ITERATE_THROUGH_REPORTERS(subcase_start, m_signature);
    }

    Subcase::Subcase(const Subcase& other)
            : m_signature(other.m_signature.m_name, other.m_signature.m_file,
                          other.m_signature.m_line)
            , m_entered(other.m_entered) {}

    Subcase::~Subcase() {
        if(m_entered) {
            ContextState* s = g_contextState;

            s->subcasesCurrentLevel--;
            // only mark the subcase as passed if no subcases have been skipped
            if(s->should_reenter == false)
                s->subcasesPassed.insert(m_signature);

            DOCTEST_ITERATE_THROUGH_REPORTERS(subcase_end, m_signature);
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

#ifdef DOCTEST_CONFIG_COLORS_WINDOWS
    HANDLE g_stdoutHandle;
    WORD   g_origFgAttrs;
    WORD   g_origBgAttrs;
    bool   g_attrsInitted = false;

    int colors_init() {
        if(!g_attrsInitted) {
            g_stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
            g_attrsInitted = true;
            CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
            GetConsoleScreenBufferInfo(g_stdoutHandle, &csbiInfo);
            g_origFgAttrs = csbiInfo.wAttributes & ~(BACKGROUND_GREEN | BACKGROUND_RED |
                                                     BACKGROUND_BLUE | BACKGROUND_INTENSITY);
            g_origBgAttrs = csbiInfo.wAttributes & ~(FOREGROUND_GREEN | FOREGROUND_RED |
                                                     FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        }
        return 0;
    }

    int dumy_init_console_colors = colors_init();
#endif // DOCTEST_CONFIG_COLORS_WINDOWS

    void color_to_stream(std::ostream& s, Color::Enum code) {
        ((void)s);    // for DOCTEST_CONFIG_COLORS_NONE or DOCTEST_CONFIG_COLORS_WINDOWS
        ((void)code); // for DOCTEST_CONFIG_COLORS_NONE
        const ContextState* p = g_contextState;
        if(p->no_colors)
            return;
#ifdef DOCTEST_CONFIG_COLORS_ANSI
        if(isatty(STDOUT_FILENO) == false && p->force_colors == false)
            return;

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
            return;

#define DOCTEST_SET_ATTR(x) SetConsoleTextAttribute(g_stdoutHandle, x | g_origBgAttrs)

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
            default:                 DOCTEST_SET_ATTR(g_origFgAttrs);
        }
            // clang-format on
#endif // DOCTEST_CONFIG_COLORS_WINDOWS
    }

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
    void toStream(std::ostream* s, int long long in) { *s << in; }
    void toStream(std::ostream* s, int long long unsigned in) { *s << in; }

    void addToContexts(IContextScope* ptr) { g_contextState->contexts.push_back(ptr); }
    void popFromContexts() { g_contextState->contexts.pop_back(); }
    DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4996) // std::uncaught_exception is deprecated in C++17
    DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH("-Wdeprecated-declarations")
    void stringifyContextIfExceptionOccurred(IContextScope* ptr) {
        if(std::uncaught_exception()) {
            std::ostringstream s;
            ptr->stringify(&s);
            g_contextState->stringifiedContexts.push_back(s.str().c_str());
        }
    }
    DOCTEST_GCC_SUPPRESS_WARNING_POP
    DOCTEST_MSVC_SUPPRESS_WARNING_POP

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
    void myOutputDebugString(const char* text) { ::OutputDebugStringA(text); }
#else
    // TODO: integration with XCode and other IDEs
    void myOutputDebugString(const char*) {}
#endif // Platform

    void addAssert(assertType::Enum at) {
        if((at & assertType::is_warn) == 0) { //!OCLINT bitwise operator in conditional
            g_contextState->numAsserts++;
            g_contextState->numAssertsForCurrentTestCase++;
        }
    }

    void addFailedAssert(assertType::Enum at) {
        if((at & assertType::is_warn) == 0) { //!OCLINT bitwise operator in conditional
            g_contextState->numAssertsFailed++;
            g_contextState->numAssertsFailedForCurrentTestCase++;
        }
    }

#if defined(DOCTEST_CONFIG_POSIX_SIGNALS) || defined(DOCTEST_CONFIG_WINDOWS_SEH)
    void reportFatal(const std::string& message) {
        g_contextState->seconds_so_far += g_timer.getElapsedSeconds();
        g_contextState->failure_flags |= TestCaseFailureReason::Crash;
        g_contextState->error_string   = message.c_str();
        g_contextState->should_reenter = false;

        // TODO: end all currently opened subcases...?

        DOCTEST_ITERATE_THROUGH_REPORTERS(test_case_end, *g_contextState);

        g_contextState->numTestCasesFailed++;

        DOCTEST_ITERATE_THROUGH_REPORTERS(test_run_end, *g_contextState);
    }
#endif // DOCTEST_CONFIG_POSIX_SIGNALS || DOCTEST_CONFIG_WINDOWS_SEH

    ResultBuilder::ResultBuilder(assertType::Enum at, const char* file, int line, const char* expr,
                                 const char* exception_type) {
        m_test_case      = g_contextState->currentTest;
        m_at             = at;
        m_file           = file;
        m_line           = line;
        m_expr           = expr;
        m_failed         = true;
        m_threw          = false;
        m_threw_as       = false;
        m_exception_type = exception_type;
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
        addAssert(m_at);

        if(m_at & assertType::is_throws) { //!OCLINT bitwise operator in conditional
            m_failed = !m_threw;
        } else if(m_at & assertType::is_throws_as) { //!OCLINT bitwise operator in conditional
            m_failed = !m_threw_as;
        } else if(m_at & assertType::is_nothrow) { //!OCLINT bitwise operator in conditional
            m_failed = m_threw;
        }

        DOCTEST_ITERATE_THROUGH_REPORTERS(log_assert, *this);

        if(m_failed)
            addFailedAssert(m_at);

        return m_failed && isDebuggerActive() && !g_contextState->no_breaks; // break into debugger
    }

    void ResultBuilder::react() const {
        if(m_failed && checkIfShouldThrow(m_at))
            throwException();
    }

    MessageBuilder::MessageBuilder(const char* file, int line, assertType::Enum severity) {
        m_stream   = createStream();
        m_file     = file;
        m_line     = line;
        m_severity = severity;
    }

    bool MessageBuilder::log() {
        m_string = getStreamResult(m_stream);
        DOCTEST_ITERATE_THROUGH_REPORTERS(log_message, *this);

        const bool isWarn = m_severity & assertType::is_warn;

        // warn is just a message in this context so we dont treat it as an assert
        if(!isWarn) {
            addAssert(m_severity);
            addFailedAssert(m_severity);
        }

        return isDebuggerActive() && !g_contextState->no_breaks && !isWarn; // break into debugger
    }

    void MessageBuilder::react() {
        if(m_severity & assertType::is_require) //!OCLINT bitwise operator in conditional
            throwException();
    }

    MessageBuilder::~MessageBuilder() { freeStream(m_stream); }

    struct ConsoleReporter : public IReporter
    {
        std::ostream&                 s;
        bool                          hasLoggedCurrentTestStart;
        std::vector<SubcaseSignature> subcasesStack;

        // caching pointers to objects of these types - safe to do
        const ContextOptions* opt;
        const TestCaseData*   tc;

        ConsoleReporter(std::ostream& in)
                : s(in) {}

        // =========================================================================================
        // WHAT FOLLOWS ARE HELPERS USED BY THE OVERRIDES OF THE VIRTUAL METHODS OF THE INTERFACE
        // =========================================================================================

        void separator_to_stream() {
            s << Color::Yellow
              << "==============================================================================="
                 "\n";
        }

        // depending on the current options this will remove the path of filenames
        const char* file_for_output(const char* file) {
            if(opt->no_path_in_filenames) {
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

        void file_line_to_stream(const char* file, int line, const char* tail = "") {
            s << Color::LightGrey << file_for_output(file) << (opt->gnu_file_line ? ":" : "(")
              << (opt->no_line_numbers ? 0 : line) // 0 or the real num depending on the option
              << (opt->gnu_file_line ? ":" : "):") << tail;
        }

        const char* getSuccessOrFailString(bool success, assertType::Enum at,
                                           const char* success_str) {
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

        Color::Enum getSuccessOrFailColor(bool success, assertType::Enum at) {
            return success ? Color::BrightGreen :
                             (at & assertType::is_warn) ? Color::Yellow : Color::Red;
        }

        void successOrFailColoredStringToStream(bool success, assertType::Enum at,
                                                const char* success_str = "SUCCESS: ") {
            s << getSuccessOrFailColor(success, at)
              << getSuccessOrFailString(success, at, success_str);
        }

        void log_contexts() {
            int num_contexts = get_num_active_contexts();
            if(num_contexts) {
                const IContextScope* const* contexts = get_active_contexts();

                s << Color::None << "  logged: ";
                for(int i = 0; i < num_contexts; ++i) {
                    s << (i == 0 ? "" : "          ");
                    contexts[i]->stringify(&s);
                    s << "\n";
                }
            }

            s << "\n";
        }

        void logTestStart() {
            if(hasLoggedCurrentTestStart)
                return;

            separator_to_stream();
            file_line_to_stream(tc->m_file, tc->m_line, "\n");
            if(tc->m_description)
                s << Color::Yellow << "DESCRIPTION: " << Color::None << tc->m_description << "\n";
            if(tc->m_test_suite && tc->m_test_suite[0] != '\0')
                s << Color::Yellow << "TEST SUITE: " << Color::None << tc->m_test_suite << "\n";
            if(strncmp(tc->m_name, "  Scenario:", 11) != 0)
                s << Color::None << "TEST CASE:  ";
            s << Color::None << tc->m_name << "\n";

            for(unsigned i = 0; i < subcasesStack.size(); ++i)
                if(subcasesStack[i].m_name[0] != '\0')
                    s << "  " << subcasesStack[i].m_name << "\n";

            s << "\n";

            hasLoggedCurrentTestStart = true;
        }

        // =========================================================================================
        // WHAT FOLLOWS ARE OVERRIDES OF THE VIRTUAL METHODS OF THE REPORTER INTERFACE
        // =========================================================================================

        void test_run_start(const ContextOptions& o) DOCTEST_OVERRIDE { opt = &o; }

        void test_run_end(const TestRunStats& p) DOCTEST_OVERRIDE {
            separator_to_stream();

            const bool anythingFailed = p.numTestCasesFailed > 0 || p.numAssertsFailed > 0;
            s << Color::Cyan << "[doctest] " << Color::None << "test cases: " << std::setw(6)
              << p.numTestCasesPassingFilters << " | "
              << ((p.numTestCasesPassingFilters == 0 || anythingFailed) ? Color::None :
                                                                          Color::Green)
              << std::setw(6) << p.numTestCasesPassingFilters - p.numTestCasesFailed << " passed"
              << Color::None << " | " << (p.numTestCasesFailed > 0 ? Color::Red : Color::None)
              << std::setw(6) << p.numTestCasesFailed << " failed" << Color::None << " | ";
            if(opt->no_skipped_summary == false) {
                const int numSkipped = p.numTestCases - p.numTestCasesPassingFilters;
                s << (numSkipped == 0 ? Color::None : Color::Yellow) << std::setw(6) << numSkipped
                  << " skipped" << Color::None;
            }
            s << "\n";
            s << Color::Cyan << "[doctest] " << Color::None << "assertions: " << std::setw(6)
              << p.numAsserts << " | "
              << ((p.numAsserts == 0 || anythingFailed) ? Color::None : Color::Green)
              << std::setw(6) << (p.numAsserts - p.numAssertsFailed) << " passed" << Color::None
              << " | " << (p.numAssertsFailed > 0 ? Color::Red : Color::None) << std::setw(6)
              << p.numAssertsFailed << " failed" << Color::None << " |\n";
            s << Color::Cyan << "[doctest] " << Color::None
              << "Status: " << (p.numTestCasesFailed > 0 ? Color::Red : Color::Green)
              << ((p.numTestCasesFailed > 0) ? "FAILURE!\n" : "SUCCESS!\n") << Color::None;
        }

        void test_case_start(const TestCaseData& in) DOCTEST_OVERRIDE {
            hasLoggedCurrentTestStart = false;
            tc                        = &in;
        }

        void test_case_end(const CurrentTestCaseStats& st) DOCTEST_OVERRIDE {
            // log the preamble of the test case only if there is something
            // else to print - something other than that an assert has failed
            if(opt->duration ||
               (st.failure_flags && st.failure_flags != TestCaseFailureReason::AssertFailure))
                logTestStart();

            // report test case exceptions and crashes
            bool crashed = st.failure_flags & TestCaseFailureReason::Crash;
            if(crashed || (st.failure_flags & TestCaseFailureReason::Exception)) {
                file_line_to_stream(tc->m_file, tc->m_line, " ");
                successOrFailColoredStringToStream(false, crashed ? assertType::is_require :
                                                                    assertType::is_check);
                s << Color::Red << (crashed ? "test case CRASHED: " : "test case THREW exception: ")
                  << Color::Cyan << st.error_string << "\n";

                int num_stringified_contexts = get_num_stringified_contexts();
                if(num_stringified_contexts) {
                    const String* stringified_contexts = get_stringified_contexts();
                    s << Color::None << "  logged: ";
                    for(int i = num_stringified_contexts - 1; i >= 0; --i) {
                        s << (i == num_stringified_contexts - 1 ? "" : "          ")
                          << stringified_contexts[i] << "\n";
                    }
                }
                s << "\n";
            }

            // means the test case will be re-entered because there are untraversed (but discovered) subcases
            if(st.should_reenter)
                return;

            if(opt->duration)
                s << Color::None << std::setprecision(6) << std::fixed << st.seconds_so_far
                  << " s: " << tc->m_name << "\n";

            if(st.failure_flags & TestCaseFailureReason::Timeout)
                s << Color::Red << "Test case exceeded time limit of " << std::setprecision(6)
                  << std::fixed << tc->m_timeout << "!\n";

            if(st.failure_flags & TestCaseFailureReason::ShouldHaveFailedButDidnt) {
                s << Color::Red << "Should have failed but didn't! Marking it as failed!\n";
            } else if(st.failure_flags & TestCaseFailureReason::ShouldHaveFailedAndDid) {
                s << Color::Yellow << "Failed as expected so marking it as not failed\n";
            } else if(st.failure_flags & TestCaseFailureReason::CouldHaveFailedAndDid) {
                s << Color::Yellow << "Allowed to fail so marking it as not failed\n";
            } else if(st.failure_flags & TestCaseFailureReason::DidntFailExactlyNumTimes) {
                s << Color::Red << "Didn't fail exactly " << tc->m_expected_failures
                  << " times so marking it as failed!\n";
            } else if(st.failure_flags & TestCaseFailureReason::FailedExactlyNumTimes) {
                s << Color::Yellow << "Failed exactly " << tc->m_expected_failures
                  << " times as expected so marking it as not failed!\n";
            }
            if(st.failure_flags & TestCaseFailureReason::TooManyFailedAsserts) {
                s << Color::Red << "Aborting - too many failed asserts!\n";
            }
            s << Color::None;
        }

        void subcase_start(const SubcaseSignature& subc) DOCTEST_OVERRIDE {
            subcasesStack.push_back(subc);
            hasLoggedCurrentTestStart = false;
        }

        void subcase_end(const SubcaseSignature& /*subc*/) DOCTEST_OVERRIDE {
            subcasesStack.pop_back();
            hasLoggedCurrentTestStart = false;
        }

        void log_assert(const AssertData& rb) DOCTEST_OVERRIDE {
            if(!rb.m_failed && !opt->success)
                return;

            logTestStart();

            file_line_to_stream(rb.m_file, rb.m_line, " ");
            successOrFailColoredStringToStream(!rb.m_failed, rb.m_at);
            if((rb.m_at & assertType::is_throws_as) == 0) //!OCLINT bitwise operator in conditional
                s << Color::Cyan << assertString(rb.m_at) << "( " << rb.m_expr << " ) "
                  << Color::None;

            if(rb.m_at & assertType::is_throws) { //!OCLINT bitwise operator in conditional
                s << (rb.m_threw ? "threw as expected!" : "did NOT throw at all!") << "\n";
            } else if(rb.m_at &
                      assertType::is_throws_as) { //!OCLINT bitwise operator in conditional
                s << Color::Cyan << assertString(rb.m_at) << "( " << rb.m_expr << ", "
                  << rb.m_exception_type << " ) " << Color::None
                  << (rb.m_threw ? (rb.m_threw_as ? "threw as expected!" :
                                                    "threw a DIFFERENT exception: ") :
                                   "did NOT throw at all!")
                  << Color::Cyan << rb.m_exception << "\n";
            } else if(rb.m_at & assertType::is_nothrow) { //!OCLINT bitwise operator in conditional
                s << (rb.m_threw ? "THREW exception: " : "didn't throw!") << Color::Cyan
                  << rb.m_exception << "\n";
            } else {
                s << (rb.m_threw ? "THREW exception: " :
                                   (!rb.m_failed ? "is correct!\n" : "is NOT correct!\n"));
                if(rb.m_threw)
                    s << rb.m_exception << "\n";
                else
                    s << "  values: " << assertString(rb.m_at) << "( " << rb.m_decomposition
                      << " )\n";
            }

            log_contexts();
        }

        void log_message(const MessageData& mb) DOCTEST_OVERRIDE {
            logTestStart();

            file_line_to_stream(mb.m_file, mb.m_line, " ");
            s << getSuccessOrFailColor(false, mb.m_severity)
              << getSuccessOrFailString(mb.m_severity & assertType::is_warn, mb.m_severity,
                                        "MESSAGE: ");
            s << Color::None << mb.m_string << "\n";
            log_contexts();
        }

        void test_case_skipped(const TestCaseData&) DOCTEST_OVERRIDE {}
    };

    // extension of the console reporter - with a bunch of helpers for the stdout stream redirection
    struct ConsoleReporterWithHelpers : public ConsoleReporter
    {
        ConsoleReporterWithHelpers(std::ostream& in)
                : ConsoleReporter(in) {}

        void printVersion() {
            if(g_contextState->no_version == false)
                s << Color::Cyan << "[doctest] " << Color::None << "doctest version is \""
                  << DOCTEST_VERSION_STR << "\"\n";
        }

        void printIntro() {
            printVersion();
            s << Color::Cyan << "[doctest] " << Color::None << "run with \"--help\" for options\n";
        }

        void printHelp() {
            printVersion();
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
            s << " -lts, --list-test-suites              lists all matching test suites\n";
            s << " -lr,  --list-reporters                lists all registered reporters\n\n";
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
            s << " -r,   --reporters=<filters>           reporters to use (console is default)\n";
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

        void printRegisteredReporters() {
            printVersion();
            s << Color::Cyan << "[doctest] " << Color::None << "listing all registered reporters\n";
            for(reporterMap::iterator it = getReporters().begin(); it != getReporters().end(); ++it)
                s << "priority: " << std::setw(5) << it->first.first
                  << " name: " << it->first.second << "\n";
        }

        void output_query_results() {
            separator_to_stream();
            if(g_contextState->count || g_contextState->list_test_cases) {
                s << Color::Cyan << "[doctest] " << Color::None
                  << "unskipped test cases passing the current filters: "
                  << g_contextState->numTestCasesPassingFilters << "\n";
            } else if(g_contextState->list_test_suites) {
                s << Color::Cyan << "[doctest] " << Color::None
                  << "unskipped test cases passing the current filters: "
                  << g_contextState->numTestCasesPassingFilters << "\n";
                s << Color::Cyan << "[doctest] " << Color::None
                  << "test suites with unskipped test cases passing the current filters: "
                  << g_contextState->numTestSuitesPassingFilters << "\n";
            }
        }

        void output_query_preamble_test_cases() {
            s << Color::Cyan << "[doctest] " << Color::None << "listing all test case names\n";
            separator_to_stream();
        }

        void output_query_preamble_test_suites() {
            s << Color::Cyan << "[doctest] " << Color::None << "listing all test suites\n";
            separator_to_stream();
        }

        void output_c_string_with_newline(const char* str) { s << Color::None << str << "\n"; }
    };

    struct DebugOutputWindowReporter : public ConsoleReporter
    {
        std::ostringstream oss;

        DebugOutputWindowReporter()
                : ConsoleReporter(oss) {}

#define DOCTEST_DEBUG_OUTPUT_WINDOW_REPORTER_OVERRIDE(func, type)                                  \
    void func(type in) DOCTEST_OVERRIDE {                                                          \
        if(isDebuggerActive()) {                                                                   \
            bool with_col             = g_contextState->no_colors;                                 \
            g_contextState->no_colors = false;                                                     \
            ConsoleReporter::func(in);                                                             \
            myOutputDebugString(oss.str().c_str());                                                \
            oss.str("");                                                                           \
            g_contextState->no_colors = with_col;                                                  \
        }                                                                                          \
    }

        DOCTEST_DEBUG_OUTPUT_WINDOW_REPORTER_OVERRIDE(test_run_start, const ContextOptions&)
        DOCTEST_DEBUG_OUTPUT_WINDOW_REPORTER_OVERRIDE(test_run_end, const TestRunStats&)
        DOCTEST_DEBUG_OUTPUT_WINDOW_REPORTER_OVERRIDE(test_case_start, const TestCaseData&)
        DOCTEST_DEBUG_OUTPUT_WINDOW_REPORTER_OVERRIDE(test_case_end, const CurrentTestCaseStats&)
        DOCTEST_DEBUG_OUTPUT_WINDOW_REPORTER_OVERRIDE(subcase_start, const SubcaseSignature&)
        DOCTEST_DEBUG_OUTPUT_WINDOW_REPORTER_OVERRIDE(subcase_end, const SubcaseSignature&)
        DOCTEST_DEBUG_OUTPUT_WINDOW_REPORTER_OVERRIDE(log_assert, const AssertData&)
        DOCTEST_DEBUG_OUTPUT_WINDOW_REPORTER_OVERRIDE(log_message, const MessageData&)
        DOCTEST_DEBUG_OUTPUT_WINDOW_REPORTER_OVERRIDE(test_case_skipped, const TestCaseData&)
    };

    DebugOutputWindowReporter g_debug_output_rep;

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
        if(parseFlagImpl(argc, argv, pattern + 3)) // 3 to skip "dt-"
            return true;
#endif // DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS
        return parseFlagImpl(argc, argv, pattern);
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
        if(parseOptionImpl(argc, argv, pattern + 3, res)) // 3 to skip "dt-"
            return true;
#endif // DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS
        return parseOptionImpl(argc, argv, pattern, res);
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
} // namespace detail

bool isRunningInTest() { return detail::g_contextState != 0; }

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
    parseCommaSepArgs(argc, argv, "dt-reporters=",          p->filters[8]);
    parseCommaSepArgs(argc, argv, "dt-r=",                  p->filters[8]);
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

    DOCTEST_PARSE_INT_OPTION("dt-first", "dt-f", first, 0);
    DOCTEST_PARSE_INT_OPTION("dt-last", "dt-l", last, UINT_MAX);

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

    if(withDefaults) {
        p->help             = false;
        p->version          = false;
        p->count            = false;
        p->list_test_cases  = false;
        p->list_test_suites = false;
        p->list_reporters   = false;
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
    if(parseFlag(argc, argv, "dt-list-reporters") || parseFlag(argc, argv, "dt-lr")) {
        p->list_reporters = true;
        p->exit           = true;
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

    g_contextState = p;
    p->resetRunData();

    ConsoleReporterWithHelpers g_con_rep(std::cout);
    registerReporter("console", 0, &g_con_rep);

    // setup default reporter if none is given through the command line
    p->reporters_currently_used.clear();
    if(p->filters[8].empty())
        p->reporters_currently_used.push_back(getReporters()[reporterMap::key_type(0, "console")]);

    // check to see if any of the registered reporters has been selected
    for(reporterMap::iterator it = getReporters().begin(); it != getReporters().end(); ++it) {
        if(matchesAny(it->first.second.c_str(), p->filters[8], false, p->case_sensitive))
            p->reporters_currently_used.push_back(it->second);
    }

    // always use the debug output window reporter
#ifdef DOCTEST_PLATFORM_WINDOWS
    if(isDebuggerActive())
        p->reporters_currently_used.push_back(&g_debug_output_rep);
#endif // DOCTEST_PLATFORM_WINDOWS

    // handle version, help and no_run
    if(p->no_run || p->version || p->help || p->list_reporters) {
        if(p->version)
            g_con_rep.printVersion();
        if(p->help)
            g_con_rep.printHelp();
        if(p->list_reporters)
            g_con_rep.printRegisteredReporters();

        g_contextState = 0;

        return EXIT_SUCCESS;
    }

    g_con_rep.printIntro();

    std::set<TestCase>& all_tests = getRegisteredTests();
    p->numTestCases               = all_tests.size();

    std::vector<const TestCase*> testArray;
    for(std::set<TestCase>::iterator it = all_tests.begin(); it != all_tests.end(); ++it)
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
            for(size_t i = testArray.size() - 1; i > 0; --i) {
                int idxToSwap = std::rand() % (i + 1); // NOLINT

                const TestCase* temp = first[i];

                first[i]         = first[idxToSwap];
                first[idxToSwap] = temp;
            }
        }
    }

    if(p->list_test_cases)
        g_con_rep.output_query_preamble_test_cases();

    std::set<String> testSuitesPassingFilt;
    if(p->list_test_suites)
        g_con_rep.output_query_preamble_test_suites();

    bool query_mode = p->count || p->list_test_cases || p->list_test_suites;

    if(!query_mode)
        DOCTEST_ITERATE_THROUGH_REPORTERS(test_run_start, *g_contextState);

    // invoke the registered functions if they match the filter criteria (or just count them)
    for(size_t i = 0; i < testArray.size(); i++) {
        const TestCase& tc = *testArray[i];

        bool skip_me = false;
        if(tc.m_skip && !p->no_skip)
            skip_me = true;

        if(!matchesAny(tc.m_file, p->filters[0], true, p->case_sensitive))
            skip_me = true;
        if(matchesAny(tc.m_file, p->filters[1], false, p->case_sensitive))
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
            g_con_rep.output_c_string_with_newline(tc.m_name);
            continue;
        }

        // print the name of the test suite if not done already and don't execute it
        if(p->list_test_suites) {
            if((testSuitesPassingFilt.count(tc.m_test_suite) == 0) && tc.m_test_suite[0] != '\0') {
                g_con_rep.output_c_string_with_newline(tc.m_test_suite);
                testSuitesPassingFilt.insert(tc.m_test_suite);
                p->numTestSuitesPassingFilters++;
            }
            continue;
        }

        // execute the test if it passes all the filtering
        {
            p->currentTest = &tc;

            p->failure_flags                      = TestCaseFailureReason::None;
            p->numAssertsFailedForCurrentTestCase = 0;
            p->numAssertsForCurrentTestCase       = 0;
            p->seconds_so_far                     = 0;
            p->error_string                       = "";

            p->subcasesPassed.clear();
            do {
                // reset some of the fields for subcases (except for the set of fully passed ones)
                p->should_reenter       = false;
                p->subcasesCurrentLevel = 0;
                p->subcasesEnteredLevels.clear();

                // reset stuff for logging with INFO()
                p->stringifiedContexts.clear();

                DOCTEST_ITERATE_THROUGH_REPORTERS(test_case_start, tc);

                g_timer.start();

#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
                try {
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS
                    FatalConditionHandler fatalConditionHandler; // Handle signals
                    // execute the test
                    tc.m_test();
                    fatalConditionHandler.reset();
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
                } catch(const TestFailureException&) {
                    p->failure_flags |= TestCaseFailureReason::AssertFailure;
                } catch(...) {
                    p->error_string = translateActiveException();
                    p->failure_flags |= TestCaseFailureReason::Exception;
                }
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS

                p->seconds_so_far += g_timer.getElapsedSeconds();

                // exit this loop if enough assertions have failed - even if there are more subcases
                if(p->abort_after > 0 && p->numAssertsFailed >= p->abort_after) {
                    p->should_reenter = false;
                    p->failure_flags |= TestCaseFailureReason::TooManyFailedAsserts;
                }

                // call it from here only if we will continue looping for other subcases and
                // call it again outside of the loop for one final time - with updated flags
                if(p->should_reenter == true)
                    DOCTEST_ITERATE_THROUGH_REPORTERS(test_case_end, *g_contextState);
            } while(p->should_reenter == true);

            if(g_contextState->numAssertsFailedForCurrentTestCase)
                p->failure_flags |= TestCaseFailureReason::AssertFailure;

            if(Approx(p->currentTest->m_timeout).epsilon(DBL_EPSILON) != 0 &&
               Approx(p->seconds_so_far).epsilon(DBL_EPSILON) > p->currentTest->m_timeout)
                p->failure_flags |= TestCaseFailureReason::Timeout;

            if(tc.m_should_fail) {
                if(p->failure_flags) {
                    p->failure_flags |= TestCaseFailureReason::ShouldHaveFailedAndDid;
                } else {
                    p->failure_flags |= TestCaseFailureReason::ShouldHaveFailedButDidnt;
                }
            } else if(p->failure_flags && tc.m_may_fail) {
                p->failure_flags |= TestCaseFailureReason::CouldHaveFailedAndDid;
            } else if(tc.m_expected_failures > 0) {
                if(p->numAssertsFailedForCurrentTestCase == tc.m_expected_failures) {
                    p->failure_flags |= TestCaseFailureReason::FailedExactlyNumTimes;
                } else {
                    p->failure_flags |= TestCaseFailureReason::DidntFailExactlyNumTimes;
                }
            }

            bool ok_to_fail = (TestCaseFailureReason::ShouldHaveFailedAndDid & p->failure_flags) ||
                              (TestCaseFailureReason::CouldHaveFailedAndDid & p->failure_flags) ||
                              (TestCaseFailureReason::FailedExactlyNumTimes & p->failure_flags);

            // if any subcase has failed - the whole test case has failed
            if(p->failure_flags && !ok_to_fail)
                p->numTestCasesFailed++;

            DOCTEST_ITERATE_THROUGH_REPORTERS(test_case_end, *g_contextState);

            // stop executing tests if enough assertions have failed
            if(p->abort_after > 0 && p->numAssertsFailed >= p->abort_after)
                break;
        }
    }

    if(!query_mode)
        DOCTEST_ITERATE_THROUGH_REPORTERS(test_run_end, *g_contextState);
    else
        g_con_rep.output_query_results();

    g_contextState = 0;

    if(p->numTestCasesFailed && !p->no_exitcode)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int IReporter::get_num_active_contexts() { return detail::g_contextState->contexts.size(); }
const IContextScope* const* IReporter::get_active_contexts() {
    return get_num_active_contexts() ? &detail::g_contextState->contexts[0] : 0;
}

int IReporter::get_num_stringified_contexts() {
    return detail::g_contextState->stringifiedContexts.size();
}
const String* IReporter::get_stringified_contexts() {
    return get_num_stringified_contexts() ? &detail::g_contextState->stringifiedContexts[0] : 0;
}

int registerReporter(const char* name, int priority, IReporter* r) {
    detail::getReporters().insert(
            detail::reporterMap::value_type(detail::reporterMap::key_type(priority, name), r));
    return 0;
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
