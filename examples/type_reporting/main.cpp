#define DOCTEST_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <climits>
#include <cstdio>
#include <string>

// @TODO: maybe also check if __cplusplus == 201103 or higher?
// #if defined(_MSC_EXTENSIONS) || (defined(_MSC_VER) && (_MSC_VER >= 1400))
// #define HAS_LONG_LONG
// #elif (defined(ULLONG_MAX) || defined(ULONG_LONG_MAX) || defined(ULONGLONG_MAX))
// #define HAS_LONG_LONG
// #endif

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Waggregate-return"
#endif

// @TODO: look at https://github.com/louisdx/cxx-prettyprint
namespace doctest
{
namespace detail
{
    String stringify(const std::string& in); // to silence GCC "-Wmissing-declarations"
    String stringify(const std::string& in) { return in.c_str(); }
} // namespace detail
} // namespace doctest

testcase(First) {
    std::string dummy1 = "omg";
    std::string dummy2 = "tralala";
    check(dummy1 == dummy2);
}
