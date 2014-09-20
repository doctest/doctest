#include "doctest.h"

#include <cstdio>

#if defined(_MSC_VER)
#define __func__ __FUNCTION__
#endif // _MSC_VER

struct MFL {};

test_fixture(MFL, qwe) { printf("%s\n", __func__); }
test(Asd) { printf("%s\n", __func__); }

test_noname { printf("%s\n", __func__); }
test(Asd) { printf("%s\n", __func__); }
test(asdfa) { printf("%s\n", __func__); }
test(Asd) { printf("%s\n", __func__); }
test(adffd) { printf("%s\n", __func__); }
test(qweqq) { printf("%s\n", __func__); }
test_noname { printf("%s\n", __func__); }

test_fixture_noname(MFL) { printf("%s\n", __func__); }

test_fixture(MFL, asdf) { printf("%s\n", __func__); }
test_fixture(MFL, qwe) { printf("%s\n", __func__); }
test_fixture(MFL, q) { printf("%s\n", __func__); }
test_fixture(MFL, dsf) { printf("%s\n", __func__); }

struct Test1 {
    Test1()
        : a(5) {}

protected:
    int a;
};

test_fixture(Test1, asdf) { printf("%d\n", a); }

test(asd) { printf("in test asd1\n"); }
test(asd) { printf("in test asd2\n"); }
test(asd) { printf("in test asd3\n"); }


