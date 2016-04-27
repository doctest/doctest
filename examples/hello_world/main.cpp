#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstdio>

TESTCASE("the only test") {
    printf("Hello world!\n");
}
