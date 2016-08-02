#define DOCTEST_CONFIG_COLORS_NONE
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

TEST_CASE("subcases") {
    SUBCASE("1") {
        SUBCASE("1.1") {}
        SUBCASE("1.2") {}
    }
    SUBCASE("2") {}
    SUBCASE("3") {}
}

#include <iostream>

using namespace std;

using doctest::toString;

TEST_CASE("throws") {
    CHECK(1 == 2);

    doctest::String a;
    a += "omg";
    const doctest::String const_str("omgomgomg");
    a = const_str.c_str();
    CHECK(a.size() == const_str.size());
    CHECK(a.length() == const_str.length());
    CHECK(a.compare(const_str, true) == 0);
    CHECK(a.compare("omgomgomg", true) == 0);

    cout << a << toString("aaa") << toString(0.5f) << toString('c')
         << toString(static_cast<long double>(0.1)) //
         << toString(static_cast<unsigned char>(1)) //
         << toString(static_cast<short>(1))         //
         << toString(static_cast<long>(1))          //
         << toString(static_cast<unsigned long>(1)) //
         << toString(static_cast<unsigned short>(1));
}

int main(int argc, char** argv) {
    doctest::Context context;

    context.addFilter("test-case-exclude", "*math*");
    context.setOption("no-breaks", true);
    context.setOption("sort", "name");

    context.applyCommandLine(argc, argv);

    int res = context.run();

    if(context.shouldExit())
        return res;

    return res;
}
