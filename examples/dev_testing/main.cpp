//#define DOCTEST_CONFIG_DISABLE

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <cstdio>
#include <exception>

static int testWrapper(void (*f)(void)) {
    try {
        f();
    } catch(std::exception& e) {
        printf("caught the bugger! %s\n", e.what());
        return 1;
    }
    return 0;
}

int main(int argc, char** argv) {
    // initialize
    doctest::Context context(argc, argv);
    context.setTestExecutionWrapper(testWrapper);

    // overrides
    context.setOption("dt-case-sensitive", true);
    context.addFilter("dt-name", "zzz");

    // run
    int res = context.runTests();

#if defined(WITH_PAUSE)
    system("pause");
#endif // _MSC_VER

    return res;
}
