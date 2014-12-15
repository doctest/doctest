#define DOCTEST_CONFIG_MAIN
#include "doctest.h"

#if defined(_WIN32)
#define implement_sleeper \
    system("timeout /t 1 /nobreak");\
    printf("%s\n", __FUNCTION__);
#else
#define implement_sleeper \
    system("sleep 5s");\
    printf("%s\n", __func__);
#endif

#include <cstdio>
#include <cstdlib>

// 10 times * 1 sec
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
