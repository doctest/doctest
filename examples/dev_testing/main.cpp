#include "doctest.h"

#include <iostream>
using namespace std;

testsuite(MAIN);
test(zzz) { cout << "main!" << endl; }
testsuite_end;

int main(int argc, char** argv)
{
    void* params = doctest::createParams(argc, argv);
    doctest::setOption(params, "doctest_case_sensitive", true);
    doctest::addFilter(params, "doctest_name", "zzz");
    doctest::runTests(params);
    doctest::freeParams(params);

#if defined(_MSC_VER)
    system("pause");
#endif // _MSC_VER

    return 0;
}
