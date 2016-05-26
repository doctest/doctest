#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstdio>

static void sleeper(int line) {
#ifdef NDEBUG
    for(int i = 0; i < 50000000; i++) ((void)0);
#else
    for(int i = 0; i < 10000000; i++) ((void)0);
#endif
    printf("hello from test case %d!\n", line);
}

TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
TEST_CASE("") { sleeper(__LINE__ - 14); }
