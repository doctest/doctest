#define DOCTEST_CONFIG_COLORS_NONE // the easy way to fix code coverage
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

// intentionally here so there are subcases on the same lines in different files
TEST_CASE("subcases") {
    SUBCASE("1") {
        SUBCASE("1.1") {}
        SUBCASE("1.2") {}
        SUBCASE("1.2") {}
    }
    // clang-format off
    SUBCASE("2") {} SUBCASE("3") {} // to have subcases on the same line with different names
    // clang-format on
}

#include <iostream>

using namespace std;

using doctest::toString;

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 6)
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif // > gcc 4.6

TEST_CASE("doctest internals") {
    // string stuff
    doctest::String       a(0);
    const doctest::String const_str("omgomgomg");
    a = const_str.c_str();
    CHECK(a.size() == const_str.size());
    CHECK(a.length() == const_str.length());
    CHECK(a.compare(const_str, true) == 0);
    CHECK(a.compare("omgomgomg", false) == 0);

    // toString
    cout << toString("aaa") << toString(0.5f) << toString('c') << toString(true)
         << toString(static_cast<long double>(0.1)) //
         << toString(static_cast<unsigned char>(1)) //
         << toString(static_cast<short>(1))         //
         << toString(static_cast<long>(1))          //
         << toString(static_cast<unsigned long>(1)) //
         << toString(static_cast<unsigned short>(1));

    // others
    CHECK(doctest::detail::rawMemoryToString(a).length() > 0u);
    cout << doctest::detail::fileForOutput("c:\\a") << doctest::detail::fileForOutput("c:/a")
         << doctest::detail::fileForOutput("a");
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
