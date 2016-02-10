#define DOCTEST_CONFIG_MAIN
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

//system("timeout /t 1 /nobreak");
//system("sleep 1s");

#if defined(_WIN32)
#define implement_sleeper \
    for(int i = 0; i < 100000000; i++)\
    ;\
    printf("%s\n", __FUNCTION__);
#else
#define implement_sleeper \
    for(int i = 0; i < 100000000; i++)\
    ;\
    printf("%s\n", __func__);
#endif

#include <cstdio>
#include <cstdlib>

doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
doctest_test(sleepy) { implement_sleeper }
