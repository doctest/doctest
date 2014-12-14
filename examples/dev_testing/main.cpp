#include "doctest.h"

#include <iostream>
using namespace std;

#define my_stupid_macro

testsuite(MAIN)
test(zzz) { cout << "main!" << endl; }
testsuite_end

int main(int argc, char** argv) {

    //doctest::Options options;
    //doctest::parseOptions(&options, argc, argv);
    //doctest::

    DOCTEST_INVOKE_ALL_TEST_FUNCTIONS(argc, argv);

#if defined(_MSC_VER)
    system("pause");
#endif // _MSC_VER

    return 0;
}
