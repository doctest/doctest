#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <cstdio>

doctest_test(First) {
    printf("Hello world!\n");
}

int main(int argc, char** argv) {
    void* params = doctest::createParams(argc, argv);
    int res = doctest::runTests(params);
    doctest::freeParams(params);

#if defined(_MSC_VER)
    system("pause");
#endif // _MSC_VER

    return res;
}
