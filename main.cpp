// use %ERRORLEVEL% to get the result of `int main()`
#include "doctest.h"

// == benchmark:
//  10k doctests in 1 cpp == 32 sec build time on my machine
//   5k doctests in 1 cpp ==  9 sec build time on my machine
// 2.5k doctests in 1 cpp ==  3 sec build time on my machine

int main(int argc, char** argv) {
    DOCTEST_INVOKE_ALL_TEST_FUNCTIONS(argc, argv);
    
#if defined(_MSC_VER)
    system("pause");
#endif // _MSC_VER

    return 0;
}
