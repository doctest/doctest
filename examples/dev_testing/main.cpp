#include "doctest.h"

test_noname { printf("main\n"); }

struct MFL1 {}; test_fixture_noname(MFL1) { printf("MAIN\n"); }

/*
testsuite(MAIN)
test(offf) { printf("+_______+\n"); }
testsuite_end
*/

int main(int argc, char** argv) {
    DOCTEST_INVOKE_ALL_TEST_FUNCTIONS(argc, argv);
    
#if defined(_MSC_VER)
    system("pause");
#endif // _MSC_VER

    return 0;
}
