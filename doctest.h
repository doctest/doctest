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

// define DOCTEST_C_INTERFACE if you want to force a C interface even for C++
#ifndef __cplusplus
#define DOCTEST_C_INTERFACE
#endif // __cplusplus

// cast wrapper for c/c++
#ifdef DOCTEST_C_INTERFACE
#define DOCTEST_STATIC_CAST(x) (x)
#define DOCTEST_REINTERPRET_CAST(x) (x)
#else // DOCTEST_C_INTERFACE
#define DOCTEST_STATIC_CAST(x) static_cast<x>
#define DOCTEST_REINTERPRET_CAST(x) reinterpret_cast<x>
#endif // DOCTEST_C_INTERFACE

// internal macro for concatenating 2 literals and making the result a string
#define DOCTEST_STR_CONCAT_TOSTR(s1, s2) DOCTEST_TOSTR(DOCTEST_STR_CONCAT(s1, s2))

namespace doctest_generated
{
// a dummy function that can be used for initializing globals (for silencing warnings)
inline int dummy()
{
    return 0;
}
} // namespace doctest_generated

// if registering is not disabled
#if !defined(DOCTEST_GLOBAL_DISABLE)

// if the user wants to include this header himself somewhere and not
// everywhere doctest.h is included (no unnecessary header inclusion)
#if !defined(DOCTEST_DONT_INCLUDE_IMPLEMENTATION)
#include "doctest_impl.h"
#else  // DOCTEST_DONT_INCLUDE_IMPLEMENTATION
namespace doctest
{
namespace detail
{
    // forward declarations of the function used by the registering macros
    int regTest(void (*f)(void), unsigned line, const char* file, const char* name);
    int setTestSuiteName(const char* name);
} // namespace detail

// forward declarations of the functions intended for direct use
void* createParams(int argc, char** argv);
void addFilter(void* params_struct, const char* filter, const char* value);
void setOption(void* params_struct, const char* option, int value);
void freeParams(void* params_struct);
void setTestExecutionWrapper(void* params_struct, int (*f)(void (*)(void)));
int runTests(void* params_struct);
} // namespace doctest
#endif // DOCTEST_DONT_INCLUDE_IMPLEMENTATION

// internal registering macros
#define DOCTEST_REGISTER_FUNCTION(f, name)                                                         \
    static int DOCTEST_ANONYMOUS(a) = doctest::detail::regTest(f, __LINE__, __FILE__, #name);

#define DOCTEST_IMPLEMENT_FIXTURE(der, base, func, name)                                           \
    namespace doctest_generated                                                                    \
    {                                                                                              \
        namespace                                                                                  \
        {                                                                                          \
            struct der : base {                                                                    \
                void f();                                                                          \
            };                                                                                     \
            static void func()                                                                     \
            {                                                                                      \
                der v;                                                                             \
                v.f();                                                                             \
            }                                                                                      \
            static int DOCTEST_ANONYMOUS(a) = doctest::detail::regTest(func, __LINE__, __FILE__,   \
                                                                       #name);                     \
        }                                                                                          \
    }                                                                                              \
    inline void doctest_generated::der::f()

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION(f, name)                                              \
    namespace doctest_generated                                                                    \
    {                                                                                              \
        static void f();                                                                           \
        DOCTEST_REGISTER_FUNCTION(f, name)                                                         \
    }                                                                                              \
    inline void doctest_generated::f()

// for registering doctests
#define doctest_test(name) DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(f), name)
#define doctest_test_noname DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(f), nameless)

// for registering doctests with a fixture
#define doctest_fixture(c, name)                                                                   \
    DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS(F), c, DOCTEST_ANONYMOUS(f), name)
#define doctest_fixture_noname(c)                                                                  \
    DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS(F), c, DOCTEST_ANONYMOUS(f), nameless)

// for starting a testsuite block
#define doctest_testsuite(name)                                                                    \
    namespace doctest_generated                                                                    \
    {                                                                                              \
        static int DOCTEST_ANONYMOUS(a) = doctest::detail::setTestSuiteName(#name);                \
    }                                                                                              \
    static int DOCTEST_ANONYMOUS(GIVE_ME_COMMA) = doctest_generated::dummy()

// for ending a testsuite block
#define doctest_testsuite_end                                                                      \
    namespace doctest_generated                                                                    \
    {                                                                                              \
        static int DOCTEST_ANONYMOUS(a) = doctest::detail::setTestSuiteName("");                   \
    }                                                                                              \
    static int DOCTEST_ANONYMOUS(GIVE_ME_COMMA) = doctest_generated::dummy()

// =============================================================================
// == WHAT FOLLOWS IS VERSIONS OF THE MACROS THAT DO NOT DO ANY REGISTERING!  ==
// == THIS CAN BE ENABLED BY DEFINING DOCTEST_GLOBAL_DISABLE GLOBALLY!        ==
// =============================================================================
#else // DOCTEST_GLOBAL_DISABLE

#define DOCTEST_IMPLEMENT_FIXTURE(der, base, func, name)                                           \
    namespace doctest_generated                                                                    \
    {                                                                                              \
        struct der : base {                                                                        \
            void f();                                                                              \
        };                                                                                         \
        inline void func()                                                                         \
        {                                                                                          \
            der v;                                                                                 \
            v.f();                                                                                 \
        }                                                                                          \
    }                                                                                              \
    inline void doctest_generated::der::f()

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION(f, name)                                              \
    namespace doctest_generated                                                                    \
    {                                                                                              \
        void f();                                                                                  \
    }                                                                                              \
    inline void doctest_generated::f()

// for registering doctests
#define doctest_test(name) DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(f), name)
#define doctest_test_noname DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(f), nameless)

// for registering doctests with a fixture
#define doctest_fixture(x, name)                                                                   \
    DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS(F), x, DOCTEST_ANONYMOUS(f), name)
#define doctest_fixture_noname(x)                                                                  \
    DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS(F), x, DOCTEST_ANONYMOUS(f), nameless)

// for starting a testsuite block
#define doctest_testsuite(name)                                                                    \
    static int DOCTEST_ANONYMOUS(GIVE_ME_COMMA) = doctest_generated::dummy()

// for ending a testsuite block
#define doctest_testsuite_end                                                                      \
    static int DOCTEST_ANONYMOUS(GIVE_ME_COMMA) = doctest_generated::dummy()

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
