#include "doctest.h"

#include <cstdio>

doctest_test(First) {
    printf("Hello world!\n");
}

int main(int argc, char** argv) {
    DOCTEST_INVOKE_ALL_TEST_FUNCTIONS(argc, argv);

#if defined(_MSC_VER)
    system("pause");
#endif // _MSC_VER

    return 0;
}
