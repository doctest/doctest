#define DOCTEST_DONT_INCLUDE_IMPLEMENTATION
#include "doctest.h"

#include "doctest_impl.h"

#include <cstdio>

doctest(First) {
    printf("IN FILE: %s\n", __FILE__);
}
