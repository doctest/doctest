#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <cstdio>

doctest_test_noname {
    printf("Anyone there?\n");
}

doctest_test(ops) {
    printf("Anyone there?\n");
}

struct Empty {};

doctest_fixture_noname(Empty) {
    printf("Help?\n");
}

doctest_fixture(Empty, ops) {
    printf("Help?\n");
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
