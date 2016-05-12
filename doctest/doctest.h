//
// doctest.h - the lightest feature rich C++ single header testing framework
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

// Suppress this globally - there is no way to silence it in the expression decomposition macros
// _Pragma() in macros doesn't work for the c++ front-end of g++
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=55578
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=69543
// Also the warning is completely worthless nowadays - http://stackoverflow.com/questions/14016993
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Waggregate-return"
#endif

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wshorten-64-to-32"
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#pragma clang diagnostic ignored "-Wcovered-switch-default"
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#endif // __clang__

#if defined(__GNUC__) && !defined(__clang__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 6)
#pragma GCC diagnostic push
#endif // > gcc 4.6
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wstrict-overflow"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Winline"
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 6)
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif // > gcc 4.6
//#pragma GCC diagnostic ignored "-Wlong-long"
#endif // __GNUC__

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996) // The compiler encountered a deprecated declaration
#pragma warning(disable : 4267) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#pragma warning(disable : 4706) // assignment within conditional expression
#pragma warning(disable : 4512) // 'class' : assignment operator could not be generated
#pragma warning(disable : 4127) // conditional expression is constant
#endif                          // _MSC_VER

#ifndef DOCTEST_LIBRARY_INCLUDED
#define DOCTEST_LIBRARY_INCLUDED

#define DOCTEST_VERSION_MAJOR 1
#define DOCTEST_VERSION_MINOR 0
#define DOCTEST_VERSION_PATCH 0
#define DOCTEST_VERSION "1.0.0"

// internal macros for string concatenation and anonymous variable name generation
#define DOCTEST_STR_CONCAT_IMPL(s1, s2) s1##s2
#define DOCTEST_STR_CONCAT(s1, s2) DOCTEST_STR_CONCAT_IMPL(s1, s2)
#ifdef __COUNTER__ // not standard and may be missing for some compilers
#define DOCTEST_ANONYMOUS(x) DOCTEST_STR_CONCAT(x, __COUNTER__)
#else // __COUNTER__
#define DOCTEST_ANONYMOUS(x) DOCTEST_STR_CONCAT(x, __LINE__)
#endif // __COUNTER__

// internal macro for making a string
#define DOCTEST_TOSTR_IMPL(x) #x
#define DOCTEST_TOSTR(x) DOCTEST_TOSTR_IMPL(x)

// internal macro for concatenating 2 literals and making the result a string
#define DOCTEST_STR_CONCAT_TOSTR(s1, s2) DOCTEST_TOSTR(DOCTEST_STR_CONCAT(s1, s2))

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

#define DOCTEST_COUNTOF(x) (sizeof(x) / sizeof(x[0]))

#define DOCTEST_GCS doctest::detail::getContextState

// snprintf() not in the C++98 standard
#ifdef _MSC_VER
#define DOCTEST_SNPRINTF _snprintf
#else
#define DOCTEST_SNPRINTF snprintf
#endif

// for anything below Visual Studio 2005 (VC++6 has no __debugbreak() - not sure about VS 2003)
#if defined(_MSC_VER) && _MSC_VER < 1400
#define __debugbreak() __asm { int 3}
#endif

#ifdef DOCTEST_PLATFORM_MAC
// The following code snippet based on:
// http://cocoawithlove.com/2008/03/break-into-debugger.html
#ifdef DEBUG
#if defined(__ppc64__) || defined(__ppc__)
#define DOCTEST_BREAK_INTO_DEBUGGER()                                                              \
    if(doctest::detail::isDebuggerActive() && !DOCTEST_GCS()->no_breaks)                           \
    __asm__("li r0, 20\nsc\nnop\nli r0, 37\nli r4, 2\nsc\nnop\n" : : : "memory", "r0", "r3", "r4")
#else // __ppc64__ || __ppc__
#define DOCTEST_BREAK_INTO_DEBUGGER()                                                              \
    if(doctest::detail::isDebuggerActive() && !DOCTEST_GCS()->no_breaks)                           \
    __asm__("int $3\n" : :)
#endif // __ppc64__ || __ppc__
#endif // DEBUG
#elif defined(_MSC_VER)
#define DOCTEST_BREAK_INTO_DEBUGGER()                                                              \
    if(doctest::detail::isDebuggerActive() && !DOCTEST_GCS()->no_breaks)                           \
    __debugbreak()
#elif defined(__MINGW32__)
extern "C" __declspec(dllimport) void __stdcall DebugBreak();
#define DOCTEST_BREAK_INTO_DEBUGGER()                                                              \
    if(doctest::detail::isDebuggerActive() && !DOCTEST_GCS()->no_breaks)                           \
    ::DebugBreak()
#else // linux
#define DOCTEST_BREAK_INTO_DEBUGGER() ((void)0)
#endif // linux

namespace doctest
{
class String
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

    int compare(const char* other, bool no_case = false) const;
    int compare(const String& other, bool no_case = false) const;
};

struct ADL_helper
{};

template <typename T>
String stringify(ADL_helper, const T&) {
    return "{?}";
}

#if !defined(DOCTEST_CONFIG_DISABLE)

String stringify(ADL_helper, const char* in);
String stringify(ADL_helper, const void* in);
String stringify(ADL_helper, bool in);
String stringify(ADL_helper, float in);
String stringify(ADL_helper, double in);
String stringify(ADL_helper, double long in);

String stringify(ADL_helper, char in);
String stringify(ADL_helper, char unsigned in);
String stringify(ADL_helper, short int in);
String stringify(ADL_helper, short int unsigned in);
String stringify(ADL_helper, int in);
String stringify(ADL_helper, int unsigned in);
String stringify(ADL_helper, long int in);
String stringify(ADL_helper, long int unsigned in);

template <typename T>
String stringify(ADL_helper, T* in) {
    return stringify(doctest::ADL_helper(), static_cast<const void*>(in));
}

namespace detail
{
    // the function type this library works with
    typedef void (*funcType)(void);

    // a struct defining a registered test callback
    struct TestData
    {
        // not used for determining uniqueness
        String   m_suite; // the test suite in which the test was added
        String   m_name;  // name of the test function
        funcType m_f;     // a function pointer to the test function

        // fields by which uniqueness of test cases shall be determined
        const char* m_file; // the file in which the test was registered
        unsigned    m_line; // the line where the test was registered

        TestData(const char* suite, const char* name, funcType f, const char* file, int line)
                : m_suite(suite)
                , m_name(name)
                , m_f(f)
                , m_file(file)
                , m_line(line) {}

        bool operator==(const TestData& other) const;
    };

    struct TestFailureException
    {};

    void checkIfShouldThrow(const char* assert_name);
    void  throwException();
    bool  always_false();
    void* getNullPtr();

    template <class T>
    class Vector
    {
        unsigned m_size;
        unsigned m_capacity;
        T*       m_buffer;

    public:
        Vector();
        Vector(unsigned num, const T& val = T());
        Vector(const Vector& other);
        ~Vector();
        Vector& operator=(const Vector& other);

        T*       data() { return m_buffer; }
        const T* data() const { return m_buffer; }
        unsigned size() const { return m_size; }

        T& operator[](unsigned index) { return m_buffer[index]; }
        const T& operator[](unsigned index) const { return m_buffer[index]; }

        void clear();
        void pop_back();
        void push_back(const T& item);
        void resize(unsigned num, const T& val = T());
    };

    // the default Hash() implementation that the HashTable class uses - returns 0 - very naive
    // specialize for better HashTable performance
    template <typename T>
    unsigned Hash(const T&) {
        return 0;
    }

    template <class T>
    class HashTable
    {
        Vector<Vector<T> > buckets;

    public:
        explicit HashTable(unsigned num_buckets)
                : buckets(num_buckets) {}

        bool has(const T& in) const {
            const Vector<T>& bucket = buckets[Hash(in) % buckets.size()];
            for(unsigned i = 0; i < bucket.size(); ++i)
                if(bucket[i] == in)
                    return true;
            return false;
        }

        void insert(const T& in) {
            if(!has(in))
                buckets[Hash(in) % buckets.size()].push_back(in);
        }

        void clear() {
            for(unsigned i = 0; i < buckets.size(); ++i)
                buckets[i].clear();
        }

        const Vector<Vector<T> >& getBuckets() const { return buckets; }
    };

    struct Subcase
    {
        String      m_name;
        const char* m_file;
        int         m_line;
        bool        m_entered;

        Subcase(const char* name, const char* file, int line);
        ~Subcase();
        Subcase(const Subcase& other);
        Subcase& operator=(const Subcase& other);

        bool operator==(const Subcase& other) const;
        operator bool() const { return m_entered; }
    };

    template <typename L, typename R>
    String stringifyBinaryExpr(const L& lhs, const char* op, const R& rhs) {
        return stringify(doctest::ADL_helper(), lhs) + " " + op + " " +
               stringify(doctest::ADL_helper(), rhs);
    }

    // TODO: think about this
    //struct STATIC_ASSERT_Expression_Too_Complex;

    struct Result
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
        operator bool() { return !m_passed; }

        void invert() { m_passed = !m_passed; }

        // clang-format off
        //template <typename R> STATIC_ASSERT_Expression_Too_Complex& operator+(const R&);
        //template <typename R> STATIC_ASSERT_Expression_Too_Complex& operator-(const R&);
        //template <typename R> STATIC_ASSERT_Expression_Too_Complex& operator/(const R&);
        //template <typename R> STATIC_ASSERT_Expression_Too_Complex& operator*(const R&);
        //template <typename R> STATIC_ASSERT_Expression_Too_Complex& operator&&(const R&);
        //template <typename R> STATIC_ASSERT_Expression_Too_Complex& operator||(const R&);
        //
        //template <typename R> STATIC_ASSERT_Expression_Too_Complex& operator==(const R&);
        //template <typename R> STATIC_ASSERT_Expression_Too_Complex& operator!=(const R&);
        //template <typename R> STATIC_ASSERT_Expression_Too_Complex& operator<(const R&);
        //template <typename R> STATIC_ASSERT_Expression_Too_Complex& operator<=(const R&);
        //template <typename R> STATIC_ASSERT_Expression_Too_Complex& operator>(const R&);
        //template <typename R> STATIC_ASSERT_Expression_Too_Complex& operator>=(const R&);
        // clang-format on
    };

    template <typename L>
    struct Expression_lhs
    {
        L lhs;

        Expression_lhs(L in)
                : lhs(in) {}

        Expression_lhs(const Expression_lhs& other)
                : lhs(other.lhs) {}

        operator Result() { return Result(!!lhs, stringify(doctest::ADL_helper(), lhs)); }

        // clang-format off
        template <typename R> Result operator==(const R& rhs) { return Result(lhs == rhs, stringifyBinaryExpr(lhs, "==", rhs)); }
        template <typename R> Result operator!=(const R& rhs) { return Result(lhs != rhs, stringifyBinaryExpr(lhs, "!=", rhs)); }
        template <typename R> Result operator< (const R& rhs) { return Result(lhs <  rhs, stringifyBinaryExpr(lhs, "<",  rhs)); }
        template <typename R> Result operator<=(const R& rhs) { return Result(lhs <= rhs, stringifyBinaryExpr(lhs, "<=", rhs)); }
        template <typename R> Result operator> (const R& rhs) { return Result(lhs >  rhs, stringifyBinaryExpr(lhs, ">",  rhs)); }
        template <typename R> Result operator>=(const R& rhs) { return Result(lhs >= rhs, stringifyBinaryExpr(lhs, ">=", rhs)); }
        // clang-format on
    };

    struct ExpressionDecomposer
    {
        template <typename L>
        Expression_lhs<const L&> operator<<(const L& operand) {
            return Expression_lhs<const L&>(operand);
        }
    };

    // forward declarations of functions used by the macros
    int regTest(void (*f)(void), unsigned line, const char* file, const char* name);
    int setTestSuiteName(const char* name);

    void addFailedAssert(const char* assert_name);

    void logTestStart(const char* name, const char* file, unsigned line);
    void logTestEnd();

    void logAssert(bool passed, const char* decomposition, bool threw, const char* expr,
                   const char* assert_name, const char* file, int line);

    void logAssertThrows(bool threw, const char* expr, const char* assert_name, const char* file,
                         int line);

    void logAssertThrowsAs(bool threw, bool threw_as, const char* as, const char* expr,
                           const char* assert_name, const char* file, int line);

    void logAssertNothrow(bool threw, const char* expr, const char* assert_name, const char* file,
                          int line);

    bool isDebuggerActive();
    void writeToDebugConsole(const String&);

    // this holds both parameters for the command line and runtime data for tests
    struct ContextState
    {
        // == parameters from the command line

        detail::Vector<detail::Vector<String> > filters;

        String   sort;      // how tests should be sorted
        unsigned rand_seed; // the seed for rand ordering

        unsigned first; // the first (matching) test to be executed
        unsigned last;  // the last (matching) test to be executed

        int  abort_after;    // stop tests after this many failed assertions
        bool success;        // include successful assertions in output
        bool case_sensitive; // if filtering should be case sensitive
        bool exit;           // if the program should be exited after the tests are ran/whatever
        bool no_overrides;   // to disable overrides from code
        bool no_throw;       // to skip exceptions-related assertion macros
        bool no_exitcode;    // if the framework should return 0 as the exitcode
        bool no_run;         // to not run the tests at all (can be done with an "*" exclude)
        bool no_colors;      // if output to the console should be colorized
        bool no_breaks;      // to not break into the debugger
        bool no_path_in_filenames; // if the path to files should be removed from the output

        bool help;                 // to print the help
        bool version;              // to print the version
        bool count;                // if only the count of matching tests is to be retreived
        bool list_tests;           // to list all tests matching the filters
        bool hash_table_histogram; // if the hash table should be printed as a histogram

        // == data for the tests being ran

        int             numAssertions;
        int             numFailedAssertions;
        int             numAssertionsForCurrentTestcase;
        int             numFailedAssertionsForCurrentTestcase;
        const TestData* currentTest;
        bool            hasLoggedCurrentTestStart;

        // stuff for subcases
        HashTable<Subcase> subcasesPassed;
        HashTable<int>     subcasesEnteredLevels;
        int                subcasesCurrentLevel;
        bool               subcasesHasSkipped;

        void resetRunData() {
            numAssertions       = 0;
            numFailedAssertions = 0;
        }

        ContextState()
                : filters(6) // 6 different filters total
                , subcasesPassed(100)
                , subcasesEnteredLevels(100) {
            resetRunData();
        }
    };

    ContextState*& getContextState();
} // namespace detail

#endif // DOCTEST_CONFIG_DISABLE

class Context
{
#if !defined(DOCTEST_CONFIG_DISABLE)
    detail::ContextState p;

    void parseArgs(int argc, const char* const* argv, bool withDefaults = false);

#endif // DOCTEST_CONFIG_DISABLE

public:
    Context(int argc, const char* const* argv);

// to fix gcc 4.7 "-Winline" warnings
#if defined(__GNUC__) && !defined(__clang__)
    __attribute__((noinline))
#endif
    ~Context() {
    }

    void addFilter(const char* filter, const char* value);
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
    static int DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_VAR_) __attribute__((unused)) =                   \
            doctest::detail::regTest(f, __LINE__, __FILE__, name);
#elif defined(__clang__)
#define DOCTEST_REGISTER_FUNCTION(f, name)                                                         \
    _Pragma("clang diagnostic push")                                                               \
            _Pragma("clang diagnostic ignored \"-Wglobal-constructors\"") static int               \
                    DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_VAR_) =                                      \
                            doctest::detail::regTest(f, __LINE__, __FILE__, name);                 \
    _Pragma("clang diagnostic pop")
#else // MSVC
#define DOCTEST_REGISTER_FUNCTION(f, name)                                                         \
    static int DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_VAR_) =                                           \
            doctest::detail::regTest(f, __LINE__, __FILE__, name);
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
    inline void f()

// for registering tests
#define DOCTEST_TEST_CASE(name)                                                                    \
    DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FUNC_), name)

// for registering tests with a fixture
#define DOCTEST_TEST_CASE_FIXTURE(c, name)                                                         \
    DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_CLASS_), c,                        \
                              DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FUNC_), name)

// for subcases
#if defined(__GNUC__)
#define DOCTEST_SUBCASE(name)                                                                      \
    if(const doctest::detail::Subcase & DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_SUBCASE_)                \
                                                __attribute__((unused)) =                          \
               doctest::detail::Subcase(name, __FILE__, __LINE__))
#else // __GNUC__
#define DOCTEST_SUBCASE(name)                                                                      \
    if(const doctest::detail::Subcase & DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_SUBCASE_) =              \
               doctest::detail::Subcase(name, __FILE__, __LINE__))
#endif // __GNUC__

// for starting a testsuite block
#if defined(__GNUC__) && !defined(__clang__)
#define DOCTEST_TEST_SUITE(name)                                                                   \
    static int DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_VAR_) __attribute__((unused)) =                   \
            doctest::detail::setTestSuiteName(name);                                               \
    void DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FOR_SEMICOLON_)()
#elif defined(__clang__)
#define DOCTEST_TEST_SUITE(name)                                                                   \
    _Pragma("clang diagnostic push")                                                               \
            _Pragma("clang diagnostic ignored \"-Wglobal-constructors\"") static int               \
                    DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_VAR_) =                                      \
                            doctest::detail::setTestSuiteName(name);                               \
    _Pragma("clang diagnostic pop") void DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FOR_SEMICOLON_)()
#else // MSVC
#define DOCTEST_TEST_SUITE(name)                                                                   \
    static int DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_VAR_) = doctest::detail::setTestSuiteName(name);  \
    void       DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FOR_SEMICOLON_)()
#endif // MSVC

// for ending a testsuite block
#if defined(__GNUC__) && !defined(__clang__)
#define DOCTEST_TEST_SUITE_END                                                                     \
    static int DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_VAR_) __attribute__((unused)) =                   \
            doctest::detail::setTestSuiteName("");                                                 \
    void DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_TESTSUITE_END_)
#elif defined(__clang__)
#define DOCTEST_TEST_SUITE_END                                                                     \
    _Pragma("clang diagnostic push")                                                               \
            _Pragma("clang diagnostic ignored \"-Wglobal-constructors\"") static int               \
                    DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_VAR_) =                                      \
                            doctest::detail::setTestSuiteName("");                                 \
    _Pragma("clang diagnostic pop") void DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_TESTSUITE_END_)
#else // MSVC
#define DOCTEST_TEST_SUITE_END                                                                     \
    static int DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_VAR_) = doctest::detail::setTestSuiteName("");    \
    void       DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_TESTSUITE_END_)
#endif // MSVC

#define DOCTEST_LOG_START()                                                                        \
    if(!DOCTEST_GCS()->hasLoggedCurrentTestStart) {                                                \
        doctest::detail::logTestStart(DOCTEST_GCS()->currentTest->m_name.c_str(),                  \
                                      DOCTEST_GCS()->currentTest->m_file,                          \
                                      DOCTEST_GCS()->currentTest->m_line);                         \
        DOCTEST_GCS()->hasLoggedCurrentTestStart = true;                                           \
    }

#define DOCTEST_ASSERT_IMPLEMENT(expr, assert_name, false_invert_op)                               \
    doctest::detail::Result res;                                                                   \
    bool                    threw = false;                                                         \
    try {                                                                                          \
        res = doctest::detail::ExpressionDecomposer() << expr;                                     \
    } catch(...) { threw = true; }                                                                 \
    false_invert_op;                                                                               \
    if(res || DOCTEST_GCS()->success) {                                                            \
        DOCTEST_LOG_START();                                                                       \
        doctest::detail::logAssert(res.m_passed, res.m_decomposition.c_str(), threw, #expr,        \
                                   assert_name, __FILE__, __LINE__);                               \
    }                                                                                              \
    DOCTEST_GCS()->numAssertionsForCurrentTestcase++;                                              \
    if(res) {                                                                                      \
        doctest::detail::addFailedAssert(assert_name);                                             \
        DOCTEST_BREAK_INTO_DEBUGGER();                                                             \
        doctest::detail::checkIfShouldThrow(assert_name);                                          \
    }

#if defined(__clang__)
#define DOCTEST_ASSERT_PROXY(expr, assert_name, false_invert_op)                                   \
    do {                                                                                           \
        _Pragma("clang diagnostic push")                                                           \
                _Pragma("clang diagnostic ignored \"-Woverloaded-shift-op-parentheses\"")          \
                        DOCTEST_ASSERT_IMPLEMENT(expr, assert_name, false_invert_op)               \
                                _Pragma("clang diagnostic pop")                                    \
    } while(doctest::detail::always_false())
#else // __clang__
#define DOCTEST_ASSERT_PROXY(expr, assert_name, false_invert_op)                                   \
    do {                                                                                           \
        DOCTEST_ASSERT_IMPLEMENT(expr, assert_name, false_invert_op)                               \
    } while(doctest::detail::always_false())
#endif // __clang__

#define DOCTEST_WARN(expr) DOCTEST_ASSERT_PROXY(expr, "WARN", ((void)0))
#define DOCTEST_CHECK(expr) DOCTEST_ASSERT_PROXY(expr, "CHECK", ((void)0))
#define DOCTEST_REQUIRE(expr) DOCTEST_ASSERT_PROXY(expr, "REQUIRE", ((void)0))

#define DOCTEST_WARN_FALSE(expr) DOCTEST_ASSERT_PROXY(expr, "WARN_FALSE", res.invert())
#define DOCTEST_CHECK_FALSE(expr) DOCTEST_ASSERT_PROXY(expr, "CHECK_FALSE", res.invert())
#define DOCTEST_REQUIRE_FALSE(expr) DOCTEST_ASSERT_PROXY(expr, "REQUIRE_FALSE", res.invert())

#define DOCTEST_ASSERT_THROWS(expr, assert_name)                                                   \
    do {                                                                                           \
        if(!DOCTEST_GCS()->no_throw) {                                                             \
            bool threw = false;                                                                    \
            try {                                                                                  \
                expr;                                                                              \
            } catch(...) { threw = true; }                                                         \
            if(!threw || DOCTEST_GCS()->success) {                                                 \
                DOCTEST_LOG_START();                                                               \
                doctest::detail::logAssertThrows(threw, #expr, assert_name, __FILE__, __LINE__);   \
            }                                                                                      \
            DOCTEST_GCS()->numAssertionsForCurrentTestcase++;                                      \
            if(!threw) {                                                                           \
                doctest::detail::addFailedAssert(assert_name);                                     \
                DOCTEST_BREAK_INTO_DEBUGGER();                                                     \
                doctest::detail::checkIfShouldThrow(assert_name);                                  \
            }                                                                                      \
        }                                                                                          \
    } while(doctest::detail::always_false())

#define DOCTEST_ASSERT_THROWS_AS(expr, as, assert_name)                                            \
    do {                                                                                           \
        if(!DOCTEST_GCS()->no_throw) {                                                             \
            bool threw    = false;                                                                 \
            bool threw_as = false;                                                                 \
            try {                                                                                  \
                expr;                                                                              \
            } catch(as) {                                                                          \
                threw    = true;                                                                   \
                threw_as = true;                                                                   \
            } catch(...) { threw = true; }                                                         \
            if(!threw_as || DOCTEST_GCS()->success) {                                              \
                DOCTEST_LOG_START();                                                               \
                doctest::detail::logAssertThrowsAs(threw, threw_as, #as, #expr, assert_name,       \
                                                   __FILE__, __LINE__);                            \
            }                                                                                      \
            DOCTEST_GCS()->numAssertionsForCurrentTestcase++;                                      \
            if(!threw_as) {                                                                        \
                doctest::detail::addFailedAssert(assert_name);                                     \
                DOCTEST_BREAK_INTO_DEBUGGER();                                                     \
                doctest::detail::checkIfShouldThrow(assert_name);                                  \
            }                                                                                      \
        }                                                                                          \
    } while(doctest::detail::always_false())

#define DOCTEST_ASSERT_NOTHROW(expr, assert_name)                                                  \
    do {                                                                                           \
        if(!DOCTEST_GCS()->no_throw) {                                                             \
            bool threw = false;                                                                    \
            try {                                                                                  \
                expr;                                                                              \
            } catch(...) { threw = true; }                                                         \
            if(threw || DOCTEST_GCS()->success) {                                                  \
                DOCTEST_LOG_START();                                                               \
                doctest::detail::logAssertNothrow(threw, #expr, assert_name, __FILE__, __LINE__);  \
            }                                                                                      \
            DOCTEST_GCS()->numAssertionsForCurrentTestcase++;                                      \
            if(threw) {                                                                            \
                doctest::detail::addFailedAssert(assert_name);                                     \
                DOCTEST_BREAK_INTO_DEBUGGER();                                                     \
                doctest::detail::checkIfShouldThrow(assert_name);                                  \
            }                                                                                      \
        }                                                                                          \
    } while(doctest::detail::always_false())

#define DOCTEST_WARN_THROWS(expr) DOCTEST_ASSERT_THROWS(expr, "WARN_THROWS")
#define DOCTEST_CHECK_THROWS(expr) DOCTEST_ASSERT_THROWS(expr, "CHECK_THROWS")
#define DOCTEST_REQUIRE_THROWS(expr) DOCTEST_ASSERT_THROWS(expr, "REQUIRE_THROWS")

#define DOCTEST_WARN_THROWS_AS(expr, ex) DOCTEST_ASSERT_THROWS_AS(expr, ex, "WARN_THROWS_AS")
#define DOCTEST_CHECK_THROWS_AS(expr, ex) DOCTEST_ASSERT_THROWS_AS(expr, ex, "CHECK_THROWS_AS")
#define DOCTEST_REQUIRE_THROWS_AS(expr, ex) DOCTEST_ASSERT_THROWS_AS(expr, ex, "REQUIRE_THROWS_AS")

#define DOCTEST_WARN_NOTHROW(expr) DOCTEST_ASSERT_NOTHROW(expr, "WARN_NOTHROW")
#define DOCTEST_CHECK_NOTHROW(expr) DOCTEST_ASSERT_NOTHROW(expr, "CHECK_NOTHROW")
#define DOCTEST_REQUIRE_NOTHROW(expr) DOCTEST_ASSERT_NOTHROW(expr, "REQUIRE_NOTHROW")

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
    DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FUNC_), name)

// for registering tests with a fixture
#define DOCTEST_TEST_CASE_FIXTURE(x, name)                                                         \
    DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_CLASS_), x,                        \
                              DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FUNC_), name)

// for subcases
#define DOCTEST_SUBCASE(name)

// for starting a testsuite block
#define DOCTEST_TEST_SUITE(name) void DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FOR_SEMICOLON_)()

// for ending a testsuite block
#define DOCTEST_TEST_SUITE_END void DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_TESTSUITE_END_)

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

#endif // DOCTEST_CONFIG_DISABLE

// == SHORT VERSIONS OF THE MACROS
#if !defined(DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES)

#define TEST_CASE DOCTEST_TEST_CASE
#define TEST_CASE_FIXTURE DOCTEST_TEST_CASE_FIXTURE
#define SUBCASE DOCTEST_SUBCASE
#define TEST_SUITE DOCTEST_TEST_SUITE
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

#endif // DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES

// this is here to clear the 'current test suite' for the current translation unit - at the top
DOCTEST_TEST_SUITE_END();

#endif // DOCTEST_LIBRARY_INCLUDED

// =================================================================================================
// == WHAT FOLLOWS IS THE IMPLEMENTATION OF THE TEST RUNNER                                       ==
// =================================================================================================
#if(defined(DOCTEST_CONFIG_IMPLEMENT) || defined(DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN))
#ifndef DOCTEST_LIBRARY_IMPLEMENTATION
#define DOCTEST_LIBRARY_IMPLEMENTATION

#if defined(DOCTEST_CONFIG_DISABLE)
namespace doctest
{
String::String(const char*) {}
String::String(const String&) {}
String::~String() {}
String& String::operator=(const String&) { return *this; }
String  String::operator+(const String&) const { return String(); }
String& String::operator+=(const String&) { return *this; }
int     String::compare(const char*, bool) const { return 0; }
int     String::compare(const String&, bool) const { return 0; }

Context::Context(int, const char* const*) {}
void Context::addFilter(const char*, const char*) {}
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
            printf("%s", buffer);                                                                  \
        }                                                                                          \
    } while(doctest::detail::always_false())

// required includes - will go only in one translation unit!
#include <cstdio>  // printf, fprintf, sprintf, snprintf
#include <cstdlib> // malloc, free, qsort, rand, srand
#include <ctime>   // time stuff
#include <cstring> // strcpy, strtok, strrchr, strncmp
#include <new>     // placement new (can be skipped if the containers require 'construct()' from T)

// the number of buckets used for the hash set
#if !defined(DOCTEST_HASH_TABLE_NUM_BUCKETS)
#define DOCTEST_HASH_TABLE_NUM_BUCKETS 1024
#endif // DOCTEST_HASH_TABLE_NUM_BUCKETS

// the buffer size used for snprintf() calls
#if !defined(DOCTEST_SNPRINTF_BUFFER_LENGTH)
#define DOCTEST_SNPRINTF_BUFFER_LENGTH 1024
#endif // DOCTEST_SNPRINTF_BUFFER_LENGTH

#if defined(_MSC_VER) || defined(__MINGW32__)
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(const char*);
extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent();
#endif // DOCTEST_PLATFORM_WINDOWS

#ifdef DOCTEST_CONFIG_COLORS_ANSI
#include <unistd.h>
#endif // DOCTEST_CONFIG_COLORS_ANSI

#ifdef DOCTEST_CONFIG_COLORS_WINDOWS

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

#endif // DOCTEST_CONFIG_COLORS_WINDOWS

// main namespace of the library
namespace doctest
{
String stringify(ADL_helper, const char* in) { return String("\"") + in + "\""; }

String stringify(ADL_helper, const void* in) {
    char buf[64];
    sprintf(buf, "0x%p", in);
    return buf;
}

String stringify(ADL_helper, bool in) { return in ? "true" : "false"; }

String stringify(ADL_helper, float in) {
    char buf[64];
    sprintf(buf, "%f", static_cast<double>(in));
    return buf;
}

String stringify(ADL_helper, double in) {
    char buf[64];
    sprintf(buf, "%f", in);
    return buf;
}

String stringify(ADL_helper, double long in) {
    char buf[64];
    sprintf(buf, "%Lf", in);
    return buf;
}

String stringify(ADL_helper, char in) {
    char buf[64];
    if(in < ' ')
        sprintf(buf, "%d", in);
    else
        sprintf(buf, "%c", in);
    return buf;
}

String stringify(ADL_helper, char unsigned in) {
    char buf[64];
    if(in < ' ')
        sprintf(buf, "%ud", in);
    else
        sprintf(buf, "%c", in);
    return buf;
}

String stringify(ADL_helper, short int in) {
    char buf[64];
    sprintf(buf, "%d", in);
    return buf;
}

String stringify(ADL_helper, short int unsigned in) {
    char buf[64];
    sprintf(buf, "%u", in);
    return buf;
}

String stringify(ADL_helper, int in) {
    char buf[64];
    sprintf(buf, "%d", in);
    return buf;
}

String stringify(ADL_helper, int unsigned in) {
    char buf[64];
    sprintf(buf, "%u", in);
    return buf;
}

String stringify(ADL_helper, long int in) {
    char buf[64];
    sprintf(buf, "%ld", in);
    return buf;
}

String stringify(ADL_helper, long int unsigned in) {
    char buf[64];
    sprintf(buf, "%lu", in);
    return buf;
}

// library internals namespace
namespace detail
{
    bool TestData::operator==(const TestData& other) const {
        return m_line == other.m_line && strcmp(m_file, other.m_file) == 0;
    }

    void checkIfShouldThrow(const char* assert_name) {
        if(strncmp(assert_name, "REQUIRE", 7) == 0)
            throwException();

        if(strncmp(assert_name, "CHECK", 5) == 0 && getContextState()->abort_after > 0) {
            if(getContextState()->numFailedAssertions >= getContextState()->abort_after)
                throwException();
        }
    }
    void  throwException() { throw doctest::detail::TestFailureException(); }
    bool  always_false() { return false; }
    void* getNullPtr() { return 0; }

    // lowers ascii letters
    char tolower(const char c) { return ((c >= 'A' && c <= 'Z') ? static_cast<char>(c + 32) : c); }

    // not using std::strlen() because of valgrind errors when optimizations are turned on
    // 'Invalid read of size 4' when the test suite len (with '\0') is not a multiple of 4
    // for details see http://stackoverflow.com/questions/35671155
    size_t my_strlen(const char* in) {
        const char* temp = in;
        while(*temp)
            ++temp;
        return temp - in;
    }

    // case insensitive strcmp
    int stricmp(char const* a, char const* b) {
        for(;; a++, b++) {
            int d = tolower(*a) - tolower(*b);
            if(d != 0 || !*a)
                return d;
        }
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

    // C string hash function (djb2) - taken from http://www.cse.yorku.ca/~oz/hash.html
    unsigned hashStr(unsigned const char* str) {
        unsigned long hash = 5381;
        char          c;

        while((c = *str++))
            hash = ((hash << 5) + hash) + c; // hash * 33 + c

        return hash;
    }

    // checks if the name matches any of the filters (and can be configured what to do when empty)
    int matchesAny(const String& name, Vector<String> filters, int matchEmpty, bool caseSensitive) {
        if(filters.size() == 0 && matchEmpty)
            return 1;
        for(unsigned i = 0; i < filters.size(); ++i)
            if(wildcmp(name.c_str(), filters[i].c_str(), caseSensitive))
                return 1;
        return 0;
    }

    template <class T>
    Vector<T>::Vector()
            : m_size(0)
            , m_capacity(0)
            , m_buffer(0) {}

    template <class T>
    Vector<T>::Vector(unsigned num, const T& val)
            : m_size(num)
            , m_capacity(num)
            , m_buffer(static_cast<T*>(malloc(sizeof(T) * m_capacity))) {
        for(unsigned i = 0; i < m_size; ++i)
            new(m_buffer + i) T(val);
    }

    template <class T>
    Vector<T>::Vector(const Vector& other)
            : m_size(other.m_size)
            , m_capacity(other.m_capacity)
            , m_buffer(static_cast<T*>(malloc(sizeof(T) * m_capacity))) {
        for(unsigned i = 0; i < m_size; ++i)
            new(m_buffer + i) T(other.m_buffer[i]);
    }

    template <class T>
    Vector<T>::~Vector() {
        for(unsigned i = 0; i < m_size; ++i)
            (*(m_buffer + i)).~T();
        free(m_buffer);
    }

    template <class T>
    Vector<T>& Vector<T>::operator=(const Vector& other) {
        if(this != &other) {
            for(unsigned i = 0; i < m_size; ++i)
                (*(m_buffer + i)).~T();
            free(m_buffer);

            m_size     = other.m_size;
            m_capacity = other.m_capacity;

            m_buffer = static_cast<T*>(malloc(sizeof(T) * m_capacity));
            for(unsigned i = 0; i < m_size; ++i)
                new(m_buffer + i) T(other.m_buffer[i]);
        }
        return *this;
    }

    template <class T>
    void Vector<T>::clear() {
        for(unsigned i = 0; i < m_size; ++i)
            (*(m_buffer + i)).~T();
        m_size = 0;
    }

    template <class T>
    void Vector<T>::pop_back() {
        if(m_size > 0)
            (*(m_buffer + --m_size)).~T();
    }

    template <class T>
    void Vector<T>::push_back(const T& item) {
        if(m_size < m_capacity) {
            new(m_buffer + m_size++) T(item);
        } else {
            if(m_capacity == 0)
                m_capacity = 5; // initial capacity
            else
                m_capacity *= 2; // capacity growth factor
            T* temp = static_cast<T*>(malloc(sizeof(T) * m_capacity));
            for(unsigned i = 0; i < m_size; ++i) {
                new(temp + i) T(m_buffer[i]);
                (*(m_buffer + i)).~T();
            }
            new(temp + m_size++) T(item);
            free(m_buffer);
            m_buffer = temp;
        }
    }

    template <class T>
    void Vector<T>::resize(unsigned num, const T& val) {
        if(num < m_size) {
            for(unsigned i = num; i < m_size; ++i)
                (*(m_buffer + i)).~T();
            m_size = num;
        } else {
            if(num > m_capacity) {
                if(m_capacity == 0) {
                    m_buffer = static_cast<T*>(malloc(sizeof(T) * num));
                } else {
                    T* temp = static_cast<T*>(malloc(sizeof(T) * num));
                    for(unsigned i = 0; i < m_size; ++i) {
                        new(temp + i) T(m_buffer[i]);
                        (*(m_buffer + i)).~T();
                    }
                }

                for(unsigned i = m_size; i < num; ++i)
                    new(m_buffer + i) T(val);

                m_size     = num;
                m_capacity = num;
            }
        }
    }

    template <>
    unsigned Hash(const Subcase& in) {
        return hashStr(reinterpret_cast<unsigned const char*>(in.m_file)) ^ in.m_line;
    }

    template <>
    unsigned Hash(const int& in) {
        return in;
    }

    // the current ContextState with which tests are being executed
    ContextState*& getContextState() {
        static ContextState* data = 0;
        return data;
    }

    Subcase::Subcase(const char* name, const char* file, int line)
            : m_name(name)
            , m_file(file)
            , m_line(line)
            , m_entered(false) {
        ContextState* s = getContextState();

        // if we have already completed it
        if(s->subcasesPassed.has(*this))
            return;

        // if a Subcase on the same level has already been entered
        if(s->subcasesEnteredLevels.has(s->subcasesCurrentLevel)) {
            s->subcasesHasSkipped = true;
            return;
        }

        s->subcasesEnteredLevels.insert(s->subcasesCurrentLevel++);
        m_entered = true;
    }

    Subcase::~Subcase() {
        if(m_entered) {
            ContextState* s = getContextState();

            s->subcasesCurrentLevel--;
            // only mark the subcase as passed if no subcases have been skipped
            if(s->subcasesHasSkipped == false)
                s->subcasesPassed.insert(*this);
        }
    }

    Subcase::Subcase(const Subcase& other)
            : m_name(other.m_name)
            , m_file(other.m_file)
            , m_line(other.m_line)
            , m_entered(other.m_entered) {}

    Subcase& Subcase::operator=(const Subcase& other) {
        m_name    = other.m_name;
        m_file    = other.m_file;
        m_line    = other.m_line;
        m_entered = other.m_entered;
        return *this;
    }

    bool Subcase::operator==(const Subcase& other) const {
        return m_line == other.m_line && strcmp(m_file, other.m_file) == 0;
    }

    unsigned Hash(const TestData& in) {
        return hashStr(reinterpret_cast<unsigned const char*>(in.m_file)) ^ in.m_line;
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
        int res = strcmp(lhs->m_file, rhs->m_file);
#endif // _MSC_VER
        if(res != 0)
            return res;
        return static_cast<int>(lhs->m_line - rhs->m_line);
    }

    // for sorting tests by suite/file/line
    int suiteOrderComparator(const void* a, const void* b) {
        const TestData* lhs = *static_cast<TestData* const*>(a);
        const TestData* rhs = *static_cast<TestData* const*>(b);

        int res = lhs->m_suite.compare(rhs->m_suite);
        if(res != 0)
            return res;
        return fileOrderComparator(a, b);
    }

    // for sorting tests by name/suite/file/line
    int nameOrderComparator(const void* a, const void* b) {
        const TestData* lhs = *static_cast<TestData* const*>(a);
        const TestData* rhs = *static_cast<TestData* const*>(b);

        int res = lhs->m_name.compare(rhs->m_name);
        if(res != 0)
            return res;
        return suiteOrderComparator(a, b);
    }

    // holds the current test suite
    const char*& getCurrentTestSuite() {
        static const char* data = 0;
        return data;
    }

    // sets the current test suite
    int setTestSuiteName(const char* name) {
        getCurrentTestSuite() = name;
        return 0;
    }

    // all the registered tests
    HashTable<TestData>& getRegisteredTests() {
        static HashTable<TestData> data(DOCTEST_HASH_TABLE_NUM_BUCKETS);
        return data;
    }

    // used by the macros for registering tests
    int regTest(funcType f, unsigned line, const char* file, const char* name) {
        getRegisteredTests().insert(TestData(getCurrentTestSuite(), name, f, file, line));
        return 0;
    }

    // this is needed because MSVC does not permit mixing 2 exception handling schemes in a function
    int callTestFunc(funcType f) {
        int res = EXIT_SUCCESS;
        try {
            f();
            if(getContextState()->numFailedAssertionsForCurrentTestcase)
                res = EXIT_FAILURE;
        } catch(const TestFailureException&) { res = EXIT_FAILURE; } catch(...) {
            printf("Unknown exception caught!\n");
            res = EXIT_FAILURE;
        }
        return res;
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

    void Color::use(Code code) {
        ContextState* p = getContextState();
        if(p->no_colors)
            return;
#ifdef DOCTEST_CONFIG_COLORS_ANSI
        if(isatty(STDOUT_FILENO)) {
            const char* col = "";
            // clang-format off
            switch(code) {
                case Color::Red:         col = "[0;31m"; break;
                case Color::Green:       col = "[0;32m"; break;
                case Color::Blue:        col = "[0:34m"; break;
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
            printf("\033%s", col);
        }
#endif // DOCTEST_CONFIG_COLORS_ANSI

#ifdef DOCTEST_CONFIG_COLORS_WINDOWS
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

    // depending on the current options this will remove the path of filenames
    const char* fileForOutput(const char* file) {
        if(getContextState()->no_path_in_filenames) {
            const char* back    = strrchr(file, '\\');
            const char* forward = strrchr(file, '/');
            if(back || forward) {
                if(back > forward)
                    forward = back;
                return forward + 1;
            }
        }
        return file;
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
        int               mib[4];
        struct kinfo_proc info;
        size_t            size;
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
    bool    isDebuggerActive() { return ::IsDebuggerPresent() != 0; }
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

    void printToDebugConsole(const String& text) {
        if(isDebuggerActive())
            myOutputDebugString(text.c_str());
    }

    void addFailedAssert(const char* assert_name) {
        if(strncmp(assert_name, "WARN", 4) != 0) {
            getContextState()->numFailedAssertionsForCurrentTestcase++;
            getContextState()->numFailedAssertions++;
        }
    }

    void logTestStart(const char* name, const char* file, unsigned line) {
        const char* newLine = "\n";

        char msg[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(msg, DOCTEST_COUNTOF(msg), "test: \"%s\"\n", name);

        char loc[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(loc, DOCTEST_COUNTOF(loc), "%s(%d)\n", fileForOutput(file), line);

        DOCTEST_PRINTF_COLORED(getSeparator(), Color::Yellow);
        DOCTEST_PRINTF_COLORED(msg, Color::None);
        DOCTEST_PRINTF_COLORED(loc, Color::LightGrey);
        DOCTEST_PRINTF_COLORED(getSeparator(), Color::Yellow);
        DOCTEST_PRINTF_COLORED(newLine, Color::None);

        printToDebugConsole(String(getSeparator()) + msg + loc + getSeparator() + newLine);
    }

    void logTestEnd() {}

    void logAssert(bool passed, const char* decomposition, bool threw, const char* expr,
                   const char* assert_name, const char* file, int line) {
        char loc[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(loc, DOCTEST_COUNTOF(loc), "%s(%d)", fileForOutput(file), line);

        char msg[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        if(passed)
            DOCTEST_SNPRINTF(msg, DOCTEST_COUNTOF(msg), " PASSED!\n");
        else
            DOCTEST_SNPRINTF(msg, DOCTEST_COUNTOF(msg), " FAILED! %s\n",
                             (threw ? "(threw exception)" : ""));

        char info1[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(info1, DOCTEST_COUNTOF(info1), "  %s( %s )\n", assert_name, expr);

        char info2[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        char info3[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        info2[0] = 0;
        if(!threw) {
            DOCTEST_SNPRINTF(info2, DOCTEST_COUNTOF(info2), "with expansion:\n");
            DOCTEST_SNPRINTF(info3, DOCTEST_COUNTOF(info3), "  %s( %s )\n\n", assert_name,
                             decomposition);
        }

        DOCTEST_PRINTF_COLORED(loc, Color::LightGrey);
        DOCTEST_PRINTF_COLORED(msg, passed ? Color::BrightGreen : Color::Red);
        DOCTEST_PRINTF_COLORED(info1, Color::Green);
        DOCTEST_PRINTF_COLORED(info2, Color::None);
        DOCTEST_PRINTF_COLORED(info3, Color::Green);

        printToDebugConsole(String(loc) + msg + info1 + info2 + info3);
    }

    void logAssertThrows(bool threw, const char* expr, const char* assert_name, const char* file,
                         int line) {
        char loc[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(loc, DOCTEST_COUNTOF(loc), "%s(%d)", fileForOutput(file), line);

        char msg[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        if(threw)
            DOCTEST_SNPRINTF(msg, DOCTEST_COUNTOF(msg), " PASSED!\n");
        else
            DOCTEST_SNPRINTF(msg, DOCTEST_COUNTOF(msg), " FAILED!\n");

        char info1[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(info1, DOCTEST_COUNTOF(info1), "  %s( %s )\n\n", assert_name, expr);

        DOCTEST_PRINTF_COLORED(loc, Color::LightGrey);
        DOCTEST_PRINTF_COLORED(msg, threw ? Color::BrightGreen : Color::Red);
        DOCTEST_PRINTF_COLORED(info1, Color::Green);

        printToDebugConsole(String(loc) + msg + info1);
    }

    void logAssertThrowsAs(bool threw, bool threw_as, const char* as, const char* expr,
                           const char* assert_name, const char* file, int line) {
        char loc[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(loc, DOCTEST_COUNTOF(loc), "%s(%d)", fileForOutput(file), line);

        char msg[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        if(threw_as)
            DOCTEST_SNPRINTF(msg, DOCTEST_COUNTOF(msg), " PASSED!\n");
        else
            DOCTEST_SNPRINTF(msg, DOCTEST_COUNTOF(msg), " FAILED! %s\n",
                             (threw ? "(threw something else)" : "(didn't throw at all)"));

        char info1[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(info1, DOCTEST_COUNTOF(info1), "  %s( %s, %s )\n\n", assert_name, expr,
                         as);

        DOCTEST_PRINTF_COLORED(loc, Color::LightGrey);
        DOCTEST_PRINTF_COLORED(msg, threw_as ? Color::BrightGreen : Color::Red);
        DOCTEST_PRINTF_COLORED(info1, Color::Green);

        printToDebugConsole(String(loc) + msg + info1);
    }

    void logAssertNothrow(bool threw, const char* expr, const char* assert_name, const char* file,
                          int line) {
        char loc[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(loc, DOCTEST_COUNTOF(loc), "%s(%d)", fileForOutput(file), line);

        char msg[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        if(!threw)
            DOCTEST_SNPRINTF(msg, DOCTEST_COUNTOF(msg), " PASSED!\n");
        else
            DOCTEST_SNPRINTF(msg, DOCTEST_COUNTOF(msg), " FAILED!\n");

        char info1[DOCTEST_SNPRINTF_BUFFER_LENGTH];
        DOCTEST_SNPRINTF(info1, DOCTEST_COUNTOF(info1), "  %s( %s )\n\n", assert_name, expr);

        DOCTEST_PRINTF_COLORED(loc, Color::LightGrey);
        DOCTEST_PRINTF_COLORED(msg, !threw ? Color::BrightGreen : Color::Red);
        DOCTEST_PRINTF_COLORED(info1, Color::Green);

        printToDebugConsole(String(loc) + msg + info1);
    }

    // the implementation of parseFlag()
    bool parseFlagImpl(int argc, const char* const* argv, const char* pattern) {
        for(int i = argc - 1; i >= 0; --i) {
            const char* temp = strstr(argv[i], pattern);
            if(temp && my_strlen(temp) == my_strlen(pattern)) {
                // eliminate strings in which the chars before the option are not '-'
                bool noBadCharsFound = true;
                while(temp != argv[i]) {
                    if(*--temp != '-') {
                        noBadCharsFound = false;
                        break;
                    }
                }
                if(noBadCharsFound)
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
            const char* temp = strstr(argv[i], pattern);
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
                if(noBadCharsFound) {
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
                           Vector<String>& res) {
        String filtersString;
        if(parseOption(argc, argv, pattern, filtersString)) {
            // tokenize with "," as a separator
            char* pch = strtok(filtersString.c_str(), ","); // modifies the string
            while(pch != 0) {
                if(my_strlen(pch))
                    res.push_back(pch);
                pch = strtok(0, ","); // uses the strtok() internal state to go to the next token
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
                        int& res, int defaultVal = int()) {
        res = defaultVal;

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
                int theInt = atoi(parsedValue.c_str());
                if(theInt != 0) {
                    res = theInt;
                    return true;
                }
            }
        }
        return false;
    }

    void printVersion() {
        DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
        printf("doctest version is \"%s\"\n", DOCTEST_VERSION);
    }

    void printHelp() {
        printVersion();
        DOCTEST_PRINTF_COLORED("[doctest]\n", Color::Cyan);
        DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
        printf("boolean values: \"1/on/yes/true\" or \"0/off/no/false\"\n");
        DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
        printf("filter  values: \"str1,str2,str3\" (comma separated strings)\n");
        DOCTEST_PRINTF_COLORED("[doctest]\n", Color::Cyan);
        DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
        printf("filters use wildcards for matching strings\n");
        DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
        printf("something passes a filter if any of the strings in a filter matches\n");
        DOCTEST_PRINTF_COLORED("[doctest]\n", Color::Cyan);
#ifndef DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS
        DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
        printf("ALL OPTIONS AND FILTERS ALSO AVAILABLE WITHOUT THE \"dt-\" PREFIX!!!\n");
#endif // DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS
        DOCTEST_PRINTF_COLORED("[doctest]\n", Color::Cyan);
        DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
        printf("the available options, flags and filters are:\n");
        printf(" -dt-help  (also \"-dt-?\"/\"-dt-h\") prints this message and exits\n");
        printf(" -dt-version                      prints the version and exits\n");
        printf(" -dt-count                        prints the number of matching tests and exits\n");
        printf(" -dt-list-tests                   lists all matching tests by name and exits\n");
        //printf(" -dt-hash-table-histogram         undocumented\n");
        // ==================================================================================== << 79
        printf(" -dt-name=<filters>               filters     tests by their name\n");
        printf(" -dt-name-exclude=<filters>       filters OUT tests by their name\n");
        printf(" -dt-file=<filters>               filters     tests by their file\n");
        printf(" -dt-file-exclude=<filters>       filters OUT tests by their file\n");
        printf(" -dt-test-suite=<filters>         filters     tests by their test suite\n");
        printf(" -dt-test-suite-exclude=<filters> filters OUT tests by their test suite\n");
        printf(" -dt-sort=<string>                how the tests should be sorted\n");
        printf("                                  <string> - by [file/suite/name/rand]\n");
        printf(" -dt-rand-seed=<int>              seed for random ordering\n");
        printf(" -dt-first=<int>                  the first test passing the filters to\n");
        printf("                                  execute - for range-based execution\n");
        printf(" -dt-last=<int>                   the last test passing the filters to\n");
        printf("                                  execute - for range-based execution\n");
        printf(" -dt-abort-after=<int>            stop after <int> failed assertions\n");
        printf(" -dt-success=<bool>               include successful assertions in output\n");
        printf(" -dt-case-sensitive=<bool>        filters being treated as case sensitive\n");
        printf(" -dt-exit=<bool>                  exits after the tests finish\n");
        printf(" -dt-no-overrides=<bool>          disables procedural overrides of options\n");
        printf(" -dt-no-throw=<bool>              skips exceptions-related assert checks\n");
        printf(" -dt-no-exitcode=<bool>           returns (or exits) always with success\n");
        printf(" -dt-no-run=<bool>                skips all runtime doctest operations\n");
        printf(" -dt-no-colors=<bool>             disables colors in output\n");
        printf(" -dt-no-breaks=<bool>             disables breakpoints in debuggers\n");
        printf(" -dt-no-path-in-filenames=<bool>  only filenames and no paths in output\n");
        // ==================================================================================== << 79

        DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
        printf("for more information visit the project documentation\n\n");
    }
} // namespace detail

String::String(const char* in) {
    m_str = static_cast<char*>(malloc(detail::my_strlen(in) + 1));
    strcpy(m_str, in);
}

String::String(const String& other)
        : m_str(0) {
    copy(other);
}

void String::copy(const String& other) {
    if(m_str)
        free(m_str);
    m_str = 0;

    if(other.m_str) {
        m_str = static_cast<char*>(malloc(detail::my_strlen(other.m_str) + 1));
        strcpy(m_str, other.m_str);
    }
}

String::~String() {
    if(m_str)
        free(m_str);
}

String& String::operator=(const String& other) {
    if(this != &other)
        copy(other);
    return *this;
}

String String::operator+(const String& other) const { return String(m_str) += other; }

String& String::operator+=(const String& other) {
    using namespace detail;
    if(m_str == 0) {
        copy(other);
    } else if(other.m_str != 0) {
        char* newStr = static_cast<char*>(malloc(my_strlen(m_str) + my_strlen(other.m_str) + 1));
        strcpy(newStr, m_str);
        strcpy(newStr + my_strlen(m_str), other.m_str);
        free(m_str);
        m_str = newStr;
    }
    return *this;
}

int String::compare(const char* other, bool no_case) const {
    if(no_case)
        return detail::stricmp(m_str, other);
    return strcmp(m_str, other);
}

int String::compare(const String& other, bool no_case) const {
    if(no_case)
        return detail::stricmp(m_str, other.m_str);
    return strcmp(m_str, other.m_str);
}

Context::Context(int argc, const char* const* argv) {
    using namespace detail;

    parseArgs(argc, argv, true);

    p.help                 = false;
    p.version              = false;
    p.count                = false;
    p.list_tests           = false;
    p.hash_table_histogram = false;
    if(parseFlag(argc, argv, "dt-help") || parseFlag(argc, argv, "dt-h") ||
       parseFlag(argc, argv, "dt-?")) {
        p.help = true;
        p.exit = true;
    }
    if(parseFlag(argc, argv, "dt-version")) {
        p.version = true;
        p.exit    = true;
    }
    if(parseFlag(argc, argv, "dt-count")) {
        p.count = true;
        p.exit  = true;
    }
    if(parseFlag(argc, argv, "dt-list-tests")) {
        p.list_tests = true;
        p.exit       = true;
    }
    if(parseFlag(argc, argv, "dt-hash-table-histogram")) {
        p.hash_table_histogram = true;
        p.exit                 = true;
    }
}

// parses args
void Context::parseArgs(int argc, const char* const* argv, bool withDefaults) {
    using namespace detail;

    parseCommaSepArgs(argc, argv, "dt-file=", p.filters[0]);
    parseCommaSepArgs(argc, argv, "dt-file-exclude=", p.filters[1]);
    parseCommaSepArgs(argc, argv, "dt-test-suite=", p.filters[2]);
    parseCommaSepArgs(argc, argv, "dt-test-suite-exclude=", p.filters[3]);
    parseCommaSepArgs(argc, argv, "dt-name=", p.filters[4]);
    parseCommaSepArgs(argc, argv, "dt-name-exclude=", p.filters[5]);

    int    intRes = 0;
    String strRes;

#define DOCTEST_PARSE_BOOL_OPTION(name, var, default)                                              \
    if(parseIntOption(argc, argv, name, option_bool, intRes, default) || withDefaults)             \
    p.var = !!intRes

#define DOCTEST_PARSE_INT_OPTION(name, var, default)                                               \
    if(parseIntOption(argc, argv, name, option_int, intRes, default) || withDefaults)              \
    p.var = intRes

#define DOCTEST_PARSE_STR_OPTION(name, var, default)                                               \
    if(parseOption(argc, argv, name, strRes, default) || withDefaults)                             \
    p.var = strRes

    DOCTEST_PARSE_STR_OPTION("dt-sort=", sort, "file");
    DOCTEST_PARSE_INT_OPTION("dt-rand-seed=", rand_seed, 0);

    DOCTEST_PARSE_INT_OPTION("dt-first=", first, 1);
    DOCTEST_PARSE_INT_OPTION("dt-last=", last, 0);

    DOCTEST_PARSE_INT_OPTION("dt-abort-after=", abort_after, 0);
    DOCTEST_PARSE_BOOL_OPTION("dt-success=", success, 0);
    DOCTEST_PARSE_BOOL_OPTION("dt-case-sensitive=", case_sensitive, 0);
    DOCTEST_PARSE_BOOL_OPTION("dt-exit=", exit, 0);
    DOCTEST_PARSE_BOOL_OPTION("dt-no-overrides=", no_overrides, 0);
    DOCTEST_PARSE_BOOL_OPTION("dt-no-throw=", no_throw, 0);
    DOCTEST_PARSE_BOOL_OPTION("dt-no-exitcode=", no_exitcode, 0);
    DOCTEST_PARSE_BOOL_OPTION("dt-no-run=", no_run, 0);
    DOCTEST_PARSE_BOOL_OPTION("dt-no-colors=", no_colors, 0);
    DOCTEST_PARSE_BOOL_OPTION("dt-no-breaks=", no_breaks, 0);
    DOCTEST_PARSE_BOOL_OPTION("dt-no-path-in-filenames=", no_path_in_filenames, 0);

#undef DOCTEST_PARSE_STR_OPTION
#undef DOCTEST_PARSE_INT_OPTION
}

// allows the user to add procedurally to the filters from the command line
void Context::addFilter(const char* filter, const char* value) { setOption(filter, value); }

// allows the user to override procedurally the int/bool options from the command line
void Context::setOption(const char* option, int value) {
    setOption(option, stringify(doctest::ADL_helper(), value).c_str());
}

// allows the user to override procedurally the string options from the command line
void Context::setOption(const char* option, const char* value) {
    using namespace detail;

    if(!p.no_overrides) {
        String      argv   = String(option) + "=" + value;
        const char* lvalue = argv.c_str();
        parseArgs(1, &lvalue);
    }
}

// users should query this in their main() and exit the program if true
bool Context::shouldExit() { return p.exit; }

// the main function that does all the filtering and test running
int Context::run() {
    using namespace detail;

    getContextState() = &p;
    p.resetRunData();

    // handle version, help and no_run
    if(p.no_run || p.version || p.help) {
        if(p.version)
            printVersion();
        if(p.help)
            printHelp();

        return EXIT_SUCCESS;
    }

    printVersion();
    DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
    printf("run with \"-dt-help\" for options\n");

    unsigned                         i       = 0; // counter used for loops - here for VC6
    const Vector<Vector<TestData> >& buckets = getRegisteredTests().getBuckets();

    Vector<const TestData*> testArray;
    for(i = 0; i < buckets.size(); i++)
        for(unsigned k = 0; k < buckets[i].size(); k++)
            testArray.push_back(&buckets[i][k]);

    if(p.hash_table_histogram) {
        // find the most full bucket
        unsigned maxInBucket = 1;
        for(i = 0; i < buckets.size(); i++)
            if(buckets[i].size() > maxInBucket)
                maxInBucket = buckets[i].size();

        // print a prettified histogram
        DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
        printf("hash table bucket histogram\n");
        printf("============================================================\n");
        printf("#bucket     |count| relative count\n");
        printf("============================================================\n");
        for(i = 0; i < buckets.size(); i++) {
            printf("bucket %4d |%4d |", static_cast<int>(i), buckets[i].size());

            float ratio = static_cast<float>(buckets[i].size()) / static_cast<float>(maxInBucket);
            unsigned numStars = static_cast<unsigned>(ratio * 41);
            for(unsigned k = 0; k < numStars; ++k)
                printf("*");
            printf("\n");
        }
        printf("\n");
        return EXIT_SUCCESS;
    }

    // sort the collected records
    if(p.sort.compare("file") == 0) {
        qsort(testArray.data(), testArray.size(), sizeof(TestData*), fileOrderComparator);
    } else if(p.sort.compare("suite") == 0) {
        qsort(testArray.data(), testArray.size(), sizeof(TestData*), suiteOrderComparator);
    } else if(p.sort.compare("name") == 0) {
        qsort(testArray.data(), testArray.size(), sizeof(TestData*), nameOrderComparator);
    } else if(p.sort.compare("rand") == 0) {
        srand(p.rand_seed);

        // random_shuffle implementation
        const TestData** first = testArray.data();
        for(i = testArray.size() - 1; i > 0; --i) {
            int idxToSwap = rand() % (i + 1);

            const TestData* temp = first[i];

            first[i]         = first[idxToSwap];
            first[idxToSwap] = temp;
        }
    }

    if(p.list_tests) {
        DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
        printf("listing all test names\n");
    }

    unsigned numTestsPassingFilters = 0;
    unsigned numFailed              = 0;
    // invoke the registered functions if they match the filter criteria (or just count them)
    for(i = 0; i < testArray.size(); i++) {
        const TestData& data = *testArray[i];
        if(!matchesAny(data.m_file, p.filters[0], 1, p.case_sensitive))
            continue;
        if(matchesAny(data.m_file, p.filters[1], 0, p.case_sensitive))
            continue;
        if(!matchesAny(data.m_suite, p.filters[2], 1, p.case_sensitive))
            continue;
        if(matchesAny(data.m_suite, p.filters[3], 0, p.case_sensitive))
            continue;
        if(!matchesAny(data.m_name, p.filters[4], 1, p.case_sensitive))
            continue;
        if(matchesAny(data.m_name, p.filters[5], 0, p.case_sensitive))
            continue;

        numTestsPassingFilters++;

        // do not execute the test if we are to only count the number of filter passing tests
        if(p.count)
            continue;

        // print the name of the test and don't execute it
        if(p.list_tests) {
            printf("%s\n", data.m_name.c_str());
            continue;
        }

        // skip the test if it is not in the execution range
        if((p.last < numTestsPassingFilters && p.first <= p.last) ||
           (p.first > numTestsPassingFilters))
            continue;

        // execute the test if it passes all the filtering
        {
#ifdef _MSC_VER
//__try {
#endif // _MSC_VER

            p.currentTest               = &data;
            p.hasLoggedCurrentTestStart = false;

            // if logging successful tests - force the start log
            if(p.success) {
                logTestStart(p.currentTest->m_name.c_str(), p.currentTest->m_file,
                             p.currentTest->m_line);
                p.hasLoggedCurrentTestStart = true;
            }

            unsigned didFail = 0;
            p.subcasesPassed.clear();
            do {
                // reset the assertion state
                p.numAssertionsForCurrentTestcase       = 0;
                p.numFailedAssertionsForCurrentTestcase = 0;

                // reset some of the fields for subcases (except for the set of fully passed ones)
                p.subcasesHasSkipped   = false;
                p.subcasesCurrentLevel = 0;
                p.subcasesEnteredLevels.clear();

                didFail += callTestFunc(data.m_f);
                p.numAssertions += p.numAssertionsForCurrentTestcase;

                // exit this loop if enough assertions have failed
                if(p.abort_after > 0 && p.numFailedAssertions >= p.abort_after)
                    p.subcasesHasSkipped = false;
            } while(p.subcasesHasSkipped == true);

            // if the start has been logged
            if(p.hasLoggedCurrentTestStart)
                logTestEnd();

            if(didFail > 0)
                numFailed++;

            // stop executing tests if enough assertions have failed
            if(p.abort_after > 0 && p.numFailedAssertions >= p.abort_after)
                break;

#ifdef _MSC_VER
//} __except(1) {
//    printf("Unknown SEH exception caught!\n");
//    numFailed++;
//}
#endif // _MSC_VER
        }
    }

    DOCTEST_PRINTF_COLORED(getSeparator(), Color::Yellow);
    if(p.count || p.list_tests) {
        DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);
        printf("number of tests passing the current filters: %d\n", numTestsPassingFilters);
    } else {
        char buff[DOCTEST_SNPRINTF_BUFFER_LENGTH];

        DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);

        DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), "test cases: %4d", numTestsPassingFilters);
        DOCTEST_PRINTF_COLORED(buff, Color::None);
        DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), " | ");
        DOCTEST_PRINTF_COLORED(buff, Color::None);
        DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), "%4d passed",
                         numTestsPassingFilters - numFailed);
        DOCTEST_PRINTF_COLORED(buff, Color::Green);
        DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), " | ");
        DOCTEST_PRINTF_COLORED(buff, Color::None);
        DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), "%4d failed\n", numFailed);
        DOCTEST_PRINTF_COLORED(buff, Color::Red);

        DOCTEST_PRINTF_COLORED("[doctest] ", Color::Cyan);

        DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), "assertions: %4d", p.numAssertions);
        DOCTEST_PRINTF_COLORED(buff, Color::None);
        DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), " | ");
        DOCTEST_PRINTF_COLORED(buff, Color::None);
        DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), "%4d passed",
                         p.numAssertions - p.numFailedAssertions);
        DOCTEST_PRINTF_COLORED(buff, Color::Green);
        DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), " | ");
        DOCTEST_PRINTF_COLORED(buff, Color::None);
        DOCTEST_SNPRINTF(buff, DOCTEST_COUNTOF(buff), "%4d failed\n", p.numFailedAssertions);
        DOCTEST_PRINTF_COLORED(buff, Color::Red);
    }

    if(numFailed && !p.no_exitcode)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE
#endif // DOCTEST_LIBRARY_IMPLEMENTATION
#endif // DOCTEST_CONFIG_IMPLEMENT

// == THIS SUPPLIES A MAIN FUNCTION AND SHOULD BE DONE ONLY IN ONE TRANSLATION UNIT
#if defined(DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN) && !defined(DOCTEST_MAIN_CONFIGURED)
#define DOCTEST_MAIN_CONFIGURED
int main(int argc, char** argv) { return doctest::Context(argc, argv).run(); }
#endif // DOCTEST_MAIN_CONFIGURED

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
