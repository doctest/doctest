#include "doctest.h"

#include <iostream>
#include <exception>
using namespace std;

testsuite(MAIN);
test(zzz)
{
    cout << "main!" << endl;
}
testsuite_end;

test(thrower)
{
    if(5 > 4) {
        throw std::exception();
    } else {
        cout << "trololo" << endl;
    }
}

int testWrapper(void (*f)(void))
{
    try {
        f();
    } catch(std::exception& e) {
        cout << e.what() << endl;
        return 1;
    }
    return 0;
}

int main(int argc, char** argv)
{
    void* params = doctest::createParams(argc, argv);
    doctest::setOption(params, "doctest_case_sensitive", true);
    // doctest::addFilter(params, "doctest_name", "zzz");
    doctest::setTestExecutionWrapper(params, testWrapper);
    doctest::runTests(params);
    doctest::freeParams(params);

#if defined(_MSC_VER)
    system("pause");
#endif // _MSC_VER

    return 0;
}
