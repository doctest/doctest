//#define DOCTEST_DISABLE

#include "doctest.h"

#include <cstdio>

#include <string>
namespace doctest
{
    namespace detail
    {
        String stringify(const std::string& in); // to silence GCC "-Wmissing-declarations"
        String stringify(const std::string& in) {
            String out;
            out += in.c_str();
            return out;
        }
    } // namespace detail
} // namespace doctest

testsuite(MAIN);
test(zzz) {
    check(true == false);
    check(std::string("OMG2") == std::string("OMG"));

    printf("main\n");
    subtest("") {
        printf("1\n");
        subtest("") { printf("1-1\n"); }
        subtest("") { printf("1-2\n"); }
    }
    subtest("") { printf("2\n"); }
}
testsuite_end;

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Weffc++"
#endif

struct Empty
{};

doctest_fixture(Empty, trololo) { printf("Help?\n"); }

// test("") { printf("TEST %d\n", __LINE__); }
// test("") { printf("TEST %d\n", __LINE__); }
// test("") { printf("TEST %d\n", __LINE__); }
// test("") { printf("TEST %d\n", __LINE__); }
// test("") { printf("TEST %d\n", __LINE__); }
// test("") { printf("TEST %d\n", __LINE__); }
// test("") { printf("TEST %d\n", __LINE__); }
// test("") { printf("TEST %d\n", __LINE__); }
// test("") { printf("TEST %d\n", __LINE__); }
// test("") { printf("TEST %d\n", __LINE__); }
// test("") { printf("TEST %d\n", __LINE__); }
