#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_CASE("inconsistent init before subcases") {
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
    static int cnt = 0;
    if (++cnt == 2) {
        throw std::runtime_error{"#782 repro exception"};
    }
    DOCTEST_SUBCASE("1") {}
    DOCTEST_SUBCASE("2") {}
    DOCTEST_SUBCASE("3") {}
#else
    FAIL("Exceptions disabled, cannot run this test");
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS
}
