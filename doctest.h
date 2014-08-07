#pragma once

// internal macros for string concatenation and anonymous variable name generation
#define DOCTEST_STR_CONCAT_IMPL(s1, s2) s1##s2
#define DOCTEST_STR_CONCAT(s1, s2) DOCTEST_STR_CONCAT_IMPL(s1, s2)
#ifdef __COUNTER__ // not standard and may be missing for some compilers
#define DOCTEST_ANONYMOUS_NAME(x) DOCTEST_STR_CONCAT(x, __COUNTER__)
#else
#define DOCTEST_ANONYMOUS_NAME(x) DOCTEST_STR_CONCAT(x, __LINE__)
#endif

#if !defined(DOCTEST_GLOBAL_DISABLE)

// if the user wants to include this header himself somewhere and not
// everywhere doctest.h is included (no unnecessary header inclusion)
#if !defined(DOCTEST_DONT_INCLUDE_IMPLEMENTATION)
#include "doctest_impl.h"
#endif

namespace doctestns {
    // forward declarations of the function used by the registering macros
    int r(void (*f)(void), unsigned line, const char* file, const char* method, const char* name);
    // the function used by the test invocation macro
    void invokeAllFunctions(int argc, char** argv);
}

// call the registered tests with this
#define DOCTEST_INVOKE_ALL_TEST_FUNCTIONS(argc, argv) \
doctestns::invokeAllFunctions(argc, argv);

// internal registering macros
#define DOCTEST_REGISTER_FUNCTION(f, name) \
static int DOCTEST_ANONYMOUS_NAME(a)=r(f,__LINE__,__FILE__,"",#name);

#define DOCTEST_REGISTER_CLASS_FUNCTION(x, m) \
namespace doctestns{static int DOCTEST_ANONYMOUS_NAME(a)=r(&x::m,0,__FILE__,#m,"");}

#define DOCTEST_IMPLEMENT_FIXTURE(der, base, func, name) \
namespace doctestns{struct der:base{void f();};inline void func(){der v;v.f();}\
static int DOCTEST_ANONYMOUS_NAME(a)=r(func,__LINE__,__FILE__,"",#name);}\
inline void doctestns::der::f()

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION(f, name) \
namespace doctestns{void f();DOCTEST_REGISTER_FUNCTION(f, name)}inline void doctestns::f()

// for registering doctests
#define doctest(name) \
DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS_NAME(f), name)
#define doctest_noname \
DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS_NAME(f), _)

// for registering doctests with a fixture
#define doctest_fixture(x, name) \
DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS_NAME(F), x, DOCTEST_ANONYMOUS_NAME(f), name)
#define doctest_fixture_noname(x) \
DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS_NAME(F), x, DOCTEST_ANONYMOUS_NAME(f), _)

// =============================================================================
// == WHAT FOLLOWS IS VERSIONS OF THE MACROS THAT DO NOT DO ANY REGISTERING!  ==
// == THIS CAN BE ENABLED BY DEFINING DOCTEST_GLOBAL_DISABLE GLOBALLY!        ==
// =============================================================================
#else // DOCTEST_GLOBAL_DISABLE

#define DOCTEST_INVOKE_ALL_TEST_FUNCTIONS(argc, argv)
#define DOCTEST_REGISTER_FUNCTION(f, name)
#define DOCTEST_REGISTER_CLASS_FUNCTION(x, m)
#define DOCTEST_IMPLEMENT_FIXTURE(der, base, func, name) \
namespace doctestns{struct der:base{void f();};inline void func(){der v;v.f();}}\
inline void doctestns::der::f()

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION(f, name) \
namespace doctestns{void f();DOCTEST_REGISTER_FUNCTION(f, name)}inline void doctestns::f()

// for registering normal doctests
#define doctest(name) \
DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS_NAME(f), name)

// for registering doctests with a fixture
#define doctest_fixture(x, name) \
DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS_NAME(F), x, DOCTEST_ANONYMOUS_NAME(f), name)

#endif // DOCTEST_GLOBAL_DISABLE
