//#define DOCTEST_DISABLE

#include "doctest.h"

#include <cstdio>

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
    check(true == false);
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

testcase(zzz) {
    CHECK(1 == 1);
    CHECK_FALSE(1 == 1);

}
