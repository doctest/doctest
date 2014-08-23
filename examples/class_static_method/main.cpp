#include "doctest.h"

#include <cstdio>

struct fizbuz {
    static void flap() { printf("zzzzzzzzzzzzzzz!\n"); }
    static void click() { printf("click! click! click!\n"); }
};

doctest_static_method(fizbuz, flap)
doctest_static_method(fizbuz, click)
doctest_static_method(fizbuz, click)
doctest_static_method(fizbuz, flap)

int main(int argc, char** argv) {
    DOCTEST_INVOKE_ALL_TEST_FUNCTIONS(argc, argv);

#if defined(_MSC_VER)
    system("pause");
#endif // _MSC_VER

    return 0;
}
