//
// doctest.h - the lightest feature-rich C++ single-header testing framework for unit tests and TDD
//
// Copyright (c) 2016 Viktor Kirilov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
// The documentation can be found at the library's page:
// https://github.com/onqtam/doctest/blob/master/doc/markdown/readme.md
//
// =================================================================================================
// =================================================================================================
// =================================================================================================
//
// The library is heavily influenced by Catch - https://github.com/philsquared/Catch
// which uses the Boost Software License - Version 1.0
// see here - https://github.com/philsquared/Catch/blob/master/LICENSE_1_0.txt
//
// The concept of subcases (sections in Catch) and expression decomposition are from there.
// Some parts of the code are taken directly:
// - stringification - the detection of "ostream& operator<<(ostream&, const T&)" and StringMaker<>
// - the Approx() helper class for floating point comparison
// - colors in the console
// - breaking into a debugger
//
// The expression decomposing templates are taken from lest - https://github.com/martinmoene/lest
// which uses the Boost Software License - Version 1.0
// see here - https://github.com/martinmoene/lest/blob/master/LICENSE_1_0.txt
//
// =================================================================================================
// =================================================================================================
// =================================================================================================

// Suppress this globally (without push/pop) - there is no way to silence it in the
// expression decomposition macros _Pragma() in macros doesn't work for the c++ front-end of g++
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=55578
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=69543
// Also the warning is completely worthless nowadays - http://stackoverflow.com/questions/14016993
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Waggregate-return"
#endif

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#pragma clang diagnostic ignored "-Wshorten-64-to-32"
#pragma clang diagnostic ignored "-Wunused-local-typedef"
#endif // __clang__

#if defined(__GNUC__) && !defined(__clang__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 6)
#pragma GCC diagnostic push
#endif // > gcc 4.6
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wstrict-overflow"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Winline"
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
#pragma warning(disable : 4706) // assignment within conditional expression
#pragma warning(disable : 4512) // 'class' : assignment operator could not be generated
#pragma warning(disable : 4127) // conditional expression is constant
#endif                          // _MSC_VER

#ifndef DOCTEST_LIBRARY_INCLUDED
#define DOCTEST_LIBRARY_INCLUDED

#define DOCTEST_VERSION_MAJOR 1
#define DOCTEST_VERSION_MINOR 1
#define DOCTEST_VERSION_PATCH 4
#define DOCTEST_VERSION_STR "1.1.4"

#define DOCTEST_VERSION                                                                            \
    (DOCTEST_VERSION_MAJOR * 10000 + DOCTEST_VERSION_MINOR * 100 + DOCTEST_VERSION_PATCH)

// =================================================================================================
// == FEATURE DETECTION ============================================================================
// =================================================================================================

#if __cplusplus >= 201103L
#ifndef DOCTEST_CONFIG_WITH_NULLPTR
#define DOCTEST_CONFIG_WITH_NULLPTR
#endif // DOCTEST_CONFIG_WITH_NULLPTR
#ifndef DOCTEST_CONFIG_WITH_LONG_LONG
#define DOCTEST_CONFIG_WITH_LONG_LONG
#endif // DOCTEST_CONFIG_WITH_LONG_LONG
#ifndef DOCTEST_CONFIG_WITH_STATIC_ASSERT
#define DOCTEST_CONFIG_WITH_STATIC_ASSERT
#endif // DOCTEST_CONFIG_WITH_STATIC_ASSERT
#endif // __cplusplus >= 201103L

// nullptr

#ifndef DOCTEST_CONFIG_WITH_NULLPTR
#ifdef __clang__
#if __has_feature(cxx_nullptr)
#define DOCTEST_CONFIG_WITH_NULLPTR
#endif // __has_feature(cxx_nullptr)
#endif // __clang__

#if defined(__GNUC__) && __GNUC__ == 4 && __GNUC_MINOR__ >= 6 && defined(__GXX_EXPERIMENTAL_CXX0X__)
#define DOCTEST_CONFIG_WITH_NULLPTR
#endif // __GNUC__

#if defined(_MSC_VER) && (_MSC_VER >= 1600) // MSVC 2010
#define DOCTEST_CONFIG_WITH_NULLPTR
#endif // _MSC_VER
#endif // DOCTEST_CONFIG_WITH_NULLPTR

#if defined(DOCTEST_CONFIG_NO_NULLPTR) && defined(DOCTEST_CONFIG_WITH_NULLPTR)
#undef DOCTEST_CONFIG_WITH_NULLPTR
#endif // DOCTEST_CONFIG_NO_NULLPTR

// long long

#ifndef DOCTEST_CONFIG_WITH_LONG_LONG
#if !defined(DOCTEST_CONFIG_WITH_LONG_LONG) && defined(_MSC_VER) && (_MSC_VER >= 1400)
#define DOCTEST_CONFIG_WITH_LONG_LONG
#endif // _MSC_VER
#endif // DOCTEST_CONFIG_WITH_LONG_LONG

#if defined(DOCTEST_CONFIG_NO_LONG_LONG) && defined(DOCTEST_CONFIG_WITH_LONG_LONG)
#undef DOCTEST_CONFIG_WITH_LONG_LONG
#endif // DOCTEST_CONFIG_NO_LONG_LONG

// static_assert

#ifndef DOCTEST_CONFIG_WITH_STATIC_ASSERT
#ifdef __clang__
#if __has_feature(cxx_static_assert)
#define DOCTEST_CONFIG_WITH_STATIC_ASSERT
#endif // __has_feature(cxx_static_assert)
#endif // __clang__

#if defined(__GNUC__) && __GNUC__ == 4 && __GNUC_MINOR__ >= 3 && defined(__GXX_EXPERIMENTAL_CXX0X__)
#define DOCTEST_CONFIG_WITH_STATIC_ASSERT
#endif // __GNUC__

#if defined(_MSC_VER) && (_MSC_VER >= 1600) // MSVC 2010
#define DOCTEST_CONFIG_WITH_STATIC_ASSERT
#endif // _MSC_VER
#endif // DOCTEST_CONFIG_WITH_STATIC_ASSERT

#if defined(DOCTEST_CONFIG_NO_STATIC_ASSERT) && defined(DOCTEST_CONFIG_WITH_STATIC_ASSERT)
#undef DOCTEST_CONFIG_WITH_STATIC_ASSERT
#endif // DOCTEST_CONFIG_NO_STATIC_ASSERT

#if defined(DOCTEST_CONFIG_WITH_NULLPTR) || defined(DOCTEST_CONFIG_WITH_LONG_LONG) ||              \
        defined(DOCTEST_CONFIG_WITH_STATIC_ASSERT)
#define DOCTEST_NO_CPP11_COMPAT
#endif // c++11 stuff

#if defined(__clang__) && defined(DOCTEST_NO_CPP11_COMPAT)
#pragma clang diagnostic ignored "-Wc++98-compat"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif // __clang__ && DOCTEST_NO_CPP11_COMPAT

#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
#if defined(__GNUC__) && !defined(__EXCEPTIONS)
#define DOCTEST_CONFIG_NO_EXCEPTIONS
#endif // clang and gcc
// in MSVC _HAS_EXCEPTIONS is defined in a header instead of as a project define
// so we can't do the automatic detection for MSVC without including some header
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS

#ifdef DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
#define DOCTEST_CONFIG_NO_EXCEPTIONS
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS

#if defined(DOCTEST_CONFIG_NO_EXCEPTIONS) && !defined(DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS)
#define DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS && !DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS

#if defined(DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN) && !defined(DOCTEST_CONFIG_IMPLEMENT)
#define DOCTEST_CONFIG_IMPLEMENT
#endif // DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#if defined _WIN32 || defined __CYGWIN__
#ifdef __GNUC__
#define DOCTEST_SYMBOL_EXPORT __attribute__((dllexport))
#define DOCTEST_SYMBOL_IMPORT __attribute__((dllimport))
#else // __GNUC__
#define DOCTEST_SYMBOL_EXPORT __declspec(dllexport)
#define DOCTEST_SYMBOL_IMPORT __declspec(dllimport)
#endif // __GNUC__
#else  // _WIN32
#define DOCTEST_SYMBOL_EXPORT __attribute__((visibility("default")))
#define DOCTEST_SYMBOL_IMPORT
#endif // _WIN32

#ifdef DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#ifdef DOCTEST_CONFIG_IMPLEMENT
#define DOCTEST_INTERFACE DOCTEST_SYMBOL_EXPORT
#else // DOCTEST_CONFIG_IMPLEMENT
#define DOCTEST_INTERFACE DOCTEST_SYMBOL_IMPORT
#endif // DOCTEST_CONFIG_IMPLEMENT
#else // DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#define DOCTEST_INTERFACE
#endif // DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL

// =================================================================================================
// == FEATURE DETECTION END ========================================================================
// =================================================================================================

// internal macros for string concatenation and anonymous variable name generation
#define DOCTEST_CAT_IMPL(s1, s2) s1##s2
#define DOCTEST_CAT(s1, s2) DOCTEST_CAT_IMPL(s1, s2)
#ifdef __COUNTER__ // not standard and may be missing for some compilers
#define DOCTEST_ANONYMOUS(x) DOCTEST_CAT(x, __COUNTER__)
#else // __COUNTER__
#define DOCTEST_ANONYMOUS(x) DOCTEST_CAT(x, __LINE__)
#endif // __COUNTER__

// macro for making a string out of an identifier
#define DOCTEST_TOSTR_IMPL(x) #x
#define DOCTEST_TOSTR(x) DOCTEST_TOSTR_IMPL(x)

// for concatenating literals and making the result a string
#define DOCTEST_STR_CONCAT_TOSTR(s1, s2) DOCTEST_TOSTR(s1) DOCTEST_TOSTR(s2)

// counts the number of elements in a C string
#define DOCTEST_COUNTOF(x) (sizeof(x) / sizeof(x[0]))

#ifndef DOCTEST_CONFIG_ASSERTION_PARAMETERS_BY_VALUE
#define DOCTEST_REF_WRAP(x) x&
#else // DOCTEST_CONFIG_ASSERTION_PARAMETERS_BY_VALUE
#define DOCTEST_REF_WRAP(x) x
#endif // DOCTEST_CONFIG_ASSERTION_PARAMETERS_BY_VALUE

// not using __APPLE__ because... this is how Catch does it
#if defined(__MAC_OS_X_VERSION_MIN_REQUIRED)
#define DOCTEST_PLATFORM_MAC
#elif defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
#define DOCTEST_PLATFORM_IPHONE
#elif defined(_WIN32) || defined(_MSC_VER)
#define DOCTEST_PLATFORM_WINDOWS
#else
#define DOCTEST_PLATFORM_LINUX
#endif

#define DOCTEST_GCS() (*doctest::detail::getTestsContextState())

// should probably take a look at https://github.com/scottt/debugbreak
#ifdef DOCTEST_PLATFORM_MAC
// The following code snippet based on:
// http://cocoawithlove.com/2008/03/break-into-debugger.html
#if defined(__ppc64__) || defined(__ppc__)
#define DOCTEST_BREAK_INTO_DEBUGGER()                                                              \
    __asm__("li r0, 20\nsc\nnop\nli r0, 37\nli r4, 2\nsc\nnop\n" : : : "memory", "r0", "r3", "r4")
#else // __ppc64__ || __ppc__
#define DOCTEST_BREAK_INTO_DEBUGGER() __asm__("int $3\n" : :)
#endif // __ppc64__ || __ppc__
#elif defined(_MSC_VER)
#define DOCTEST_BREAK_INTO_DEBUGGER() __debugbreak()
#elif defined(__MINGW32__)
extern "C" __declspec(dllimport) void __stdcall DebugBreak();
#define DOCTEST_BREAK_INTO_DEBUGGER() ::DebugBreak()
#else // linux
#define DOCTEST_BREAK_INTO_DEBUGGER() ((void)0)
#endif // linux

#define DOCTEST_BREAK_INTO_DEBUGGER_CHECKED()                                                      \
    if(doctest::detail::isDebuggerActive() && !DOCTEST_GCS().no_breaks)                            \
        DOCTEST_BREAK_INTO_DEBUGGER();

#ifdef __clang__
// to detect if libc++ is being used with clang (the _LIBCPP_VERSION identifier)
#include <ciso646>
#endif // __clang__

#ifdef _LIBCPP_VERSION
// not forward declaring ostream for libc++ because I had some problems (inline namespaces vs c++98)
// so the <iosfwd> header is used - also it is very light and doesn't drag a ton of stuff
#include <iosfwd>
#else // _LIBCPP_VERSION
#ifndef DOCTEST_CONFIG_USE_IOSFWD
namespace std
{
template <class charT>
struct char_traits;
template <>
struct char_traits<char>;
template <class charT, class traits>
class basic_ostream;
typedef basic_ostream<char, char_traits<char> > ostream;
}
#else // DOCTEST_CONFIG_USE_IOSFWD
#include <iosfwd>
#endif // DOCTEST_CONFIG_USE_IOSFWD
#endif // _LIBCPP_VERSION

// static assert macro - because of the c++98 support requires that the message is an
// identifier (no spaces and not a C string) - example without quotes: I_am_a_message
// taken from here: http://stackoverflow.com/a/1980156/3162383
#ifdef DOCTEST_CONFIG_WITH_STATIC_ASSERT
#define DOCTEST_STATIC_ASSERT(expression, message) static_assert(expression, #message)
#else // DOCTEST_CONFIG_WITH_STATIC_ASSERT
#define DOCTEST_STATIC_ASSERT(expression, message)                                                 \
    struct DOCTEST_CAT(__static_assertion_at_line_, __LINE__)                                      \
    {                                                                                              \
        doctest::detail::static_assert_impl::StaticAssertion<static_cast<bool>((expression))>      \
                DOCTEST_CAT(DOCTEST_CAT(DOCTEST_CAT(STATIC_ASSERTION_FAILED_AT_LINE_, __LINE__),   \
                                        _),                                                        \
                            message);                                                              \
    };                                                                                             \
    typedef doctest::detail::static_assert_impl::StaticAssertionTest<sizeof(                       \
            DOCTEST_CAT(__static_assertion_at_line_, __LINE__))>                                   \
            DOCTEST_CAT(__static_assertion_test_at_line_, __LINE__)
#endif // DOCTEST_CONFIG_WITH_STATIC_ASSERT

#ifdef DOCTEST_CONFIG_WITH_NULLPTR
#ifdef _LIBCPP_VERSION
#include <cstddef>
#else  // _LIBCPP_VERSION
namespace std
{ typedef decltype(nullptr) nullptr_t; }
#endif // _LIBCPP_VERSION
#endif // DOCTEST_CONFIG_WITH_NULLPTR

// in a separate namespace outside of doctest because the DOCTEST_TEST_SUITE macro 
// introduces an anonymous namespace in which getCurrentTestSuite gets overrided
namespace doctest_detail_test_suite_ns
{
DOCTEST_INTERFACE const char*& getCurrentTestSuite();
} // namespace doctest_detail_test_suite_ns

namespace doctest
{
class DOCTEST_INTERFACE String
{
    char* m_str;

    void copy(const String& other);

public:
    String(const char* in = "");
    String(const String& other);
    ~String();

    String& operator=(const String& other);

    String operator+(const String& other) const;
    String& operator+=(const String& other);

    char& operator[](unsigned pos) { return m_str[pos]; }
    const char& operator[](unsigned pos) const { return m_str[pos]; }

    char*       c_str() { return m_str; }
    const char* c_str() const { return m_str; }

    unsigned size() const;
    unsigned length() const;

    int compare(const char* other, bool no_case = false) const;
    int compare(const String& other, bool no_case = false) const;
};

// clang-format off
inline bool operator==(const String& lhs, const String& rhs) { return lhs.compare(rhs) == 0; }
inline bool operator!=(const String& lhs, const String& rhs) { return lhs.compare(rhs) != 0; }
inline bool operator< (const String& lhs, const String& rhs) { return lhs.compare(rhs) < 0; }
inline bool operator> (const String& lhs, const String& rhs) { return lhs.compare(rhs) > 0; }
inline bool operator<=(const String& lhs, const String& rhs) { return (lhs != rhs) ? lhs.compare(rhs) < 0 : true; }
inline bool operator>=(const String& lhs, const String& rhs) { return (lhs != rhs) ? lhs.compare(rhs) > 0 : true; }
// clang-format on

DOCTEST_INTERFACE std::ostream& operator<<(std::ostream& stream, const String& in);

namespace detail
{
#ifndef DOCTEST_CONFIG_WITH_STATIC_ASSERT
    namespace static_assert_impl
    {
        template <bool>
        struct StaticAssertion;

        template <>
        struct StaticAssertion<true>
        {};

        template <int i>
        struct StaticAssertionTest
        {};
    }  // namespace static_assert_impl
#endif // DOCTEST_CONFIG_WITH_STATIC_ASSERT

    template <typename T>
    struct deferred_false
    { static const bool value = false; };

    namespace has_insertion_operator_impl
    {
        typedef char no;
        typedef char yes[2];

        struct any_t
        {
            template <typename T>
            any_t(const DOCTEST_REF_WRAP(T));
        };

        yes& testStreamable(std::ostream&);
        no   testStreamable(no);

        no operator<<(const std::ostream&, const any_t&);

        template <typename T>
        struct has_insertion_operator
        {
            static std::ostream& s;
            static const DOCTEST_REF_WRAP(T) t;
            static const bool value = sizeof(testStreamable(s << t)) == sizeof(yes);
        };
    } // namespace has_insertion_operator_impl

    template <typename T>
    struct has_insertion_operator : has_insertion_operator_impl::has_insertion_operator<T>
    {};

    DOCTEST_INTERFACE std::ostream* createStream();
    DOCTEST_INTERFACE String        getStreamResult(std::ostream*);
    DOCTEST_INTERFACE void          freeStream(std::ostream*);

    template <bool C>
    struct StringMakerBase
    {
        template <typename T>
        static String convert(const DOCTEST_REF_WRAP(T)) {
            return "{?}";
        }
    };

    template <>
    struct StringMakerBase<true>
    {
        template <typename T>
        static String convert(const DOCTEST_REF_WRAP(T) in) {
            std::ostream* stream = createStream();
            *stream << in;
            String result = getStreamResult(stream);
            freeStream(stream);
            return result;
        }
    };

    DOCTEST_INTERFACE String rawMemoryToString(const void* object, unsigned size);

    template <typename T>
    String rawMemoryToString(const DOCTEST_REF_WRAP(T) object) {
        return rawMemoryToString(&object, sizeof(object));
    }
} // namespace detail

template <typename T>
struct StringMaker : detail::StringMakerBase<detail::has_insertion_operator<T>::value>
{};

template <typename T>
struct StringMaker<T*>
{
    template <typename U>
    static String convert(U* p) {
        if(!p)
            return "NULL";
        else
            return detail::rawMemoryToString(p);
    }
};

template <typename R, typename C>
struct StringMaker<R C::*>
{
    static String convert(R C::*p) {
        if(!p)
            return "NULL";
        else
            return detail::rawMemoryToString(p);
    }
};

template <typename T>
String toString(const DOCTEST_REF_WRAP(T) value) {
    return StringMaker<T>::convert(value);
}

#ifdef DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
DOCTEST_INTERFACE String toString(char* in);
DOCTEST_INTERFACE String toString(const char* in);
#endif // DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
DOCTEST_INTERFACE String toString(bool in);
DOCTEST_INTERFACE String toString(float in);
DOCTEST_INTERFACE String toString(double in);
DOCTEST_INTERFACE String toString(double long in);

DOCTEST_INTERFACE String toString(char in);
DOCTEST_INTERFACE String toString(char unsigned in);
DOCTEST_INTERFACE String toString(int short in);
DOCTEST_INTERFACE String toString(int short unsigned in);
DOCTEST_INTERFACE String toString(int in);
DOCTEST_INTERFACE String toString(int unsigned in);
DOCTEST_INTERFACE String toString(int long in);
DOCTEST_INTERFACE String toString(int long unsigned in);

#ifdef DOCTEST_CONFIG_WITH_LONG_LONG
DOCTEST_INTERFACE String toString(int long long in);
DOCTEST_INTERFACE String toString(int long long unsigned in);
#endif // DOCTEST_CONFIG_WITH_LONG_LONG

#ifdef DOCTEST_CONFIG_WITH_NULLPTR
DOCTEST_INTERFACE String toString(std::nullptr_t in);
#endif // DOCTEST_CONFIG_WITH_NULLPTR

class DOCTEST_INTERFACE Approx
{
public:
    explicit Approx(double value);

    Approx(Approx const& other)
            : m_epsilon(other.m_epsilon)
            , m_scale(other.m_scale)
            , m_value(other.m_value) {}

    Approx operator()(double value) {
        Approx approx(value);
        approx.epsilon(m_epsilon);
        approx.scale(m_scale);
        return approx;
    }

    // clang-format off
    DOCTEST_INTERFACE friend bool operator==(double lhs, Approx const& rhs);
    friend bool operator==(Approx const& lhs, double rhs) { return operator==(rhs, lhs); }
    friend bool operator!=(double lhs, Approx const& rhs) { return !operator==(lhs, rhs); }
    friend bool operator!=(Approx const& lhs, double rhs) { return !operator==(rhs, lhs); }
    friend bool operator<=(double lhs, Approx const& rhs) { return lhs < rhs.m_value || lhs == rhs; }
    friend bool operator<=(Approx const& lhs, double rhs) { return lhs.m_value < rhs || lhs == rhs; }
    friend bool operator>=(double lhs, Approx const& rhs) { return lhs > rhs.m_value || lhs == rhs; }
    friend bool operator>=(Approx const& lhs, double rhs) { return lhs.m_value > rhs || lhs == rhs; }
    friend bool operator< (double lhs, Approx const& rhs) { return lhs < rhs.m_value && lhs != rhs; }
    friend bool operator< (Approx const& lhs, double rhs) { return lhs.m_value < rhs && lhs != rhs; }
    friend bool operator> (double lhs, Approx const& rhs) { return lhs > rhs.m_value && lhs != rhs; }
    friend bool operator> (Approx const& lhs, double rhs) { return lhs.m_value > rhs && lhs != rhs; }
    // clang-format on

    Approx& epsilon(double newEpsilon) {
        m_epsilon = newEpsilon;
        return *this;
    }

    Approx& scale(double newScale) {
        m_scale = newScale;
        return *this;
    }

    String toString() const;

private:
    double m_epsilon;
    double m_scale;
    double m_value;
};

template <>
inline String toString<Approx>(const DOCTEST_REF_WRAP(Approx) value) {
    return value.toString();
}

#if !defined(DOCTEST_CONFIG_DISABLE)

namespace detail
{
    // the function type this library works with
    typedef void (*funcType)(void);

    namespace assertType
    {
        enum Enum
        {
            // macro traits

            is_warn    = 1,
            is_check   = 2,
            is_require = 4,

            is_throws    = 8,
            is_throws_as = 16,
            is_nothrow   = 32,

            is_fast  = 64, // not checked anywhere - used just to distinguish the types
            is_false = 128,
            is_unary = 256,

            is_eq = 512,
            is_ne = 1024,

            is_lt = 2048,
            is_gt = 4096,

            is_ge = 8192,
            is_le = 16384,

            // macro types

            DT_WARN    = is_warn,
            DT_CHECK   = is_check,
            DT_REQUIRE = is_require,

            DT_WARN_FALSE    = is_false | is_warn,
            DT_CHECK_FALSE   = is_false | is_check,
            DT_REQUIRE_FALSE = is_false | is_require,

            DT_WARN_THROWS    = is_throws | is_warn,
            DT_CHECK_THROWS   = is_throws | is_check,
            DT_REQUIRE_THROWS = is_throws | is_require,

            DT_WARN_THROWS_AS    = is_throws_as | is_warn,
            DT_CHECK_THROWS_AS   = is_throws_as | is_check,
            DT_REQUIRE_THROWS_AS = is_throws_as | is_require,

            DT_WARN_NOTHROW    = is_nothrow | is_warn,
            DT_CHECK_NOTHROW   = is_nothrow | is_check,
            DT_REQUIRE_NOTHROW = is_nothrow | is_require,

            DT_WARN_EQ    = is_eq | is_warn,
            DT_CHECK_EQ   = is_eq | is_check,
            DT_REQUIRE_EQ = is_eq | is_require,

            DT_WARN_NE    = is_ne | is_warn,
            DT_CHECK_NE   = is_ne | is_check,
            DT_REQUIRE_NE = is_ne | is_require,

            DT_WARN_GT    = is_gt | is_warn,
            DT_CHECK_GT   = is_gt | is_check,
            DT_REQUIRE_GT = is_gt | is_require,

            DT_WARN_LT    = is_lt | is_warn,
            DT_CHECK_LT   = is_lt | is_check,
            DT_REQUIRE_LT = is_lt | is_require,

            DT_WARN_GE    = is_ge | is_warn,
            DT_CHECK_GE   = is_ge | is_check,
            DT_REQUIRE_GE = is_ge | is_require,

            DT_WARN_LE    = is_le | is_warn,
            DT_CHECK_LE   = is_le | is_check,
            DT_REQUIRE_LE = is_le | is_require,

            DT_WARN_UNARY    = is_unary | is_warn,
            DT_CHECK_UNARY   = is_unary | is_check,
            DT_REQUIRE_UNARY = is_unary | is_require,

            DT_WARN_UNARY_FALSE    = is_false | is_unary | is_warn,
            DT_CHECK_UNARY_FALSE   = is_false | is_unary | is_check,
            DT_REQUIRE_UNARY_FALSE = is_false | is_unary | is_require,

            DT_FAST_WARN_EQ    = is_fast | is_eq | is_warn,
            DT_FAST_CHECK_EQ   = is_fast | is_eq | is_check,
            DT_FAST_REQUIRE_EQ = is_fast | is_eq | is_require,

            DT_FAST_WARN_NE    = is_fast | is_ne | is_warn,
            DT_FAST_CHECK_NE   = is_fast | is_ne | is_check,
            DT_FAST_REQUIRE_NE = is_fast | is_ne | is_require,

            DT_FAST_WARN_GT    = is_fast | is_gt | is_warn,
            DT_FAST_CHECK_GT   = is_fast | is_gt | is_check,
            DT_FAST_REQUIRE_GT = is_fast | is_gt | is_require,

            DT_FAST_WARN_LT    = is_fast | is_lt | is_warn,
            DT_FAST_CHECK_LT   = is_fast | is_lt | is_check,
            DT_FAST_REQUIRE_LT = is_fast | is_lt | is_require,

            DT_FAST_WARN_GE    = is_fast | is_ge | is_warn,
            DT_FAST_CHECK_GE   = is_fast | is_ge | is_check,
            DT_FAST_REQUIRE_GE = is_fast | is_ge | is_require,

            DT_FAST_WARN_LE    = is_fast | is_le | is_warn,
            DT_FAST_CHECK_LE   = is_fast | is_le | is_check,
            DT_FAST_REQUIRE_LE = is_fast | is_le | is_require,

            DT_FAST_WARN_UNARY    = is_fast | is_unary | is_warn,
            DT_FAST_CHECK_UNARY   = is_fast | is_unary | is_check,
            DT_FAST_REQUIRE_UNARY = is_fast | is_unary | is_require,

            DT_FAST_WARN_UNARY_FALSE    = is_fast | is_false | is_unary | is_warn,
            DT_FAST_CHECK_UNARY_FALSE   = is_fast | is_false | is_unary | is_check,
            DT_FAST_REQUIRE_UNARY_FALSE = is_fast | is_false | is_unary | is_require
        };
    } // namespace assertType

    DOCTEST_INTERFACE const char* getAssertString(assertType::Enum val);

    // clang-format off
    template<class T>               struct decay_array       { typedef T type; };
    template<class T, unsigned N>   struct decay_array<T[N]> { typedef T* type; };
    template<class T>               struct decay_array<T[]>  { typedef T* type; };

    template<class T>   struct not_char_pointer              { enum { value = true }; };
    template<>          struct not_char_pointer<char*>       { enum { value = false }; };
    template<>          struct not_char_pointer<const char*> { enum { value = false }; };

    template<class T> struct can_use_op : not_char_pointer<typename decay_array<T>::type> {};

    template<bool, class = void> struct enable_if {};
    template<class T> struct enable_if<true, T> { typedef T type; };
    // clang-format on

    struct TestFailureException
    {};

    DOCTEST_INTERFACE bool checkIfShouldThrow(assertType::Enum assert_type);
    DOCTEST_INTERFACE void fastAssertThrowIfFlagSet(int flags);
    DOCTEST_INTERFACE void throwException();
    DOCTEST_INTERFACE bool always_false();

    // a struct defining a registered test callback
    struct DOCTEST_INTERFACE TestData
    {
        // not used for determining uniqueness
        const char* m_suite; // the test suite in which the test was added
        const char* m_name;  // name of the test function
        funcType    m_f;     // a function pointer to the test function

        // fields by which uniqueness of test cases shall be determined
        const char* m_file; // the file in which the test was registered
        unsigned    m_line; // the line where the test was registered

        TestData(const char* suite, const char* name, funcType f, const char* file, unsigned line)
                : m_suite(suite)
                , m_name(name)
                , m_f(f)
                , m_file(file)
                , m_line(line) {}

        bool operator<(const TestData& other) const;
    };

    struct DOCTEST_INTERFACE SubcaseSignature
    {
        const char* m_name;
        const char* m_file;
        int         m_line;

        SubcaseSignature(const char* name, const char* file, int line)
                : m_name(name)
                , m_file(file)
                , m_line(line) {}

        bool operator<(const SubcaseSignature& other) const;
    };

    struct DOCTEST_INTERFACE Subcase
    {
        SubcaseSignature m_signature;
        bool             m_entered;

        Subcase(const char* name, const char* file, int line);
        Subcase(const Subcase& other);
        ~Subcase();

        operator bool() const { return m_entered; }
    };

    template <typename L, typename R>
    String stringifyBinaryExpr(const DOCTEST_REF_WRAP(L) lhs, const char* op,
                               const DOCTEST_REF_WRAP(R) rhs) {
        return toString(lhs) + op + toString(rhs);
    }

    struct DOCTEST_INTERFACE Result
    {
        bool   m_passed;
        String m_decomposition;

// to fix gcc 4.7 "-Winline" warnings
#if defined(__GNUC__) && !defined(__clang__)
        __attribute__((noinline))
#endif
        ~Result() {
        }

        Result(bool passed = false, const String& decomposition = String())
                : m_passed(passed)
                , m_decomposition(decomposition) {}

        Result(const Result& other)
                : m_passed(other.m_passed)
                , m_decomposition(other.m_decomposition) {}

// to fix gcc 4.7 "-Winline" warnings
#if defined(__GNUC__) && !defined(__clang__)
        __attribute__((noinline))
#endif
        Result&
        operator=(const Result& other) {
            m_passed        = other.m_passed;
            m_decomposition = other.m_decomposition;

            return *this;
        }

        operator bool() { return !m_passed; }

        void invert() { m_passed = !m_passed; }

        // clang-format off
        // forbidding some expressions based on this table: http://en.cppreference.com/w/cpp/language/operator_precedence
        template <typename R> Result operator&  (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return Result(); }
        template <typename R> Result operator^  (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return Result(); }
        template <typename R> Result operator|  (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return Result(); }
        template <typename R> Result operator&& (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return Result(); }
        template <typename R> Result operator|| (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return Result(); }
        template <typename R> Result operator== (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return Result(); }
        template <typename R> Result operator!= (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return Result(); }
        template <typename R> Result operator<  (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return Result(); }
        template <typename R> Result operator>  (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return Result(); }
        template <typename R> Result operator<= (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return Result(); }
        template <typename R> Result operator>= (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return Result(); }
        template <typename R> Result operator=  (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return Result(); }
        template <typename R> Result operator+= (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return Result(); }
        template <typename R> Result operator-= (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return Result(); }
        template <typename R> Result operator*= (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return Result(); }
        template <typename R> Result operator/= (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return Result(); }
        template <typename R> Result operator%= (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return Result(); }
        template <typename R> Result operator<<=(const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return Result(); }
        template <typename R> Result operator>>=(const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return Result(); }
        template <typename R> Result operator&= (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return Result(); }
        template <typename R> Result operator^= (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return Result(); }
        template <typename R> Result operator|= (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return Result(); }
        // clang-format on
    };

#ifndef DOCTEST_CONFIG_NO_COMPARISON_WARNING_SUPPRESSION

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wsign-compare"
#pragma clang diagnostic ignored "-Wdouble-promotion"
//#pragma clang diagnostic ignored "-Wconversion"
//#pragma clang diagnostic ignored "-Wfloat-equal"
#endif // __clang__

#if defined(__GNUC__) && !defined(__clang__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 6)
#pragma GCC diagnostic push
#endif // > gcc 4.6
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wsign-compare"
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 5)
#pragma GCC diagnostic ignored "-Wdouble-promotion"
#endif // > gcc 4.5
//#pragma GCC diagnostic ignored "-Wconversion"
//#pragma GCC diagnostic ignored "-Wfloat-equal"
#endif // __GNUC__

#ifdef _MSC_VER
#pragma warning(push)
// http://stackoverflow.com/questions/39479163 what's the difference between C4018 and C4389
#pragma warning(disable : 4389) // 'operator' : signed/unsigned mismatch
#pragma warning(disable : 4018) // 'expression' : signed/unsigned mismatch
//#pragma warning(disable : 4805) // 'operation' : unsafe mix of type 'type' and type 'type' in operation
#endif // _MSC_VER

#endif // DOCTEST_CONFIG_NO_COMPARISON_WARNING_SUPPRESSION

// clang-format off
#ifndef DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
#define DOCTEST_COMPARISON_RETURN_TYPE bool
#else // DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
#define DOCTEST_COMPARISON_RETURN_TYPE typename enable_if<can_use_op<L>::value || can_use_op<R>::value, bool>::type
    inline bool eq(const char* lhs, const char* rhs) { return String(lhs) == String(rhs); }
    inline bool ne(const char* lhs, const char* rhs) { return String(lhs) != String(rhs); }
    inline bool lt(const char* lhs, const char* rhs) { return String(lhs) <  String(rhs); }
    inline bool gt(const char* lhs, const char* rhs) { return String(lhs) >  String(rhs); }
    inline bool le(const char* lhs, const char* rhs) { return String(lhs) <= String(rhs); }
    inline bool ge(const char* lhs, const char* rhs) { return String(lhs) >= String(rhs); }
#endif // DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING

    template <typename L, typename R> DOCTEST_COMPARISON_RETURN_TYPE eq(const DOCTEST_REF_WRAP(L) lhs, const DOCTEST_REF_WRAP(R) rhs) { return lhs == rhs; }
    template <typename L, typename R> DOCTEST_COMPARISON_RETURN_TYPE ne(const DOCTEST_REF_WRAP(L) lhs, const DOCTEST_REF_WRAP(R) rhs) { return lhs != rhs; }
    template <typename L, typename R> DOCTEST_COMPARISON_RETURN_TYPE lt(const DOCTEST_REF_WRAP(L) lhs, const DOCTEST_REF_WRAP(R) rhs) { return lhs <  rhs; }
    template <typename L, typename R> DOCTEST_COMPARISON_RETURN_TYPE gt(const DOCTEST_REF_WRAP(L) lhs, const DOCTEST_REF_WRAP(R) rhs) { return lhs >  rhs; }
    template <typename L, typename R> DOCTEST_COMPARISON_RETURN_TYPE le(const DOCTEST_REF_WRAP(L) lhs, const DOCTEST_REF_WRAP(R) rhs) { return lhs <= rhs; }
    template <typename L, typename R> DOCTEST_COMPARISON_RETURN_TYPE ge(const DOCTEST_REF_WRAP(L) lhs, const DOCTEST_REF_WRAP(R) rhs) { return lhs >= rhs; }
    // clang-format on

    template <typename L>
    struct Expression_lhs
    {
        L lhs;

        Expression_lhs(L in)
                : lhs(in) {}

        Expression_lhs(const Expression_lhs& other)
                : lhs(other.lhs) {}

        operator Result() { return Result(!!lhs, toString(lhs)); }

// clang-format off
#ifndef DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
        template <typename R> Result operator==(const DOCTEST_REF_WRAP(R) rhs) { return Result(lhs == rhs, stringifyBinaryExpr(lhs, " == ", rhs)); }
        template <typename R> Result operator!=(const DOCTEST_REF_WRAP(R) rhs) { return Result(lhs != rhs, stringifyBinaryExpr(lhs, " != ", rhs)); }
        template <typename R> Result operator< (const DOCTEST_REF_WRAP(R) rhs) { return Result(lhs <  rhs, stringifyBinaryExpr(lhs, " < " , rhs)); }
        template <typename R> Result operator<=(const DOCTEST_REF_WRAP(R) rhs) { return Result(lhs <= rhs, stringifyBinaryExpr(lhs, " <= ", rhs)); }
        template <typename R> Result operator> (const DOCTEST_REF_WRAP(R) rhs) { return Result(lhs >  rhs, stringifyBinaryExpr(lhs, " > " , rhs)); }
        template <typename R> Result operator>=(const DOCTEST_REF_WRAP(R) rhs) { return Result(lhs >= rhs, stringifyBinaryExpr(lhs, " >= ", rhs)); }
#else  // DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
        template <typename R> Result operator==(const DOCTEST_REF_WRAP(R) rhs) { return Result(eq(lhs, rhs), stringifyBinaryExpr(lhs, " == ", rhs)); }
        template <typename R> Result operator!=(const DOCTEST_REF_WRAP(R) rhs) { return Result(ne(lhs, rhs), stringifyBinaryExpr(lhs, " != ", rhs)); }
        template <typename R> Result operator< (const DOCTEST_REF_WRAP(R) rhs) { return Result(lt(lhs, rhs), stringifyBinaryExpr(lhs, " < " , rhs)); }
        template <typename R> Result operator<=(const DOCTEST_REF_WRAP(R) rhs) { return Result(le(lhs, rhs), stringifyBinaryExpr(lhs, " <= ", rhs)); }
        template <typename R> Result operator> (const DOCTEST_REF_WRAP(R) rhs) { return Result(gt(lhs, rhs), stringifyBinaryExpr(lhs, " > " , rhs)); }
        template <typename R> Result operator>=(const DOCTEST_REF_WRAP(R) rhs) { return Result(ge(lhs, rhs), stringifyBinaryExpr(lhs, " >= ", rhs)); }
#endif // DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
        // clang-format on

        // clang-format off
        // forbidding some expressions based on this table: http://en.cppreference.com/w/cpp/language/operator_precedence
        template <typename R> int operator&  (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return int(); }
        template <typename R> int operator^  (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return int(); }
        template <typename R> int operator|  (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return int(); }
        template <typename R> int operator&& (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return int(); }
        template <typename R> int operator|| (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return int(); }
        template <typename R> int operator=  (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return int(); }
        template <typename R> int operator+= (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return int(); }
        template <typename R> int operator-= (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return int(); }
        template <typename R> int operator*= (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return int(); }
        template <typename R> int operator/= (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return int(); }
        template <typename R> int operator%= (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return int(); }
        template <typename R> int operator<<=(const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return int(); }
        template <typename R> int operator>>=(const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return int(); }
        template <typename R> int operator&= (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return int(); }
        template <typename R> int operator^= (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return int(); }
        template <typename R> int operator|= (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison); return int(); }
        // these 2 are unfortunate because they should be allowed - they have higher precedence over the comparisons, but the
        // ExpressionDecomposer class uses the left shift operator to capture the left operand of the binary expression...
        template <typename R> int operator<< (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Please_Surround_The_Left_Shift_Operation_With_Parenthesis); return int(); }
        template <typename R> int operator>> (const R&) { DOCTEST_STATIC_ASSERT(deferred_false<R>::value, Please_Surround_The_Right_Shift_Operation_With_Parenthesis); return int(); }
        // clang-format on
    };

#ifndef DOCTEST_CONFIG_NO_COMPARISON_WARNING_SUPPRESSION

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

#endif // DOCTEST_CONFIG_NO_COMPARISON_WARNING_SUPPRESSION

    struct ExpressionDecomposer
    {
        template <typename L>
        Expression_lhs<const DOCTEST_REF_WRAP(L)> operator<<(const DOCTEST_REF_WRAP(L) operand) {
            return Expression_lhs<const DOCTEST_REF_WRAP(L)>(operand);
        }
    };

    // forward declarations of functions used by the macros
    DOCTEST_INTERFACE int regTest(void (*f)(void), unsigned line, const char* file, const char* name, const char* suite);
    DOCTEST_INTERFACE int setTestSuiteName(const char* name);

    DOCTEST_INTERFACE void addFailedAssert(assertType::Enum assert_type);

    DOCTEST_INTERFACE void logTestStart(const char* name, const char* file, unsigned line);
    DOCTEST_INTERFACE void logTestEnd();

    DOCTEST_INTERFACE void logTestCrashed();
    DOCTEST_INTERFACE void logTestException(const char* what);

    DOCTEST_INTERFACE void logAssert(bool passed, const char* decomposition, bool threw, const char* expr,
                   assertType::Enum assert_type, const char* file, int line);

    DOCTEST_INTERFACE void logAssertThrows(bool threw, const char* expr, assertType::Enum assert_type,
                         const char* file, int line);

    DOCTEST_INTERFACE void logAssertThrowsAs(bool threw, bool threw_as, const char* as, const char* expr,
                           assertType::Enum assert_type, const char* file, int line);

    DOCTEST_INTERFACE void logAssertNothrow(bool threw, const char* expr, assertType::Enum assert_type,
                          const char* file, int line);

    DOCTEST_INTERFACE bool isDebuggerActive();
    DOCTEST_INTERFACE void writeToDebugConsole(const String&);

    struct TestAccessibleContextState
    {
        bool            success;   // include successful assertions in output
        bool            no_throw;  // to skip exceptions-related assertion macros
        bool            no_breaks; // to not break into the debugger
        const TestData* currentTest;
        bool            hasLoggedCurrentTestStart;
        int             numAssertionsForCurrentTestcase;
    };

    struct ContextState;

    TestAccessibleContextState* getTestsContextState();

    namespace binaryAssertComparison
    {
        enum Enum
        {
            eq = 0,
            ne,
            gt,
            lt,
            ge,
            le
        };
    } // namespace binaryAssertComparison

    // clang-format off
    template <int, class L, class R> struct RelationalComparator     { bool operator()(const DOCTEST_REF_WRAP(L),     const DOCTEST_REF_WRAP(R)    ) const { return false;        } };
    template <class L, class R> struct RelationalComparator<0, L, R> { bool operator()(const DOCTEST_REF_WRAP(L) lhs, const DOCTEST_REF_WRAP(R) rhs) const { return eq(lhs, rhs); } };
    template <class L, class R> struct RelationalComparator<1, L, R> { bool operator()(const DOCTEST_REF_WRAP(L) lhs, const DOCTEST_REF_WRAP(R) rhs) const { return ne(lhs, rhs); } };
    template <class L, class R> struct RelationalComparator<2, L, R> { bool operator()(const DOCTEST_REF_WRAP(L) lhs, const DOCTEST_REF_WRAP(R) rhs) const { return gt(lhs, rhs); } };
    template <class L, class R> struct RelationalComparator<3, L, R> { bool operator()(const DOCTEST_REF_WRAP(L) lhs, const DOCTEST_REF_WRAP(R) rhs) const { return lt(lhs, rhs); } };
    template <class L, class R> struct RelationalComparator<4, L, R> { bool operator()(const DOCTEST_REF_WRAP(L) lhs, const DOCTEST_REF_WRAP(R) rhs) const { return ge(lhs, rhs); } };
    template <class L, class R> struct RelationalComparator<5, L, R> { bool operator()(const DOCTEST_REF_WRAP(L) lhs, const DOCTEST_REF_WRAP(R) rhs) const { return le(lhs, rhs); } };
    // clang-format on

    struct DOCTEST_INTERFACE ResultBuilder
    {
        assertType::Enum m_assert_type;
        const char*      m_file;
        int              m_line;
        const char*      m_expr;
        const char*      m_exception_type;

        Result m_result;
        bool   m_threw;
        bool   m_threw_as;
        bool   m_failed;

        ResultBuilder(assertType::Enum assert_type, const char* file, int line, const char* expr,
                      const char* exception_type = "");

// to fix gcc 4.7 "-Winline" warnings
#if defined(__GNUC__) && !defined(__clang__)
        __attribute__((noinline))
#endif
        ~ResultBuilder() {
        }

        void setResult(const Result& res) { m_result = res; }

        template <int comparison, typename L, typename R>
        void          binary_assert(const DOCTEST_REF_WRAP(L) lhs, const DOCTEST_REF_WRAP(R) rhs) {
            m_result.m_passed        = RelationalComparator<comparison, L, R>()(lhs, rhs);
            m_result.m_decomposition = stringifyBinaryExpr(lhs, ", ", rhs);
        }

        template <typename L>
        void unary_assert(const DOCTEST_REF_WRAP(L) val) {
            m_result.m_passed        = !!val;
            m_result.m_decomposition = toString(val);
        }

        bool log();
        void react() const;
    };

    namespace assertAction
    {
        enum Enum
        {
            nothing     = 0,
            dbgbreak    = 1,
            shouldthrow = 2
        };
    } // namespace assertAction

    template <int comparison, typename L, typename R>
    int fast_binary_assert(assertType::Enum assert_type, const char* file, int line,
                           const char* lhs_str, const char* rhs_str, const DOCTEST_REF_WRAP(L) lhs,
                           const DOCTEST_REF_WRAP(R) rhs) {
        String        expr     = String(lhs_str) + ", " + rhs_str;
        const char*   expr_str = expr.c_str();
        ResultBuilder rb(assert_type, file, line, expr_str);

        rb.m_result.m_passed        = RelationalComparator<comparison, L, R>()(lhs, rhs);
        rb.m_result.m_decomposition = stringifyBinaryExpr(lhs, ", ", rhs);

        int res = 0;

        if(rb.log())
            res |= assertAction::dbgbreak;

        if(rb.m_failed && checkIfShouldThrow(assert_type))
            res |= assertAction::shouldthrow;

#ifdef DOCTEST_CONFIG_SUPER_FAST_ASSERTS
        // #########################################################################################
        // IF THE DEBUGGER BREAKS HERE - GO 1 LEVEL UP IN THE CALLSTACK TO SEE THE FAILING ASSERTION
        // THIS IS THE EFFECT OF HAVING 'DOCTEST_CONFIG_SUPER_FAST_ASSERTS' DEFINED
        // #########################################################################################
        if(res & assertAction::dbgbreak)
            DOCTEST_BREAK_INTO_DEBUGGER();
        fastAssertThrowIfFlagSet(res);
#endif // DOCTEST_CONFIG_SUPER_FAST_ASSERTS

        return res;
    }

    template <typename L>
    int fast_unary_assert(assertType::Enum assert_type, const char* file, int line,
                          const char* val_str, const DOCTEST_REF_WRAP(L) val) {
        ResultBuilder rb(assert_type, file, line, val_str);

        rb.m_result.m_passed        = !!val;
        rb.m_result.m_decomposition = toString(val);

        int res = 0;

        if(rb.log())
            res |= assertAction::dbgbreak;

        if(rb.m_failed && checkIfShouldThrow(assert_type))
            res |= assertAction::shouldthrow;

#ifdef DOCTEST_CONFIG_SUPER_FAST_ASSERTS
        // #########################################################################################
        // IF THE DEBUGGER BREAKS HERE - GO 1 LEVEL UP IN THE CALLSTACK TO SEE THE FAILING ASSERTION
        // THIS IS THE EFFECT OF HAVING 'DOCTEST_CONFIG_SUPER_FAST_ASSERTS' DEFINED
        // #########################################################################################
        if(res & assertAction::dbgbreak)
            DOCTEST_BREAK_INTO_DEBUGGER();
        fastAssertThrowIfFlagSet(res);
#endif // DOCTEST_CONFIG_SUPER_FAST_ASSERTS

        return res;
    }
} // namespace detail

#endif // DOCTEST_CONFIG_DISABLE

class DOCTEST_INTERFACE Context
{
#if !defined(DOCTEST_CONFIG_DISABLE)
    detail::ContextState* p;

    void parseArgs(int argc, const char* const* argv, bool withDefaults = false);

#endif // DOCTEST_CONFIG_DISABLE

public:
    Context(int argc = 0, const char* const* argv = 0);

// to fix gcc 4.7 "-Winline" warnings
#if defined(__GNUC__) && !defined(__clang__)
    __attribute__((noinline))
#endif
    ~Context();

    void applyCommandLine(int argc, const char* const* argv);

    void addFilter(const char* filter, const char* value);
    void clearFilters();
    void setOption(const char* option, int value);
    void setOption(const char* option, const char* value);

    bool shouldExit();

    int run();
};

} // namespace doctest

// if registering is not disabled
#if !defined(DOCTEST_CONFIG_DISABLE)

// registers the test by initializing a dummy var with a function
#if defined(__GNUC__) && !defined(__clang__)
#define DOCTEST_REGISTER_FUNCTION(f, name)                                                         \
    static int DOCTEST_ANONYMOUS(_DOCTEST_ANON_VAR_) __attribute__((unused)) =                     \
            doctest::detail::regTest(f, __LINE__, __FILE__, name, doctest_detail_test_suite_ns::getCurrentTestSuite());
#elif defined(__clang__)
#define DOCTEST_REGISTER_FUNCTION(f, name)                                                         \
    _Pragma("clang diagnostic push")                                                               \
            _Pragma("clang diagnostic ignored \"-Wglobal-constructors\"") static int               \
                    DOCTEST_ANONYMOUS(_DOCTEST_ANON_VAR_) =                                        \
                            doctest::detail::regTest(f, __LINE__, __FILE__, name, doctest_detail_test_suite_ns::getCurrentTestSuite()); \
    _Pragma("clang diagnostic pop")
#else // MSVC
#define DOCTEST_REGISTER_FUNCTION(f, name)                                                         \
    static int DOCTEST_ANONYMOUS(_DOCTEST_ANON_VAR_) =                                             \
            doctest::detail::regTest(f, __LINE__, __FILE__, name, doctest_detail_test_suite_ns::getCurrentTestSuite());
#endif // MSVC

#define DOCTEST_IMPLEMENT_FIXTURE(der, base, func, name)                                           \
    namespace                                                                                      \
    {                                                                                              \
        struct der : base                                                                          \
        { void f(); };                                                                             \
        static void func() {                                                                       \
            der v;                                                                                 \
            v.f();                                                                                 \
        }                                                                                          \
        DOCTEST_REGISTER_FUNCTION(func, name)                                                      \
    }                                                                                              \
    inline void der::f()

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION(f, name)                                              \
    static void f();                                                                               \
    DOCTEST_REGISTER_FUNCTION(f, name)                                                             \
    static void f()

// for registering tests
#define DOCTEST_TEST_CASE(name)                                                                    \
    DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(_DOCTEST_ANON_FUNC_), name)

// for registering tests with a fixture
#define DOCTEST_TEST_CASE_FIXTURE(c, name)                                                         \
    DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS(_DOCTEST_ANON_CLASS_), c,                          \
                              DOCTEST_ANONYMOUS(_DOCTEST_ANON_FUNC_), name)

// for subcases
#if defined(__GNUC__)
#define DOCTEST_SUBCASE(name)                                                                      \
    if(const doctest::detail::Subcase & DOCTEST_ANONYMOUS(_DOCTEST_ANON_SUBCASE_)                  \
                                                __attribute__((unused)) =                          \
               doctest::detail::Subcase(name, __FILE__, __LINE__))
#else // __GNUC__
#define DOCTEST_SUBCASE(name)                                                                      \
    if(const doctest::detail::Subcase & DOCTEST_ANONYMOUS(_DOCTEST_ANON_SUBCASE_) =                \
               doctest::detail::Subcase(name, __FILE__, __LINE__))
#endif // __GNUC__

#define DOCTEST_TEST_SUITE_IMPL(name, ns_name)                                                     \
    namespace ns_name {                                                                            \
        namespace doctest_detail_test_suite_ns {                                                   \
            inline const char* getCurrentTestSuite() {                                             \
                return name;                                                                       \
            }                                                                                      \
        }                                                                                          \
    }                                                                                              \
    namespace ns_name

#define DOCTEST_TEST_SUITE(name) DOCTEST_TEST_SUITE_IMPL(name, DOCTEST_ANONYMOUS(_DOCTEST_ANON_SUITE_))

// for starting a testsuite block
#if defined(__GNUC__) && !defined(__clang__)
#define DOCTEST_TEST_SUITE_BEGIN(name)                                                             \
    static int DOCTEST_ANONYMOUS(_DOCTEST_ANON_VAR_) __attribute__((unused)) =                     \
            doctest::detail::setTestSuiteName(name);                                               \
    typedef int DOCTEST_ANONYMOUS(_DOCTEST_ANON_FOR_SEMICOLON_)
#elif defined(__clang__)
#define DOCTEST_TEST_SUITE_BEGIN(name)                                                             \
    _Pragma("clang diagnostic push")                                                               \
            _Pragma("clang diagnostic ignored \"-Wglobal-constructors\"") static int               \
                    DOCTEST_ANONYMOUS(_DOCTEST_ANON_VAR_) =                                        \
                            doctest::detail::setTestSuiteName(name);                               \
    _Pragma("clang diagnostic pop") typedef int DOCTEST_ANONYMOUS(_DOCTEST_ANON_FOR_SEMICOLON_)
#else // MSVC
#define DOCTEST_TEST_SUITE_BEGIN(name)                                                             \
    static int  DOCTEST_ANONYMOUS(_DOCTEST_ANON_VAR_) = doctest::detail::setTestSuiteName(name);   \
    typedef int DOCTEST_ANONYMOUS(_DOCTEST_ANON_FOR_SEMICOLON_)
#endif // MSVC

// for ending a testsuite block
#if defined(__GNUC__) && !defined(__clang__)
#define DOCTEST_TEST_SUITE_END                                                                     \
    static int DOCTEST_ANONYMOUS(_DOCTEST_ANON_VAR_) __attribute__((unused)) =                     \
            doctest::detail::setTestSuiteName("");                                                 \
    typedef int DOCTEST_ANONYMOUS(_DOCTEST_ANON_FOR_SEMICOLON_)
#elif defined(__clang__)
#define DOCTEST_TEST_SUITE_END                                                                                                 \
    _Pragma("clang diagnostic push")                                                                                           \
            _Pragma("clang diagnostic ignored \"-Wglobal-constructors\"") static int                                           \
                                                DOCTEST_ANONYMOUS(_DOCTEST_ANON_VAR_) = doctest::detail::setTestSuiteName(""); \
    _Pragma("clang diagnostic pop") typedef int DOCTEST_ANONYMOUS(_DOCTEST_ANON_FOR_SEMICOLON_)
#else // MSVC
#define DOCTEST_TEST_SUITE_END                                                                     \
    static int  DOCTEST_ANONYMOUS(_DOCTEST_ANON_VAR_) = doctest::detail::setTestSuiteName("");     \
    typedef int DOCTEST_ANONYMOUS(_DOCTEST_ANON_FOR_SEMICOLON_)
#endif // MSVC

#define DOCTEST_ASSERT_LOG_AND_REACT(rb)                                                           \
    if(rb.log())                                                                                   \
        DOCTEST_BREAK_INTO_DEBUGGER();                                                             \
    rb.react()

#ifdef DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS
#define DOCTEST_WRAP_IN_TRY(x) x;
#else // DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS
#define DOCTEST_WRAP_IN_TRY(x)                                                                     \
    try {                                                                                          \
        x;                                                                                         \
    } catch(...) { _DOCTEST_RB.m_threw = true; }
#endif // DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS

#define DOCTEST_ASSERT_IMPLEMENT(expr, assert_type)                                                \
    doctest::detail::ResultBuilder _DOCTEST_RB(doctest::detail::assertType::assert_type, __FILE__, \
                                               __LINE__, #expr);                                   \
    DOCTEST_WRAP_IN_TRY(_DOCTEST_RB.setResult(doctest::detail::ExpressionDecomposer() << expr))    \
    DOCTEST_ASSERT_LOG_AND_REACT(_DOCTEST_RB);

#if defined(__clang__)
#define DOCTEST_ASSERT_PROXY(expr, assert_type)                                                    \
    do {                                                                                           \
        _Pragma("clang diagnostic push")                                                           \
                _Pragma("clang diagnostic ignored \"-Woverloaded-shift-op-parentheses\"")          \
                        DOCTEST_ASSERT_IMPLEMENT(expr, assert_type)                                \
                                _Pragma("clang diagnostic pop")                                    \
    } while(doctest::detail::always_false())
#else // __clang__
#define DOCTEST_ASSERT_PROXY(expr, assert_type)                                                    \
    do {                                                                                           \
        DOCTEST_ASSERT_IMPLEMENT(expr, assert_type)                                                \
    } while(doctest::detail::always_false())
#endif // __clang__

#define DOCTEST_WARN(expr) DOCTEST_ASSERT_PROXY(expr, DT_WARN)
#define DOCTEST_CHECK(expr) DOCTEST_ASSERT_PROXY(expr, DT_CHECK)
#define DOCTEST_REQUIRE(expr) DOCTEST_ASSERT_PROXY(expr, DT_REQUIRE)

#define DOCTEST_WARN_FALSE(expr) DOCTEST_ASSERT_PROXY(expr, DT_WARN_FALSE)
#define DOCTEST_CHECK_FALSE(expr) DOCTEST_ASSERT_PROXY(expr, DT_CHECK_FALSE)
#define DOCTEST_REQUIRE_FALSE(expr) DOCTEST_ASSERT_PROXY(expr, DT_REQUIRE_FALSE)

#define DOCTEST_ASSERT_THROWS(expr, assert_type)                                                   \
    do {                                                                                           \
        if(!DOCTEST_GCS().no_throw) {                                                              \
            doctest::detail::ResultBuilder _DOCTEST_RB(doctest::detail::assertType::assert_type,   \
                                                       __FILE__, __LINE__, #expr);                 \
            try {                                                                                  \
                expr;                                                                              \
            } catch(...) { _DOCTEST_RB.m_threw = true; }                                           \
            DOCTEST_ASSERT_LOG_AND_REACT(_DOCTEST_RB);                                             \
        }                                                                                          \
    } while(doctest::detail::always_false())

#define DOCTEST_ASSERT_THROWS_AS(expr, as, assert_type)                                            \
    do {                                                                                           \
        if(!DOCTEST_GCS().no_throw) {                                                              \
            doctest::detail::ResultBuilder _DOCTEST_RB(doctest::detail::assertType::assert_type,   \
                                                       __FILE__, __LINE__, #expr, #as);            \
            try {                                                                                  \
                expr;                                                                              \
            } catch(as) {                                                                          \
                _DOCTEST_RB.m_threw    = true;                                                     \
                _DOCTEST_RB.m_threw_as = true;                                                     \
            } catch(...) { _DOCTEST_RB.m_threw = true; }                                           \
            DOCTEST_ASSERT_LOG_AND_REACT(_DOCTEST_RB);                                             \
        }                                                                                          \
    } while(doctest::detail::always_false())

#define DOCTEST_ASSERT_NOTHROW(expr, assert_type)                                                  \
    do {                                                                                           \
        if(!DOCTEST_GCS().no_throw) {                                                              \
            doctest::detail::ResultBuilder _DOCTEST_RB(doctest::detail::assertType::assert_type,   \
                                                       __FILE__, __LINE__, #expr);                 \
            try {                                                                                  \
                expr;                                                                              \
            } catch(...) { _DOCTEST_RB.m_threw = true; }                                           \
            DOCTEST_ASSERT_LOG_AND_REACT(_DOCTEST_RB);                                             \
        }                                                                                          \
    } while(doctest::detail::always_false())

#define DOCTEST_WARN_THROWS(expr) DOCTEST_ASSERT_THROWS(expr, DT_WARN_THROWS)
#define DOCTEST_CHECK_THROWS(expr) DOCTEST_ASSERT_THROWS(expr, DT_CHECK_THROWS)
#define DOCTEST_REQUIRE_THROWS(expr) DOCTEST_ASSERT_THROWS(expr, DT_REQUIRE_THROWS)

#define DOCTEST_WARN_THROWS_AS(expr, ex) DOCTEST_ASSERT_THROWS_AS(expr, ex, DT_WARN_THROWS_AS)
#define DOCTEST_CHECK_THROWS_AS(expr, ex) DOCTEST_ASSERT_THROWS_AS(expr, ex, DT_CHECK_THROWS_AS)
#define DOCTEST_REQUIRE_THROWS_AS(expr, ex) DOCTEST_ASSERT_THROWS_AS(expr, ex, DT_REQUIRE_THROWS_AS)

#define DOCTEST_WARN_NOTHROW(expr) DOCTEST_ASSERT_NOTHROW(expr, DT_WARN_NOTHROW)
#define DOCTEST_CHECK_NOTHROW(expr) DOCTEST_ASSERT_NOTHROW(expr, DT_CHECK_NOTHROW)
#define DOCTEST_REQUIRE_NOTHROW(expr) DOCTEST_ASSERT_NOTHROW(expr, DT_REQUIRE_NOTHROW)

#define DOCTEST_BINARY_ASSERT(assert_type, lhs, rhs, comp)                                         \
    do {                                                                                           \
        doctest::detail::ResultBuilder _DOCTEST_RB(doctest::detail::assertType::assert_type,       \
                                                   __FILE__, __LINE__, #lhs ", " #rhs);            \
        DOCTEST_WRAP_IN_TRY(                                                                       \
                _DOCTEST_RB.binary_assert<doctest::detail::binaryAssertComparison::comp>(lhs,      \
                                                                                         rhs))     \
        DOCTEST_ASSERT_LOG_AND_REACT(_DOCTEST_RB);                                                 \
    } while(doctest::detail::always_false())

#define DOCTEST_UNARY_ASSERT(assert_type, val)                                                     \
    do {                                                                                           \
        doctest::detail::ResultBuilder _DOCTEST_RB(doctest::detail::assertType::assert_type,       \
                                                   __FILE__, __LINE__, #val);                      \
        DOCTEST_WRAP_IN_TRY(_DOCTEST_RB.unary_assert(val))                                         \
        DOCTEST_ASSERT_LOG_AND_REACT(_DOCTEST_RB);                                                 \
    } while(doctest::detail::always_false())

#define DOCTEST_WARN_EQ(lhs, rhs) DOCTEST_BINARY_ASSERT(DT_WARN_EQ, lhs, rhs, eq)
#define DOCTEST_CHECK_EQ(lhs, rhs) DOCTEST_BINARY_ASSERT(DT_CHECK_EQ, lhs, rhs, eq)
#define DOCTEST_REQUIRE_EQ(lhs, rhs) DOCTEST_BINARY_ASSERT(DT_REQUIRE_EQ, lhs, rhs, eq)
#define DOCTEST_WARN_NE(lhs, rhs) DOCTEST_BINARY_ASSERT(DT_WARN_NE, lhs, rhs, ne)
#define DOCTEST_CHECK_NE(lhs, rhs) DOCTEST_BINARY_ASSERT(DT_CHECK_NE, lhs, rhs, ne)
#define DOCTEST_REQUIRE_NE(lhs, rhs) DOCTEST_BINARY_ASSERT(DT_REQUIRE_NE, lhs, rhs, ne)
#define DOCTEST_WARN_GT(lhs, rhs) DOCTEST_BINARY_ASSERT(DT_WARN_GT, lhs, rhs, gt)
#define DOCTEST_CHECK_GT(lhs, rhs) DOCTEST_BINARY_ASSERT(DT_CHECK_GT, lhs, rhs, gt)
#define DOCTEST_REQUIRE_GT(lhs, rhs) DOCTEST_BINARY_ASSERT(DT_REQUIRE_GT, lhs, rhs, gt)
#define DOCTEST_WARN_LT(lhs, rhs) DOCTEST_BINARY_ASSERT(DT_WARN_LT, lhs, rhs, lt)
#define DOCTEST_CHECK_LT(lhs, rhs) DOCTEST_BINARY_ASSERT(DT_CHECK_LT, lhs, rhs, lt)
#define DOCTEST_REQUIRE_LT(lhs, rhs) DOCTEST_BINARY_ASSERT(DT_REQUIRE_LT, lhs, rhs, lt)
#define DOCTEST_WARN_GE(lhs, rhs) DOCTEST_BINARY_ASSERT(DT_WARN_GE, lhs, rhs, ge)
#define DOCTEST_CHECK_GE(lhs, rhs) DOCTEST_BINARY_ASSERT(DT_CHECK_GE, lhs, rhs, ge)
#define DOCTEST_REQUIRE_GE(lhs, rhs) DOCTEST_BINARY_ASSERT(DT_REQUIRE_GE, lhs, rhs, ge)
#define DOCTEST_WARN_LE(lhs, rhs) DOCTEST_BINARY_ASSERT(DT_WARN_LE, lhs, rhs, le)
#define DOCTEST_CHECK_LE(lhs, rhs) DOCTEST_BINARY_ASSERT(DT_CHECK_LE, lhs, rhs, le)
#define DOCTEST_REQUIRE_LE(lhs, rhs) DOCTEST_BINARY_ASSERT(DT_REQUIRE_LE, lhs, rhs, le)

#define DOCTEST_WARN_UNARY(v) DOCTEST_UNARY_ASSERT(DT_WARN_UNARY, v)
#define DOCTEST_CHECK_UNARY(v) DOCTEST_UNARY_ASSERT(DT_CHECK_UNARY, v)
#define DOCTEST_REQUIRE_UNARY(v) DOCTEST_UNARY_ASSERT(DT_REQUIRE_UNARY, v)
#define DOCTEST_WARN_UNARY_FALSE(v) DOCTEST_UNARY_ASSERT(DT_WARN_UNARY_FALSE, v)
#define DOCTEST_CHECK_UNARY_FALSE(v) DOCTEST_UNARY_ASSERT(DT_CHECK_UNARY_FALSE, v)
#define DOCTEST_REQUIRE_UNARY_FALSE(v) DOCTEST_UNARY_ASSERT(DT_REQUIRE_UNARY_FALSE, v)

#ifndef DOCTEST_CONFIG_SUPER_FAST_ASSERTS

#define DOCTEST_FAST_BINARY_ASSERT(assert_type, lhs, rhs, comparison)                              \
    do {                                                                                           \
        int _DOCTEST_FAST_RES = doctest::detail::fast_binary_assert<                               \
                doctest::detail::binaryAssertComparison::comparison>(                              \
                doctest::detail::assertType::assert_type, __FILE__, __LINE__, #lhs, #rhs, lhs,     \
                rhs);                                                                              \
        if(_DOCTEST_FAST_RES & doctest::detail::assertAction::dbgbreak)                            \
            DOCTEST_BREAK_INTO_DEBUGGER();                                                         \
        doctest::detail::fastAssertThrowIfFlagSet(_DOCTEST_FAST_RES);                              \
    } while(doctest::detail::always_false())

#define DOCTEST_FAST_UNARY_ASSERT(assert_type, val)                                                \
    do {                                                                                           \
        int _DOCTEST_FAST_RES = doctest::detail::fast_unary_assert(                                \
                doctest::detail::assertType::assert_type, __FILE__, __LINE__, #val, val);          \
        if(_DOCTEST_FAST_RES & doctest::detail::assertAction::dbgbreak)                            \
            DOCTEST_BREAK_INTO_DEBUGGER();                                                         \
        doctest::detail::fastAssertThrowIfFlagSet(_DOCTEST_FAST_RES);                              \
    } while(doctest::detail::always_false())

#else // DOCTEST_CONFIG_SUPER_FAST_ASSERTS

#define DOCTEST_FAST_BINARY_ASSERT(assert_type, lhs, rhs, comparison)                              \
    doctest::detail::fast_binary_assert<doctest::detail::binaryAssertComparison::comparison>(      \
            doctest::detail::assertType::assert_type, __FILE__, __LINE__, #lhs, #rhs, lhs, rhs)

#define DOCTEST_FAST_UNARY_ASSERT(assert_type, val)                                                \
    doctest::detail::fast_unary_assert(doctest::detail::assertType::assert_type, __FILE__,         \
                                       __LINE__, #val, val)

#endif // DOCTEST_CONFIG_SUPER_FAST_ASSERTS

#define DOCTEST_FAST_WARN_EQ(l, r) DOCTEST_FAST_BINARY_ASSERT(DT_FAST_WARN_EQ, l, r, eq)
#define DOCTEST_FAST_CHECK_EQ(l, r) DOCTEST_FAST_BINARY_ASSERT(DT_FAST_CHECK_EQ, l, r, eq)
#define DOCTEST_FAST_REQUIRE_EQ(l, r) DOCTEST_FAST_BINARY_ASSERT(DT_FAST_REQUIRE_EQ, l, r, eq)
#define DOCTEST_FAST_WARN_NE(l, r) DOCTEST_FAST_BINARY_ASSERT(DT_FAST_WARN_NE, l, r, ne)
#define DOCTEST_FAST_CHECK_NE(l, r) DOCTEST_FAST_BINARY_ASSERT(DT_FAST_CHECK_NE, l, r, ne)
#define DOCTEST_FAST_REQUIRE_NE(l, r) DOCTEST_FAST_BINARY_ASSERT(DT_FAST_REQUIRE_NE, l, r, ne)
#define DOCTEST_FAST_WARN_GT(l, r) DOCTEST_FAST_BINARY_ASSERT(DT_FAST_WARN_GT, l, r, gt)
#define DOCTEST_FAST_CHECK_GT(l, r) DOCTEST_FAST_BINARY_ASSERT(DT_FAST_CHECK_GT, l, r, gt)
#define DOCTEST_FAST_REQUIRE_GT(l, r) DOCTEST_FAST_BINARY_ASSERT(DT_FAST_REQUIRE_GT, l, r, gt)
#define DOCTEST_FAST_WARN_LT(l, r) DOCTEST_FAST_BINARY_ASSERT(DT_FAST_WARN_LT, l, r, lt)
#define DOCTEST_FAST_CHECK_LT(l, r) DOCTEST_FAST_BINARY_ASSERT(DT_FAST_CHECK_LT, l, r, lt)
#define DOCTEST_FAST_REQUIRE_LT(l, r) DOCTEST_FAST_BINARY_ASSERT(DT_FAST_REQUIRE_LT, l, r, lt)
#define DOCTEST_FAST_WARN_GE(l, r) DOCTEST_FAST_BINARY_ASSERT(DT_FAST_WARN_GE, l, r, ge)
#define DOCTEST_FAST_CHECK_GE(l, r) DOCTEST_FAST_BINARY_ASSERT(DT_FAST_CHECK_GE, l, r, ge)
#define DOCTEST_FAST_REQUIRE_GE(l, r) DOCTEST_FAST_BINARY_ASSERT(DT_FAST_REQUIRE_GE, l, r, ge)
#define DOCTEST_FAST_WARN_LE(l, r) DOCTEST_FAST_BINARY_ASSERT(DT_FAST_WARN_LE, l, r, le)
#define DOCTEST_FAST_CHECK_LE(l, r) DOCTEST_FAST_BINARY_ASSERT(DT_FAST_CHECK_LE, l, r, le)
#define DOCTEST_FAST_REQUIRE_LE(l, r) DOCTEST_FAST_BINARY_ASSERT(DT_FAST_REQUIRE_LE, l, r, le)

#define DOCTEST_FAST_WARN_UNARY(v) DOCTEST_FAST_UNARY_ASSERT(DT_FAST_WARN_UNARY, v)
#define DOCTEST_FAST_CHECK_UNARY(v) DOCTEST_FAST_UNARY_ASSERT(DT_FAST_CHECK_UNARY, v)
#define DOCTEST_FAST_REQUIRE_UNARY(v) DOCTEST_FAST_UNARY_ASSERT(DT_FAST_REQUIRE_UNARY, v)
#define DOCTEST_FAST_WARN_UNARY_FALSE(v) DOCTEST_FAST_UNARY_ASSERT(DT_FAST_WARN_UNARY_FALSE, v)
#define DOCTEST_FAST_CHECK_UNARY_FALSE(v) DOCTEST_FAST_UNARY_ASSERT(DT_FAST_CHECK_UNARY_FALSE, v)
#define DOCTEST_FAST_REQUIRE_UNARY_FALSE(v)                                                        \
    DOCTEST_FAST_UNARY_ASSERT(DT_FAST_REQUIRE_UNARY_FALSE, v)

#ifdef DOCTEST_CONFIG_NO_EXCEPTIONS

#undef DOCTEST_WARN_THROWS
#undef DOCTEST_CHECK_THROWS
#undef DOCTEST_REQUIRE_THROWS
#undef DOCTEST_WARN_THROWS_AS
#undef DOCTEST_CHECK_THROWS_AS
#undef DOCTEST_REQUIRE_THROWS_AS
#undef DOCTEST_WARN_NOTHROW
#undef DOCTEST_CHECK_NOTHROW
#undef DOCTEST_REQUIRE_NOTHROW

#ifdef DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS

#define DOCTEST_WARN_THROWS(expr) ((void)0)
#define DOCTEST_WARN_THROWS_AS(expr, ex) ((void)0)
#define DOCTEST_WARN_NOTHROW(expr) ((void)0)
#define DOCTEST_CHECK_THROWS(expr) ((void)0)
#define DOCTEST_CHECK_THROWS_AS(expr, ex) ((void)0)
#define DOCTEST_CHECK_NOTHROW(expr) ((void)0)
#define DOCTEST_REQUIRE_THROWS(expr) ((void)0)
#define DOCTEST_REQUIRE_THROWS_AS(expr, ex) ((void)0)
#define DOCTEST_REQUIRE_NOTHROW(expr) ((void)0)

#else // DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS

#undef DOCTEST_REQUIRE
#undef DOCTEST_REQUIRE_FALSE
#undef DOCTEST_REQUIRE_EQ
#undef DOCTEST_REQUIRE_NE
#undef DOCTEST_REQUIRE_GT
#undef DOCTEST_REQUIRE_LT
#undef DOCTEST_REQUIRE_GE
#undef DOCTEST_REQUIRE_LE
#undef DOCTEST_REQUIRE_UNARY
#undef DOCTEST_REQUIRE_UNARY_FALSE
#undef DOCTEST_FAST_REQUIRE_EQ
#undef DOCTEST_FAST_REQUIRE_NE
#undef DOCTEST_FAST_REQUIRE_GT
#undef DOCTEST_FAST_REQUIRE_LT
#undef DOCTEST_FAST_REQUIRE_GE
#undef DOCTEST_FAST_REQUIRE_LE
#undef DOCTEST_FAST_REQUIRE_UNARY
#undef DOCTEST_FAST_REQUIRE_UNARY_FALSE

#endif // DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS

#endif // DOCTEST_CONFIG_NO_EXCEPTIONS

// =================================================================================================
// == WHAT FOLLOWS IS VERSIONS OF THE MACROS THAT DO NOT DO ANY REGISTERING!                      ==
// == THIS CAN BE ENABLED BY DEFINING DOCTEST_CONFIG_DISABLE GLOBALLY!                            ==
// =================================================================================================
#else // DOCTEST_CONFIG_DISABLE

#define DOCTEST_IMPLEMENT_FIXTURE(der, base, func, name)                                           \
    namespace                                                                                      \
    {                                                                                              \
        template <typename T>                                                                      \
        struct der : base                                                                          \
        { void f(); };                                                                             \
    }                                                                                              \
    template <typename T>                                                                          \
    inline void der<T>::f()

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION(f, name)                                              \
    template <typename T>                                                                          \
    static inline void f()

// for registering tests
#define DOCTEST_TEST_CASE(name)                                                                    \
    DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(_DOCTEST_ANON_FUNC_), name)

// for registering tests with a fixture
#define DOCTEST_TEST_CASE_FIXTURE(x, name)                                                         \
    DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS(_DOCTEST_ANON_CLASS_), x,                          \
                              DOCTEST_ANONYMOUS(_DOCTEST_ANON_FUNC_), name)

// for subcases
#define DOCTEST_SUBCASE(name)

// for a testsuite block
#define DOCTEST_TEST_SUITE(name) namespace

// for starting a testsuite block
#define DOCTEST_TEST_SUITE_BEGIN(name) typedef int DOCTEST_ANONYMOUS(_DOCTEST_ANON_FOR_SEMICOLON_)

// for ending a testsuite block
#define DOCTEST_TEST_SUITE_END typedef int DOCTEST_ANONYMOUS(_DOCTEST_ANON_FOR_SEMICOLON_)

#define DOCTEST_WARN(expr) ((void)0)
#define DOCTEST_WARN_FALSE(expr) ((void)0)
#define DOCTEST_WARN_THROWS(expr) ((void)0)
#define DOCTEST_WARN_THROWS_AS(expr, ex) ((void)0)
#define DOCTEST_WARN_NOTHROW(expr) ((void)0)
#define DOCTEST_CHECK(expr) ((void)0)
#define DOCTEST_CHECK_FALSE(expr) ((void)0)
#define DOCTEST_CHECK_THROWS(expr) ((void)0)
#define DOCTEST_CHECK_THROWS_AS(expr, ex) ((void)0)
#define DOCTEST_CHECK_NOTHROW(expr) ((void)0)
#define DOCTEST_REQUIRE(expr) ((void)0)
#define DOCTEST_REQUIRE_FALSE(expr) ((void)0)
#define DOCTEST_REQUIRE_THROWS(expr) ((void)0)
#define DOCTEST_REQUIRE_THROWS_AS(expr, ex) ((void)0)
#define DOCTEST_REQUIRE_NOTHROW(expr) ((void)0)

#define DOCTEST_WARN_EQ(lhs, rhs) ((void)0)
#define DOCTEST_CHECK_EQ(lhs, rhs) ((void)0)
#define DOCTEST_REQUIRE_EQ(lhs, rhs) ((void)0)
#define DOCTEST_WARN_NE(lhs, rhs) ((void)0)
#define DOCTEST_CHECK_NE(lhs, rhs) ((void)0)
#define DOCTEST_REQUIRE_NE(lhs, rhs) ((void)0)
#define DOCTEST_WARN_GT(lhs, rhs) ((void)0)
#define DOCTEST_CHECK_GT(lhs, rhs) ((void)0)
#define DOCTEST_REQUIRE_GT(lhs, rhs) ((void)0)
#define DOCTEST_WARN_LT(lhs, rhs) ((void)0)
#define DOCTEST_CHECK_LT(lhs, rhs) ((void)0)
#define DOCTEST_REQUIRE_LT(lhs, rhs) ((void)0)
#define DOCTEST_WARN_GE(lhs, rhs) ((void)0)
#define DOCTEST_CHECK_GE(lhs, rhs) ((void)0)
#define DOCTEST_REQUIRE_GE(lhs, rhs) ((void)0)
#define DOCTEST_WARN_LE(lhs, rhs) ((void)0)
#define DOCTEST_CHECK_LE(lhs, rhs) ((void)0)
#define DOCTEST_REQUIRE_LE(lhs, rhs) ((void)0)

#define DOCTEST_WARN_UNARY(val) ((void)0)
#define DOCTEST_CHECK_UNARY(val) ((void)0)
#define DOCTEST_REQUIRE_UNARY(val) ((void)0)
#define DOCTEST_WARN_UNARY_FALSE(val) ((void)0)
#define DOCTEST_CHECK_UNARY_FALSE(val) ((void)0)
#define DOCTEST_REQUIRE_UNARY_FALSE(val) ((void)0)

#define DOCTEST_FAST_WARN_EQ(lhs, rhs) ((void)0)
#define DOCTEST_FAST_CHECK_EQ(lhs, rhs) ((void)0)
#define DOCTEST_FAST_REQUIRE_EQ(lhs, rhs) ((void)0)
#define DOCTEST_FAST_WARN_NE(lhs, rhs) ((void)0)
#define DOCTEST_FAST_CHECK_NE(lhs, rhs) ((void)0)
#define DOCTEST_FAST_REQUIRE_NE(lhs, rhs) ((void)0)
#define DOCTEST_FAST_WARN_GT(lhs, rhs) ((void)0)
#define DOCTEST_FAST_CHECK_GT(lhs, rhs) ((void)0)
#define DOCTEST_FAST_REQUIRE_GT(lhs, rhs) ((void)0)
#define DOCTEST_FAST_WARN_LT(lhs, rhs) ((void)0)
#define DOCTEST_FAST_CHECK_LT(lhs, rhs) ((void)0)
#define DOCTEST_FAST_REQUIRE_LT(lhs, rhs) ((void)0)
#define DOCTEST_FAST_WARN_GE(lhs, rhs) ((void)0)
#define DOCTEST_FAST_CHECK_GE(lhs, rhs) ((void)0)
#define DOCTEST_FAST_REQUIRE_GE(lhs, rhs) ((void)0)
#define DOCTEST_FAST_WARN_LE(lhs, rhs) ((void)0)
#define DOCTEST_FAST_CHECK_LE(lhs, rhs) ((void)0)
#define DOCTEST_FAST_REQUIRE_LE(lhs, rhs) ((void)0)

#define DOCTEST_FAST_WARN_UNARY(val) ((void)0)
#define DOCTEST_FAST_CHECK_UNARY(val) ((void)0)
#define DOCTEST_FAST_REQUIRE_UNARY(val) ((void)0)
#define DOCTEST_FAST_WARN_UNARY_FALSE(val) ((void)0)
#define DOCTEST_FAST_CHECK_UNARY_FALSE(val) ((void)0)
#define DOCTEST_FAST_REQUIRE_UNARY_FALSE(val) ((void)0)

#endif // DOCTEST_CONFIG_DISABLE

// BDD style macros
// clang-format off
#define DOCTEST_SCENARIO(name)  TEST_CASE("  Scenario: " name)
#define DOCTEST_GIVEN(name)     SUBCASE("   Given: " name)
#define DOCTEST_WHEN(name)      SUBCASE("    When: " name)
#define DOCTEST_AND_WHEN(name)  SUBCASE("And when: " name)
#define DOCTEST_THEN(name)      SUBCASE("    Then: " name)
#define DOCTEST_AND_THEN(name)  SUBCASE("     And: " name)
// clang-format on

// == SHORT VERSIONS OF THE MACROS
#if !defined(DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES)

#define TEST_CASE DOCTEST_TEST_CASE
#define TEST_CASE_FIXTURE DOCTEST_TEST_CASE_FIXTURE
#define SUBCASE DOCTEST_SUBCASE
#define TEST_SUITE DOCTEST_TEST_SUITE
#define TEST_SUITE_BEGIN DOCTEST_TEST_SUITE_BEGIN
#define TEST_SUITE_END DOCTEST_TEST_SUITE_END
#define WARN DOCTEST_WARN
#define WARN_FALSE DOCTEST_WARN_FALSE
#define WARN_THROWS DOCTEST_WARN_THROWS
#define WARN_THROWS_AS DOCTEST_WARN_THROWS_AS
#define WARN_NOTHROW DOCTEST_WARN_NOTHROW
#define CHECK DOCTEST_CHECK
#define CHECK_FALSE DOCTEST_CHECK_FALSE
#define CHECK_THROWS DOCTEST_CHECK_THROWS
#define CHECK_THROWS_AS DOCTEST_CHECK_THROWS_AS
#define CHECK_NOTHROW DOCTEST_CHECK_NOTHROW
#define REQUIRE DOCTEST_REQUIRE
#define REQUIRE_FALSE DOCTEST_REQUIRE_FALSE
#define REQUIRE_THROWS DOCTEST_REQUIRE_THROWS
#define REQUIRE_THROWS_AS DOCTEST_REQUIRE_THROWS_AS
#define REQUIRE_NOTHROW DOCTEST_REQUIRE_NOTHROW

#define SCENARIO DOCTEST_SCENARIO
#define GIVEN DOCTEST_GIVEN
#define WHEN DOCTEST_WHEN
#define AND_WHEN DOCTEST_AND_WHEN
#define THEN DOCTEST_THEN
#define AND_THEN DOCTEST_AND_THEN

#define WARN_EQ DOCTEST_WARN_EQ
#define CHECK_EQ DOCTEST_CHECK_EQ
#define REQUIRE_EQ DOCTEST_REQUIRE_EQ
#define WARN_NE DOCTEST_WARN_NE
#define CHECK_NE DOCTEST_CHECK_NE
#define REQUIRE_NE DOCTEST_REQUIRE_NE
#define WARN_GT DOCTEST_WARN_GT
#define CHECK_GT DOCTEST_CHECK_GT
#define REQUIRE_GT DOCTEST_REQUIRE_GT
#define WARN_LT DOCTEST_WARN_LT
#define CHECK_LT DOCTEST_CHECK_LT
#define REQUIRE_LT DOCTEST_REQUIRE_LT
#define WARN_GE DOCTEST_WARN_GE
#define CHECK_GE DOCTEST_CHECK_GE
#define REQUIRE_GE DOCTEST_REQUIRE_GE
#define WARN_LE DOCTEST_WARN_LE
#define CHECK_LE DOCTEST_CHECK_LE
#define REQUIRE_LE DOCTEST_REQUIRE_LE
#define WARN_UNARY DOCTEST_WARN_UNARY
#define CHECK_UNARY DOCTEST_CHECK_UNARY
#define REQUIRE_UNARY DOCTEST_REQUIRE_UNARY
#define WARN_UNARY_FALSE DOCTEST_WARN_UNARY_FALSE
#define CHECK_UNARY_FALSE DOCTEST_CHECK_UNARY_FALSE
#define REQUIRE_UNARY_FALSE DOCTEST_REQUIRE_UNARY_FALSE

#define FAST_WARN_EQ DOCTEST_FAST_WARN_EQ
#define FAST_CHECK_EQ DOCTEST_FAST_CHECK_EQ
#define FAST_REQUIRE_EQ DOCTEST_FAST_REQUIRE_EQ
#define FAST_WARN_NE DOCTEST_FAST_WARN_NE
#define FAST_CHECK_NE DOCTEST_FAST_CHECK_NE
#define FAST_REQUIRE_NE DOCTEST_FAST_REQUIRE_NE
#define FAST_WARN_GT DOCTEST_FAST_WARN_GT
#define FAST_CHECK_GT DOCTEST_FAST_CHECK_GT
#define FAST_REQUIRE_GT DOCTEST_FAST_REQUIRE_GT
#define FAST_WARN_LT DOCTEST_FAST_WARN_LT
#define FAST_CHECK_LT DOCTEST_FAST_CHECK_LT
#define FAST_REQUIRE_LT DOCTEST_FAST_REQUIRE_LT
#define FAST_WARN_GE DOCTEST_FAST_WARN_GE
#define FAST_CHECK_GE DOCTEST_FAST_CHECK_GE
#define FAST_REQUIRE_GE DOCTEST_FAST_REQUIRE_GE
#define FAST_WARN_LE DOCTEST_FAST_WARN_LE
#define FAST_CHECK_LE DOCTEST_FAST_CHECK_LE
#define FAST_REQUIRE_LE DOCTEST_FAST_REQUIRE_LE
#define FAST_WARN_UNARY DOCTEST_FAST_WARN_UNARY
#define FAST_CHECK_UNARY DOCTEST_FAST_CHECK_UNARY
#define FAST_REQUIRE_UNARY DOCTEST_FAST_REQUIRE_UNARY
#define FAST_WARN_UNARY_FALSE DOCTEST_FAST_WARN_UNARY_FALSE
#define FAST_CHECK_UNARY_FALSE DOCTEST_FAST_CHECK_UNARY_FALSE
#define FAST_REQUIRE_UNARY_FALSE DOCTEST_FAST_REQUIRE_UNARY_FALSE

#endif // DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES

// this is here to clear the 'current test suite' for the current translation unit - at the top
DOCTEST_TEST_SUITE_END();

#endif // DOCTEST_LIBRARY_INCLUDED

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
