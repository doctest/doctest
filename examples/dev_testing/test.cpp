//#define DOCTEST_CONFIG_DISABLE

#include "doctest.h"

#include <cstdio>

#include <exception>
#include <string>
#include <vector>

namespace doctest
{
template <>
String stringify(ADL_helper, const std::string& in) {
    return String("\"") + in.c_str() + "\"";
}

template <typename T>
String stringify(ADL_helper, const std::vector<T>& in) {
    String out("vector[");
    for(unsigned i = 0; i < in.size(); ++i)
        out += stringify(ADL_helper(), in[i]) + (i + 1 == in.size() ? "]" : ", ");
    return out;
}
} // namespace doctest

TEST_SUITE("MAIN");
TEST_CASE("zzz") {
    CHECK(std::string("OMG2") == std::string("OMG"));

    std::vector<int> vec1;
    vec1.push_back(1);
    vec1.push_back(2);
    vec1.push_back(3);

    std::vector<int> vec2;
    vec2.push_back(1);
    vec2.push_back(2);
    vec2.push_back(4);

    CHECK(vec1 == vec2);

    //REQUIRE(true == false);
    //
    //printf("main\n");
    //SUBCASE("") {
    //    printf("1\n");
    //    SUBCASE("") { printf("1-1\n"); }
    //    SUBCASE("") { printf("1-2\n"); }
    //}
    //SUBCASE("") { printf("2\n"); }
}
TEST_SUITE_END;

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Weffc++"
#endif

struct Empty
{};

TEST_CASE_FIXTURE(Empty, "trololo") { printf("Help?\n"); }

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

TEST_CASE("zzz") {
    int a = 5;
    int b = 5;
    CHECK(&a == &b);

    CHECK(0);

    CHECK(1 == 1);
    REQUIRE(1 == 1);

    CHECK_FALSE(0);
    REQUIRE_FALSE(0);

    CHECK_THROWS(throws());
    REQUIRE_THROWS(throws());

    CHECK_THROWS_AS(throws(), std::exception);
    REQUIRE_THROWS_AS(throws(), std::exception);

    CHECK_NOTHROW(nothrows());
    REQUIRE_NOTHROW(nothrows());
}
