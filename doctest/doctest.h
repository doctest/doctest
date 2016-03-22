// Suppress this globally - there is no way to silence it in the expression decomposition macros
// _Pragma() in macros doesn't work for the c++ front-end of g++
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=55578
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=69543
// Also it is completely worthless nowadays - http://stackoverflow.com/questions/14016993
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
#pragma warning(disable : 4996)
#pragma warning(disable : 4267)
#endif // _MSC_VER

#ifndef DOCTEST_LIBRARY_INCLUDED
#define DOCTEST_LIBRARY_INCLUDED

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

namespace doctest
{
// the function type this library works with
typedef void (*funcType)(void);

class String
{
    char* m_str;

    void copy(const String& other);

public:
    String(const char* in = 0);
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

#if !defined(DOCTEST_DISABLE)

namespace detail
{
    template <class T>
    class Vector
    {
        unsigned m_size;
        unsigned m_capacity;
        T*       m_buffer;

    public:
        Vector();
        explicit Vector(unsigned num);
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

    String stringify(const char* in);
    String stringify(bool in);
    String stringify(char in);
    String stringify(int in);
    String stringify(long in);
    //String stringify(long long in);
    String stringify(unsigned in);
    String stringify(unsigned long in);
    //String stringify(unsigned long long in);
    String stringify(float in);
    String stringify(double in);
    String stringify(long double in);

    template <typename T>
    String stringify(const T&) {
        return "{?}";
    }

    // pointers???
    //template <typename T>
    //String stringify(const T*&) {
    //    return "{?}";
    //}

    template <typename L, typename R>
    String stringify(const L& lhs, const char* op, const R& rhs) {
        return stringify(lhs) + " " + op + " " + stringify(rhs);
    }

    struct Result
    {
        const bool   m_passed;
        const String m_decomposition;

        Result(bool passed, const String& decomposition)
                : m_passed(passed)
                , m_decomposition(decomposition) {}
        operator bool() { return !m_passed; }
    };

    template <typename L>
    struct Expression_lhs
    {
        const L lhs;

        Expression_lhs(L in)
                : lhs(in) {}

        operator Result() { return Result(!!lhs, stringify(lhs)); }

        // clang-format off
        template <typename R> Result operator==(const R& rhs) { return Result(lhs == rhs, stringify(lhs, "==", rhs)); }
        template <typename R> Result operator!=(const R& rhs) { return Result(lhs != rhs, stringify(lhs, "!=", rhs)); }
        template <typename R> Result operator< (const R& rhs) { return Result(lhs <  rhs, stringify(lhs, "<",  rhs)); }
        template <typename R> Result operator<=(const R& rhs) { return Result(lhs <= rhs, stringify(lhs, "<=", rhs)); }
        template <typename R> Result operator> (const R& rhs) { return Result(lhs >  rhs, stringify(lhs, ">",  rhs)); }
        template <typename R> Result operator>=(const R& rhs) { return Result(lhs >= rhs, stringify(lhs, ">=", rhs)); }
        // clang-format on
    };

    struct ExpressionDecomposer
    {
        template <typename L>
        Expression_lhs<const L&> operator<<(const L& operand) {
            return Expression_lhs<const L&>(operand);
        }
    };
} // namespace detail

#endif // DOCTEST_DISABLE

class Context
{
#if !defined(DOCTEST_DISABLE)

    detail::Vector<detail::Vector<String> > filters;

    bool count;                // if only the count of matching tests is to be retreived
    bool case_sensitive;       // if filtering should be case sensitive
    bool separate_process;     // if each test should be executed in a separate process
    bool allow_overrides;      // all but this can be overriden
    bool exit_after_tests;     // calls exit() after the tests are ran/counted
    bool hash_table_histogram; // if the hash table should be printed as a histogram

    bool no_run; // to not run the tests at all (can be done with an "*" exclude)
    int  first;  // the first (matching) test to be executed
    int  last;   // the last (matching) test to be executed

    int (*testExecutionWrapper)(funcType); // wrapper for test execution

#endif // DOCTEST_DISABLE

public:
    Context(int argc, char** argv);

    void addFilter(const char* filter, const char* value);
    void setOption(const char* option, int value);
    void setTestExecutionWrapper(int (*f)(funcType));
    int  runTests();
};

} // namespace doctest

// if registering is not disabled
#if !defined(DOCTEST_DISABLE)

namespace doctest
{
namespace detail
{
    // forward declarations of the functions used by the registering macros
    int regTest(void (*f)(void), unsigned line, const char* file, const char* name);
    int setTestSuiteName(const char* name);
} // namespace detail
} // namespace doctest

// registers the test by initializing a dummy var with a function
#if defined(__GNUC__) && !defined(__clang__)
#define DOCTEST_REGISTER_FUNCTION(f, name)                                                         \
    static int DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_VAR_) __attribute__((unused)) =                   \
            doctest::detail::regTest(f, __LINE__, __FILE__, #name);
#elif defined(__clang__)
#define DOCTEST_REGISTER_FUNCTION(f, name)                                                         \
    _Pragma("clang diagnostic push")                                                               \
            _Pragma("clang diagnostic ignored \"-Wglobal-constructors\"") static int               \
                    DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_VAR_) =                                      \
                            doctest::detail::regTest(f, __LINE__, __FILE__, #name);                \
    _Pragma("clang diagnostic pop")
#else // MSVC
#define DOCTEST_REGISTER_FUNCTION(f, name)                                                         \
    static int DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_VAR_) =                                           \
            doctest::detail::regTest(f, __LINE__, __FILE__, #name);
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
#define DOCTEST_TESTCASE(name)                                                                     \
    DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FUNC_), name)

// for registering tests with a fixture
#define DOCTEST_TESTCASE_FIXTURE(c, name)                                                          \
    DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_CLASS_), c,                        \
                              DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FUNC_), name)

// for subcases
#if defined(__GNUC__)
#define DOCTEST_SUBCASE(name)                                                                      \
    if(const doctest::detail::Subcase & DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_SUBCASE_)                \
                                                __attribute__((unused)) =                          \
               doctest::detail::Subcase(#name, __FILE__, __LINE__))
#else // __GNUC__
#define DOCTEST_SUBCASE(name)                                                                      \
    if(const doctest::detail::Subcase & DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_SUBCASE_) =              \
               doctest::detail::Subcase(#name, __FILE__, __LINE__))
#endif // __GNUC__

// for starting a testsuite block
#if defined(__GNUC__) && !defined(__clang__)
#define DOCTEST_TESTSUITE(name)                                                                    \
    static int DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_VAR_) __attribute__((unused)) =                   \
            doctest::detail::setTestSuiteName(#name);                                              \
    void DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FOR_SEMICOLON_)()
#elif defined(__clang__)
#define DOCTEST_TESTSUITE(name)                                                                    \
    _Pragma("clang diagnostic push")                                                               \
            _Pragma("clang diagnostic ignored \"-Wglobal-constructors\"") static int               \
                    DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_VAR_) =                                      \
                            doctest::detail::setTestSuiteName(#name);                              \
    _Pragma("clang diagnostic pop") void DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FOR_SEMICOLON_)()
#else // MSVC
#define DOCTEST_TESTSUITE(name)                                                                    \
    static int DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_VAR_) = doctest::detail::setTestSuiteName(#name); \
    void       DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FOR_SEMICOLON_)()
#endif // MSVC

// for ending a testsuite block
#if defined(__GNUC__) && !defined(__clang__)
#define DOCTEST_TESTSUITE_END                                                                      \
    static int DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_VAR_) __attribute__((unused)) =                   \
            doctest::detail::setTestSuiteName("");                                                 \
    void DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FOR_SEMICOLON_)()
#elif defined(__clang__)
#define DOCTEST_TESTSUITE_END                                                                      \
    _Pragma("clang diagnostic push")                                                               \
            _Pragma("clang diagnostic ignored \"-Wglobal-constructors\"") static int               \
                    DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_VAR_) =                                      \
                            doctest::detail::setTestSuiteName("");                                 \
    _Pragma("clang diagnostic pop") void DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FOR_SEMICOLON_)()
#else // MSVC
#define DOCTEST_TESTSUITE_END                                                                      \
    static int DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_VAR_) = doctest::detail::setTestSuiteName("");    \
    void       DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FOR_SEMICOLON_)()
#endif // MSVC

#if defined(__clang__)
#define DOCTEST_CHECK(expr)                                                                        \
    do {                                                                                           \
        _Pragma("clang diagnostic push")                                                           \
                _Pragma("clang diagnostic ignored \"-Woverloaded-shift-op-parentheses\"") if(      \
                        doctest::detail::Result failed =                                           \
                                (doctest::detail::ExpressionDecomposer() << expr))                 \
                        printf("%s\n", failed.m_decomposition.c_str());                            \
        _Pragma("clang diagnostic pop")                                                            \
    } while(false)
#else // __clang__
#define DOCTEST_CHECK(expr)                                                                        \
    do {                                                                                           \
        if(doctest::detail::Result failed = (doctest::detail::ExpressionDecomposer() << expr))     \
            printf("%s\n", failed.m_decomposition.c_str());                                        \
    } while(false)
#endif // __clang__

// =============================================================================
// == WHAT FOLLOWS IS VERSIONS OF THE MACROS THAT DO NOT DO ANY REGISTERING!  ==
// == THIS CAN BE ENABLED BY DEFINING DOCTEST_DISABLE GLOBALLY!        ==
// =============================================================================
#else // DOCTEST_DISABLE

namespace doctest
{
inline String::String(const char*) {}
inline String::String(const String&) {}
inline String::~String() {}
inline String& String::operator=(const String&) { return *this; }
inline String  String::operator+(const String&) const { return String(); }
inline String& String::operator+=(const String&) { return *this; }
inline int     String::compare(const char*, bool) const { return 0; }
inline int     String::compare(const String&, bool) const { return 0; }

inline Context::Context(int, char**) {}
inline void Context::addFilter(const char*, const char*) {}
inline void Context::setOption(const char*, int) {}
inline void Context::setTestExecutionWrapper(int (*)(void (*)(void))) {}
inline int  Context::runTests() { return 0; }
} // namespace doctest

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
#define DOCTEST_TESTCASE(name)                                                                     \
    DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FUNC_), name)

// for registering tests with a fixture
#define DOCTEST_TESTCASE_FIXTURE(x, name)                                                          \
    DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_CLASS_), x,                        \
                              DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FUNC_), name)

// for subcases
#define DOCTEST_SUBCASE(name)

// for starting a testsuite block
#define DOCTEST_TESTSUITE(name) void DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FOR_SEMICOLON_)()

// for ending a testsuite block
#define DOCTEST_TESTSUITE_END void DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FOR_SEMICOLON_)()

#define DOCTEST_CHECK(expr) expr;

#endif // DOCTEST_DISABLE

#define doctest_testcase(name) DOCTEST_TESTCASE(name)
#define doctest_testcase_fixture(c, name) DOCTEST_TESTCASE_FIXTURE(c, name)
#define doctest_subcase(name) DOCTEST_SUBCASE(name)
#define doctest_testsuite(name) DOCTEST_TESTSUITE(name)
#define doctest_testsuite_end DOCTEST_TESTSUITE_END
#define doctest_check DOCTEST_CHECK

// == SHORT VERSIONS OF THE TEST/FIXTURE/TESTSUITE MACROS
#ifndef DOCTEST_NO_SHORT_MACRO_NAMES

#define TESTCASE(name) DOCTEST_TESTCASE(name)
#define TESTCASE_FIXTURE(c, name) DOCTEST_TESTCASE_FIXTURE(c, name)
#define SUBCASE(name) DOCTEST_SUBCASE(name)
#define TESTSUITE(name) DOCTEST_TESTSUITE(name)
#define TESTSUITE_END DOCTEST_TESTSUITE_END
#define CHECK DOCTEST_CHECK

#define testcase(name) doctest_testcase(name)
#define testcase_fixture(c, name) doctest_testcase_fixture(c, name)
#define subcase(name) doctest_subcase(name)
#define testsuite(name) doctest_testsuite(name)
#define testsuite_end doctest_testsuite_end
#define check doctest_check

#endif // DOCTEST_NO_SHORT_MACRO_NAMES

// this is here to clear the 'current test suite' for the current translation unit - at the top
doctest_testsuite_end;

#endif // DOCTEST_LIBRARY_INCLUDED

// =============================================================================
// == WHAT FOLLOWS IS THE IMPLEMENTATION OF THE TEST RUNNER                   ==
// =============================================================================
#if(defined(DOCTEST_IMPLEMENT) || defined(DOCTEST_IMPLEMENT_WITH_MAIN)) && !defined(DOCTEST_DISABLE)
#ifndef DOCTEST_LIBRARY_IMPLEMENTATION
#define DOCTEST_LIBRARY_IMPLEMENTATION

// required includes
#include <cstdio>  // printf, sprintf and friends
#include <cstdlib> // malloc, free, qsort
#include <cstring> // strcpy, strtok
#include <new>     // placement new (can be skipped if the containers require 'construct()' from T)

// the number of buckets used for the hash set
#if !defined(DOCTEST_HASH_TABLE_NUM_BUCKETS)
#define DOCTEST_HASH_TABLE_NUM_BUCKETS 1024
#endif // DOCTEST_HASH_TABLE_NUM_BUCKETS

// main namespace of the library
namespace doctest
{
// library internals namespace
namespace detail
{
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
        for(size_t i = 0; i < filters.size(); ++i)
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
    Vector<T>::Vector(unsigned num)
            : m_size(num)
            , m_capacity(num)
            , m_buffer(static_cast<T*>(malloc(sizeof(T) * m_capacity))) {
        for(unsigned i = 0; i < m_size; ++i)
            new(m_buffer + i) T();
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
            for(size_t i = 0; i < m_size; ++i)
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

    unsigned Hash(const Subcase& in) {
        return hashStr(reinterpret_cast<unsigned const char*>(in.m_file)) ^ in.m_line;
    }

    unsigned Hash(int in) { return in; }

    // requires that 'bool operator==(T&)' and 'unsigned Hash(T&)' are both present for T
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

    // stuff for subcases
    HashTable<Subcase>& getSubcasesPassed() {
        static HashTable<Subcase> data(100);
        return data;
    }

    HashTable<int>& getSubcasesEnteredLevels() {
        static HashTable<int> data(100);
        return data;
    }

    int& getSubcasesCurrentLevel() {
        static int data = 0;
        return data;
    }

    bool& getSubcasesHasSkipped() {
        static bool data = false;
        return data;
    }

    Subcase::Subcase(const char* name, const char* file, int line)
            : m_name(name)
            , m_file(file)
            , m_line(line)
            , m_entered(false) {
        // if we have already completed it
        if(getSubcasesPassed().has(*this))
            return;

        // if a Subcase on the same level has already been entered
        if(getSubcasesEnteredLevels().has(getSubcasesCurrentLevel())) {
            getSubcasesHasSkipped() = true;
            return;
        }

        getSubcasesEnteredLevels().insert(getSubcasesCurrentLevel()++);
        m_entered = true;
    }

    Subcase::~Subcase() {
        if(m_entered) {
            getSubcasesCurrentLevel()--;
            // only mark the subcase as passed if no subcases have been skipped
            if(getSubcasesHasSkipped() == false)
                getSubcasesPassed().insert(*this);
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

    String stringify(const char* in) {
        String out("\"");
        out += in;
        out += "\"";
        return out;
    }

    String stringify(bool in) {
        String out("\"");
        out += (in ? "true" : "false");
        out += "\"";
        return out;
    }

    String stringify(char in) {
        char buf[64];
        if(in < ' ')
            sprintf(buf, "%d", in);
        else
            sprintf(buf, "%c", in);
        return buf;
    }

    String stringify(int in) {
        char buf[64];
        sprintf(buf, "%d", in);
        return buf;
    }

    String stringify(long in) {
        char buf[64];
        sprintf(buf, "%ld", in);
        return buf;
    }

    //String stringify(long long in) {
    //    char buf[64];
    //    sprintf(buf, "%lld", in);
    //    return buf;
    //}

    String stringify(unsigned in) {
        char buf[64];
        sprintf(buf, "%u", in);
        return buf;
    }

    String stringify(unsigned long in) {
        char buf[64];
        sprintf(buf, "%lu", in);
        return buf;
    }

    //String stringify(unsigned long long in) {
    //    char buf[64];
    //    sprintf(buf, "%llu", in);
    //    return buf;
    //}

    String stringify(float in) {
        char buf[64];
        sprintf(buf, "%f", static_cast<double>(in));
        return buf;
    }

    String stringify(double in) {
        char buf[64];
        sprintf(buf, "%f", in);
        return buf;
    }

    String stringify(long double in) {
        char buf[64];
        sprintf(buf, "%Lf", in);
        return buf;
    }

    // a struct defining a registered test callback
    struct TestData
    {
        // not used for comparing
        String   m_suite; // the test suite in which the test was added
        String   m_name;  // name of the test function
        funcType m_f;     // a function pointer to the test function

        // fields by which difference of test functions shall be determined
        const char* m_file; // the file in which the test was registered
        unsigned    m_line; // the line where the test was registered

        TestData(const char* suite, const char* name, funcType f, const char* file, int line)
                : m_suite()
                , m_name()
                , m_f(f)
                , m_file(file)
                , m_line(line) {
            // trimming quotes of name
            if(name) {
                if(*name == '"')
                    ++name;
                size_t name_len = my_strlen(name);
                if(name[name_len] != '"') {
                    m_name = name;
                } else {
                    m_name                   = name;
                    m_name.c_str()[name_len] = '\0';
                }
            }

            // trimming quotes of suite
            if(suite) {
                if(*suite == '"')
                    ++suite;
                size_t suite_len = my_strlen(suite);
                if(suite[suite_len] != '"') {
                    m_suite = suite;
                } else {
                    m_suite            = suite;
                    m_suite[suite_len] = '\0';
                }
            }
        }

        bool operator==(const TestData& other) const {
            return m_line == other.m_line && strcmp(m_file, other.m_file) == 0;
        }
    };

    unsigned Hash(const TestData& in) {
        return hashStr(reinterpret_cast<unsigned const char*>(in.m_file)) ^ in.m_line;
    }

    // a comparison function for using qsort on arrays with pointers to TestData structures
    int TestDataComparator(const void* a, const void* b) {
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
    int callTestFunc(int (*testExecutionWrapper)(funcType), funcType f) {
        try {
            if(testExecutionWrapper) {
                return testExecutionWrapper(f);
            } else {
                f();
            }
        } catch(...) {
            printf("Unknown exception caught!\n");
            return 1;
        }
        return 0;
    }

    // parses a comma separated list of words after a pattern in one of the arguments in argv
    void parseFilter(int argc, char** argv, const char* pattern, Vector<String>& filters) {
        String filtersString;
        for(int i = 0; i < argc; ++i) {
            const char* temp = strstr(argv[i], pattern);
            if(temp) {
                temp += my_strlen(pattern);
                size_t len = my_strlen(temp);
                if(len) {
                    filtersString = temp;
                    break;
                }
            }
        }

        // if we have found the filter string
        if(filtersString.c_str()) {
            // tokenize with "," as a separator
            char* pch = strtok(filtersString.c_str(), ","); // modifies the string
            while(pch != 0) {
                if(my_strlen(pch))
                    filters.push_back(pch);
                pch = strtok(0, ","); // uses the strtok() internal state to go to the next token
            }
        }
    }

    // parses an option from the command line (bool: type == 0, int: type == 1)
    int parseOption(int argc, char** argv, const char* option, int type, int defaultVal) {
        int outVal = defaultVal;

        Vector<String> parsedValues;
        parseFilter(argc, argv, option, parsedValues);

        // if the option has been found (and there is only 1 value in the "comma separated list")
        if(parsedValues.size() == 1) {
            if(type == 0) {
                // boolean
                const char positive[][5] = {"1", "true", "on", "yes"}; // 5 - strlen("true") + 1
                const char negative[][6] = {"0", "false", "off", "no"};

                // if the value matches any of the positive/negative possibilities
                for(size_t i = 0; i < 4; i++) {
                    if(parsedValues[0].compare(positive[i], true) == 0) {
                        outVal = 1;
                        break;
                    }
                    if(parsedValues[0].compare(negative[i], true) == 0) {
                        outVal = 0;
                        break;
                    }
                }
            } else {
                // integer
                int res = atoi(parsedValues[0].c_str());
                if(res != 0)
                    outVal = res;
            }
        }
        return outVal;
    }

} // namespace detail

String::String(const char* in)
        : m_str(0) {
    if(in == 0)
        return;

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
        // not using std::strlen() because of valgrind errors when optimizations are turned on
        // 'Invalid read of size 4' when the test suite len (with '\0') is not a multiple of 4
        // for details see http://stackoverflow.com/questions/35671155

        const char* temp = other.m_str;
        while(*temp)
            ++temp;
        size_t len = temp - other.m_str;

        m_str = static_cast<char*>(malloc(len + 1));
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

Context::Context(int argc, char** argv)
        : filters(6) // 6 different filters total
{
    using namespace detail;

    testExecutionWrapper = 0;

    parseFilter(argc, argv, "-dt-file=", filters[0]);
    parseFilter(argc, argv, "-dt-file-exclude=", filters[1]);
    parseFilter(argc, argv, "-dt-suite=", filters[2]);
    parseFilter(argc, argv, "-dt-suite-exclude=", filters[3]);
    parseFilter(argc, argv, "-dt-name=", filters[4]);
    parseFilter(argc, argv, "-dt-name-exclude=", filters[5]);

    count                = !!parseOption(argc, argv, "-dt-count=", 0, 0);
    case_sensitive       = !!parseOption(argc, argv, "-dt-case-sensitive=", 0, 0);
    allow_overrides      = !!parseOption(argc, argv, "-dt-override=", 0, 1);
    separate_process     = !!parseOption(argc, argv, "-dt-separate-process=", 0, 0);
    exit_after_tests     = !!parseOption(argc, argv, "-dt-exit=", 0, 0);
    first                = parseOption(argc, argv, "-dt-first=", 1, 1);
    last                 = parseOption(argc, argv, "-dt-last=", 1, 0);
    hash_table_histogram = !!parseOption(argc, argv, "-dt-hash-table-histogram=", 0, 0);
    no_run               = !!parseOption(argc, argv, "-dt-no-run=", 0, 0);
}

// allows the user to add procedurally to the filters from the command line
void Context::addFilter(const char* filter, const char* value) {
    using namespace detail;

    if(allow_overrides) {
        size_t idx = 42;
        if(strcmp(filter, "dt-file") == 0)
            idx = 0;
        if(strcmp(filter, "dt-file-exclude") == 0)
            idx = 1;
        if(strcmp(filter, "dt-suite") == 0)
            idx = 2;
        if(strcmp(filter, "dt-suite-exclude") == 0)
            idx = 3;
        if(strcmp(filter, "dt-name") == 0)
            idx = 4;
        if(strcmp(filter, "dt-name-exclude") == 0)
            idx = 5;
        // if the filter name is valid
        if(idx != 42 && my_strlen(value))
            filters[idx].push_back(value);
    }
}

// allows the user to override procedurally the options from the command line
void Context::setOption(const char* option, int value) {
    using namespace detail;

    if(allow_overrides) {
        if(strcmp(option, "dt-count") == 0)
            count = !!value;
        if(strcmp(option, "dt-case-sensitive") == 0)
            case_sensitive = !!value;
        if(strcmp(option, "dt-separate-process") == 0)
            separate_process = !!value;
        if(strcmp(option, "dt-exit") == 0)
            exit_after_tests = !!value;
        if(strcmp(option, "dt-first") == 0)
            first = value;
        if(strcmp(option, "dt-last") == 0)
            last = value;
        if(strcmp(option, "dt-hash-table-histogram") == 0)
            hash_table_histogram = !!value;
        if(strcmp(option, "dt-no-run") == 0)
            no_run = !!value;
    }
}

// allows the user to provide a test execution wrapper for custom exception handling
void Context::setTestExecutionWrapper(int (*f)(funcType)) {
    using namespace detail;

    testExecutionWrapper = f;
}

// the main function that does all the filtering and test running
int Context::runTests() {
    using namespace detail;

    // exit right now
    if(no_run)
        return 0;

    const Vector<Vector<TestData> >& buckets = getRegisteredTests().getBuckets();

    Vector<const TestData*> testDataArray;
    for(size_t i = 0; i < buckets.size(); i++)
        for(size_t k = 0; k < buckets[i].size(); k++)
            testDataArray.push_back(&buckets[i][k]);

    // sort the collected records
    qsort(testDataArray.data(), testDataArray.size(), sizeof(TestData*), TestDataComparator);

    if(hash_table_histogram) {
        // find the most full bucket
        size_t maxInBucket = 0;
        for(size_t i = 0; i < buckets.size(); i++)
            if(buckets[i].size() > maxInBucket)
                maxInBucket = buckets[i].size();

        // print a prettified histogram
        printf("[doctest] hash table bucket histogram\n");
        printf("============================================================\n");
        printf("#bucket     |count| relative count\n");
        printf("============================================================\n");
        for(size_t i = 0; i < buckets.size(); i++) {
            printf("bucket %4d |%4d |", static_cast<int>(i), buckets[i].size());

            float ratio = static_cast<float>(buckets[i].size()) / static_cast<float>(maxInBucket);
            int   numStars = static_cast<int>(ratio * 41);
            for(int k = 0; k < numStars; ++k)
                printf("*");
            printf("\n");
        }
        printf("\n");
    }

    int numFilterPassedTests = 0;
    int numFailed            = 0;
    // invoke the registered functions if they match the filter criteria (or just count them)
    for(size_t i = 0; i < testDataArray.size(); i++) {
        const TestData& data = *testDataArray[i];
        if(!matchesAny(data.m_file, filters[0], 1, case_sensitive))
            continue;
        if(matchesAny(data.m_file, filters[1], 0, case_sensitive))
            continue;
        if(!matchesAny(data.m_suite, filters[2], 1, case_sensitive))
            continue;
        if(matchesAny(data.m_suite, filters[3], 0, case_sensitive))
            continue;
        if(!matchesAny(data.m_name, filters[4], 1, case_sensitive))
            continue;
        if(matchesAny(data.m_name, filters[5], 0, case_sensitive))
            continue;

        numFilterPassedTests++;
        // do not execute the test if we are to only count the number of filter passing tests
        if(count)
            continue;

        // skip the test if it is not in the execution range
        if((last < numFilterPassedTests && first <= last) || (first > numFilterPassedTests))
            continue;

        // execute the test if it passes all the filtering
        {
            int res = 0;

#ifdef _MSC_VER
//__try {
#endif // _MSC_VER

            getSubcasesPassed().clear();
            do {
                getSubcasesHasSkipped()   = false;
                getSubcasesCurrentLevel() = 0;
                getSubcasesEnteredLevels().clear();

                res += callTestFunc(testExecutionWrapper, data.m_f);

            } while(getSubcasesHasSkipped() == true);

#ifdef _MSC_VER
//} __except(1) {
//    printf("Unknown SEH exception caught!\n");
//    res = 1;
//}
#endif // _MSC_VER

            if(res) {
                printf("Test failed!\n");
                numFailed++;
            }
        }
    }

    if(count)
        printf("[doctest] number of registered tests passing the current filters: "
               "%d\n",
               numFilterPassedTests);

    if(exit_after_tests) {
        exit(numFailed); // @TODO: is this legal? Or should I only pass EXIT_SUCCESS/EXIT_FAILURE?
    }

    return numFailed;
}
} // namespace doctest

#endif // DOCTEST_LIBRARY_IMPLEMENTATION
#endif // DOCTEST_IMPLEMENT

// == THIS SUPPLIES A MAIN FUNCTION AND SHOULD BE DONE ONLY IN ONE TRANSLATION UNIT
#if defined(DOCTEST_IMPLEMENT_WITH_MAIN) && !defined(DOCTEST_MAIN_CONFIGURED)
#define DOCTEST_MAIN_CONFIGURED
int main(int argc, char** argv) {
    doctest::Context context(argc, argv);
    return context.runTests();
}
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
