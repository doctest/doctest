//#define DOCTEST_CONFIG_DISABLE

//#include "doctest.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <cstdio>

int main(int argc, char** argv) {
    // initialize
    doctest::Context context(argc, argv);

    // overrides
    context.setOption("dt-case-sensitive", true);
    context.setOption("no-breaks", true);
    //context.setOption("dt-no-colors", true);
    context.addFilter("dt-name", "zzz");

    // run
    int res = context.runTests();

#if defined(WITH_PAUSE)
    system("pause");
#endif // WITH_PAUSE

    return res;
}
