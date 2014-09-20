#define DOCTEST_DONT_INCLUDE_IMPLEMENTATION
#include "doctest.h"

#include <cstdio>

doctest_test(main) {
    printf("Here we don't have std::map included because the doctest implementation is included only in a single cpp file elsewhere!\n");
}

int main(int argc, char** argv) {
    DOCTEST_INVOKE_ALL_TEST_FUNCTIONS(argc, argv);

#if defined(_MSC_VER)
    system("pause");
#endif // _MSC_VER

    return 0;
}
