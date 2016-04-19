//#define DOCTEST_DISABLE

#include "doctest.h"

#include <cstdio>

#include <exception>
#include <string>

namespace doctest
{
namespace detail
{
    String stringify(const std::string& in); // to silence GCC "-Wmissing-declarations"
    String stringify(const std::string& in) { return String("\"") + in.c_str() + "\""; }
} // namespace detail
} // namespace doctest

testsuite(MAIN);
testcase(zzz) {
    require(true == false);
    check(std::string("OMG2") == std::string("OMG"));

    printf("main\n");
    subcase("") {
        printf("1\n");
        subcase("") { printf("1-1\n"); }
        subcase("") { printf("1-2\n"); }
    }
    subcase("") { printf("2\n"); }
}
testsuite_end;

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Weffc++"
#endif

struct Empty
{};

testcase_fixture(Empty, trololo) { printf("Help?\n"); }

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

// to silence GCC "-Wmissing-declarations"
// and the attribute is to silence "-Wmissing-noreturn" on clang
#ifdef __clang__
void throws() __attribute__((noreturn));
#else
void throws();
#endif

void throws() { throw std::exception(); }
void nothrows(); // to silence GCC "-Wmissing-declarations"
void nothrows() {}

testcase(zzz) {
    check(1 == 0);
    check_false(1 == 0);

    check(1 == 1);
    require(1 == 1);

    check_false(0);
    require_false(0);

    check_throws(throws());
    require_throws(throws());

    check_throws_as(throws(), std::exception);
    require_throws_as(throws(), std::exception);

    check_nothrow(nothrows());
    require_nothrow(nothrows());
}
