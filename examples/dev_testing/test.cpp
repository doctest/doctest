//#define DOCTEST_CONFIG_DISABLE

#include "doctest.h"

#include <cstdio>

#include <exception>
#include <string>
#include <vector>
#include <ostream>
#include <cstring>

namespace doctest {
template <typename T>
String toString(const std::vector<T>&) {
    return "aaa";
}
}

template <typename T>
std::ostream& operator<<(std::ostream& s, const std::vector<T>& in) {
    s << "[";
    for(size_t i = 0; i < in.size(); ++i)
        if(i < in.size() - 1)
            s << in[i] << ", ";
        else
            s << in[i];
    s << "]";
    return s;
}

namespace crap {
template<typename T, typename T2>
struct myType { T data; T2 op; };

struct myType2 : myType<int, float> {};

template<typename T, typename T2>
bool operator==(const myType<T, T2>&, const myType<T, T2>&) { return false; }

template<typename T, typename T2>
std::ostream& operator<<(std::ostream& s, const myType<T, T2>&) { s << "myType"; return s; }
}



#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996) // The compiler encountered a deprecated declaration
#pragma warning(disable : 4267) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#pragma warning(disable : 4706) // assignment within conditional expression
#pragma warning(disable : 4512) // 'class' : assignment operator could not be generated
#pragma warning(disable : 4127) // conditional expression is constant
#endif                          // _MSC_VER

TEST_SUITE("MAIN");
TEST_CASE("zzz") {
    //CHECK(std::string("OMG2") == std::string("OMG"));
    //CHECK("OMG2" == std::string("OMG2"));
    char* foo = new char[10];
    strcpy(foo, "xxx");
    char* bar = new char[5];
    strcpy(bar, "xxx");
    const char* const op = "xxx";
    const char* op2 = "xxx";
    CHECK(op == "xxx");
    CHECK(foo == bar);
    CHECK(op == op2);
    CHECK(op2 == bar);

    //doctest::detail::eq(foo, op2);

    CHECK(doctest::Approx(0.2) == 0.2);

    std::vector<int> vec1;
    vec1.push_back(1);
    vec1.push_back(2);
    vec1.push_back(3);

    std::vector<int> vec2;
    vec2.push_back(1);
    vec2.push_back(2);
    vec2.push_back(4);

    crap::myType2 opA;
    crap::myType2 opB;

    CHECK(opA == opB);

    CHECK(vec1 == vec2);
    //CHECK(vec1 == vec2);

    REQUIRE(true == false);
    //
    //printf("main\n");
    //SUBCASE("") {
    //    printf("1\n");
    //    SUBCASE("") { printf("1-1\n"); }
    //    SUBCASE("") { printf("1-2\n"); }
    //}
    //SUBCASE("") { printf("2\n"); }
}
TEST_SUITE_END();

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Weffc++"
#endif

struct Empty
{};

TEST_CASE_FIXTURE(Empty, "trololo") { printf("Help?\n"); }

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

    CHECK(1);

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

// testing randomness
TEST_SUITE("randomness");
TEST_CASE("") { printf("TEST %d\n", __LINE__ - 100); }
TEST_CASE("") { printf("TEST %d\n", __LINE__ - 100); }
TEST_CASE("") { printf("TEST %d\n", __LINE__ - 100); }
TEST_CASE("") { printf("TEST %d\n", __LINE__ - 100); }
TEST_CASE("") { printf("TEST %d\n", __LINE__ - 100); }
TEST_CASE("") { printf("TEST %d\n", __LINE__ - 100); }
TEST_CASE("") { printf("TEST %d\n", __LINE__ - 100); }
TEST_CASE("") { printf("TEST %d\n", __LINE__ - 100); }
TEST_CASE("") { printf("TEST %d\n", __LINE__ - 100); }
TEST_CASE("") { printf("TEST %d\n", __LINE__ - 100); }
TEST_SUITE_END();
