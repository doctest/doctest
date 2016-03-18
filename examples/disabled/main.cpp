#define DOCTEST_CONFIG_DISABLE

#define DOCTEST_CONFIG_MAIN
#define DOCTEST_CONFIG_IMPLEMENT
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

struct Empty
{};

doctest_fixture(Empty, "name") {
    printf("Help?\n");
}

doctest_fixture(Empty, ops) {
    printf("Help?\n");
}
