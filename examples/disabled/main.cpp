#define DOCTEST_DISABLE

#define DOCTEST_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstdio>

doctest_test("name1") {
    printf("Anyone there?\n");
}

doctest_testsuite(the testsuite!);

doctest_test(ops) {
    printf("Anyone there?\n");
}

doctest_testsuite_end;

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Weffc++"
#endif

struct Empty
{};

doctest_fixture(Empty, "name") {
    printf("Help?\n");
}

doctest_fixture(Empty, ops) {
    printf("Help?\n");
}
