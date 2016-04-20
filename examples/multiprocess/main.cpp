#define DOCTEST_IMPLEMENT_WITH_MAIN
#include "doctest.h"

//system("timeout /t 1 /nobreak");
//system("sleep 1s");

#include <cstdio>
#include <cstdlib>

static void sleeper(int line) {
#ifdef NDEBUG
    for(int i = 0; i < 50000000; i++)
#else
    for(int i = 0; i < 10000000; i++)
#endif
    ;
    printf("%d\n", line);
//#if defined(_WIN32)
//    printf("%s\n", __FUNCTION__);
//#else
//    printf("%s\n", __func__);
//#endif
}

TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
TESTCASE("sleepy") { sleeper(__LINE__); }
