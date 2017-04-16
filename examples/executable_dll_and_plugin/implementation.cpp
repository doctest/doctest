#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <cstdio>

TEST_CASE("implementation") {
    printf("I am a test from the implementation!\n");
}
