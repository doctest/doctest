#define DOCTEST_CONFIG_MAIN
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <cstdio>

doctest_test(First) {
    printf("Hello world!\n");
}
