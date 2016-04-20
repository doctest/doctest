#define DOCTEST_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstdio>

TESTCASE("the only test") {
    printf("Hello world!\n");
}
