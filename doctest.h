#pragma once

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

// if registering is not disabled
#if !defined(DOCTEST_GLOBAL_DISABLE)

// if the user wants to include this header himself somewhere and not
// everywhere doctest.h is included (no unnecessary header inclusion)
#if !defined(DOCTEST_DONT_INCLUDE_IMPLEMENTATION)
#include "doctest_impl.h"
#endif

namespace doctestns
{
// forward declarations of the function used by the registering macros
int registerFunction(void (*f)(void), unsigned line, const char* file, const char* method,
                     const char* name);
// the function used by the test invocation macro
void invokeAllFunctions(int argc, char** argv);
}

// call the registered tests with this
#define DOCTEST_INVOKE_ALL_TEST_FUNCTIONS(argc, argv)                                              \
    doctestns::invokeAllFunctions(argc, argv);

// internal registering macros
#define DOCTEST_REGISTER_FUNCTION(f, name)                                                         \
    static int DOCTEST_ANONYMOUS(a) = registerFunction(f, __LINE__, __FILE__, "", #name);

#define DOCTEST_IMPLEMENT_FIXTURE(der, base, func, name)                                           \
    namespace doctestns                                                                            \
    {                                                                                              \
        namespace                                                                                  \
        {                                                                                          \
            struct der : base                                                                      \
            {                                                                                      \
                void f();                                                                          \
            };                                                                                     \
            static void func()                                                                     \
            {                                                                                      \
                der v;                                                                             \
                v.f();                                                                             \
            }                                                                                      \
            static int DOCTEST_ANONYMOUS(a) = registerFunction(func, __LINE__, __FILE__, "",       \
                                                               #name);                             \
        }                                                                                          \
    }                                                                                              \
    inline void doctestns::der::f()

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION(f, name)                                              \
    namespace doctestns                                                                            \
    {                                                                                              \
        static void f();                                                                           \
        DOCTEST_REGISTER_FUNCTION(f, name)                                                         \
    }                                                                                              \
    inline void doctestns::f()

// for registering doctests
#define doctest_test(name)                                                                         \
    DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(f), name)
#define doctest_test_noname                                                                        \
    DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(f), nameless)

// for registering doctests with a fixture
#define doctest_fixture(c, name)                                                                   \
    DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS(F), c, DOCTEST_ANONYMOUS(f), name)
#define doctest_fixture_noname(c)                                                                  \
    DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS(F), c, DOCTEST_ANONYMOUS(f), nameless)

// for registering static methods of classes
#define doctest_static_method(c, m)                                                                \
    namespace doctestns                                                                            \
    {                                                                                              \
        static int DOCTEST_ANONYMOUS(a) = registerFunction(&c::m, 0, "",                           \
                                                           DOCTEST_STR_CONCAT_TOSTR(c, m),         \
                                                           DOCTEST_STR_CONCAT_TOSTR(c, m));        \
    }

// for starting a testsuite block
#define doctest_testsuite(name)                                                                    \
    namespace doctestns                                                                            \
    {                                                                                              \
        static int DOCTEST_ANONYMOUS(a) = setTestSuiteName(#name);                                 \
    }

// for ending a testsuite block
#define doctest_testsuite_end                                                                      \
    namespace doctestns                                                                            \
    {                                                                                              \
        static int DOCTEST_ANONYMOUS(a) = setTestSuiteName("");                                    \
    }

// =============================================================================
// == WHAT FOLLOWS IS VERSIONS OF THE MACROS THAT DO NOT DO ANY REGISTERING!  ==
// == THIS CAN BE ENABLED BY DEFINING DOCTEST_GLOBAL_DISABLE GLOBALLY!        ==
// =============================================================================
#else // DOCTEST_GLOBAL_DISABLE

// hack for silencing warning about unused variables when registration/invocation is disabled
namespace doctestns { inline void dmy(int i, char** c) { int a = i; i = a; char** t = c; c = t; } }

#define DOCTEST_INVOKE_ALL_TEST_FUNCTIONS(argc, argv)                                              \
    doctestns::dmy(argc, argv);
#define DOCTEST_REGISTER_FUNCTION(f, name)
#define DOCTEST_IMPLEMENT_FIXTURE(der, base, func, name)                                           \
    namespace doctestns                                                                            \
    {                                                                                              \
        struct der : base                                                                          \
        {                                                                                          \
            void f();                                                                              \
        };                                                                                         \
        inline void func()                                                                         \
        {                                                                                          \
            der v;                                                                                 \
            v.f();                                                                                 \
        }                                                                                          \
    }                                                                                              \
    inline void doctestns::der::f()

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION(f, name)                                              \
    namespace doctestns                                                                            \
    {                                                                                              \
        void f();                                                                                  \
        DOCTEST_REGISTER_FUNCTION(f, name)                                                         \
    }                                                                                              \
    inline void doctestns::f()

// for registering doctests
#define doctest_test(name)                                                                         \
    DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(f), name)
#define doctest_test_noname                                                                        \
    DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(f), nameless)

// for registering doctests with a fixture
#define doctest_fixture(x, name)                                                                   \
    DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS(F), x, DOCTEST_ANONYMOUS(f), name)
#define doctest_fixture_noname(x)                                                                  \
    DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS(F), x, DOCTEST_ANONYMOUS(f), nameless)

// for registering static methods of classes
#define doctest_static_method(c, m)

// for starting a testsuite block
#define doctest_testsuite(name)

// for ending a testsuite block
#define doctest_testsuite_end

#endif // DOCTEST_GLOBAL_DISABLE

// === SHORT VERSIONS OF THE TEST/FIXTURE/TESTSUITE MACROS
#ifdef DOCTEST_SHORT_MACRO_NAMES

#define test(name) doctest_test(name)
#define test_noname doctest_test_noname
#define fixture(c, name) doctest_fixture(c, name)
#define fixture_noname(c) doctest_fixture_noname(c)
// static method missing - TODO!
#define testsuite(name) doctest_testsuite(name)
#define testsuite_end doctest_testsuite_end

#endif // DOCTEST_SHORT_MACRO_NAMES
