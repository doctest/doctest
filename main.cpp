// use %ERRORLEVEL% to get the result of `int main()`
#include "doctest.h"

int main(int argc, char** argv) {
    DOCTEST_INVOKE_ALL_TEST_FUNCTIONS(argc, argv);
    
#if defined(_MSC_VER)
    system("pause");
#endif // _MSC_VER

    return 0;
}
