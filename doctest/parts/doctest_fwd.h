//
// doctest.h - the lightest feature-rich C++ single-header testing framework for unit tests and TDD
//
// Copyright (c) 2016-2023 Viktor Kirilov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
// The documentation can be found at the library's page:
// https://github.com/doctest/doctest/blob/master/doc/markdown/readme.md
//
// =================================================================================================
// =================================================================================================
// =================================================================================================
//
// The library is heavily influenced by Catch - https://github.com/catchorg/Catch2
// which uses the Boost Software License - Version 1.0
// see here - https://github.com/catchorg/Catch2/blob/master/LICENSE.txt
//
// The concept of subcases (sections in Catch) and expression decomposition are from there.
// Some parts of the code are taken directly:
// - stringification - the detection of "ostream& operator<<(ostream&, const T&)" and StringMaker<>
// - the Approx() helper class for floating point comparison
// - colors in the console
// - breaking into a debugger
// - signal / SEH handling
// - timer
// - XmlWriter class - thanks to Phil Nash for allowing the direct reuse (AKA copy/paste)
//
// The expression decomposing templates are taken from lest - https://github.com/martinmoene/lest
// which uses the Boost Software License - Version 1.0
// see here - https://github.com/martinmoene/lest/blob/master/LICENSE.txt
//
// =================================================================================================
// =================================================================================================
// =================================================================================================

#ifndef DOCTEST_LIBRARY_INCLUDED
#define DOCTEST_LIBRARY_INCLUDED

#include <doctest/parts/public/version.h>
#include <doctest/parts/public/compiler.h>
#include <doctest/parts/public/warnings.h>

DOCTEST_SUPPRESS_COMMON_WARNINGS_PUSH

DOCTEST_CLANG_SUPPRESS_WARNING_PUSH
DOCTEST_CLANG_SUPPRESS_WARNING("-Wnon-virtual-dtor")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wdeprecated")

DOCTEST_GCC_SUPPRESS_WARNING_PUSH
DOCTEST_GCC_SUPPRESS_WARNING("-Wctor-dtor-privacy")
DOCTEST_GCC_SUPPRESS_WARNING("-Wnon-virtual-dtor")
DOCTEST_GCC_SUPPRESS_WARNING("-Wsign-promo")

DOCTEST_MSVC_SUPPRESS_WARNING_PUSH
DOCTEST_MSVC_SUPPRESS_WARNING(4623) // default constructor was implicitly defined as deleted

#include <doctest/parts/public/config.h>
#include <doctest/parts/public/utility.h>
#include <doctest/parts/public/platform.h>
#include <doctest/parts/public/debugger.h>
#include <doctest/parts/public/std/fwd.h>
#include <doctest/parts/public/std/type_traits.h>
#include <doctest/parts/public/std/utility.h>
#include <doctest/parts/public/string.h>
#include <doctest/parts/public/matchers/contains.h>
#include <doctest/parts/public/matchers/approx.h>
#include <doctest/parts/public/matchers/is_nan.h>
#include <doctest/parts/public/context/options.h>
#include <doctest/parts/public/assert/type.h>
#include <doctest/parts/public/assert/data.h>
#include <doctest/parts/public/assert/comparator.h>
#include <doctest/parts/public/assert/result.h>
#include <doctest/parts/public/assert/expression.h>
#include <doctest/parts/public/color.h>

namespace doctest {

DOCTEST_INTERFACE extern bool is_running_in_test;

DOCTEST_INTERFACE const char* skipPathFromFilename(const char* file);

struct DOCTEST_INTERFACE TestCaseData
{
    String      m_file;       // the file in which the test was registered (using String - see #350)
    unsigned    m_line;       // the line where the test was registered
    const char* m_name;       // name of the test case
    const char* m_test_suite; // the test suite in which the test was added
    const char* m_description;
    bool        m_skip;
    bool        m_no_breaks;
    bool        m_no_output;
    bool        m_may_fail;
    bool        m_should_fail;
    int         m_expected_failures;
    double      m_timeout;
};

struct DOCTEST_INTERFACE MessageData
{
    String           m_string;
    const char*      m_file;
    int              m_line;
    assertType::Enum m_severity;
};

struct DOCTEST_INTERFACE SubcaseSignature
{
    String      m_name;
    const char* m_file;
    int         m_line;

    bool operator==(const SubcaseSignature& other) const;
    bool operator<(const SubcaseSignature& other) const;
};

struct DOCTEST_INTERFACE IContextScope
{
    DOCTEST_DECLARE_INTERFACE(IContextScope)
    virtual void stringify(std::ostream*) const = 0;
};

} // namespace doctest

#include <doctest/parts/public/exceptions.h>
#include <doctest/parts/public/assert/handler.h>

namespace doctest {
#ifndef DOCTEST_CONFIG_DISABLE
namespace detail {

    struct DOCTEST_INTERFACE Subcase
    {
        SubcaseSignature m_signature;
        bool             m_entered = false;

        Subcase(const String& name, const char* file, int line);
        Subcase(const Subcase&) = delete;
        Subcase(Subcase&&) = delete;
        Subcase& operator=(const Subcase&) = delete;
        Subcase& operator=(Subcase&&) = delete;
        ~Subcase();

        operator bool() const;

        private:
            bool checkFilters();
    };

    struct DOCTEST_INTERFACE TestSuite
    {
        const char* m_test_suite = nullptr;
        const char* m_description = nullptr;
        bool        m_skip = false;
        bool        m_no_breaks = false;
        bool        m_no_output = false;
        bool        m_may_fail = false;
        bool        m_should_fail = false;
        int         m_expected_failures = 0;
        double      m_timeout = 0;

        TestSuite& operator*(const char* in);

        template <typename T>
        TestSuite& operator*(const T& in) {
            in.fill(*this);
            return *this;
        }
    };

    using funcType = void (*)();

    struct DOCTEST_INTERFACE TestCase : public TestCaseData
    {
        funcType m_test; // a function pointer to the test case

        String m_type; // for templated test cases - gets appended to the real name
        int m_template_id; // an ID used to distinguish between the different versions of a templated test case
        String m_full_name; // contains the name (only for templated test cases!) + the template type

        TestCase(funcType test, const char* file, unsigned line, const TestSuite& test_suite,
                 const String& type = String(), int template_id = -1);

        TestCase(const TestCase& other);
        TestCase(TestCase&&) = delete;

        DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(26434) // hides a non-virtual function
        TestCase& operator=(const TestCase& other);
        DOCTEST_MSVC_SUPPRESS_WARNING_POP

        TestCase& operator=(TestCase&&) = delete;

        TestCase& operator*(const char* in);

        template <typename T>
        TestCase& operator*(const T& in) {
            in.fill(*this);
            return *this;
        }

        bool operator<(const TestCase& other) const;

        ~TestCase() = default;
    };

    // forward declarations of functions used by the macros
    DOCTEST_INTERFACE int  regTest(const TestCase& tc);
    DOCTEST_INTERFACE int  setTestSuite(const TestSuite& ts);

    template<typename T>
    int instantiationHelper(const T&) { return 0; }

    struct DOCTEST_INTERFACE IExceptionTranslator
    {
        DOCTEST_DECLARE_INTERFACE(IExceptionTranslator)
        virtual bool translate(String&) const = 0;
    };

    template <typename T>
    class ExceptionTranslator : public IExceptionTranslator //!OCLINT destructor of virtual class
    {
    public:
        explicit ExceptionTranslator(String (*translateFunction)(T))
                : m_translateFunction(translateFunction) {}

        bool translate(String& res) const override {
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
            try {
                throw; // lgtm [cpp/rethrow-no-exception]
                // cppcheck-suppress catchExceptionByValue
            } catch(const T& ex) {
                res = m_translateFunction(ex); //!OCLINT parameter reassignment
                return true;
            } catch(...) {}         //!OCLINT -  empty catch statement
#endif                              // DOCTEST_CONFIG_NO_EXCEPTIONS
            static_cast<void>(res); // to silence -Wunused-parameter
            return false;
        }

    private:
        String (*m_translateFunction)(T);
    };

    DOCTEST_INTERFACE void registerExceptionTranslatorImpl(const IExceptionTranslator* et);

    // ContextScope base class used to allow implementing methods of ContextScope
    // that don't depend on the template parameter in doctest.cpp.
    struct DOCTEST_INTERFACE ContextScopeBase : public IContextScope {
        ContextScopeBase(const ContextScopeBase&) = delete;

        ContextScopeBase& operator=(const ContextScopeBase&) = delete;
        ContextScopeBase& operator=(ContextScopeBase&&) = delete;

        ~ContextScopeBase() override = default;

    protected:
        ContextScopeBase();
        ContextScopeBase(ContextScopeBase&& other) noexcept;

        void destroy();
        bool need_to_destroy{true};
    };

    template <typename L> class ContextScope : public ContextScopeBase
    {
        L lambda_;

    public:
        explicit ContextScope(const L &lambda) : lambda_(lambda) {}
        explicit ContextScope(L&& lambda) : lambda_(static_cast<L&&>(lambda)) { }

        ContextScope(const ContextScope&) = delete;
        ContextScope(ContextScope&&) noexcept = default;

        ContextScope& operator=(const ContextScope&) = delete;
        ContextScope& operator=(ContextScope&&) = delete;

        void stringify(std::ostream* s) const override { lambda_(s); }

        ~ContextScope() override {
            if (need_to_destroy) {
                destroy();
            }
        }
    };

    struct DOCTEST_INTERFACE MessageBuilder : public MessageData
    {
        std::ostream* m_stream;
        bool          logged = false;

        MessageBuilder(const char* file, int line, assertType::Enum severity);

        MessageBuilder(const MessageBuilder&) = delete;
        MessageBuilder(MessageBuilder&&) = delete;

        MessageBuilder& operator=(const MessageBuilder&) = delete;
        MessageBuilder& operator=(MessageBuilder&&) = delete;

        ~MessageBuilder();

        // the preferred way of chaining parameters for stringification
DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4866)
        template <typename T>
        MessageBuilder& operator,(const T& in) {
            *m_stream << (DOCTEST_STRINGIFY(in));
            return *this;
        }
DOCTEST_MSVC_SUPPRESS_WARNING_POP

        // kept here just for backwards-compatibility - the comma operator should be preferred now
        template <typename T>
        MessageBuilder& operator<<(const T& in) { return this->operator,(in); }

        // the `,` operator has the lowest operator precedence - if `<<` is used by the user then
        // the `,` operator will be called last which is not what we want and thus the `*` operator
        // is used first (has higher operator precedence compared to `<<`) so that we guarantee that
        // an operator of the MessageBuilder class is called first before the rest of the parameters
        template <typename T>
        MessageBuilder& operator*(const T& in) { return this->operator,(in); }

        bool log();
        void react();
    };

    template <typename L>
    ContextScope<L> MakeContextScope(const L &lambda) {
        return ContextScope<L>(lambda);
    }
} // namespace detail

#define DOCTEST_DEFINE_DECORATOR(name, type, def)                                                  \
    struct name                                                                                    \
    {                                                                                              \
        type data;                                                                                 \
        name(type in = def)                                                                        \
                : data(in) {}                                                                      \
        void fill(detail::TestCase& state) const { state.DOCTEST_CAT(m_, name) = data; }           \
        void fill(detail::TestSuite& state) const { state.DOCTEST_CAT(m_, name) = data; }          \
    }

DOCTEST_DEFINE_DECORATOR(test_suite, const char*, "");
DOCTEST_DEFINE_DECORATOR(description, const char*, "");
DOCTEST_DEFINE_DECORATOR(skip, bool, true);
DOCTEST_DEFINE_DECORATOR(no_breaks, bool, true);
DOCTEST_DEFINE_DECORATOR(no_output, bool, true);
DOCTEST_DEFINE_DECORATOR(timeout, double, 0);
DOCTEST_DEFINE_DECORATOR(may_fail, bool, true);
DOCTEST_DEFINE_DECORATOR(should_fail, bool, true);
DOCTEST_DEFINE_DECORATOR(expected_failures, int, 0);

template <typename T>
int registerExceptionTranslator(String (*translateFunction)(T)) {
    DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wexit-time-destructors")
    static detail::ExceptionTranslator<T> exceptionTranslator(translateFunction);
    DOCTEST_CLANG_SUPPRESS_WARNING_POP
    detail::registerExceptionTranslatorImpl(&exceptionTranslator);
    return 0;
}

} // namespace doctest

// in a separate namespace outside of doctest because the DOCTEST_TEST_SUITE macro
// introduces an anonymous namespace in which getCurrentTestSuite gets overridden
namespace doctest_detail_test_suite_ns {
DOCTEST_INTERFACE doctest::detail::TestSuite& getCurrentTestSuite();
} // namespace doctest_detail_test_suite_ns

namespace doctest {
#else  // DOCTEST_CONFIG_DISABLE
template <typename T>
int registerExceptionTranslator(String (*)(T)) {
    return 0;
}
#endif // DOCTEST_CONFIG_DISABLE

namespace detail {
    using assert_handler = void (*)(const AssertData&);
    struct ContextState;
} // namespace detail

class DOCTEST_INTERFACE Context
{
    detail::ContextState* p;

    void parseArgs(int argc, const char* const* argv, bool withDefaults = false);

public:
    explicit Context(int argc = 0, const char* const* argv = nullptr);

    Context(const Context&) = delete;
    Context(Context&&) = delete;

    Context& operator=(const Context&) = delete;
    Context& operator=(Context&&) = delete;

    ~Context(); // NOLINT(performance-trivially-destructible)

    void applyCommandLine(int argc, const char* const* argv);

    void addFilter(const char* filter, const char* value);
    void clearFilters();
    void setOption(const char* option, bool value);
    void setOption(const char* option, int value);
    void setOption(const char* option, const char* value);

    bool shouldExit();

    void setAsDefaultForAssertsOutOfTestCases();

    void setAssertHandler(detail::assert_handler ah);

    void setCout(std::ostream* out);

    int run();
};

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

#ifdef DOCTEST_CONFIG_ASSERTS_RETURN_VALUES
#define DOCTEST_FUNC_EMPTY [] { return false; }()
#else
#define DOCTEST_FUNC_EMPTY (void)0
#endif

// if registering is not disabled
#ifndef DOCTEST_CONFIG_DISABLE

#ifdef DOCTEST_CONFIG_ASSERTS_RETURN_VALUES
#define DOCTEST_FUNC_SCOPE_BEGIN [&]
#define DOCTEST_FUNC_SCOPE_END ()
#define DOCTEST_FUNC_SCOPE_RET(v) return v
#else
#define DOCTEST_FUNC_SCOPE_BEGIN do
#define DOCTEST_FUNC_SCOPE_END while(false)
#define DOCTEST_FUNC_SCOPE_RET(v) (void)0
#endif

// common code in asserts - for convenience
#define DOCTEST_ASSERT_LOG_REACT_RETURN(b)                                                         \
    if(b.log()) DOCTEST_BREAK_INTO_DEBUGGER();                                                     \
    b.react();                                                                                     \
    DOCTEST_FUNC_SCOPE_RET(!b.m_failed)

#ifdef DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS
#define DOCTEST_WRAP_IN_TRY(x) x;
#else // DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS
#define DOCTEST_WRAP_IN_TRY(x)                                                                     \
    try {                                                                                          \
        x;                                                                                         \
    } catch(...) { DOCTEST_RB.translateException(); }
#endif // DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS

#ifdef DOCTEST_CONFIG_VOID_CAST_EXPRESSIONS
#define DOCTEST_CAST_TO_VOID(...)                                                                  \
    DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH("-Wuseless-cast")                                       \
    static_cast<void>(__VA_ARGS__);                                                                \
    DOCTEST_GCC_SUPPRESS_WARNING_POP
#else // DOCTEST_CONFIG_VOID_CAST_EXPRESSIONS
#define DOCTEST_CAST_TO_VOID(...) __VA_ARGS__;
#endif // DOCTEST_CONFIG_VOID_CAST_EXPRESSIONS

// registers the test by initializing a dummy var with a function
#define DOCTEST_REGISTER_FUNCTION(global_prefix, f, decorators)                                    \
    global_prefix DOCTEST_GLOBAL_NO_WARNINGS(DOCTEST_ANONYMOUS(DOCTEST_ANON_VAR_), /* NOLINT */    \
            doctest::detail::regTest(                                                              \
                    doctest::detail::TestCase(                                                     \
                            f, __FILE__, __LINE__,                                                 \
                            doctest_detail_test_suite_ns::getCurrentTestSuite()) *                 \
                    decorators))

#define DOCTEST_IMPLEMENT_FIXTURE(der, base, func, decorators)                                     \
    namespace { /* NOLINT */                                                                       \
        struct der : public base                                                                   \
        {                                                                                          \
            void f();                                                                              \
        };                                                                                         \
        static DOCTEST_INLINE_NOINLINE void func() {                                               \
            der v;                                                                                 \
            v.f();                                                                                 \
        }                                                                                          \
        DOCTEST_REGISTER_FUNCTION(DOCTEST_EMPTY, func, decorators)                                 \
    }                                                                                              \
    DOCTEST_INLINE_NOINLINE void der::f() // NOLINT(misc-definitions-in-headers)

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION(f, decorators)                                        \
    static void f();                                                                               \
    DOCTEST_REGISTER_FUNCTION(DOCTEST_EMPTY, f, decorators)                                        \
    static void f()

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION_IN_CLASS(f, proxy, decorators)                        \
    static doctest::detail::funcType proxy() { return f; }                                         \
    DOCTEST_REGISTER_FUNCTION(inline, proxy(), decorators)                                         \
    static void f()

// for registering tests
#define DOCTEST_TEST_CASE(decorators)                                                              \
    DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(DOCTEST_ANON_FUNC_), decorators)

// for registering tests in classes - requires C++17 for inline variables!
#if DOCTEST_CPLUSPLUS >= 201703L
#define DOCTEST_TEST_CASE_CLASS(decorators)                                                        \
    DOCTEST_CREATE_AND_REGISTER_FUNCTION_IN_CLASS(DOCTEST_ANONYMOUS(DOCTEST_ANON_FUNC_),           \
                                                  DOCTEST_ANONYMOUS(DOCTEST_ANON_PROXY_),          \
                                                  decorators)
#else // DOCTEST_TEST_CASE_CLASS
#define DOCTEST_TEST_CASE_CLASS(...)                                                               \
    TEST_CASES_CAN_BE_REGISTERED_IN_CLASSES_ONLY_IN_CPP17_MODE_OR_WITH_VS_2017_OR_NEWER
#endif // DOCTEST_TEST_CASE_CLASS

// for registering tests with a fixture
#define DOCTEST_TEST_CASE_FIXTURE(c, decorators)                                                   \
    DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS(DOCTEST_ANON_CLASS_), c,                           \
                              DOCTEST_ANONYMOUS(DOCTEST_ANON_FUNC_), decorators)

// for converting types to strings without the <typeinfo> header and demangling
#define DOCTEST_TYPE_TO_STRING_AS(str, ...)                                                        \
    namespace doctest {                                                                            \
        template <>                                                                                \
        inline String toString<__VA_ARGS__>() {                                                    \
            return str;                                                                            \
        }                                                                                          \
    }                                                                                              \
    static_assert(true, "")

#define DOCTEST_TYPE_TO_STRING(...) DOCTEST_TYPE_TO_STRING_AS(#__VA_ARGS__, __VA_ARGS__)

#define DOCTEST_TEST_CASE_TEMPLATE_DEFINE_IMPL(dec, T, iter, func)                                 \
    template <typename T>                                                                          \
    static void func();                                                                            \
    namespace { /* NOLINT */                                                                       \
        template <typename Tuple>                                                                  \
        struct iter;                                                                               \
        template <typename Type, typename... Rest>                                                 \
        struct iter<std::tuple<Type, Rest...>>                                                     \
        {                                                                                          \
            iter(const char* file, unsigned line, int index) {                                     \
                doctest::detail::regTest(doctest::detail::TestCase(func<Type>, file, line,         \
                                            doctest_detail_test_suite_ns::getCurrentTestSuite(),   \
                                            doctest::toString<Type>(),                             \
                                            int(line) * 1000 + index)                              \
                                         * dec);                                                   \
                iter<std::tuple<Rest...>>(file, line, index + 1);                                  \
            }                                                                                      \
        };                                                                                         \
        template <>                                                                                \
        struct iter<std::tuple<>>                                                                  \
        {                                                                                          \
            iter(const char*, unsigned, int) {}                                                    \
        };                                                                                         \
    }                                                                                              \
    template <typename T>                                                                          \
    static void func()

#define DOCTEST_TEST_CASE_TEMPLATE_DEFINE(dec, T, id)                                              \
    DOCTEST_TEST_CASE_TEMPLATE_DEFINE_IMPL(dec, T, DOCTEST_CAT(id, ITERATOR),                      \
                                           DOCTEST_ANONYMOUS(DOCTEST_ANON_TMP_))

#define DOCTEST_TEST_CASE_TEMPLATE_INSTANTIATE_IMPL(id, anon, ...)                                 \
    DOCTEST_GLOBAL_NO_WARNINGS(DOCTEST_CAT(anon, DUMMY), /* NOLINT(cert-err58-cpp, fuchsia-statically-constructed-objects) */ \
        doctest::detail::instantiationHelper(                                                      \
            DOCTEST_CAT(id, ITERATOR)<__VA_ARGS__>(__FILE__, __LINE__, 0)))

#define DOCTEST_TEST_CASE_TEMPLATE_INVOKE(id, ...)                                                 \
    DOCTEST_TEST_CASE_TEMPLATE_INSTANTIATE_IMPL(id, DOCTEST_ANONYMOUS(DOCTEST_ANON_TMP_), std::tuple<__VA_ARGS__>) \
    static_assert(true, "")

#define DOCTEST_TEST_CASE_TEMPLATE_APPLY(id, ...)                                                  \
    DOCTEST_TEST_CASE_TEMPLATE_INSTANTIATE_IMPL(id, DOCTEST_ANONYMOUS(DOCTEST_ANON_TMP_), __VA_ARGS__) \
    static_assert(true, "")

#define DOCTEST_TEST_CASE_TEMPLATE_IMPL(dec, T, anon, ...)                                         \
    DOCTEST_TEST_CASE_TEMPLATE_DEFINE_IMPL(dec, T, DOCTEST_CAT(anon, ITERATOR), anon);             \
    DOCTEST_TEST_CASE_TEMPLATE_INSTANTIATE_IMPL(anon, anon, std::tuple<__VA_ARGS__>)               \
    template <typename T>                                                                          \
    static void anon()

#define DOCTEST_TEST_CASE_TEMPLATE(dec, T, ...)                                                    \
    DOCTEST_TEST_CASE_TEMPLATE_IMPL(dec, T, DOCTEST_ANONYMOUS(DOCTEST_ANON_TMP_), __VA_ARGS__)

// for subcases
#define DOCTEST_SUBCASE(name)                                                                      \
    if(const doctest::detail::Subcase & DOCTEST_ANONYMOUS(DOCTEST_ANON_SUBCASE_) DOCTEST_UNUSED =  \
               doctest::detail::Subcase(name, __FILE__, __LINE__))

// for grouping tests in test suites by using code blocks
#define DOCTEST_TEST_SUITE_IMPL(decorators, ns_name)                                               \
    namespace ns_name { namespace doctest_detail_test_suite_ns {                                   \
            static DOCTEST_NOINLINE doctest::detail::TestSuite& getCurrentTestSuite() noexcept {   \
                DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4640)                                      \
                DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wexit-time-destructors")                \
                DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH("-Wmissing-field-initializers")             \
                static doctest::detail::TestSuite data{};                                          \
                static bool                       inited = false;                                  \
                DOCTEST_MSVC_SUPPRESS_WARNING_POP                                                  \
                DOCTEST_CLANG_SUPPRESS_WARNING_POP                                                 \
                DOCTEST_GCC_SUPPRESS_WARNING_POP                                                   \
                if(!inited) {                                                                      \
                    data* decorators;                                                              \
                    inited = true;                                                                 \
                }                                                                                  \
                return data;                                                                       \
            }                                                                                      \
        }                                                                                          \
    }                                                                                              \
    namespace ns_name

#define DOCTEST_TEST_SUITE(decorators)                                                             \
    DOCTEST_TEST_SUITE_IMPL(decorators, DOCTEST_ANONYMOUS(DOCTEST_ANON_SUITE_))

// for starting a testsuite block
#define DOCTEST_TEST_SUITE_BEGIN(decorators)                                                       \
    DOCTEST_GLOBAL_NO_WARNINGS(DOCTEST_ANONYMOUS(DOCTEST_ANON_VAR_), /* NOLINT(cert-err58-cpp) */  \
            doctest::detail::setTestSuite(doctest::detail::TestSuite() * decorators))              \
    static_assert(true, "")

// for ending a testsuite block
#define DOCTEST_TEST_SUITE_END                                                                     \
    DOCTEST_GLOBAL_NO_WARNINGS(DOCTEST_ANONYMOUS(DOCTEST_ANON_VAR_), /* NOLINT(cert-err58-cpp) */  \
            doctest::detail::setTestSuite(doctest::detail::TestSuite() * ""))                      \
    using DOCTEST_ANONYMOUS(DOCTEST_ANON_FOR_SEMICOLON_) = int

// for registering exception translators
#define DOCTEST_REGISTER_EXCEPTION_TRANSLATOR_IMPL(translatorName, signature)                      \
    inline doctest::String translatorName(signature);                                              \
    DOCTEST_GLOBAL_NO_WARNINGS(DOCTEST_ANONYMOUS(DOCTEST_ANON_TRANSLATOR_), /* NOLINT(cert-err58-cpp) */ \
            doctest::registerExceptionTranslator(translatorName))                                  \
    doctest::String translatorName(signature)

#define DOCTEST_REGISTER_EXCEPTION_TRANSLATOR(signature)                                           \
    DOCTEST_REGISTER_EXCEPTION_TRANSLATOR_IMPL(DOCTEST_ANONYMOUS(DOCTEST_ANON_TRANSLATOR_),        \
                                               signature)

// for registering reporters
#define DOCTEST_REGISTER_REPORTER(name, priority, reporter)                                        \
    DOCTEST_GLOBAL_NO_WARNINGS(DOCTEST_ANONYMOUS(DOCTEST_ANON_REPORTER_), /* NOLINT(cert-err58-cpp) */ \
            doctest::registerReporter<reporter>(name, priority, true))                             \
    static_assert(true, "")

// for registering listeners
#define DOCTEST_REGISTER_LISTENER(name, priority, reporter)                                        \
    DOCTEST_GLOBAL_NO_WARNINGS(DOCTEST_ANONYMOUS(DOCTEST_ANON_REPORTER_), /* NOLINT(cert-err58-cpp) */ \
            doctest::registerReporter<reporter>(name, priority, false))                            \
    static_assert(true, "")

// clang-format off
// for logging - disabling formatting because it's important to have these on 2 separate lines - see PR #557
#define DOCTEST_INFO(...)                                                                          \
    DOCTEST_INFO_IMPL(DOCTEST_ANONYMOUS(DOCTEST_CAPTURE_),                                         \
                      DOCTEST_ANONYMOUS(DOCTEST_CAPTURE_OTHER_),                                   \
                      __VA_ARGS__)
// clang-format on

#define DOCTEST_INFO_IMPL(mb_name, s_name, ...)                                       \
    auto DOCTEST_ANONYMOUS(DOCTEST_CAPTURE_) = doctest::detail::MakeContextScope(                  \
        [&](std::ostream* s_name) {                                                                \
        doctest::detail::MessageBuilder mb_name(__FILE__, __LINE__, doctest::assertType::is_warn); \
        mb_name.m_stream = s_name;                                                                 \
        mb_name * __VA_ARGS__;                                                                     \
    })

#define DOCTEST_CAPTURE(x) DOCTEST_INFO(#x " := ", x)

#define DOCTEST_ADD_AT_IMPL(type, file, line, mb, ...)                                             \
    DOCTEST_FUNC_SCOPE_BEGIN {                                                                     \
        doctest::detail::MessageBuilder mb(file, line, doctest::assertType::type);                 \
        mb * __VA_ARGS__;                                                                          \
        if(mb.log())                                                                               \
            DOCTEST_BREAK_INTO_DEBUGGER();                                                         \
        mb.react();                                                                                \
    } DOCTEST_FUNC_SCOPE_END

// clang-format off
#define DOCTEST_ADD_MESSAGE_AT(file, line, ...) DOCTEST_ADD_AT_IMPL(is_warn, file, line, DOCTEST_ANONYMOUS(DOCTEST_MESSAGE_), __VA_ARGS__)
#define DOCTEST_ADD_FAIL_CHECK_AT(file, line, ...) DOCTEST_ADD_AT_IMPL(is_check, file, line, DOCTEST_ANONYMOUS(DOCTEST_MESSAGE_), __VA_ARGS__)
#define DOCTEST_ADD_FAIL_AT(file, line, ...) DOCTEST_ADD_AT_IMPL(is_require, file, line, DOCTEST_ANONYMOUS(DOCTEST_MESSAGE_), __VA_ARGS__)
// clang-format on

#define DOCTEST_MESSAGE(...) DOCTEST_ADD_MESSAGE_AT(__FILE__, __LINE__, __VA_ARGS__)
#define DOCTEST_FAIL_CHECK(...) DOCTEST_ADD_FAIL_CHECK_AT(__FILE__, __LINE__, __VA_ARGS__)
#define DOCTEST_FAIL(...) DOCTEST_ADD_FAIL_AT(__FILE__, __LINE__, __VA_ARGS__)

#define DOCTEST_TO_LVALUE(...) __VA_ARGS__ // Not removed to keep backwards compatibility.

#ifndef DOCTEST_CONFIG_SUPER_FAST_ASSERTS

#define DOCTEST_ASSERT_IMPLEMENT_2(assert_type, ...)                                               \
    DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Woverloaded-shift-op-parentheses")                  \
    /* NOLINTNEXTLINE(clang-analyzer-cplusplus.NewDeleteLeaks) */                                  \
    doctest::detail::ResultBuilder DOCTEST_RB(doctest::assertType::assert_type, __FILE__,          \
                                               __LINE__, #__VA_ARGS__);                            \
    DOCTEST_WRAP_IN_TRY(DOCTEST_RB.setResult(                                                      \
            doctest::detail::ExpressionDecomposer(doctest::assertType::assert_type)                \
            << __VA_ARGS__)) /* NOLINTNEXTLINE(clang-analyzer-cplusplus.NewDeleteLeaks) */         \
    DOCTEST_ASSERT_LOG_REACT_RETURN(DOCTEST_RB)                                                    \
    DOCTEST_CLANG_SUPPRESS_WARNING_POP

#define DOCTEST_ASSERT_IMPLEMENT_1(assert_type, ...)                                               \
    DOCTEST_FUNC_SCOPE_BEGIN {                                                                     \
        DOCTEST_ASSERT_IMPLEMENT_2(assert_type, __VA_ARGS__);                                      \
    } DOCTEST_FUNC_SCOPE_END // NOLINT(clang-analyzer-cplusplus.NewDeleteLeaks)

#define DOCTEST_BINARY_ASSERT(assert_type, comp, ...)                                              \
    DOCTEST_FUNC_SCOPE_BEGIN {                                                                     \
        doctest::detail::ResultBuilder DOCTEST_RB(doctest::assertType::assert_type, __FILE__,      \
                                                   __LINE__, #__VA_ARGS__);                        \
        DOCTEST_WRAP_IN_TRY(                                                                       \
                DOCTEST_RB.binary_assert<doctest::detail::binaryAssertComparison::comp>(           \
                        __VA_ARGS__))                                                              \
        DOCTEST_ASSERT_LOG_REACT_RETURN(DOCTEST_RB);                                               \
    } DOCTEST_FUNC_SCOPE_END

#define DOCTEST_UNARY_ASSERT(assert_type, ...)                                                     \
    DOCTEST_FUNC_SCOPE_BEGIN {                                                                     \
        doctest::detail::ResultBuilder DOCTEST_RB(doctest::assertType::assert_type, __FILE__,      \
                                                   __LINE__, #__VA_ARGS__);                        \
        DOCTEST_WRAP_IN_TRY(DOCTEST_RB.unary_assert(__VA_ARGS__))                                  \
        DOCTEST_ASSERT_LOG_REACT_RETURN(DOCTEST_RB);                                               \
    } DOCTEST_FUNC_SCOPE_END

#else // DOCTEST_CONFIG_SUPER_FAST_ASSERTS

// necessary for <ASSERT>_MESSAGE
#define DOCTEST_ASSERT_IMPLEMENT_2 DOCTEST_ASSERT_IMPLEMENT_1

#define DOCTEST_ASSERT_IMPLEMENT_1(assert_type, ...)                                               \
    DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Woverloaded-shift-op-parentheses")                  \
    doctest::detail::decomp_assert(                                                                \
            doctest::assertType::assert_type, __FILE__, __LINE__, #__VA_ARGS__,                    \
            doctest::detail::ExpressionDecomposer(doctest::assertType::assert_type)                \
                    << __VA_ARGS__) DOCTEST_CLANG_SUPPRESS_WARNING_POP

#define DOCTEST_BINARY_ASSERT(assert_type, comparison, ...)                                        \
    doctest::detail::binary_assert<doctest::detail::binaryAssertComparison::comparison>(           \
            doctest::assertType::assert_type, __FILE__, __LINE__, #__VA_ARGS__, __VA_ARGS__)

#define DOCTEST_UNARY_ASSERT(assert_type, ...)                                                     \
    doctest::detail::unary_assert(doctest::assertType::assert_type, __FILE__, __LINE__,            \
                                  #__VA_ARGS__, __VA_ARGS__)

#endif // DOCTEST_CONFIG_SUPER_FAST_ASSERTS

#define DOCTEST_WARN(...) DOCTEST_ASSERT_IMPLEMENT_1(DT_WARN, __VA_ARGS__)
#define DOCTEST_CHECK(...) DOCTEST_ASSERT_IMPLEMENT_1(DT_CHECK, __VA_ARGS__)
#define DOCTEST_REQUIRE(...) DOCTEST_ASSERT_IMPLEMENT_1(DT_REQUIRE, __VA_ARGS__)
#define DOCTEST_WARN_FALSE(...) DOCTEST_ASSERT_IMPLEMENT_1(DT_WARN_FALSE, __VA_ARGS__)
#define DOCTEST_CHECK_FALSE(...) DOCTEST_ASSERT_IMPLEMENT_1(DT_CHECK_FALSE, __VA_ARGS__)
#define DOCTEST_REQUIRE_FALSE(...) DOCTEST_ASSERT_IMPLEMENT_1(DT_REQUIRE_FALSE, __VA_ARGS__)

// clang-format off
#define DOCTEST_WARN_MESSAGE(cond, ...) DOCTEST_FUNC_SCOPE_BEGIN { DOCTEST_INFO(__VA_ARGS__); DOCTEST_ASSERT_IMPLEMENT_2(DT_WARN, cond); } DOCTEST_FUNC_SCOPE_END
#define DOCTEST_CHECK_MESSAGE(cond, ...) DOCTEST_FUNC_SCOPE_BEGIN { DOCTEST_INFO(__VA_ARGS__); DOCTEST_ASSERT_IMPLEMENT_2(DT_CHECK, cond); } DOCTEST_FUNC_SCOPE_END
#define DOCTEST_REQUIRE_MESSAGE(cond, ...) DOCTEST_FUNC_SCOPE_BEGIN { DOCTEST_INFO(__VA_ARGS__); DOCTEST_ASSERT_IMPLEMENT_2(DT_REQUIRE, cond); } DOCTEST_FUNC_SCOPE_END
#define DOCTEST_WARN_FALSE_MESSAGE(cond, ...) DOCTEST_FUNC_SCOPE_BEGIN { DOCTEST_INFO(__VA_ARGS__); DOCTEST_ASSERT_IMPLEMENT_2(DT_WARN_FALSE, cond); } DOCTEST_FUNC_SCOPE_END
#define DOCTEST_CHECK_FALSE_MESSAGE(cond, ...) DOCTEST_FUNC_SCOPE_BEGIN { DOCTEST_INFO(__VA_ARGS__); DOCTEST_ASSERT_IMPLEMENT_2(DT_CHECK_FALSE, cond); } DOCTEST_FUNC_SCOPE_END
#define DOCTEST_REQUIRE_FALSE_MESSAGE(cond, ...) DOCTEST_FUNC_SCOPE_BEGIN { DOCTEST_INFO(__VA_ARGS__); DOCTEST_ASSERT_IMPLEMENT_2(DT_REQUIRE_FALSE, cond); } DOCTEST_FUNC_SCOPE_END
// clang-format on

#define DOCTEST_WARN_EQ(...) DOCTEST_BINARY_ASSERT(DT_WARN_EQ, eq, __VA_ARGS__)
#define DOCTEST_CHECK_EQ(...) DOCTEST_BINARY_ASSERT(DT_CHECK_EQ, eq, __VA_ARGS__)
#define DOCTEST_REQUIRE_EQ(...) DOCTEST_BINARY_ASSERT(DT_REQUIRE_EQ, eq, __VA_ARGS__)
#define DOCTEST_WARN_NE(...) DOCTEST_BINARY_ASSERT(DT_WARN_NE, ne, __VA_ARGS__)
#define DOCTEST_CHECK_NE(...) DOCTEST_BINARY_ASSERT(DT_CHECK_NE, ne, __VA_ARGS__)
#define DOCTEST_REQUIRE_NE(...) DOCTEST_BINARY_ASSERT(DT_REQUIRE_NE, ne, __VA_ARGS__)
#define DOCTEST_WARN_GT(...) DOCTEST_BINARY_ASSERT(DT_WARN_GT, gt, __VA_ARGS__)
#define DOCTEST_CHECK_GT(...) DOCTEST_BINARY_ASSERT(DT_CHECK_GT, gt, __VA_ARGS__)
#define DOCTEST_REQUIRE_GT(...) DOCTEST_BINARY_ASSERT(DT_REQUIRE_GT, gt, __VA_ARGS__)
#define DOCTEST_WARN_LT(...) DOCTEST_BINARY_ASSERT(DT_WARN_LT, lt, __VA_ARGS__)
#define DOCTEST_CHECK_LT(...) DOCTEST_BINARY_ASSERT(DT_CHECK_LT, lt, __VA_ARGS__)
#define DOCTEST_REQUIRE_LT(...) DOCTEST_BINARY_ASSERT(DT_REQUIRE_LT, lt, __VA_ARGS__)
#define DOCTEST_WARN_GE(...) DOCTEST_BINARY_ASSERT(DT_WARN_GE, ge, __VA_ARGS__)
#define DOCTEST_CHECK_GE(...) DOCTEST_BINARY_ASSERT(DT_CHECK_GE, ge, __VA_ARGS__)
#define DOCTEST_REQUIRE_GE(...) DOCTEST_BINARY_ASSERT(DT_REQUIRE_GE, ge, __VA_ARGS__)
#define DOCTEST_WARN_LE(...) DOCTEST_BINARY_ASSERT(DT_WARN_LE, le, __VA_ARGS__)
#define DOCTEST_CHECK_LE(...) DOCTEST_BINARY_ASSERT(DT_CHECK_LE, le, __VA_ARGS__)
#define DOCTEST_REQUIRE_LE(...) DOCTEST_BINARY_ASSERT(DT_REQUIRE_LE, le, __VA_ARGS__)

#define DOCTEST_WARN_UNARY(...) DOCTEST_UNARY_ASSERT(DT_WARN_UNARY, __VA_ARGS__)
#define DOCTEST_CHECK_UNARY(...) DOCTEST_UNARY_ASSERT(DT_CHECK_UNARY, __VA_ARGS__)
#define DOCTEST_REQUIRE_UNARY(...) DOCTEST_UNARY_ASSERT(DT_REQUIRE_UNARY, __VA_ARGS__)
#define DOCTEST_WARN_UNARY_FALSE(...) DOCTEST_UNARY_ASSERT(DT_WARN_UNARY_FALSE, __VA_ARGS__)
#define DOCTEST_CHECK_UNARY_FALSE(...) DOCTEST_UNARY_ASSERT(DT_CHECK_UNARY_FALSE, __VA_ARGS__)
#define DOCTEST_REQUIRE_UNARY_FALSE(...) DOCTEST_UNARY_ASSERT(DT_REQUIRE_UNARY_FALSE, __VA_ARGS__)

#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS

#define DOCTEST_ASSERT_THROWS_AS(expr, assert_type, message, ...)                                  \
    DOCTEST_FUNC_SCOPE_BEGIN {                                                                     \
        if(!doctest::getContextOptions()->no_throw) {                                              \
            doctest::detail::ResultBuilder DOCTEST_RB(doctest::assertType::assert_type, __FILE__,  \
                                                       __LINE__, #expr, #__VA_ARGS__, message);    \
            try {                                                                                  \
                DOCTEST_CAST_TO_VOID(expr)                                                         \
            } catch(const typename doctest::detail::types::remove_const<                           \
                    typename doctest::detail::types::remove_reference<__VA_ARGS__>::type>::type&) {\
                DOCTEST_RB.translateException();                                                   \
                DOCTEST_RB.m_threw_as = true;                                                      \
            } catch(...) { DOCTEST_RB.translateException(); }                                      \
            DOCTEST_ASSERT_LOG_REACT_RETURN(DOCTEST_RB);                                           \
        } else { /* NOLINT(*-else-after-return) */                                                 \
            DOCTEST_FUNC_SCOPE_RET(false);                                                         \
        }                                                                                          \
    } DOCTEST_FUNC_SCOPE_END

#define DOCTEST_ASSERT_THROWS_WITH(expr, expr_str, assert_type, ...)                               \
    DOCTEST_FUNC_SCOPE_BEGIN {                                                                     \
        if(!doctest::getContextOptions()->no_throw) {                                              \
            doctest::detail::ResultBuilder DOCTEST_RB(doctest::assertType::assert_type, __FILE__,  \
                                                       __LINE__, expr_str, "", __VA_ARGS__);       \
            try {                                                                                  \
                DOCTEST_CAST_TO_VOID(expr)                                                         \
            } catch(...) { DOCTEST_RB.translateException(); }                                      \
            DOCTEST_ASSERT_LOG_REACT_RETURN(DOCTEST_RB);                                           \
        } else { /* NOLINT(*-else-after-return) */                                                 \
           DOCTEST_FUNC_SCOPE_RET(false);                                                          \
        }                                                                                          \
    } DOCTEST_FUNC_SCOPE_END

#define DOCTEST_ASSERT_NOTHROW(assert_type, ...)                                                   \
    DOCTEST_FUNC_SCOPE_BEGIN {                                                                     \
        doctest::detail::ResultBuilder DOCTEST_RB(doctest::assertType::assert_type, __FILE__,      \
                                                   __LINE__, #__VA_ARGS__);                        \
        try {                                                                                      \
            DOCTEST_CAST_TO_VOID(__VA_ARGS__)                                                      \
        } catch(...) { DOCTEST_RB.translateException(); }                                          \
        DOCTEST_ASSERT_LOG_REACT_RETURN(DOCTEST_RB);                                               \
    } DOCTEST_FUNC_SCOPE_END

// clang-format off
#define DOCTEST_WARN_THROWS(...) DOCTEST_ASSERT_THROWS_WITH((__VA_ARGS__), #__VA_ARGS__, DT_WARN_THROWS, "")
#define DOCTEST_CHECK_THROWS(...) DOCTEST_ASSERT_THROWS_WITH((__VA_ARGS__), #__VA_ARGS__, DT_CHECK_THROWS, "")
#define DOCTEST_REQUIRE_THROWS(...) DOCTEST_ASSERT_THROWS_WITH((__VA_ARGS__), #__VA_ARGS__, DT_REQUIRE_THROWS, "")

#define DOCTEST_WARN_THROWS_AS(expr, ...) DOCTEST_ASSERT_THROWS_AS(expr, DT_WARN_THROWS_AS, "", __VA_ARGS__)
#define DOCTEST_CHECK_THROWS_AS(expr, ...) DOCTEST_ASSERT_THROWS_AS(expr, DT_CHECK_THROWS_AS, "", __VA_ARGS__)
#define DOCTEST_REQUIRE_THROWS_AS(expr, ...) DOCTEST_ASSERT_THROWS_AS(expr, DT_REQUIRE_THROWS_AS, "", __VA_ARGS__)

#define DOCTEST_WARN_THROWS_WITH(expr, ...) DOCTEST_ASSERT_THROWS_WITH(expr, #expr, DT_WARN_THROWS_WITH, __VA_ARGS__)
#define DOCTEST_CHECK_THROWS_WITH(expr, ...) DOCTEST_ASSERT_THROWS_WITH(expr, #expr, DT_CHECK_THROWS_WITH, __VA_ARGS__)
#define DOCTEST_REQUIRE_THROWS_WITH(expr, ...) DOCTEST_ASSERT_THROWS_WITH(expr, #expr, DT_REQUIRE_THROWS_WITH, __VA_ARGS__)

#define DOCTEST_WARN_THROWS_WITH_AS(expr, message, ...) DOCTEST_ASSERT_THROWS_AS(expr, DT_WARN_THROWS_WITH_AS, message, __VA_ARGS__)
#define DOCTEST_CHECK_THROWS_WITH_AS(expr, message, ...) DOCTEST_ASSERT_THROWS_AS(expr, DT_CHECK_THROWS_WITH_AS, message, __VA_ARGS__)
#define DOCTEST_REQUIRE_THROWS_WITH_AS(expr, message, ...) DOCTEST_ASSERT_THROWS_AS(expr, DT_REQUIRE_THROWS_WITH_AS, message, __VA_ARGS__)

#define DOCTEST_WARN_NOTHROW(...) DOCTEST_ASSERT_NOTHROW(DT_WARN_NOTHROW, __VA_ARGS__)
#define DOCTEST_CHECK_NOTHROW(...) DOCTEST_ASSERT_NOTHROW(DT_CHECK_NOTHROW, __VA_ARGS__)
#define DOCTEST_REQUIRE_NOTHROW(...) DOCTEST_ASSERT_NOTHROW(DT_REQUIRE_NOTHROW, __VA_ARGS__)

#define DOCTEST_WARN_THROWS_MESSAGE(expr, ...) DOCTEST_FUNC_SCOPE_BEGIN { DOCTEST_INFO(__VA_ARGS__); DOCTEST_WARN_THROWS(expr); } DOCTEST_FUNC_SCOPE_END
#define DOCTEST_CHECK_THROWS_MESSAGE(expr, ...) DOCTEST_FUNC_SCOPE_BEGIN { DOCTEST_INFO(__VA_ARGS__); DOCTEST_CHECK_THROWS(expr); } DOCTEST_FUNC_SCOPE_END
#define DOCTEST_REQUIRE_THROWS_MESSAGE(expr, ...) DOCTEST_FUNC_SCOPE_BEGIN { DOCTEST_INFO(__VA_ARGS__); DOCTEST_REQUIRE_THROWS(expr); } DOCTEST_FUNC_SCOPE_END
#define DOCTEST_WARN_THROWS_AS_MESSAGE(expr, ex, ...) DOCTEST_FUNC_SCOPE_BEGIN { DOCTEST_INFO(__VA_ARGS__); DOCTEST_WARN_THROWS_AS(expr, ex); } DOCTEST_FUNC_SCOPE_END
#define DOCTEST_CHECK_THROWS_AS_MESSAGE(expr, ex, ...) DOCTEST_FUNC_SCOPE_BEGIN { DOCTEST_INFO(__VA_ARGS__); DOCTEST_CHECK_THROWS_AS(expr, ex); } DOCTEST_FUNC_SCOPE_END
#define DOCTEST_REQUIRE_THROWS_AS_MESSAGE(expr, ex, ...) DOCTEST_FUNC_SCOPE_BEGIN { DOCTEST_INFO(__VA_ARGS__); DOCTEST_REQUIRE_THROWS_AS(expr, ex); } DOCTEST_FUNC_SCOPE_END
#define DOCTEST_WARN_THROWS_WITH_MESSAGE(expr, with, ...) DOCTEST_FUNC_SCOPE_BEGIN { DOCTEST_INFO(__VA_ARGS__); DOCTEST_WARN_THROWS_WITH(expr, with); } DOCTEST_FUNC_SCOPE_END
#define DOCTEST_CHECK_THROWS_WITH_MESSAGE(expr, with, ...) DOCTEST_FUNC_SCOPE_BEGIN { DOCTEST_INFO(__VA_ARGS__); DOCTEST_CHECK_THROWS_WITH(expr, with); } DOCTEST_FUNC_SCOPE_END
#define DOCTEST_REQUIRE_THROWS_WITH_MESSAGE(expr, with, ...) DOCTEST_FUNC_SCOPE_BEGIN { DOCTEST_INFO(__VA_ARGS__); DOCTEST_REQUIRE_THROWS_WITH(expr, with); } DOCTEST_FUNC_SCOPE_END
#define DOCTEST_WARN_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) DOCTEST_FUNC_SCOPE_BEGIN { DOCTEST_INFO(__VA_ARGS__); DOCTEST_WARN_THROWS_WITH_AS(expr, with, ex); } DOCTEST_FUNC_SCOPE_END
#define DOCTEST_CHECK_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) DOCTEST_FUNC_SCOPE_BEGIN { DOCTEST_INFO(__VA_ARGS__); DOCTEST_CHECK_THROWS_WITH_AS(expr, with, ex); } DOCTEST_FUNC_SCOPE_END
#define DOCTEST_REQUIRE_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) DOCTEST_FUNC_SCOPE_BEGIN { DOCTEST_INFO(__VA_ARGS__); DOCTEST_REQUIRE_THROWS_WITH_AS(expr, with, ex); } DOCTEST_FUNC_SCOPE_END
#define DOCTEST_WARN_NOTHROW_MESSAGE(expr, ...) DOCTEST_FUNC_SCOPE_BEGIN { DOCTEST_INFO(__VA_ARGS__); DOCTEST_WARN_NOTHROW(expr); } DOCTEST_FUNC_SCOPE_END
#define DOCTEST_CHECK_NOTHROW_MESSAGE(expr, ...) DOCTEST_FUNC_SCOPE_BEGIN { DOCTEST_INFO(__VA_ARGS__); DOCTEST_CHECK_NOTHROW(expr); } DOCTEST_FUNC_SCOPE_END
#define DOCTEST_REQUIRE_NOTHROW_MESSAGE(expr, ...) DOCTEST_FUNC_SCOPE_BEGIN { DOCTEST_INFO(__VA_ARGS__); DOCTEST_REQUIRE_NOTHROW(expr); } DOCTEST_FUNC_SCOPE_END
// clang-format on

#endif // DOCTEST_CONFIG_NO_EXCEPTIONS

// =================================================================================================
// == WHAT FOLLOWS IS VERSIONS OF THE MACROS THAT DO NOT DO ANY REGISTERING!                      ==
// == THIS CAN BE ENABLED BY DEFINING DOCTEST_CONFIG_DISABLE GLOBALLY!                            ==
// =================================================================================================
#else // DOCTEST_CONFIG_DISABLE

#define DOCTEST_IMPLEMENT_FIXTURE(der, base, func, name)                                           \
    namespace /* NOLINT */ {                                                                       \
        template <typename DOCTEST_UNUSED_TEMPLATE_TYPE>                                           \
        struct der : public base                                                                   \
        { void f(); };                                                                             \
    }                                                                                              \
    template <typename DOCTEST_UNUSED_TEMPLATE_TYPE>                                               \
    inline void der<DOCTEST_UNUSED_TEMPLATE_TYPE>::f()

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION(f, name)                                              \
    template <typename DOCTEST_UNUSED_TEMPLATE_TYPE>                                               \
    static inline void f()

// for registering tests
#define DOCTEST_TEST_CASE(name)                                                                    \
    DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(DOCTEST_ANON_FUNC_), name)

// for registering tests in classes
#define DOCTEST_TEST_CASE_CLASS(name)                                                              \
    DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(DOCTEST_ANON_FUNC_), name)

// for registering tests with a fixture
#define DOCTEST_TEST_CASE_FIXTURE(x, name)                                                         \
    DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS(DOCTEST_ANON_CLASS_), x,                           \
                              DOCTEST_ANONYMOUS(DOCTEST_ANON_FUNC_), name)

// for converting types to strings without the <typeinfo> header and demangling
#define DOCTEST_TYPE_TO_STRING_AS(str, ...) static_assert(true, "")
#define DOCTEST_TYPE_TO_STRING(...) static_assert(true, "")

// for typed tests
#define DOCTEST_TEST_CASE_TEMPLATE(name, type, ...)                                                \
    template <typename type>                                                                       \
    inline void DOCTEST_ANONYMOUS(DOCTEST_ANON_TMP_)()

#define DOCTEST_TEST_CASE_TEMPLATE_DEFINE(name, type, id)                                          \
    template <typename type>                                                                       \
    inline void DOCTEST_ANONYMOUS(DOCTEST_ANON_TMP_)()

#define DOCTEST_TEST_CASE_TEMPLATE_INVOKE(id, ...) static_assert(true, "")
#define DOCTEST_TEST_CASE_TEMPLATE_APPLY(id, ...) static_assert(true, "")

// for subcases
#define DOCTEST_SUBCASE(name)

// for a testsuite block
#define DOCTEST_TEST_SUITE(name) namespace // NOLINT

// for starting a testsuite block
#define DOCTEST_TEST_SUITE_BEGIN(name) static_assert(true, "")

// for ending a testsuite block
#define DOCTEST_TEST_SUITE_END using DOCTEST_ANONYMOUS(DOCTEST_ANON_FOR_SEMICOLON_) = int

#define DOCTEST_REGISTER_EXCEPTION_TRANSLATOR(signature)                                           \
    template <typename DOCTEST_UNUSED_TEMPLATE_TYPE>                                               \
    static inline doctest::String DOCTEST_ANONYMOUS(DOCTEST_ANON_TRANSLATOR_)(signature)

#define DOCTEST_REGISTER_REPORTER(name, priority, reporter)
#define DOCTEST_REGISTER_LISTENER(name, priority, reporter)

#define DOCTEST_INFO(...) (static_cast<void>(0))
#define DOCTEST_CAPTURE(x) (static_cast<void>(0))
#define DOCTEST_ADD_MESSAGE_AT(file, line, ...) (static_cast<void>(0))
#define DOCTEST_ADD_FAIL_CHECK_AT(file, line, ...) (static_cast<void>(0))
#define DOCTEST_ADD_FAIL_AT(file, line, ...) (static_cast<void>(0))
#define DOCTEST_MESSAGE(...) (static_cast<void>(0))
#define DOCTEST_FAIL_CHECK(...) (static_cast<void>(0))
#define DOCTEST_FAIL(...) (static_cast<void>(0))

#if defined(DOCTEST_CONFIG_EVALUATE_ASSERTS_EVEN_WHEN_DISABLED)                                    \
 && defined(DOCTEST_CONFIG_ASSERTS_RETURN_VALUES)

#define DOCTEST_WARN(...) [&] { return __VA_ARGS__; }()
#define DOCTEST_CHECK(...) [&] { return __VA_ARGS__; }()
#define DOCTEST_REQUIRE(...) [&] { return __VA_ARGS__; }()
#define DOCTEST_WARN_FALSE(...) [&] { return !(__VA_ARGS__); }()
#define DOCTEST_CHECK_FALSE(...) [&] { return !(__VA_ARGS__); }()
#define DOCTEST_REQUIRE_FALSE(...) [&] { return !(__VA_ARGS__); }()

#define DOCTEST_WARN_MESSAGE(cond, ...) [&] { return cond; }()
#define DOCTEST_CHECK_MESSAGE(cond, ...) [&] { return cond; }()
#define DOCTEST_REQUIRE_MESSAGE(cond, ...) [&] { return cond; }()
#define DOCTEST_WARN_FALSE_MESSAGE(cond, ...) [&] { return !(cond); }()
#define DOCTEST_CHECK_FALSE_MESSAGE(cond, ...) [&] { return !(cond); }()
#define DOCTEST_REQUIRE_FALSE_MESSAGE(cond, ...) [&] { return !(cond); }()

namespace doctest {
namespace detail {
#define DOCTEST_RELATIONAL_OP(name, op)                                                            \
    template <typename L, typename R>                                                              \
    bool name(const DOCTEST_REF_WRAP(L) lhs, const DOCTEST_REF_WRAP(R) rhs) { return lhs op rhs; }

    DOCTEST_RELATIONAL_OP(eq, ==)
    DOCTEST_RELATIONAL_OP(ne, !=)
    DOCTEST_RELATIONAL_OP(lt, <)
    DOCTEST_RELATIONAL_OP(gt, >)
    DOCTEST_RELATIONAL_OP(le, <=)
    DOCTEST_RELATIONAL_OP(ge, >=)
} // namespace detail
} // namespace doctest

#define DOCTEST_WARN_EQ(...) [&] { return doctest::detail::eq(__VA_ARGS__); }()
#define DOCTEST_CHECK_EQ(...) [&] { return doctest::detail::eq(__VA_ARGS__); }()
#define DOCTEST_REQUIRE_EQ(...) [&] { return doctest::detail::eq(__VA_ARGS__); }()
#define DOCTEST_WARN_NE(...) [&] { return doctest::detail::ne(__VA_ARGS__); }()
#define DOCTEST_CHECK_NE(...) [&] { return doctest::detail::ne(__VA_ARGS__); }()
#define DOCTEST_REQUIRE_NE(...) [&] { return doctest::detail::ne(__VA_ARGS__); }()
#define DOCTEST_WARN_LT(...) [&] { return doctest::detail::lt(__VA_ARGS__); }()
#define DOCTEST_CHECK_LT(...) [&] { return doctest::detail::lt(__VA_ARGS__); }()
#define DOCTEST_REQUIRE_LT(...) [&] { return doctest::detail::lt(__VA_ARGS__); }()
#define DOCTEST_WARN_GT(...) [&] { return doctest::detail::gt(__VA_ARGS__); }()
#define DOCTEST_CHECK_GT(...) [&] { return doctest::detail::gt(__VA_ARGS__); }()
#define DOCTEST_REQUIRE_GT(...) [&] { return doctest::detail::gt(__VA_ARGS__); }()
#define DOCTEST_WARN_LE(...) [&] { return doctest::detail::le(__VA_ARGS__); }()
#define DOCTEST_CHECK_LE(...) [&] { return doctest::detail::le(__VA_ARGS__); }()
#define DOCTEST_REQUIRE_LE(...) [&] { return doctest::detail::le(__VA_ARGS__); }()
#define DOCTEST_WARN_GE(...) [&] { return doctest::detail::ge(__VA_ARGS__); }()
#define DOCTEST_CHECK_GE(...) [&] { return doctest::detail::ge(__VA_ARGS__); }()
#define DOCTEST_REQUIRE_GE(...) [&] { return doctest::detail::ge(__VA_ARGS__); }()
#define DOCTEST_WARN_UNARY(...) [&] { return __VA_ARGS__; }()
#define DOCTEST_CHECK_UNARY(...) [&] { return __VA_ARGS__; }()
#define DOCTEST_REQUIRE_UNARY(...) [&] { return __VA_ARGS__; }()
#define DOCTEST_WARN_UNARY_FALSE(...) [&] { return !(__VA_ARGS__); }()
#define DOCTEST_CHECK_UNARY_FALSE(...) [&] { return !(__VA_ARGS__); }()
#define DOCTEST_REQUIRE_UNARY_FALSE(...) [&] { return !(__VA_ARGS__); }()

#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS

#define DOCTEST_WARN_THROWS_WITH(expr, with, ...) [] { static_assert(false, "Exception translation is not available when doctest is disabled."); return false; }()
#define DOCTEST_CHECK_THROWS_WITH(expr, with, ...) DOCTEST_WARN_THROWS_WITH(,,)
#define DOCTEST_REQUIRE_THROWS_WITH(expr, with, ...) DOCTEST_WARN_THROWS_WITH(,,)
#define DOCTEST_WARN_THROWS_WITH_AS(expr, with, ex, ...) DOCTEST_WARN_THROWS_WITH(,,)
#define DOCTEST_CHECK_THROWS_WITH_AS(expr, with, ex, ...) DOCTEST_WARN_THROWS_WITH(,,)
#define DOCTEST_REQUIRE_THROWS_WITH_AS(expr, with, ex, ...) DOCTEST_WARN_THROWS_WITH(,,)

#define DOCTEST_WARN_THROWS_WITH_MESSAGE(expr, with, ...) DOCTEST_WARN_THROWS_WITH(,,)
#define DOCTEST_CHECK_THROWS_WITH_MESSAGE(expr, with, ...) DOCTEST_WARN_THROWS_WITH(,,)
#define DOCTEST_REQUIRE_THROWS_WITH_MESSAGE(expr, with, ...) DOCTEST_WARN_THROWS_WITH(,,)
#define DOCTEST_WARN_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) DOCTEST_WARN_THROWS_WITH(,,)
#define DOCTEST_CHECK_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) DOCTEST_WARN_THROWS_WITH(,,)
#define DOCTEST_REQUIRE_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) DOCTEST_WARN_THROWS_WITH(,,)

#define DOCTEST_WARN_THROWS(...) [&] { try { __VA_ARGS__; return false; } catch (...) { return true; } }()
#define DOCTEST_CHECK_THROWS(...) [&] { try { __VA_ARGS__; return false; } catch (...) { return true; } }()
#define DOCTEST_REQUIRE_THROWS(...) [&] { try { __VA_ARGS__; return false; } catch (...) { return true; } }()
#define DOCTEST_WARN_THROWS_AS(expr, ...) [&] { try { expr; } catch (__VA_ARGS__) { return true; } catch (...) { } return false; }()
#define DOCTEST_CHECK_THROWS_AS(expr, ...) [&] { try { expr; } catch (__VA_ARGS__) { return true; } catch (...) { } return false; }()
#define DOCTEST_REQUIRE_THROWS_AS(expr, ...) [&] { try { expr; } catch (__VA_ARGS__) { return true; } catch (...) { } return false; }()
#define DOCTEST_WARN_NOTHROW(...) [&] { try { __VA_ARGS__; return true; } catch (...) { return false; } }()
#define DOCTEST_CHECK_NOTHROW(...) [&] { try { __VA_ARGS__; return true; } catch (...) { return false; } }()
#define DOCTEST_REQUIRE_NOTHROW(...) [&] { try { __VA_ARGS__; return true; } catch (...) { return false; } }()

#define DOCTEST_WARN_THROWS_MESSAGE(expr, ...) [&] { try { __VA_ARGS__; return false; } catch (...) { return true; } }()
#define DOCTEST_CHECK_THROWS_MESSAGE(expr, ...) [&] { try { __VA_ARGS__; return false; } catch (...) { return true; } }()
#define DOCTEST_REQUIRE_THROWS_MESSAGE(expr, ...) [&] { try { __VA_ARGS__; return false; } catch (...) { return true; } }()
#define DOCTEST_WARN_THROWS_AS_MESSAGE(expr, ex, ...) [&] { try { expr; } catch (__VA_ARGS__) { return true; } catch (...) { } return false; }()
#define DOCTEST_CHECK_THROWS_AS_MESSAGE(expr, ex, ...) [&] { try { expr; } catch (__VA_ARGS__) { return true; } catch (...) { } return false; }()
#define DOCTEST_REQUIRE_THROWS_AS_MESSAGE(expr, ex, ...) [&] { try { expr; } catch (__VA_ARGS__) { return true; } catch (...) { } return false; }()
#define DOCTEST_WARN_NOTHROW_MESSAGE(expr, ...) [&] { try { __VA_ARGS__; return true; } catch (...) { return false; } }()
#define DOCTEST_CHECK_NOTHROW_MESSAGE(expr, ...) [&] { try { __VA_ARGS__; return true; } catch (...) { return false; } }()
#define DOCTEST_REQUIRE_NOTHROW_MESSAGE(expr, ...) [&] { try { __VA_ARGS__; return true; } catch (...) { return false; } }()

#endif // DOCTEST_CONFIG_NO_EXCEPTIONS

#else // DOCTEST_CONFIG_EVALUATE_ASSERTS_EVEN_WHEN_DISABLED

#define DOCTEST_WARN(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_CHECK(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_REQUIRE(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_WARN_FALSE(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_CHECK_FALSE(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_REQUIRE_FALSE(...) DOCTEST_FUNC_EMPTY

#define DOCTEST_WARN_MESSAGE(cond, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_CHECK_MESSAGE(cond, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_REQUIRE_MESSAGE(cond, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_WARN_FALSE_MESSAGE(cond, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_CHECK_FALSE_MESSAGE(cond, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_REQUIRE_FALSE_MESSAGE(cond, ...) DOCTEST_FUNC_EMPTY

#define DOCTEST_WARN_EQ(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_CHECK_EQ(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_REQUIRE_EQ(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_WARN_NE(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_CHECK_NE(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_REQUIRE_NE(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_WARN_GT(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_CHECK_GT(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_REQUIRE_GT(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_WARN_LT(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_CHECK_LT(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_REQUIRE_LT(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_WARN_GE(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_CHECK_GE(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_REQUIRE_GE(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_WARN_LE(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_CHECK_LE(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_REQUIRE_LE(...) DOCTEST_FUNC_EMPTY

#define DOCTEST_WARN_UNARY(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_CHECK_UNARY(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_REQUIRE_UNARY(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_WARN_UNARY_FALSE(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_CHECK_UNARY_FALSE(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_REQUIRE_UNARY_FALSE(...) DOCTEST_FUNC_EMPTY

#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS

#define DOCTEST_WARN_THROWS(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_CHECK_THROWS(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_REQUIRE_THROWS(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_WARN_THROWS_AS(expr, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_CHECK_THROWS_AS(expr, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_REQUIRE_THROWS_AS(expr, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_WARN_THROWS_WITH(expr, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_CHECK_THROWS_WITH(expr, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_REQUIRE_THROWS_WITH(expr, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_WARN_THROWS_WITH_AS(expr, with, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_CHECK_THROWS_WITH_AS(expr, with, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_REQUIRE_THROWS_WITH_AS(expr, with, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_WARN_NOTHROW(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_CHECK_NOTHROW(...) DOCTEST_FUNC_EMPTY
#define DOCTEST_REQUIRE_NOTHROW(...) DOCTEST_FUNC_EMPTY

#define DOCTEST_WARN_THROWS_MESSAGE(expr, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_CHECK_THROWS_MESSAGE(expr, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_REQUIRE_THROWS_MESSAGE(expr, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_WARN_THROWS_AS_MESSAGE(expr, ex, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_CHECK_THROWS_AS_MESSAGE(expr, ex, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_REQUIRE_THROWS_AS_MESSAGE(expr, ex, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_WARN_THROWS_WITH_MESSAGE(expr, with, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_CHECK_THROWS_WITH_MESSAGE(expr, with, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_REQUIRE_THROWS_WITH_MESSAGE(expr, with, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_WARN_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_CHECK_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_REQUIRE_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_WARN_NOTHROW_MESSAGE(expr, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_CHECK_NOTHROW_MESSAGE(expr, ...) DOCTEST_FUNC_EMPTY
#define DOCTEST_REQUIRE_NOTHROW_MESSAGE(expr, ...) DOCTEST_FUNC_EMPTY

#endif // DOCTEST_CONFIG_NO_EXCEPTIONS

#endif // DOCTEST_CONFIG_EVALUATE_ASSERTS_EVEN_WHEN_DISABLED

#endif // DOCTEST_CONFIG_DISABLE

#ifdef DOCTEST_CONFIG_NO_EXCEPTIONS

#ifdef DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#define DOCTEST_EXCEPTION_EMPTY_FUNC DOCTEST_FUNC_EMPTY
#else // DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#define DOCTEST_EXCEPTION_EMPTY_FUNC [] { static_assert(false, "Exceptions are disabled! " \
    "Use DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS if you want to compile with exceptions disabled."); return false; }()

#undef DOCTEST_REQUIRE
#undef DOCTEST_REQUIRE_FALSE
#undef DOCTEST_REQUIRE_MESSAGE
#undef DOCTEST_REQUIRE_FALSE_MESSAGE
#undef DOCTEST_REQUIRE_EQ
#undef DOCTEST_REQUIRE_NE
#undef DOCTEST_REQUIRE_GT
#undef DOCTEST_REQUIRE_LT
#undef DOCTEST_REQUIRE_GE
#undef DOCTEST_REQUIRE_LE
#undef DOCTEST_REQUIRE_UNARY
#undef DOCTEST_REQUIRE_UNARY_FALSE

#define DOCTEST_REQUIRE DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_REQUIRE_FALSE DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_REQUIRE_MESSAGE DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_REQUIRE_FALSE_MESSAGE DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_REQUIRE_EQ DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_REQUIRE_NE DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_REQUIRE_GT DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_REQUIRE_LT DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_REQUIRE_GE DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_REQUIRE_LE DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_REQUIRE_UNARY DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_REQUIRE_UNARY_FALSE DOCTEST_EXCEPTION_EMPTY_FUNC

#endif // DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS

#define DOCTEST_WARN_THROWS(...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_CHECK_THROWS(...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_REQUIRE_THROWS(...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_WARN_THROWS_AS(expr, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_CHECK_THROWS_AS(expr, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_REQUIRE_THROWS_AS(expr, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_WARN_THROWS_WITH(expr, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_CHECK_THROWS_WITH(expr, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_REQUIRE_THROWS_WITH(expr, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_WARN_THROWS_WITH_AS(expr, with, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_CHECK_THROWS_WITH_AS(expr, with, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_REQUIRE_THROWS_WITH_AS(expr, with, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_WARN_NOTHROW(...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_CHECK_NOTHROW(...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_REQUIRE_NOTHROW(...) DOCTEST_EXCEPTION_EMPTY_FUNC

#define DOCTEST_WARN_THROWS_MESSAGE(expr, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_CHECK_THROWS_MESSAGE(expr, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_REQUIRE_THROWS_MESSAGE(expr, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_WARN_THROWS_AS_MESSAGE(expr, ex, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_CHECK_THROWS_AS_MESSAGE(expr, ex, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_REQUIRE_THROWS_AS_MESSAGE(expr, ex, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_WARN_THROWS_WITH_MESSAGE(expr, with, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_CHECK_THROWS_WITH_MESSAGE(expr, with, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_REQUIRE_THROWS_WITH_MESSAGE(expr, with, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_WARN_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_CHECK_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_REQUIRE_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_WARN_NOTHROW_MESSAGE(expr, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_CHECK_NOTHROW_MESSAGE(expr, ...) DOCTEST_EXCEPTION_EMPTY_FUNC
#define DOCTEST_REQUIRE_NOTHROW_MESSAGE(expr, ...) DOCTEST_EXCEPTION_EMPTY_FUNC

#endif // DOCTEST_CONFIG_NO_EXCEPTIONS

// clang-format off
// KEPT FOR BACKWARDS COMPATIBILITY - FORWARDING TO THE RIGHT MACROS
#define DOCTEST_FAST_WARN_EQ             DOCTEST_WARN_EQ
#define DOCTEST_FAST_CHECK_EQ            DOCTEST_CHECK_EQ
#define DOCTEST_FAST_REQUIRE_EQ          DOCTEST_REQUIRE_EQ
#define DOCTEST_FAST_WARN_NE             DOCTEST_WARN_NE
#define DOCTEST_FAST_CHECK_NE            DOCTEST_CHECK_NE
#define DOCTEST_FAST_REQUIRE_NE          DOCTEST_REQUIRE_NE
#define DOCTEST_FAST_WARN_GT             DOCTEST_WARN_GT
#define DOCTEST_FAST_CHECK_GT            DOCTEST_CHECK_GT
#define DOCTEST_FAST_REQUIRE_GT          DOCTEST_REQUIRE_GT
#define DOCTEST_FAST_WARN_LT             DOCTEST_WARN_LT
#define DOCTEST_FAST_CHECK_LT            DOCTEST_CHECK_LT
#define DOCTEST_FAST_REQUIRE_LT          DOCTEST_REQUIRE_LT
#define DOCTEST_FAST_WARN_GE             DOCTEST_WARN_GE
#define DOCTEST_FAST_CHECK_GE            DOCTEST_CHECK_GE
#define DOCTEST_FAST_REQUIRE_GE          DOCTEST_REQUIRE_GE
#define DOCTEST_FAST_WARN_LE             DOCTEST_WARN_LE
#define DOCTEST_FAST_CHECK_LE            DOCTEST_CHECK_LE
#define DOCTEST_FAST_REQUIRE_LE          DOCTEST_REQUIRE_LE

#define DOCTEST_FAST_WARN_UNARY          DOCTEST_WARN_UNARY
#define DOCTEST_FAST_CHECK_UNARY         DOCTEST_CHECK_UNARY
#define DOCTEST_FAST_REQUIRE_UNARY       DOCTEST_REQUIRE_UNARY
#define DOCTEST_FAST_WARN_UNARY_FALSE    DOCTEST_WARN_UNARY_FALSE
#define DOCTEST_FAST_CHECK_UNARY_FALSE   DOCTEST_CHECK_UNARY_FALSE
#define DOCTEST_FAST_REQUIRE_UNARY_FALSE DOCTEST_REQUIRE_UNARY_FALSE

#define DOCTEST_TEST_CASE_TEMPLATE_INSTANTIATE(id, ...) DOCTEST_TEST_CASE_TEMPLATE_INVOKE(id,__VA_ARGS__)
// clang-format on

// BDD style macros
// clang-format off
#define DOCTEST_SCENARIO(name) DOCTEST_TEST_CASE("  Scenario: " name)
#define DOCTEST_SCENARIO_CLASS(name) DOCTEST_TEST_CASE_CLASS("  Scenario: " name)
#define DOCTEST_SCENARIO_TEMPLATE(name, T, ...)  DOCTEST_TEST_CASE_TEMPLATE("  Scenario: " name, T, __VA_ARGS__)
#define DOCTEST_SCENARIO_TEMPLATE_DEFINE(name, T, id) DOCTEST_TEST_CASE_TEMPLATE_DEFINE("  Scenario: " name, T, id)

#define DOCTEST_GIVEN(name)     DOCTEST_SUBCASE("   Given: " name)
#define DOCTEST_WHEN(name)      DOCTEST_SUBCASE("    When: " name)
#define DOCTEST_AND_WHEN(name)  DOCTEST_SUBCASE("And when: " name)
#define DOCTEST_THEN(name)      DOCTEST_SUBCASE("    Then: " name)
#define DOCTEST_AND_THEN(name)  DOCTEST_SUBCASE("     And: " name)
// clang-format on

// == SHORT VERSIONS OF THE MACROS
#ifndef DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES

#define TEST_CASE(name) DOCTEST_TEST_CASE(name)
#define TEST_CASE_CLASS(name) DOCTEST_TEST_CASE_CLASS(name)
#define TEST_CASE_FIXTURE(x, name) DOCTEST_TEST_CASE_FIXTURE(x, name)
#define TYPE_TO_STRING_AS(str, ...) DOCTEST_TYPE_TO_STRING_AS(str, __VA_ARGS__)
#define TYPE_TO_STRING(...) DOCTEST_TYPE_TO_STRING(__VA_ARGS__)
#define TEST_CASE_TEMPLATE(name, T, ...) DOCTEST_TEST_CASE_TEMPLATE(name, T, __VA_ARGS__)
#define TEST_CASE_TEMPLATE_DEFINE(name, T, id) DOCTEST_TEST_CASE_TEMPLATE_DEFINE(name, T, id)
#define TEST_CASE_TEMPLATE_INVOKE(id, ...) DOCTEST_TEST_CASE_TEMPLATE_INVOKE(id, __VA_ARGS__)
#define TEST_CASE_TEMPLATE_APPLY(id, ...) DOCTEST_TEST_CASE_TEMPLATE_APPLY(id, __VA_ARGS__)
#define SUBCASE(name) DOCTEST_SUBCASE(name)
#define TEST_SUITE(decorators) DOCTEST_TEST_SUITE(decorators)
#define TEST_SUITE_BEGIN(name) DOCTEST_TEST_SUITE_BEGIN(name)
#define TEST_SUITE_END DOCTEST_TEST_SUITE_END
#define REGISTER_EXCEPTION_TRANSLATOR(signature) DOCTEST_REGISTER_EXCEPTION_TRANSLATOR(signature)
#define REGISTER_REPORTER(name, priority, reporter) DOCTEST_REGISTER_REPORTER(name, priority, reporter)
#define REGISTER_LISTENER(name, priority, reporter) DOCTEST_REGISTER_LISTENER(name, priority, reporter)
#define INFO(...) DOCTEST_INFO(__VA_ARGS__)
#define CAPTURE(x) DOCTEST_CAPTURE(x)
#define ADD_MESSAGE_AT(file, line, ...) DOCTEST_ADD_MESSAGE_AT(file, line, __VA_ARGS__)
#define ADD_FAIL_CHECK_AT(file, line, ...) DOCTEST_ADD_FAIL_CHECK_AT(file, line, __VA_ARGS__)
#define ADD_FAIL_AT(file, line, ...) DOCTEST_ADD_FAIL_AT(file, line, __VA_ARGS__)
#define MESSAGE(...) DOCTEST_MESSAGE(__VA_ARGS__)
#define FAIL_CHECK(...) DOCTEST_FAIL_CHECK(__VA_ARGS__)
#define FAIL(...) DOCTEST_FAIL(__VA_ARGS__)
#define TO_LVALUE(...) DOCTEST_TO_LVALUE(__VA_ARGS__)

#define WARN(...) DOCTEST_WARN(__VA_ARGS__)
#define WARN_FALSE(...) DOCTEST_WARN_FALSE(__VA_ARGS__)
#define WARN_THROWS(...) DOCTEST_WARN_THROWS(__VA_ARGS__)
#define WARN_THROWS_AS(expr, ...) DOCTEST_WARN_THROWS_AS(expr, __VA_ARGS__)
#define WARN_THROWS_WITH(expr, ...) DOCTEST_WARN_THROWS_WITH(expr, __VA_ARGS__)
#define WARN_THROWS_WITH_AS(expr, with, ...) DOCTEST_WARN_THROWS_WITH_AS(expr, with, __VA_ARGS__)
#define WARN_NOTHROW(...) DOCTEST_WARN_NOTHROW(__VA_ARGS__)
#define CHECK(...) DOCTEST_CHECK(__VA_ARGS__)
#define CHECK_FALSE(...) DOCTEST_CHECK_FALSE(__VA_ARGS__)
#define CHECK_THROWS(...) DOCTEST_CHECK_THROWS(__VA_ARGS__)
#define CHECK_THROWS_AS(expr, ...) DOCTEST_CHECK_THROWS_AS(expr, __VA_ARGS__)
#define CHECK_THROWS_WITH(expr, ...) DOCTEST_CHECK_THROWS_WITH(expr, __VA_ARGS__)
#define CHECK_THROWS_WITH_AS(expr, with, ...) DOCTEST_CHECK_THROWS_WITH_AS(expr, with, __VA_ARGS__)
#define CHECK_NOTHROW(...) DOCTEST_CHECK_NOTHROW(__VA_ARGS__)
#define REQUIRE(...) DOCTEST_REQUIRE(__VA_ARGS__)
#define REQUIRE_FALSE(...) DOCTEST_REQUIRE_FALSE(__VA_ARGS__)
#define REQUIRE_THROWS(...) DOCTEST_REQUIRE_THROWS(__VA_ARGS__)
#define REQUIRE_THROWS_AS(expr, ...) DOCTEST_REQUIRE_THROWS_AS(expr, __VA_ARGS__)
#define REQUIRE_THROWS_WITH(expr, ...) DOCTEST_REQUIRE_THROWS_WITH(expr, __VA_ARGS__)
#define REQUIRE_THROWS_WITH_AS(expr, with, ...) DOCTEST_REQUIRE_THROWS_WITH_AS(expr, with, __VA_ARGS__)
#define REQUIRE_NOTHROW(...) DOCTEST_REQUIRE_NOTHROW(__VA_ARGS__)

#define WARN_MESSAGE(cond, ...) DOCTEST_WARN_MESSAGE(cond, __VA_ARGS__)
#define WARN_FALSE_MESSAGE(cond, ...) DOCTEST_WARN_FALSE_MESSAGE(cond, __VA_ARGS__)
#define WARN_THROWS_MESSAGE(expr, ...) DOCTEST_WARN_THROWS_MESSAGE(expr, __VA_ARGS__)
#define WARN_THROWS_AS_MESSAGE(expr, ex, ...) DOCTEST_WARN_THROWS_AS_MESSAGE(expr, ex, __VA_ARGS__)
#define WARN_THROWS_WITH_MESSAGE(expr, with, ...) DOCTEST_WARN_THROWS_WITH_MESSAGE(expr, with, __VA_ARGS__)
#define WARN_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) DOCTEST_WARN_THROWS_WITH_AS_MESSAGE(expr, with, ex, __VA_ARGS__)
#define WARN_NOTHROW_MESSAGE(expr, ...) DOCTEST_WARN_NOTHROW_MESSAGE(expr, __VA_ARGS__)
#define CHECK_MESSAGE(cond, ...) DOCTEST_CHECK_MESSAGE(cond, __VA_ARGS__)
#define CHECK_FALSE_MESSAGE(cond, ...) DOCTEST_CHECK_FALSE_MESSAGE(cond, __VA_ARGS__)
#define CHECK_THROWS_MESSAGE(expr, ...) DOCTEST_CHECK_THROWS_MESSAGE(expr, __VA_ARGS__)
#define CHECK_THROWS_AS_MESSAGE(expr, ex, ...) DOCTEST_CHECK_THROWS_AS_MESSAGE(expr, ex, __VA_ARGS__)
#define CHECK_THROWS_WITH_MESSAGE(expr, with, ...) DOCTEST_CHECK_THROWS_WITH_MESSAGE(expr, with, __VA_ARGS__)
#define CHECK_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) DOCTEST_CHECK_THROWS_WITH_AS_MESSAGE(expr, with, ex, __VA_ARGS__)
#define CHECK_NOTHROW_MESSAGE(expr, ...) DOCTEST_CHECK_NOTHROW_MESSAGE(expr, __VA_ARGS__)
#define REQUIRE_MESSAGE(cond, ...) DOCTEST_REQUIRE_MESSAGE(cond, __VA_ARGS__)
#define REQUIRE_FALSE_MESSAGE(cond, ...) DOCTEST_REQUIRE_FALSE_MESSAGE(cond, __VA_ARGS__)
#define REQUIRE_THROWS_MESSAGE(expr, ...) DOCTEST_REQUIRE_THROWS_MESSAGE(expr, __VA_ARGS__)
#define REQUIRE_THROWS_AS_MESSAGE(expr, ex, ...) DOCTEST_REQUIRE_THROWS_AS_MESSAGE(expr, ex, __VA_ARGS__)
#define REQUIRE_THROWS_WITH_MESSAGE(expr, with, ...) DOCTEST_REQUIRE_THROWS_WITH_MESSAGE(expr, with, __VA_ARGS__)
#define REQUIRE_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) DOCTEST_REQUIRE_THROWS_WITH_AS_MESSAGE(expr, with, ex, __VA_ARGS__)
#define REQUIRE_NOTHROW_MESSAGE(expr, ...) DOCTEST_REQUIRE_NOTHROW_MESSAGE(expr, __VA_ARGS__)

#define SCENARIO(name) DOCTEST_SCENARIO(name)
#define SCENARIO_CLASS(name) DOCTEST_SCENARIO_CLASS(name)
#define SCENARIO_TEMPLATE(name, T, ...) DOCTEST_SCENARIO_TEMPLATE(name, T, __VA_ARGS__)
#define SCENARIO_TEMPLATE_DEFINE(name, T, id) DOCTEST_SCENARIO_TEMPLATE_DEFINE(name, T, id)
#define GIVEN(name) DOCTEST_GIVEN(name)
#define WHEN(name) DOCTEST_WHEN(name)
#define AND_WHEN(name) DOCTEST_AND_WHEN(name)
#define THEN(name) DOCTEST_THEN(name)
#define AND_THEN(name) DOCTEST_AND_THEN(name)

#define WARN_EQ(...) DOCTEST_WARN_EQ(__VA_ARGS__)
#define CHECK_EQ(...) DOCTEST_CHECK_EQ(__VA_ARGS__)
#define REQUIRE_EQ(...) DOCTEST_REQUIRE_EQ(__VA_ARGS__)
#define WARN_NE(...) DOCTEST_WARN_NE(__VA_ARGS__)
#define CHECK_NE(...) DOCTEST_CHECK_NE(__VA_ARGS__)
#define REQUIRE_NE(...) DOCTEST_REQUIRE_NE(__VA_ARGS__)
#define WARN_GT(...) DOCTEST_WARN_GT(__VA_ARGS__)
#define CHECK_GT(...) DOCTEST_CHECK_GT(__VA_ARGS__)
#define REQUIRE_GT(...) DOCTEST_REQUIRE_GT(__VA_ARGS__)
#define WARN_LT(...) DOCTEST_WARN_LT(__VA_ARGS__)
#define CHECK_LT(...) DOCTEST_CHECK_LT(__VA_ARGS__)
#define REQUIRE_LT(...) DOCTEST_REQUIRE_LT(__VA_ARGS__)
#define WARN_GE(...) DOCTEST_WARN_GE(__VA_ARGS__)
#define CHECK_GE(...) DOCTEST_CHECK_GE(__VA_ARGS__)
#define REQUIRE_GE(...) DOCTEST_REQUIRE_GE(__VA_ARGS__)
#define WARN_LE(...) DOCTEST_WARN_LE(__VA_ARGS__)
#define CHECK_LE(...) DOCTEST_CHECK_LE(__VA_ARGS__)
#define REQUIRE_LE(...) DOCTEST_REQUIRE_LE(__VA_ARGS__)
#define WARN_UNARY(...) DOCTEST_WARN_UNARY(__VA_ARGS__)
#define CHECK_UNARY(...) DOCTEST_CHECK_UNARY(__VA_ARGS__)
#define REQUIRE_UNARY(...) DOCTEST_REQUIRE_UNARY(__VA_ARGS__)
#define WARN_UNARY_FALSE(...) DOCTEST_WARN_UNARY_FALSE(__VA_ARGS__)
#define CHECK_UNARY_FALSE(...) DOCTEST_CHECK_UNARY_FALSE(__VA_ARGS__)
#define REQUIRE_UNARY_FALSE(...) DOCTEST_REQUIRE_UNARY_FALSE(__VA_ARGS__)

// KEPT FOR BACKWARDS COMPATIBILITY
#define FAST_WARN_EQ(...) DOCTEST_FAST_WARN_EQ(__VA_ARGS__)
#define FAST_CHECK_EQ(...) DOCTEST_FAST_CHECK_EQ(__VA_ARGS__)
#define FAST_REQUIRE_EQ(...) DOCTEST_FAST_REQUIRE_EQ(__VA_ARGS__)
#define FAST_WARN_NE(...) DOCTEST_FAST_WARN_NE(__VA_ARGS__)
#define FAST_CHECK_NE(...) DOCTEST_FAST_CHECK_NE(__VA_ARGS__)
#define FAST_REQUIRE_NE(...) DOCTEST_FAST_REQUIRE_NE(__VA_ARGS__)
#define FAST_WARN_GT(...) DOCTEST_FAST_WARN_GT(__VA_ARGS__)
#define FAST_CHECK_GT(...) DOCTEST_FAST_CHECK_GT(__VA_ARGS__)
#define FAST_REQUIRE_GT(...) DOCTEST_FAST_REQUIRE_GT(__VA_ARGS__)
#define FAST_WARN_LT(...) DOCTEST_FAST_WARN_LT(__VA_ARGS__)
#define FAST_CHECK_LT(...) DOCTEST_FAST_CHECK_LT(__VA_ARGS__)
#define FAST_REQUIRE_LT(...) DOCTEST_FAST_REQUIRE_LT(__VA_ARGS__)
#define FAST_WARN_GE(...) DOCTEST_FAST_WARN_GE(__VA_ARGS__)
#define FAST_CHECK_GE(...) DOCTEST_FAST_CHECK_GE(__VA_ARGS__)
#define FAST_REQUIRE_GE(...) DOCTEST_FAST_REQUIRE_GE(__VA_ARGS__)
#define FAST_WARN_LE(...) DOCTEST_FAST_WARN_LE(__VA_ARGS__)
#define FAST_CHECK_LE(...) DOCTEST_FAST_CHECK_LE(__VA_ARGS__)
#define FAST_REQUIRE_LE(...) DOCTEST_FAST_REQUIRE_LE(__VA_ARGS__)

#define FAST_WARN_UNARY(...) DOCTEST_FAST_WARN_UNARY(__VA_ARGS__)
#define FAST_CHECK_UNARY(...) DOCTEST_FAST_CHECK_UNARY(__VA_ARGS__)
#define FAST_REQUIRE_UNARY(...) DOCTEST_FAST_REQUIRE_UNARY(__VA_ARGS__)
#define FAST_WARN_UNARY_FALSE(...) DOCTEST_FAST_WARN_UNARY_FALSE(__VA_ARGS__)
#define FAST_CHECK_UNARY_FALSE(...) DOCTEST_FAST_CHECK_UNARY_FALSE(__VA_ARGS__)
#define FAST_REQUIRE_UNARY_FALSE(...) DOCTEST_FAST_REQUIRE_UNARY_FALSE(__VA_ARGS__)

#define TEST_CASE_TEMPLATE_INSTANTIATE(id, ...) DOCTEST_TEST_CASE_TEMPLATE_INSTANTIATE(id, __VA_ARGS__)

#endif // DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES

#ifndef DOCTEST_CONFIG_DISABLE

// this is here to clear the 'current test suite' for the current translation unit - at the top
DOCTEST_TEST_SUITE_END();

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_CLANG_SUPPRESS_WARNING_POP
DOCTEST_MSVC_SUPPRESS_WARNING_POP
DOCTEST_GCC_SUPPRESS_WARNING_POP

DOCTEST_SUPPRESS_COMMON_WARNINGS_POP

#endif // DOCTEST_LIBRARY_INCLUDED
