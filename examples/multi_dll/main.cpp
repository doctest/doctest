#define DOCTEST_DONT_INCLUDE_IMPLEMENTATION
#include "doctest.h"

//#include "doctest_impl.h"

#include <cstdlib>
#include <cstdio>

doctest(First) {
    printf("IN FILE: %s\n", __FILE__);
}

int main(int argc, char** argv) {
    DOCTEST_INVOKE_ALL_TEST_FUNCTIONS(argc, argv);

#if defined(_MSC_VER)
    system("pause");
#endif // _MSC_VER

    return 0;
}
