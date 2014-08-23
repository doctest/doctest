#include "doctest.h"

#include <cstdio>
#if defined(_MSC_VER)
#define __func__ __FUNCTION__
#endif // _MSC_VER

struct MFL {};

doctest_fixture(MFL, qwe) { printf("%s\n", __func__); }
doctest(Asd) { printf("%s\n", __func__); }

doctest_noname { printf("%s\n", __func__); }
doctest(Asd) { printf("%s\n", __func__); }
doctest(asdfa) { printf("%s\n", __func__); }
doctest(Asd) { printf("%s\n", __func__); }
doctest(adffd) { printf("%s\n", __func__); }
doctest(qweqq) { printf("%s\n", __func__); }
doctest_noname { printf("%s\n", __func__); }

doctest_fixture_noname(MFL) { printf("%s\n", __func__); }

doctest_fixture(MFL, asdf) { printf("%s\n", __func__); }
doctest_fixture(MFL, qwe) { printf("%s\n", __func__); }
doctest_fixture(MFL, q) { printf("%s\n", __func__); }
doctest_fixture(MFL, dsf) { printf("%s\n", __func__); }

struct Test1 {
    Test1()
        : a(5) {}

protected:
    int a;
};

doctest_fixture(Test1, asdf) { printf("%d\n", a); }


