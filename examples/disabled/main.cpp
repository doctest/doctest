#define DOCTEST_GLOBAL_DISABLE
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

struct Stuff {
    static void check() { printf("halp!!!!!!!!!\n"); }
};

doctest_static_method(Stuff, check)

int main(int argc, char** argv) {
    DOCTEST_INVOKE_ALL_TEST_FUNCTIONS(argc, argv);

#if defined(_MSC_VER)
    system("pause");
#endif // _MSC_VER

    return 0;
}
