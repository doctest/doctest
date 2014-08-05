// use %ERRORLEVEL% to get the result of `int main()`

#include "doctest.h"

struct Y {
    static void check() { int a = 5; }
};

DOCTEST_REGISTER_CLASS_CHECK_FUNCTION(Y);

#include "u.h"

#include <vector>
DOCTEST_IMPLEMENT_GLOBALS();

int main() {
    DOCTEST_INVOKE_ALL_TEST_FUNCTIONS();
    
    //system("pause");
	
    return 0;
}
