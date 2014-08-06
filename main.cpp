// use %ERRORLEVEL% to get the result of `int main()`

#include "crap.h"

int main() {
    DOCTEST_INVOKE_ALL_TEST_FUNCTIONS();
    
#if defined(_MSC_VER)
    system("pause");
#endif // _MSC_VER

    return 0;
}
