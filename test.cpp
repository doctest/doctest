#include "doctest.h"

#include <cstdio>
#if defined(_MSC_VER)
#define __func__ __FUNCTION__
#endif // _MSC_VER

struct MFL{};

#define doctest_named doctest
#define doctest_fixture_named doctest_fixture
doctest_fixture_named(MFL, qwe) { printf("%s\n", __func__); }
doctest_named(Asd) { printf("%s\n", __func__); }

doctest() { printf("%s\n", __func__); }
doctest(Asd) { printf("%s\n", __func__); }
doctest(asdfa) { printf("%s\n", __func__); }
doctest(Asd) { printf("%s\n", __func__); }
doctest(adffd) { printf("%s\n", __func__); }
doctest(qweqq) { printf("%s\n", __func__); }
doctest( ) { printf("%s\n", __func__); }

doctest_fixture(MFL, ) { printf("%s\n", __func__); }
doctest_fixture(MFL, qwe) { printf("%s\n", __func__); }
doctest_fixture(MFL, q) { printf("%s\n", __func__); }
doctest_fixture(MFL, dsf) { printf("%s\n", __func__); }

struct Test1 {
    Test1() : a(5) {}
protected:
    int a;
};

doctest_fixture(Test1, ) { printf("%d\n", a); }
