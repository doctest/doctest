#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#include "doctest.h"

#include <cstdio>

TEST_CASE("plugin") {
    printf("I am a test from the plugin!\n");
}
