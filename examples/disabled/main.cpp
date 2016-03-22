#define DOCTEST_DISABLE

#define DOCTEST_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstdio>

testcase("name1") {
    printf("Anyone there?\n");
}

testsuite(the testsuite!);

testcase(ops) {
    printf("Anyone there?\n");
}

testsuite_end;

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Weffc++"
#endif

struct Empty
{};

testcase_fixture(Empty, "name") {
    printf("Help?\n");
}

testcase_fixture(Empty, ops) {
    printf("Help?\n");
}
