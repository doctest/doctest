#define DOCTEST_CONFIG_DISABLE

#define DOCTEST_CONFIG_MAIN
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <cstdio>

doctest_test_noname {
    printf("Anyone there?\n");
}

doctest_test(ops) {
    printf("Anyone there?\n");
}

struct Empty {};

doctest_fixture_noname(Empty) {
    printf("Help?\n");
}

doctest_fixture(Empty, ops) {
    printf("Help?\n");
}
